//
// Created by toor on 2/16/24.
//

#include "TrainEngine.h"
#include "ATOM/Application.h"


namespace Atom {
    TrainEngine::TrainEngine()
        : Layer("TrainEngine") {
#ifdef NDEBUG
        m_Model = new YoloV8("/home/toor/Code/ATOM_Brain/app/ASSETS/best.onnx", config);
        m_Frame = &Application::GetApp().GetFrame().GetNativeFrame();
#endif
    }

    TrainEngine::~TrainEngine() {
    }

    void TrainEngine::OnAttach() {
    }

    void TrainEngine::OnDetach() {
    }

    void TrainEngine::OnUpdate() {
    }

    void TrainEngine::OnFixedUpdate() {
#ifdef NDEBUG
        if (!m_Frame->empty()) {
            if (!isTextureGenerated) {
                glGenTextures(1, &m_Texture);
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Frame->cols,
                             m_Frame->rows, 0, GL_BGR, GL_UNSIGNED_BYTE,
                             m_Frame->data);
                glBindTexture(GL_TEXTURE_2D, 0);

                m_AspectRatio = (float) m_Frame->cols / (float) m_Frame->rows;
                isTextureGenerated = true;
            }

            if (isTextureGenerated && m_Texture != 0) {
                cv::copyTo(*m_Frame, m_LocalFrame, cv::noArray());
                // m_LocalFrame
                cv::flip(m_LocalFrame,m_LocalFrame,
                    90);
                const auto objects = m_Model->detectObjects(m_LocalFrame);
                // Draw the bounding boxes on the image
                m_Model->drawObjectLabels(m_LocalFrame, objects);
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                //get type of image
                if (m_LocalFrame.type() == CV_8UC3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_LocalFrame.cols,
                                 m_LocalFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE,
                                 m_LocalFrame.data);
                } else if (m_LocalFrame.type() == CV_8UC4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_LocalFrame.cols,
                                 m_LocalFrame.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                                 m_LocalFrame.data);
                }
                glBindTexture(GL_TEXTURE_2D, 0);

                //complete the vector of signs
                m_Signs.clear();
                for (const auto &object: objects) {
                    Sign sign;
                    float pixelFromCenter = (object.rect.x + object.rect.width / 2) - m_LocalFrame.cols / 2;
                    sign.pixelFromCenter = pixelFromCenter;
                    // ATLOG_INFO("Angle from center: {0}", sign.pixelFromCenter);
                    sign.confidence = object.probability;
                    sign.label = m_Model->getLabels()[object.label];
                    m_Signs.push_back(sign);
                }
            }
        }
#endif
    }

    void TrainEngine::OnImGuiRender() {
        ImGui::Begin("Ai Engine");
#ifdef NDEBUG

        if (isTextureGenerated == true) {
            ImVec2 canvas_left_top = ImGui::GetCursorScreenPos();
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
            ImVec2 canvas_bottom_right = ImVec2(canvas_left_top.x + canvas_size.x, canvas_left_top.y + canvas_size.y);
            ImVec2 available = ImGui::GetContentRegionAvail();
            if (available.x > available.y * m_AspectRatio) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (available.x / 2 - available.y * m_AspectRatio / 2));
                canvas_left_top.x = ImGui::GetCursorScreenPos().x;
                canvas_bottom_right.x = canvas_left_top.x + available.y * m_AspectRatio;
                available.x = available.y * m_AspectRatio;
            } else {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (available.y / 2 - available.x / m_AspectRatio / 2));
                canvas_left_top.y = ImGui::GetCursorScreenPos().y;
                canvas_bottom_right.y = canvas_left_top.y + available.x / m_AspectRatio;
                available.y = available.x / m_AspectRatio;
            }
            ImGuiIO &io = ImGui::GetIO();
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(canvas_left_top, canvas_bottom_right, IM_COL32(50, 50, 50, 255));


            //draw image to background
            draw_list->AddImage((void *) m_Texture, canvas_left_top, canvas_bottom_right, ImVec2(0, 0),
                                ImVec2(1, 1));


            draw_list->AddRect(canvas_left_top, canvas_bottom_right, IM_COL32(255, 255, 255, 255));
            draw_list->PushClipRect(canvas_left_top, canvas_bottom_right, true);
            draw_list->PopClipRect();
        } else {
            ImGui::Text("Please Open camera before");
        }

#endif

        ImGui::End();
    }
}

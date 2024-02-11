//
// Created by toor on 2/4/24.
//

#include "DetectLines.h"

#include "ATOM/Application.h"


namespace Atom {
    DetectLines::DetectLines()
        : Layer("DetectLines") {
        m_Frame = &Application::GetApp().GetFrame().GetNativeFrame();
        m_FindLines = new FindLines(*m_Frame);
        m_LineSettings = &m_FindLines->GetLineSettings();
        m_Texture = 0;
    }

    DetectLines::~DetectLines() {
    }

    void DetectLines::OnAttach() {
    }

    void DetectLines::OnDetach() {
    }

    void DetectLines::OnUpdate() {
    }

    void DetectLines::OnFixedUpdate() {
        if (!m_Frame->empty()) {
            m_FindLines->Update();
            m_FrameExist = true;
            // ATLOG_INFO("Offset: {0}", m_FindLines->GetOffsetCenter());
            if (m_AspectRatio == 0) {
                m_AspectRatio = m_AspectRatio = (float) m_Frame->cols / (float) m_Frame->rows;
                ATLOG_WARN("Change aaspect ratio")
            }
        } else {
        }

        if (m_FindLines->GetLineSettings().showDebug) {
            if (m_Texture == 0) {
                glGenTextures(1, &m_Texture);
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_FindLines->GetPreprocessed().cols,
                             m_FindLines->GetPreprocessed().rows, 0, GL_BGR, GL_UNSIGNED_BYTE,
                             m_FindLines->GetPreprocessed().data);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            if (m_LineSettings->showDebug && showDebugWindow) {
                if (m_Texture != 0) {
                    glBindTexture(GL_TEXTURE_2D, m_Texture);
                    //get type of image

                    if (m_FindLines->GetPreprocessed().type() == CV_8UC3) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_FindLines->GetPreprocessed().cols,
                                     m_FindLines->GetPreprocessed().rows, 0, GL_BGR, GL_UNSIGNED_BYTE,
                                     m_FindLines->GetPreprocessed().data);
                    } else if (m_FindLines->GetPreprocessed().type() == CV_8UC4) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_FindLines->GetPreprocessed().cols,
                                     m_FindLines->GetPreprocessed().rows, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                                     m_FindLines->GetPreprocessed().data);
                    }
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
        }
    }

    void DetectLines::OnImGuiRender() {
        ImGui::Begin("Line Settings");

        ImGui::SliderInt("Min h", &h, 0, 255);
        ImGui::SliderInt("Min s", &s, 0, 255);
        ImGui::SliderInt("Min v", &v, 0, 255);
        m_FindLines->GetLineSettings().lowerWhite = cv::Scalar(h, s, v);
        //slider for offset
        ImGui::SliderInt("Offset", &m_FindLines->GetLineSettings().offsetSides, 0, 240);


        if (m_FrameExist == true) {
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
            draw_list->AddImage((void *) m_Texture, canvas_left_top, canvas_bottom_right, ImVec2(1, 0),
                                ImVec2(0, 1));


            sprintf(buffer, "Offeset: (%f)", m_FindLines->GetOffsetCenter());
            draw_list->AddText(ImVec2(canvas_left_top.x + 10, canvas_left_top.y + 30), IM_COL32(255, 255, 255, 255),
                               buffer);


            draw_list->AddRect(canvas_left_top, canvas_bottom_right, IM_COL32(255, 255, 255, 255));
            draw_list->PushClipRect(canvas_left_top, canvas_bottom_right, true);
            draw_list->PopClipRect();
        }else {
            ImGui::Text("Please Open camera before");
        }


        ImGui::End();
    }
}

//
// Created by toor on 1/19/24.
//

#include "DrawMap.h"

#include "ATOM/Application.h"


namespace Atom {
    void MatToTexture(const cv::Mat &mat, GLuint &texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    DrawMap::DrawMap()
        : Layer("DrawMap") {
        m_Window = (GLFWwindow *) Application::GetApp().GetWindow().GetNativeWindow();

        GenerateTextures();
        m_Matrix = new MatrixElement *[m_LinesX];
        for (int i = 0; i < m_LinesX; ++i) {
            m_Matrix[i] = new MatrixElement[m_LinesY];
        }
        // m_ImgHSV
        cv::cvtColor(m_ImgColorBackground, m_ImgHSV, cv::COLOR_BGR2HSV);

        //copy treshold values for blue color
        m_TresholdsMin = glm::vec3(119.0, 120.0, 255.0);
        m_TresholdsMax = glm::vec3(255.0, 255.0, 255.0);
        //main road tresold blue color

        cv::inRange(m_ImgHSV, cv::Scalar(119.0,120.0,255.0), cv::Scalar(255.0,255.0,255.0), m_MainRoad);
        cv::erode(m_MainRoad, m_MainRoad, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));




        //side road tresold red color
        cv::inRange(m_ImgHSV, cv::Scalar(0, 200, 100), cv::Scalar(10, 255, 255), m_SideRoad);

        //extracting points on matrix that is part of main road
        for (int i = 0; i < m_LinesX; ++i) {
            for (int j = 0; j < m_LinesY; ++j) {
                if (m_MainRoad.at<uchar>(j * m_MainRoad.rows / m_LinesY, i * m_MainRoad.cols / m_LinesX) > 50) {
                    m_Matrix[i][j].type = ElementType::MainRoad;
                }else {
                    m_Matrix[i][j].type = ElementType::Empty;
                }

            }
        }


        for (int i = 0; i < m_LinesX; ++i) {
            for (int j = 0; j < m_LinesY; ++j) {
                //detect if point is part of main road
                if (m_Matrix[i][j].type == ElementType::MainRoad) {

                }

            }
        }


        m_FrameWidth = m_ImgBackground.cols;
        m_FrameHeight = m_ImgBackground.rows;
        m_AspectRatio = (float) m_FrameWidth / (float) m_FrameHeight;



    }

    DrawMap::~DrawMap() {
    }

    void DrawMap::OnAttach() {
//        MatToTexture(m_ImgBackground, textureID);
    }

    void DrawMap::OnDetach() {
    }

    void DrawMap::OnUpdate() {
        if(!m_ImgColorBackground.empty()){


            // if (m_MapSetings.isChanged) {
                m_MapSetings.isChanged = false;
                // MatToTexture(m_ImgColorBackground, textureID);
                // m_UpdateTexture = true;

                switch (m_MapSetings.background) {
                    case MapBackground::Track:
                        MatToTexture(m_ImgBackground, textureID);
                        break;
                    case MapBackground::ColorTrack:
                        MatToTexture(m_ImgColorBackground, textureID);
                        break;
                    case MapBackground::MainRoad:
                    //     cv::cvtColor(m_ImgColorBackground, m_ImgHSV, cv::COLOR_BGR2HSV);
                    //
                    //     cv::inRange(m_ImgHSV, cv::Scalar(m_TresholdsMin.x, m_TresholdsMin.y, m_TresholdsMin.z),
                    // cv::Scalar(m_TresholdsMax.x, m_TresholdsMax.y, m_TresholdsMax.z), m_MainRoad);
                        //check color format
                        if (m_MainRoad.type() == CV_8UC1) {
                            cv::cvtColor(m_MainRoad, m_MainRoad, cv::COLOR_GRAY2BGR);
                        }
                        MatToTexture(m_MainRoad, textureID);
                        break;
                    default:
                        MatToTexture(m_ImgBackground, textureID);
                        break;

                }
            // }
        }

    }

    void DrawMap::OnImGuiRender() {
        ImGui::Begin("DrawMap");

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
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_left_top, canvas_bottom_right, IM_COL32(50, 50, 50, 255));



        //draw image to background
        draw_list->AddImage((void *) textureID, canvas_left_top, canvas_bottom_right);





        m_DeltaDashLineX = available.x / m_LinesX;
        m_DeltaDashLineY = available.y / m_LinesY;
        // draw grid in available space
        for (int i = 0; i < m_LinesX; ++i) {
            draw_list->AddLine(ImVec2(canvas_left_top.x + i * m_DeltaDashLineX, canvas_left_top.y),
                               ImVec2(canvas_left_top.x + i * m_DeltaDashLineX, canvas_bottom_right.y),
                               IM_COL32(0, 255, 255, 50));
        }
        // horizontal lines
        for (int i = 0; i < m_LinesY; ++i) {
            draw_list->AddLine(ImVec2(canvas_left_top.x, canvas_left_top.y + i * m_DeltaDashLineY),
                               ImVec2(canvas_bottom_right.x, canvas_left_top.y + i * m_DeltaDashLineY),
                               IM_COL32(0, 255, 255, 50));
        }
        if(m_MapSetings.showPoints) {
            // draw points from matrix that are part of main road
            for (int i = 0; i < m_LinesX; ++i) {
                for (int j = 0; j < m_LinesY; ++j) {
                    if (m_Matrix[i][j].type == ElementType::MainRoad) {
                        // draw_list->AddRectFilled(ImVec2(canvas_left_top.x + i * m_DeltaDashLineX, canvas_left_top.y + j * m_DeltaDashLineY),
                        //                         ImVec2(canvas_left_top.x + (i + 1) * m_DeltaDashLineX,
                        //                                canvas_left_top.y + (j + 1) * m_DeltaDashLineY),
                        //                         IM_COL32(0, 255, 0, 255));
                        //draw as circle
                        draw_list->AddCircleFilled(ImVec2(canvas_left_top.x + i * m_DeltaDashLineX ,
                                                          canvas_left_top.y + j * m_DeltaDashLineY ),
                                                  2, IM_COL32(0, 255, 0, 255), 10);
                    }
                }
            }
        }

        ImGui::InvisibleButton("canvas", canvas_size,
                               ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool is_hovered = ImGui::IsItemHovered(); // Hovered
        const bool is_active = ImGui::IsItemActive(); // Held
        const ImVec2 origin(canvas_left_top.x , canvas_left_top.y); // Lock scrolled origin
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);


        //Add text to canvas , mouse position

        sprintf(buffer, "Mouse Position: (%.1f, %.1f)", mouse_pos_in_canvas.x, mouse_pos_in_canvas.y);
        if(is_hovered){
            draw_list->AddText(ImVec2(canvas_left_top.x + 10, canvas_left_top.y + 10), IM_COL32(255, 255, 255, 255), buffer);
            draw_list->AddCircleFilled(ImVec2(mouse_pos_in_canvas.x + origin.x, mouse_pos_in_canvas.y + origin.y), 8,
                                   IM_COL32(255, 255, 255, 255), 10);
            if(!m_StartFinish.StartFlagPlaced) {
                draw_list->AddText(ImVec2(mouse_pos_in_canvas.x + origin.x + 10, mouse_pos_in_canvas.y + origin.y + 10),
                                   IM_COL32(0, 255, 255, 255), "Start");
            }

            if((!m_StartFinish.FinishFlagPlaced )&& m_StartFinish.StartFlagPlaced == true) {
                draw_list->AddText(ImVec2(mouse_pos_in_canvas.x + origin.x + 10, mouse_pos_in_canvas.y + origin.y + 10),
                                   IM_COL32(0, 255, 255, 255), "Finish");
            }


        }



        //input sliders for tresholds
        // ImGui::SliderFloat("Min H", &m_TresholdsMin.x, 0, 255);
        // ImGui::SliderFloat("Max H", &m_TresholdsMax.x, 0, 255);
        // ImGui::SliderFloat("Min S", &m_TresholdsMin.y, 0, 255);
        // ImGui::SliderFloat("Max S", &m_TresholdsMax.y, 0, 255);
        // ImGui::SliderFloat("Min V", &m_TresholdsMin.z, 0, 255);
        // ImGui::SliderFloat("Max V", &m_TresholdsMax.z, 0, 255);



        if(m_StartFinish.StartFlagPlaced){
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.StartFlagPos.x , m_StartFinish.StartFlagPos.y), 8,
                                       IM_COL32(255, 255, 255, 255), 10);
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.StartFlagPos.x , m_StartFinish.StartFlagPos.y ), 5,
                                       IM_COL32(255, 0, 0, 255), 10);
        }
        if(m_StartFinish.FinishFlagPlaced){
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.FinishFlagPos.x, m_StartFinish.FinishFlagPos.y ), 8,
                                       IM_COL32(255, 255, 255, 255), 10);
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.FinishFlagPos.x , m_StartFinish.FinishFlagPos.y ), 5,
                                       IM_COL32(0, 0, 255, 255), 10);
        }



        if(is_hovered) {
            //calculate coordinates of mouse in grid
            int x = (int) (mouse_pos_in_canvas.x / m_DeltaDashLineX);
            int y = (int) (mouse_pos_in_canvas.y / m_DeltaDashLineY);
            sprintf(buffer, "Grid Position: (%d, %d)", x, y);
            draw_list->AddText(ImVec2(canvas_left_top.x + 10, canvas_left_top.y + 30), IM_COL32(255, 255, 255, 255), buffer);

            //if is hover and left mouse button is pressed
            if (is_hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                m_MousePos = glm::vec2(mouse_pos_in_canvas.x + origin.x, mouse_pos_in_canvas.y + origin.y);
                m_StartFinish.StartFlagPlaced = true;
                m_StartFinish.StartFlagPos = m_MousePos;
                m_StartFinish.GridStartFlagPos = glm::vec2(x, y);
            }
            if (is_hovered && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                m_MousePos = glm::vec2(mouse_pos_in_canvas.x + origin.x, mouse_pos_in_canvas.y + origin.y);
                m_StartFinish.FinishFlagPlaced = true;
                m_StartFinish.FinishFlagPos = m_MousePos;
                m_StartFinish.GridFinishFlagPos = glm::vec2(x, y);
            }
            //left mouse button is pressed reset start and finish flag
            if (is_hovered  && ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
                m_StartFinish.StartFlagPlaced = false;
                m_StartFinish.FinishFlagPlaced = false;
            }
        }





        draw_list->AddRect(canvas_left_top, canvas_bottom_right, IM_COL32(255, 255, 255, 255));
        draw_list->PushClipRect(canvas_left_top, canvas_bottom_right, true);
        draw_list->PopClipRect();







        ImGui::End();
    }




    void DrawMap::GenerateTextures() {

        //open image
        m_ImgBackground = cv::imread("ASSETS/track.png");
        if (m_ImgBackground.empty()) {
            ATLOG_CRITICAL("Error to open image")
        }
        m_ImgColorBackground = cv::imread("ASSETS/colortrack.png");
        if (m_ImgColorBackground.empty()) {
            ATLOG_CRITICAL("Error to open image")
        }
        cv::resize(m_ImgBackground, m_ImgBackground, cv::Size(m_RoadWidth, m_RoadHeight));
        cv::resize(m_ImgColorBackground, m_ImgColorBackground, cv::Size(m_RoadWidth, m_RoadHeight));
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);


    }
}

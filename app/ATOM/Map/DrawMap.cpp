//
// Created by toor on 1/19/24.
//

#include "DrawMap.h"
#include <set>
#include <unordered_map>
#include <algorithm>

#include "ATOM/Application.h"


struct Node {
    int x, y;

    bool operator<(const Node& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }

    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }
};


namespace std {
    template <>
    struct hash<Node> {
        std::size_t operator()(const Node& node) const {
            return hash<int>()(node.x) ^ hash<int>()(node.y);
        }
    };
}

int heuristic(const Node& a, const Node& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

//A* algorithm || Vitek
std::vector<Node> astar(const Node& start, const Node& goal, const std::vector<std::vector<int>>& grid) {
    std::set<Node> open_set;
    std::unordered_map<Node, Node> came_from;
    std::unordered_map<Node, int> g_score;

    auto f_score = [&](const Node& node) {
        return g_score[node] + heuristic(node, goal);
    };

    open_set.insert(start);
    g_score[start] = 0;

    while (!open_set.empty()) {
        auto current = *min_element(open_set.begin(), open_set.end(),
                                    [&](const Node& a, const Node& b) {
                                        return f_score(a) < f_score(b);
                                    });

        if (current == goal) {
            std::vector<Node> path;
            while (came_from.find(current) != came_from.end()) {
                path.push_back(current);
                current = came_from[current];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        open_set.erase(current);

        const std::vector<std::pair<int, int>> neighbors = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& delta : neighbors) {
            Node neighbor = {current.x + delta.first, current.y + delta.second};

            if (neighbor.x < 0 || neighbor.x >= grid.size() || neighbor.y < 0 || neighbor.y >= grid[0].size() || grid[neighbor.x][neighbor.y] == 1) {
                continue;
            }

            int tentative_g_score = g_score[current] + 1;

            if (g_score.find(neighbor) == g_score.end() || tentative_g_score < g_score[neighbor]) {
                came_from[neighbor] = current;
                g_score[neighbor] = tentative_g_score;
                open_set.insert(neighbor);
            }
        }
    }

    return {};
}


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

        //main road tresold blue color
        ErodeValueChanges();

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


    }

    void DrawMap::OnFixedUpdate() {

        if(m_ErodeValue != m_LastErodeValue)
        {
            m_LastErodeValue = m_ErodeValue;
            ATLOG_TRACE("Erode Value Changed to {0}", m_ErodeValue);
            ErodeValueChanges();
        }

        if(!m_ImgColorBackground.empty()){


            if (m_MapSetings.isChanged) {
                m_MapSetings.isChanged = false;
                MatToTexture(m_ImgColorBackground, textureID);

                switch (m_MapSetings.background) {
                    case MapBackground::Track:
                        MatToTexture(m_ImgBackground, textureID);
                    break;
                    case MapBackground::ColorTrack:
                        MatToTexture(m_ImgColorBackground, textureID);
                    break;
                    case MapBackground::MainRoad:
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
            }
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
        draw_list->AddImage((void *) textureID, canvas_left_top, canvas_bottom_right );





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





            if(m_MapSetings.showPoints) {
                // draw points from matrix that are part of main road
                for (int i = 0; i < m_LinesX; ++i) {
                    for (int j = 0; j < m_LinesY; ++j) {
                        if (m_Matrix[i][j].type == ElementType::MainRoad) {
                            draw_list->AddCircleFilled(ImVec2(canvas_left_top.x + i * m_DeltaDashLineX ,
                                                              canvas_left_top.y + j * m_DeltaDashLineY ),
                                                      2, IM_COL32(0, 255, 0, 255), 5);
                        }
                    }
                }
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
            // StartFlagPos
            // draw_list->AddCircleFilled(ImVec2(m_StartFinish.StartFlagPos.x , m_StartFinish.StartFlagPos.y), 8,
            //                            IM_COL32(255, 255, 255, 255), 10);
            // draw_list->AddCircleFilled(ImVec2(m_StartFinish.StartFlagPos.x , m_StartFinish.StartFlagPos.y ), 5,
            //                            IM_COL32(255, 0, 0, 255), 10);

            //GridStartFlagPos
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.GridStartFlagPos.x * m_DeltaDashLineX + canvas_left_top.x,
                                              m_StartFinish.GridStartFlagPos.y * m_DeltaDashLineY + canvas_left_top.y), 8,
                                       IM_COL32(255, 255, 255, 255), 10);
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.GridStartFlagPos.x * m_DeltaDashLineX + canvas_left_top.x,
                                              m_StartFinish.GridStartFlagPos.y * m_DeltaDashLineY + canvas_left_top.y), 5,
                                       IM_COL32(255, 0, 0, 255), 10);

        }
        if(m_StartFinish.FinishFlagPlaced){
            //
            // draw_list->AddCircleFilled(ImVec2(m_StartFinish.FinishFlagPos.x, m_StartFinish.FinishFlagPos.y ), 8,
            //                            IM_COL32(255, 255, 255, 255), 10);
            // draw_list->AddCircleFilled(ImVec2(m_StartFinish.FinishFlagPos.x , m_StartFinish.FinishFlagPos.y ), 5,
            //                            IM_COL32(0, 0, 255, 255), 10);

            //GridFinishFlagPos
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.GridFinishFlagPos.x * m_DeltaDashLineX + canvas_left_top.x,
                                              m_StartFinish.GridFinishFlagPos.y * m_DeltaDashLineY + canvas_left_top.y), 8,
                                       IM_COL32(255, 255, 255, 255), 10);
            draw_list->AddCircleFilled(ImVec2(m_StartFinish.GridFinishFlagPos.x * m_DeltaDashLineX + canvas_left_top.x,
                                              m_StartFinish.GridFinishFlagPos.y * m_DeltaDashLineY + canvas_left_top.y), 5,
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


        //NEW SHIT #Vitek was here =)
        if (is_hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {

            if (m_StartFinish.StartFlagPlaced && m_StartFinish.FinishFlagPlaced) {

                ImVec2 start_pos = ImVec2(m_StartFinish.GridStartFlagPos.x * m_DeltaDashLineX + canvas_left_top.x,
                                          m_StartFinish.GridStartFlagPos.y * m_DeltaDashLineY + canvas_left_top.y);
                ImVec2 finish_pos = ImVec2(m_StartFinish.GridFinishFlagPos.x * m_DeltaDashLineX + canvas_left_top.x,
                                           m_StartFinish.GridFinishFlagPos.y * m_DeltaDashLineY + canvas_left_top.y);


                draw_list->AddLine(start_pos, finish_pos, IM_COL32(255, 255, 255, 255));
            }
        }




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
        m_BlueFilter = cv::imread("ASSETS/MainRoad.png");
        if (m_BlueFilter.empty()) {
            ATLOG_CRITICAL("Error to open image")
        }
        cv::resize(m_ImgBackground, m_ImgBackground, cv::Size(m_RoadWidth, m_RoadHeight));
        cv::resize(m_ImgColorBackground, m_ImgColorBackground, cv::Size(m_RoadWidth, m_RoadHeight));
        cv::resize(m_BlueFilter, m_BlueFilter, cv::Size(m_RoadWidth, m_RoadHeight));

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);


    }

    void DrawMap::ErodeValueChanges() {
        cv::cvtColor(m_BlueFilter, m_ImgHSV, cv::COLOR_BGR2HSV);

        cv::inRange(m_ImgHSV, cv::Scalar(119.0,120.0,255.0), cv::Scalar(255.0,255.0,255.0), m_MainRoad);
        cv::erode(m_MainRoad, m_MainRoad, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(m_ErodeValue, m_ErodeValue)));




        //side road tresold blue color
        cv::inRange(m_ImgHSV, cv::Scalar(0, 200, 100), cv::Scalar(10, 255, 255), m_SideRoad);
        m_MapSetings.isChanged = true;
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

    }
}

//
// Created by toor on 1/19/24.
//

#include "DrawMap.h"

#include "ATOM/Application.h"


namespace Atom {
    DrawMap::DrawMap()
        : Layer("DrawMap") {


        m_Window = (SDL_Window *) Application::GetApp().GetWindow().GetNativeWindow();
        m_Renderer = (SDL_Renderer *) Application::GetApp().GetWindow().GetRederer();

        //open image
        m_ImgBackground = cv::imread("ASSETS/track.png");
        if(m_ImgBackground.empty()){
            ATLOG_CRITICAL("Error to open image")
        }
        m_ImgColorBackground= cv::imread("ASSETS/colortrack.png");
        if(m_ImgColorBackground.empty()){
            ATLOG_CRITICAL("Error to open image")
        }


        //flip both images horizontaly
        cv::flip(m_ImgBackground, m_ImgBackground, 0);
        cv::flip(m_ImgColorBackground, m_ImgColorBackground, 0);

        m_Width = m_ImgBackground.cols;
        m_Height = m_ImgBackground.rows;
        m_AspectRatio = (float)m_Width / (float)m_Height;

        m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_TARGET, m_Width ,
                                      m_Height);

        //copy image to texture
        SDL_UpdateTexture(m_Texture, nullptr, m_ImgBackground.data, m_ImgBackground.cols * 3);


        m_DeltaDashLineX = (int)std::round(m_Width / m_LinesX);
        m_DeltaDashLineY = (int)std::round(m_Height / m_LinesY);

        m_Matrix = new MatrixElement *[m_LinesX];
        for (int i = 0; i < m_LinesX; ++i) {
            m_Matrix[i] = new MatrixElement[m_LinesY];
        }


    }

    DrawMap::~DrawMap() {
    }

    void DrawMap::OnAttach() {
    }

    void DrawMap::OnDetach() {
    }

    void DrawMap::OnUpdate() {
        SDL_SetRenderTarget(m_Renderer, m_Texture);
        // m_ImgColorBackground.copyTo(m_OutImg);
        // //
        // //  switch (m_MapSetings.background) {
        // //      case MapBackground::None:
        // //          SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
        // //          SDL_RenderClear(m_Renderer);
        // //          break;
        // //      case MapBackground::Track:
        // SDL_UpdateTexture(m_Texture, nullptr, m_ImgBackground.data, m_ImgBackground.cols * 3);
        // //          break;
        // //      case MapBackground::ColorTrack:
        // //          SDL_UpdateTexture(m_Texture, nullptr, m_OutImg.data, m_OutImg.cols * 3);
        // //          break;
        // //      case MapBackground::Intersection:
        // //         cv::cvtColor(m_OutImg, imgHSV, cv::COLOR_BGR2HSV);
        // //         // cv::inRange(imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), imgThresholded);
        // //         // cv::inRange(imgHSV, cv::Scalar(110, 50, 50), cv::Scalar(130, 255, 255), imgThresholded);
        // //         cv::inRange(imgHSV, cv::Scalar(50, 100, 100), cv::Scalar(70, 255, 255), imgThresholded);
        // //         cv::cvtColor(imgThresholded, imgThresholded, cv::COLOR_GRAY2BGR);
        // //         cv::bitwise_and(m_OutImg, imgThresholded, m_OutImg);
        // //         SDL_UpdateTexture(m_Texture, nullptr, m_OutImg.data, m_OutImg.cols * 3);
        // //         break;
        // //      case MapBackground::MainRoad:
        // //          cv::cvtColor(m_OutImg, imgHSV, cv::COLOR_BGR2HSV);
        // //          // cv::inRange(imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), imgThresholded);
        // //          cv::inRange(imgHSV, cv::Scalar(110, 50, 50), cv::Scalar(130, 255, 255), imgThresholded);
        // //          cv::cvtColor(imgThresholded, imgThresholded, cv::COLOR_GRAY2BGR);
        // //          cv::bitwise_and(m_OutImg, imgThresholded, m_OutImg);
        // //          SDL_UpdateTexture(m_Texture, nullptr, m_OutImg.data, m_OutImg.cols * 3);
        // //          break;
        // //      case MapBackground::SideRoad:
        // //          cv::cvtColor(m_OutImg, imgHSV, cv::COLOR_BGR2HSV);
        // //          cv::inRange(imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), imgThresholded);
        // //          cv::cvtColor(imgThresholded, imgThresholded, cv::COLOR_GRAY2BGR);
        // //          cv::bitwise_and(m_OutImg, imgThresholded, m_OutImg);
        // //          SDL_UpdateTexture(m_Texture, nullptr, m_OutImg.data, m_OutImg.cols * 3);
        // //          break;
        // //      case MapBackground::Parking:
        // //         cv::cvtColor(m_OutImg, imgHSV, cv::COLOR_BGR2HSV);
        // //         //treshhold for purple
        // //         cv::inRange(imgHSV, cv::Scalar(130, 100, 100), cv::Scalar(160, 255, 255), imgThresholded);
        // //         cv::cvtColor(imgThresholded, imgThresholded, cv::COLOR_GRAY2BGR);
        // //         cv::bitwise_and(m_OutImg, imgThresholded, m_OutImg);
        // //         SDL_UpdateTexture(m_Texture, nullptr, m_OutImg.data, m_OutImg.cols * 3);
        // //         break;
        // //      case MapBackground::PedestrianCrossing:
        // //          cv::cvtColor(m_OutImg, imgHSV, cv::COLOR_BGR2HSV);
        // //          //treshhold for yellow
        // //             cv::inRange(imgHSV, cv::Scalar(25, 230, 230), cv::Scalar(35, 255, 255), imgThresholded);
        // //          cv::cvtColor(imgThresholded, imgThresholded, cv::COLOR_GRAY2BGR);
        // //          cv::bitwise_and(m_OutImg, imgThresholded, m_OutImg);
        // //          SDL_UpdateTexture(m_Texture, nullptr, m_OutImg.data, m_OutImg.cols * 3);
        // //          break;
        // //
        // //  }
        //
        //
        //
        // //draw dashed line , horizontal
        //
        // for(int i = 0;i < m_Height;i+=m_DeltaDashLineY) {
        //     DrawDashLine(0,i , m_Width,i, 5, 2, glm::vec4(0,25,25,255));
        //
        // }
        // //draw dashed line , vertical
        // for(int i = 0;i < m_Width;i+=m_DeltaDashLineX) {
        //     DrawDashLine(i,0 , i,m_Height, 5, 2 , glm::vec4(0,25,25,255));
        //
        // }
        //
        // // draw circle on mause position
        DrawFilledCircle((int)m_MousePos.x,  m_Height - (int)m_MousePos.y, 10, glm::vec4(0, 255, 255, 255));
        //
        //
        // SDL_RenderTexture(m_Renderer, m_Texture, nullptr, nullptr);
        //
        //  //free target
        SDL_SetRenderTarget(m_Renderer, NULL);

    }

    void DrawMap::OnImGuiRender() {
        ImGui::Begin("DrawMap");

        //get mause position inside current imgui window
        ImVec2 mousePos = ImGui::GetMousePos();
        mousePos.x -= ImGui::GetCursorScreenPos().x;
        mousePos.y -= ImGui::GetCursorScreenPos().y;
        m_MousePos = glm::vec2(mousePos.x, mousePos.y);
        ImGui::Image((void *) m_Texture, ImVec2(m_Width, m_Height), ImVec2(0, 1), ImVec2(1, 0));


        ImGui::End();
    }

    void DrawMap::DrawDashLine(int x1, int y1, int x2, int y2, int dash, int gap, glm::vec4 color) {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        int len = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        for (int i = 0; i < len; i += dash + gap) {
            xa = x1 + (double) ((x2 - x1) * i) / len;
            ya = y1 + (double) ((y2 - y1) * i) / len;
            xb = x1 + (double) ((x2 - x1) * (i + dash)) / len;
            yb = y1 + (double) ((y2 - y1) * (i + dash)) / len;
            SDL_RenderLine(m_Renderer, (int) xa, (int) ya, (int) xb, (int) yb);
        }
    }

    void DrawMap::DrawFilledCircle(int centerX, int centerY, int radius, glm::vec4 color) {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);

        //draw circle
        for (int x = -radius; x <= radius; x++) {
            int height = static_cast<int>(sqrt(radius * radius - x * x) + 0.5);
            SDL_RenderLine(m_Renderer, centerX + x, centerY - height, centerX + x, centerY + height);
        }

    }
}
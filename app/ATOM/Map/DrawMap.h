//
// Created by toor on 1/19/24.
//

#ifndef DRAWMAP_H
#define DRAWMAP_H
#include <opencv2/core/mat.hpp>

#include "ATOM/atompch.h"


namespace Atom {
    enum class MapBackground {
        None = 0,
        Track,
        ColorTrack,
        Intersection,
        MainRoad,
        SideRoad,
        Parking,
        PedestrianCrossing,

    };
    typedef struct MapSetings {
        MapBackground background = MapBackground::Track;
    } MapSetings;



    enum class ElementType {
        Empty = 0,
        Wall ,
        Start,
        End ,
        Path ,
        Visited ,
        Current ,
        Open ,
        Closed ,
        ShortestPath ,
        Car ,
        Sign
    };
    typedef struct MatrixElement {
        ElementType type;

    } MatrixElement;


    class DrawMap :public Layer {
    public:
        DrawMap();
        ~DrawMap();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;

        MapSetings* GetMapSetings() { return &m_MapSetings; }


        void DrawDashLine(int x1, int y1, int x2, int y2, int dash, int gap , glm::vec4 color = glm::vec4(0, 255, 255, 255));
        void DrawFilledCircle(int x0, int y0, int radius, glm::vec4 color = glm::vec4(0, 255, 255, 255));


    private:
        bool IsRunning = true;
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        SDL_Texture* m_Texture;
        int m_Width = 640;
        int m_Height = 480;
        int m_LastWidth = 640;
        int m_LastHeight = 480;
        float m_AspectRatio = 1.0f;
        bool m_HasResized = false;


        MapSetings m_MapSetings;







        // matrix 400x400
        MatrixElement** m_Matrix;




        //for dash line
        int len;
        double xa = 0;
        double ya = 0;
        double xb = 0;
        double yb = 0;
        int m_LinesX = 200;
        int m_LinesY = 200;
        int m_DeltaDashLineX = 10;
        int m_DeltaDashLineY = 10;

        cv::Mat m_ImgBackground;
        cv::Mat m_ImgColorBackground;
        cv::Mat imgHSV;
        cv::Mat imgThresholded;
        cv::Mat m_OutImg;

        glm::vec2 m_MousePos;
    };

}


#endif //DRAWMAP_H

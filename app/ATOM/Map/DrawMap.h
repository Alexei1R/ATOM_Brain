//
// Created by toor on 1/19/24.
//

#ifndef DRAWMAP_H
#define DRAWMAP_H
#include <opencv2/core/mat.hpp>

#include "ATOM/atompch.h"
#include "ATOM/AI/TrainEngine.h"

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
        bool showPoints = false;
        bool isChanged = true;
        float m_CameraCurvature = 1.15f;
        float m_Fov = 90.0f;
    } MapSetings;


    enum class ElementType {
        Empty = 0,
        MainRoad,
        SideRoad,
        Visited,
        ShortestPath,
        Car,
        Sign
    };


    typedef struct MatrixElement {
        ElementType type;
    } MatrixElement;


    typedef struct StartFinish {
        bool StartFlagPlaced = false;
        glm::vec2 StartFlagPos;
        glm::vec2 GridStartFlagPos;
        bool FinishFlagPlaced = false;
        glm::vec2 FinishFlagPos;
        glm::vec2 GridFinishFlagPos;
    } StartFinish;

    class DrawMap : public Layer {
    public:
        DrawMap(TrainEngine *trainEngine);

        ~DrawMap();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        MapSetings *GetMapSetings() { return &m_MapSetings; }

        void SetLidarData(std::vector<std::pair<float, float>> &data) { m_LidarData = data; }
    private:
        void GenerateTextures();

    private:
        TrainEngine *m_TrainEngine;

        bool IsRunning = true;
        GLFWwindow *m_Window;
        int m_FrameWidth = 640;
        int m_FrameHeight = 480;
        float m_AspectRatio = 1.0f;


        MapSetings m_MapSetings;
        StartFinish m_StartFinish;
        char buffer[100];


        cv::Mat m_ImgHSV;
        cv::Mat imgThresholded;
        GLuint textureID;

        glm::vec2 m_CarPos;


        //Lidar data
        std::vector<std::pair<float, float>> m_LidarData;
        float m_LidarScalingFactor = 0.5f;

        //pixel to angle ratio
        float m_PixelToAngle = 1.0f;

        //Signs
        std::vector<Sign> m_Signs;

        // matrix 400x400
        MatrixElement **m_Matrix;

        int m_RoadWidth = 1920;
        int m_RoadHeight = 1080;
        int m_LinesX = 200;
        int m_LinesY = 200;
        float m_DeltaDashLineX = 1;
        float m_DeltaDashLineY = 1;

        cv::Mat m_ImgBackground;
        cv::Mat m_ImgColorBackground;

        cv::Mat m_MainRoad;
        cv::Mat m_SideRoad;

        glm::vec2 m_MousePos;
        glm::vec3 m_TresholdsMin;
        glm::vec3 m_TresholdsMax;
    };
}


#endif //DRAWMAP_H

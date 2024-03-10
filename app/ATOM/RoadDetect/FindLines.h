//
// Created by toor on 2/7/24.
//

#ifndef FINDLINES_H
#define FINDLINES_H

#include "ATOM/atompch.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


namespace Atom {
    struct LineSettings {
        int offsetSides = 80;
        int topOffset = 220;
        int bottomOffset = 480;

        cv::Scalar lowerWhite;
        cv::Scalar upperWhite = cv::Scalar(255, 255, 255);

        bool showDebug = true;
    };

    class FindLines {
    public:
        FindLines(cv::Mat &inputFrame);

        ~FindLines();

        void Update();

        cv::Mat &GetPreprocessed();

        [[nodiscard]] float GetOffsetCenter() const { return laneCenterOffset; }
        LineSettings &GetLineSettings() { return lineSettings; }
        glm::vec<2 , int> &GetRectSize() { return RectSize; }

    private:
        void PreprocessLine(cv::Mat &original);

    private:
        // glm::vec2 RectSize = glm::vec2(60, 30);
        //int vec2 rectsize;
        glm::vec<2 , int> RectSize = glm::vec2(60, 30);

        cv::Mat &frame;
        cv::Mat original;

        glm::vec2 frameSize;
        cv::Mat preprocessed;
        float laneCenterOffset = 0.0f;

        // Birds eye view
        cv::Point2f srcVertices[4];
        cv::Point2f dstVertices[4];
        cv::Mat perspectiveMatrix;
        cv::Mat dst;
        cv::Mat invertedPerspectiveMatrix;

        // Mask
        cv::Mat whiteMask;

        //processed
        cv::Mat processed;
        cv::Mat img; // Working image
        cv::Mat out;
        cv::Mat overlay;

        LineSettings lineSettings;
        std::vector<cv::Point2f> leftLanePts;
        std::vector<cv::Point2f> rightLanePts;
        int lastOffset = 80;
    };
}


#endif //FINDLINES_H

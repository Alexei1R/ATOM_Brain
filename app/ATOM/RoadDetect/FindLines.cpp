//
// Created by toor on 2/7/24.
//

#include "FindLines.h"


namespace Atom {
    std::vector<cv::Point2f> slidingWindow(cv::Mat image, cv::Rect window);

    float calculateOffset(const std::vector<cv::Point2f> &points, const cv::Size &imageSize);


    FindLines::FindLines(cv::Mat &inputFrame) : frame(inputFrame) {
        frameSize = glm::vec2(frame.cols, frame.rows);

        lineSettings = LineSettings();

        srcVertices[0] = cv::Point(lineSettings.offsetSides, lineSettings.topOffset);
        srcVertices[1] = cv::Point(640 - lineSettings.offsetSides, lineSettings.topOffset);
        srcVertices[2] = cv::Point(640, lineSettings.bottomOffset);
        srcVertices[3] = cv::Point(0, lineSettings.bottomOffset);

        dstVertices[0] = cv::Point(0, 0);
        dstVertices[1] = cv::Point(640, 0);
        dstVertices[2] = cv::Point(640, lineSettings.bottomOffset);
        dstVertices[3] = cv::Point(0, lineSettings.bottomOffset);

        perspectiveMatrix = cv::getPerspectiveTransform(srcVertices, dstVertices);
        dst = cv::Mat(480, 640, CV_8UC3);

        cv::invert(perspectiveMatrix, invertedPerspectiveMatrix);
    }

    FindLines::~FindLines() {
    }

    void FindLines::Update() {
        PreprocessLine(frame);
    }

    cv::Mat &FindLines::GetPreprocessed() {
        if(lastOffset != lineSettings.offsetSides) {
            srcVertices[0] = cv::Point(lineSettings.offsetSides, lineSettings.topOffset);
            srcVertices[1] = cv::Point(640 - lineSettings.offsetSides, lineSettings.topOffset);
            srcVertices[2] = cv::Point(640, lineSettings.bottomOffset);
            srcVertices[3] = cv::Point(0, lineSettings.bottomOffset);

            dstVertices[0] = cv::Point(0, 0);
            dstVertices[1] = cv::Point(640, 0);
            dstVertices[2] = cv::Point(640, lineSettings.bottomOffset);
            dstVertices[3] = cv::Point(0, lineSettings.bottomOffset);

            perspectiveMatrix = cv::getPerspectiveTransform(srcVertices, dstVertices);
            dst = cv::Mat(480, 640, CV_8UC3);

            cv::invert(perspectiveMatrix, invertedPerspectiveMatrix);
            lastOffset = lineSettings.offsetSides;
            ATLOG_WARN("Offset Update");
        }



        return preprocessed;
    }

    void FindLines::PreprocessLine(cv::Mat &org) {

        org.copyTo(original);

        cv::warpPerspective(original, dst, perspectiveMatrix, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

        cv::cvtColor(dst, img, cv::COLOR_RGB2GRAY);

        // whiteMask
        cv::inRange(img, lineSettings.lowerWhite, lineSettings.upperWhite, whiteMask);
        //copy whiteMask to processed
        whiteMask.copyTo(processed);
        //
        const cv::Size kernelSize = cv::Size(9, 9);
        cv::GaussianBlur(processed, processed, kernelSize, 0);

        cv::Mat kernel = cv::Mat::ones(15, 15, CV_8U);
        cv::dilate(processed, processed, kernel);
        cv::erode(processed, processed, kernel);
        cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, kernel);

        const int thresholdVal = 120;
        cv::threshold(processed, processed, thresholdVal, 255, cv::THRESH_BINARY);

        std::vector<cv::Point2f> pts = slidingWindow(processed, cv::Rect(0, 450, 90, 30));
        std::vector<cv::Point> allPts; // Used for the end polygon at the end.

        std::vector<cv::Point2f> outPts;
        cv::perspectiveTransform(pts, outPts, invertedPerspectiveMatrix);
        // Transform points back into original image space
        leftLanePts = outPts;
        for (unsigned int i = 0; i < outPts.size() - 1; ++i) {
            line(original, outPts[i], outPts[i + 1], cv::Scalar(255, 0, 0), 3);
            allPts.push_back(cv::Point(outPts[i].x, outPts[i].y));
        }

        allPts.push_back(cv::Point(outPts[outPts.size() - 1].x, outPts[outPts.size() - 1].y));

        cvtColor(processed, out, cv::COLOR_GRAY2BGR);
        // Conver the processing image to color so that we can visualise the lines

        // for (unsigned int i = 0; i < pts.size() - 1; ++i) // Draw a line on the processed image
        //     line(out, pts[i], pts[i + 1], cv::Scalar(255, 0, 0));

        // Sliding window for the right side
        pts = slidingWindow(processed, cv::Rect(550, 450, 90, 30));
        cv::rectangle(original, cv::Rect(550, 450, 90, 30), cv::Scalar(0, 255, 0), 2);
        perspectiveTransform(pts, outPts, invertedPerspectiveMatrix);
        rightLanePts = outPts;

        for (unsigned int i = 0; i < outPts.size() - 1; ++i) {
            line(original, outPts[i], outPts[i + 1], cv::Scalar(0, 0, 255), 3);
            allPts.push_back(cv::Point(outPts[outPts.size() - i - 1].x, outPts[outPts.size() - i - 1].y));
        }

        allPts.push_back(cv::Point(outPts[0].x - (outPts.size() - 1), outPts[0].y));

        // for (unsigned int i = 0; i < pts.size() - 1; ++i)
        //     line(out, pts[i], pts[i + 1], cv::Scalar(0, 0, 255));

        std::vector<std::vector<cv::Point> > arr;
        arr.push_back(allPts);
        overlay = cv::Mat::zeros(original.size(), original.type());
        cv::fillPoly(overlay, arr, cv::Scalar(0, 255, 100));
        cv::addWeighted(original, 1, overlay, 0.5, 0, original); // Overlay it

        cv::circle(original, srcVertices[0], 10, cv::Scalar(25, 0, 255), cv::FILLED);
        cv::circle(original, srcVertices[1], 10, cv::Scalar(25, 0, 255), cv::FILLED);
        cv::circle(original, srcVertices[2], 10, cv::Scalar(25, 0, 255), cv::FILLED);
        cv::circle(original, srcVertices[3], 10, cv::Scalar(25, 0, 255), cv::FILLED);

        float leftOffset = calculateOffset(leftLanePts, frame.size());
        float rightOffset = calculateOffset(rightLanePts, frame.size());

        // You can then calculate the overall lane center offset by taking the average of the left and right offsets.
        laneCenterOffset = (leftOffset + rightOffset) / 2.0f;
        //print lane center offset
        // cv::putText(original, "Lane center offset: " + std::to_string(laneCenterOffset), cv::Point(10, 30),
        //             cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        //

        // std::cout << "Lane center offset: " << laneCenterOffset << std::endl;


        if (lineSettings.showDebug) {
            original.copyTo(preprocessed);
        }else {
            // show whiteMask
            cv::cvtColor(whiteMask, preprocessed, cv::COLOR_GRAY2BGR);
            cv::bitwise_and(original, preprocessed, preprocessed);
        }
    }


    //helper funcions
    std::vector<cv::Point2f> slidingWindow(cv::Mat image, cv::Rect window) {
        std::vector<cv::Point2f> points;
        const cv::Size imgSize = image.size();
        bool shouldBreak = false;

        while (true) {
            float currentX = window.x + window.width * 0.5f;

            cv::Mat roi = image(window); // Extract region of interest
            std::vector<cv::Point2f> locations;

            findNonZero(roi, locations); // Get all non-black pixels. All are white in our case

            float avgX = 0.0f;

            for (unsigned int i = 0; i < locations.size(); ++i) // Calculate average X position
            {
                float x = locations[i].x;
                avgX += window.x + x;
            }

            avgX = locations.empty() ? currentX : avgX / locations.size();

            cv::Point point(avgX, window.y + window.height * 0.1f);
            points.push_back(point);

            // Move the window up
            window.y -= window.height;

            // For the uppermost position
            if (window.y < 0) {
                window.y = 0;
                shouldBreak = true;
            }

            // Move x position
            window.x += (point.x - currentX);

            // Make sure the window doesn't overflow, we get an error if we try to get data outside the matrix
            if (window.x < 0)
                window.x = 0;
            if (window.x + window.width >= imgSize.width)
                window.x = imgSize.width - window.width - 1;

            if (shouldBreak)
                break;
        }

        return points;
    }

    float calculateOffset(const std::vector<cv::Point2f> &points, const cv::Size &imageSize) {
        float centerX = static_cast<float>(imageSize.width) / 2.0f;
        float sumOffset = 0.0f;

        for (const cv::Point2f &point: points) {
            float pointX = point.x;
            float offset = pointX - centerX;
            sumOffset += offset;
        }

        float averageOffset = sumOffset / points.size();
        return averageOffset;
    }
}

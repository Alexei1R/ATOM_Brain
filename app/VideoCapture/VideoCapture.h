//
// Created by toor on 11/25/23.
//

#ifndef ATOM_VIDEOCAPTURE_H
#define ATOM_VIDEOCAPTURE_H
#include "ATOM/atompch.h"
#include <opencv2/opencv.hpp>


namespace Atom {
    class VideoCapture {
    public:
        using FrameReceivedCallback = std::function<void(cv::Mat& frame)>;

    public:
        VideoCapture();
        ~VideoCapture();
        void Open( const std::string& pipeline);
        void Close() ;
        [[nodiscard]] bool isOpened() const { return m_IsOpen; };


        void SetFrameRecivedCallback(const FrameReceivedCallback& function) { m_FrameReceivedCallback = function;};

        cv::Mat& GetFrame() { return m_Frame; };


    private:
        void FrameThreadFunc(const std::string &pipeline);

    private:
        std::thread m_FrameThread;
        cv::VideoCapture m_Capture;
        cv::Mat m_Frame;
        bool m_IsOpen = false;
        FrameReceivedCallback m_FrameReceivedCallback;

        std::mutex m_FrameMutex;


    };

}

#endif //ATOM_VIDEOCAPTURE_H

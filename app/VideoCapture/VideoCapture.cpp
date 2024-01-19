//
// Created by toor on 11/25/23.
//

#include "VideoCapture.h"

Atom::VideoCapture::VideoCapture() {

}

Atom::VideoCapture::~VideoCapture() {
    m_IsOpen = false;
    if(m_FrameThread.joinable()){
        m_FrameThread.join();
    }
}

void Atom::VideoCapture::Open(const std::string &pipeline) {
    if(m_IsOpen){
        return;
    }
    m_FrameThread = std::thread([&, pipeline]() { FrameThreadFunc(pipeline); });

}

void Atom::VideoCapture::FrameThreadFunc(const std::string &pipeline) {

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    ATLOG_INFO("Camera is opening...");
    ATLOG_INFO("Pipeline: {0}", pipeline);
    m_Capture.open(pipeline, cv::CAP_GSTREAMER);
    if(!m_Capture.isOpened()){
        ATLOG_WARN("Error opening the camera");
    }
    ATLOG_INFO("Camera is opened");
    m_IsOpen = true;

    while(m_IsOpen){
        m_Capture >> m_Frame;
        if(m_Frame.empty()){
            ATLOG_WARN("Frame is empty");
            continue;
        }
        if(m_FrameReceivedCallback){
            m_FrameReceivedCallback(m_Frame);
        }
        if(m_IsOpen) {
            ATLOG_INFO("Shutdown Camera")
        }
    }
    ATLOG_INFO("Shutdown Camera")


}

void Atom::VideoCapture::Close() {
    m_IsOpen = false;
}

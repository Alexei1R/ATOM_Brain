
//
// Created by toor on 2/4/24.
//

#include "DetectLines.h"

#include "ATOM/Application.h"


namespace Atom {
    

    DetectLines::DetectLines()
        : Layer("DetectLines") {

        m_Frame = &Application::GetApp().GetFrame().GetNativeFrame();
    }

    DetectLines::~DetectLines() {
    }

    void DetectLines::OnAttach() {
    }

    void DetectLines::OnDetach() {
    }

    void DetectLines::OnUpdate() {
        if(!m_Frame->empty() ) {
            ATLOG_INFO("Frame width: {0}, height: {1}", m_Frame->cols, m_Frame->rows);
        }else{
            ATLOG_INFO("Frame is empty");
        }


    }

    void DetectLines::OnImGuiRender() {
    }


}

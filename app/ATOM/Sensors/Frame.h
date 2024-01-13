//
// Created by toor on 11/20/23.
//

#ifndef ATOM_FRAME_H
#define ATOM_FRAME_H




#include "ATOM/atompch.h"
#include "Networking/Client.h"
#include <opencv2/opencv.hpp>
#include "VideoCapture/VideoCapture.h"


namespace Atom {
    class Frame : public Layer {
    public:
        Frame();

        ~Frame();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;


    private:
        Atom::VideoCapture m_VideoCapture;
        cv::Mat m_Frame;
        // GLuint textureID;/**/
    };


}


#endif //ATOM_FRAME_H

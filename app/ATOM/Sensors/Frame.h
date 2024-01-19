//
// Created by toor on 11/20/23.
//

#ifndef ATOM_FRAME_H
#define ATOM_FRAME_H




#include "ATOM/atompch.h"
#include "Networking/Client.h"
#include <opencv2/opencv.hpp>
#include "VideoCapture/VideoCapture.h"
#include "SDL3/SDL.h"

namespace Atom {
    class Frame : public Layer {
    public:
        Frame();

        ~Frame();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;

        void Shutdown();
    private:
        Atom::VideoCapture m_VideoCapture;
        cv::Mat m_Frame;
        SDL_Renderer *m_Renderer;
        SDL_Texture* m_Texture;
        float m_AspectRatio;
    };


}


#endif //ATOM_FRAME_H

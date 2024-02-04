//
// Created by toor on 2/4/24.
//

#ifndef DETECTLINES_H
#define DETECTLINES_H

#include "ATOM/atompch.h"
#include <opencv2/opencv.hpp>

namespace Atom {

class DetectLines: public Layer {
    public:
        DetectLines();

        ~DetectLines();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;

    private:
        cv::Mat* m_Frame;

    };
}

#endif //DETECTLINES_H

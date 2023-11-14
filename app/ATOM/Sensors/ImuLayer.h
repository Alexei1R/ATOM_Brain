//
// Created by toor on 11/14/23.
//

#ifndef ATOM_IMULAYER_H
#define ATOM_IMULAYER_H

#include "ATOM/atompch.h"

namespace Atom {
    class ImuLayer : public Layer {
    public:
        ImuLayer();

        ~ImuLayer();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;


    private:
    };


}


#endif //ATOM_IMULAYER_H

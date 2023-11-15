//
// Created by toor on 11/15/23.
//

#ifndef ATOM_LIDAR_H
#define ATOM_LIDAR_H


#include "ATOM/atompch.h"

namespace Atom {
    class Lidar : public Layer {
    public:
        Lidar();

        ~Lidar();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;


    private:

    };


}

#endif //ATOM_LIDAR_H

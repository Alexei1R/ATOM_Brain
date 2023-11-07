//
// Created by toor on 11/7/23.
//

#ifndef ATOM_LAYERSTACK_H
#define ATOM_LAYERSTACK_H
#include "Layer.h"
#include <vector>
#include "ATOM/atompch.h"



namespace Atom {
    class LayerStack:public Layer{
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;

    };



}


#endif //ATOM_LAYERSTACK_H

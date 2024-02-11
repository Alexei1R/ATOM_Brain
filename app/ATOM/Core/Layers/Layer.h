//
// Created by toor on 11/7/23.
//

#ifndef ATOM_LAYER_H
#define ATOM_LAYER_H
#include <string>



namespace Atom {
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnFixedUpdate() {}
        virtual void OnImGuiRender() {}
//        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_Name; }
    protected:
        std::string m_Name;
    };

}


#endif //ATOM_LAYER_H

//
// Created by toor on 11/10/23.
//

#include "Transform.h"


namespace Atom{

    Transform::Transform(Atom::Shader& shader)
            :shader(shader)
    {

    }



    void Transform::SendUniform()
    {
        shader.Bind();
        shader.SetUniformMatrix4f("model", glm::value_ptr(model), 1);
        shader.SetUniformMatrix4f("proj", glm::value_ptr(proj), 1);
        shader.SetUniformMatrix4f("view", glm::value_ptr(view), 1);
        shader.SetUniform3f("viewPos", camPos.x, camPos.y, camPos.z);
//        shader.SetUniform3f("viewDir", camFront.x, camFront.y, camFront.z);

    }




    void  Transform::UpdateCam(glm::mat4 viewmat, glm::vec3 camPos, glm::vec3 camFront,float w_height,float w_width)
    {
        this->camPos = camPos;
        this->camFront = camFront;

        proj = glm::perspective(glm::radians(65.0f), (float)w_height / w_width, 0.1f, 1000.0f);
        view = viewmat;

        SendUniform();
    }


    void Transform::rotateRadians(float angle, glm::vec3 vector)
    {
        model = glm::rotate(model,glm::radians(angle), vector);
    }

    void Transform::rotate(float angle, glm::vec3 vector)
    {
        model = glm::rotate(model, angle, vector);
    }

    void Transform::transform(glm::vec3 vector)
    {
        model = glm::translate(model, vector);
    }

    void Transform::scale(glm::vec3 vector)
    {
        model = glm::scale(model, vector);
    }

}
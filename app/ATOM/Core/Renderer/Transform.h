//
// Created by toor on 11/10/23.
//

#ifndef ATOM_TRANSFORM_H
#define ATOM_TRANSFORM_H
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ATOM/atompch.h"
#include "Shader.h"




namespace Atom{

    class Transform
    {
    private:
        Atom::Shader shader;

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 camPos = glm::vec3(0.0f,0.0f,0.0f);
        glm::vec3 camFront = glm::vec3(0.0f, 0.0f, 0.0f);


    public:
        Transform(Atom::Shader& shader);



    private:





    public:
        void SendUniform();

        void UpdateCam(glm::mat4 viewmat, glm::vec3 camPos, glm::vec3 camFront, float w_height, float w_width);
        void rotate(float angle, glm::vec3 vector);
        void rotateRadians(float angle, glm::vec3 vector);
        void transform(glm::vec3 vector);
        void scale(glm::vec3 vector);





    };

}

#endif //ATOM_TRANSFORM_H

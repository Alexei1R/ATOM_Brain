//
// Created by toor on 11/10/23.
//

#ifndef ATOM_SHADER_H
#define ATOM_SHADER_H

#include "ATOM/atompch.h"



namespace Atom
{

    class Shader
    {
    public:
        unsigned int program;
    private:
        std::string vertexData;
        std::string fragmentData;
        std::unordered_map<std::string, int > m_UniformLocationCache;

    public:
        Shader(std::string vertexShader, std::string fragmentShader);
        void Bind();
        void UnBind();
        //   Set Uniform
        void SetUniformMatrix4f(const std::string name, float* value, unsigned int count);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniform3f(const std::string& name, float v0, float v1, float v2);
        void SetUniform1i(const std::string& name, int value);
        void SetUniform1f(const std::string& name, float value);

    private:
        std::string LoadShaderSRC(const char* filepath);
        unsigned int CompileShader(std::string& source, unsigned int type);
        unsigned int CreateShader();
        int GetUniformLocation(const std::string& name);


    };

}



#endif //ATOM_SHADER_H

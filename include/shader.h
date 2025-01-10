#ifndef SHADER
#define SHADER

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string getFile(const char* filename){
    std::ifstream in(filename, std::ios::binary);
    if(in){
        std::string content;
        in.seekg(0, std::ios::end);
        content.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&content[0], content.size());
        in.close();
        return(content);
    }
    throw(errno);
}

class Shader {
    public:
        GLuint _ID;
        Shader(const char* vertex, const char* frag){

            std::string vertexCode = getFile(vertex);
            std::string fragmentCode = getFile(frag);

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vShaderCode, NULL);
            glCompileShader(vertexShader);

            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
            glCompileShader(fragmentShader);

            _ID = glCreateProgram();
            glAttachShader(_ID, vertexShader);
            glAttachShader(_ID, fragmentShader);
            glLinkProgram(_ID);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        void use(){
            glUseProgram(_ID);
        }
        void del(){
            glDeleteProgram(_ID);
        }

};

#endif
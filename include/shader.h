#ifndef SHADER
#define SHADER

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
    //program ID
    unsigned int _ID;

    Shader(const char* p_vertexPath, const char* p_fragmentPath){
        
        //get the sourcecode
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try{
            //open the files
            vShaderFile.open(p_vertexPath);
            fShaderFile.open(p_fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            //read into ss
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            //close files
            vShaderFile.close();
            fShaderFile.close();
            //convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str(); 
        } catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        //vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        //errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success){
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        //errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success){
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //shader program
        _ID = glCreateProgram();
        glAttachShader(_ID, vertex);
        glAttachShader(_ID, fragment);
        glLinkProgram(_ID);
        glGetProgramiv(_ID, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(_ID, 512, NULL, infoLog);
            std::cout << "ERror\n" << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use(){
        glUseProgram(_ID);
    }

    void setBool(const std::string &p_name, bool p_value) const{ glUniform1i(glGetUniformLocation(_ID, p_name.c_str()), (int)p_value); }
    void setInt(const std::string &p_name, int p_value) const{ glUniform1i(glGetUniformLocation(_ID, p_name.c_str()), p_value); }
    void setFloat(const std::string &p_name, float p_value) const{ glUniform1i(glGetUniformLocation(_ID, p_name.c_str()), p_value); }
};

#endif /* SHADER */

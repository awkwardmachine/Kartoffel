//
// Created by nil on 02/03/2026.
//

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

#include "Shader.hpp"

Shader::Shader() : id_(0), loaded_(false) {}

Shader::~Shader() { Delete(); }

bool Shader::LoadFromFile(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string   vertexCode;
    std::string   fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode   = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR: Shader file not successfully read: " << e.what() << std::endl;
        return false;
    }

    unsigned int vertex, fragment;

    vertex = CompileShader(GL_VERTEX_SHADER, vertexCode);
    if (!vertex)
        return false;

    fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);
    if (!fragment)
        return false;

    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    glLinkProgram(id_);

    if (!CheckLinkErrors(id_)) {
        glDeleteProgram(id_);
        id_ = 0;
        return false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    loaded_ = true;
    return true;
}

void Shader::Use() const { glUseProgram(id_); }

void Shader::Delete() {
    if (id_) {
        glDeleteProgram(id_);
        id_     = 0;
        loaded_ = false;
    }
}

unsigned int Shader::CompileShader(const unsigned int type, const std::string &source) {
    const unsigned int shader = glCreateShader(type);
    const char        *src    = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    if (!CheckCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")) {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::CheckCompileErrors(const unsigned int shader, const std::string &type) {
    int  success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "ERROR: Shader compilation error of type: " << type << "\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::CheckLinkErrors(const unsigned int program) {
    int  success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "ERROR: Program linking error\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

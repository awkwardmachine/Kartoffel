//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_SHADER_HPP
#define KARTOFFEL_SHADER_HPP

#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;
    Shader(const Shader &)            = delete;
    Shader &operator=(const Shader &) = delete;

    bool LoadFromFile(const std::string &vertexPath, const std::string &fragmentPath);
    void Use() const;
    void Delete();

    [[nodiscard]] unsigned int GetId()      const { return id_; }
    [[nodiscard]] bool         IsLoaded()   const { return loaded_; }

private:
    unsigned int id_;
    bool         loaded_;

    static unsigned int CompileShader(unsigned int type, const std::string &source);
    static bool         CheckCompileErrors(unsigned int shader, const std::string &type);
    static bool         CheckLinkErrors(unsigned int program);
};

#endif // KARTOFFEL_SHADER_HPP

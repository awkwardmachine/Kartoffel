//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_SCENELOADER_HPP
#define KARTOFFEL_SCENELOADER_HPP

#include <string>

class World;

class SceneLoader {
public:
    static bool Load(const std::string &path, World &world);
    static bool Save(const std::string &path, const World &world);

    SceneLoader()                               = delete;
    SceneLoader(const SceneLoader &)            = delete;
    SceneLoader &operator=(const SceneLoader &) = delete;
};

#endif // KARTOFFEL_SCENELOADER_HPP

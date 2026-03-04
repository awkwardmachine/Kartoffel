//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_MESHCLEANUPSYSTEM_HPP
#define KARTOFFEL_MESHCLEANUPSYSTEM_HPP

#include "ecs/System.hpp"

class World;

class MeshCleanupSystem : public System {
public:
    void Update(World &world, float dt) override {}
    void Shutdown(World &world) override;
};

#endif // KARTOFFEL_MESHCLEANUPSYSTEM_HPP

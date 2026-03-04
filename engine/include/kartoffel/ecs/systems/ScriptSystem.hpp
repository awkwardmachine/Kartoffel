//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_SCRIPTSYSTEM_HPP
#define KARTOFFEL_SCRIPTSYSTEM_HPP

#include "kartoffel/ecs/System.hpp"

class ScriptSystem : public System {
public:
    void Update(World &world, float dt) override;
};

#endif // KARTOFFEL_SCRIPTSYSTEM_HPP

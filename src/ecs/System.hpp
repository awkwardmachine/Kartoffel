//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_SYSTEM_HPP
#define KARTOFFEL_SYSTEM_HPP

class World;

class System {
public:
    virtual ~System() = default;
    virtual void Init(World &world) {}
    virtual void Update(World &world, float dt) = 0;
    virtual void Shutdown(World &world) {}
};

#endif // KARTOFFEL_SYSTEM_HPP

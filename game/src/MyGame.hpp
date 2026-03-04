//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#pragma once

#include <kartoffel/app/Application.hpp>

class MyGame : public Kartoffel::Application {
public:
    MyGame();

    void OnInit() override;
    void OnUpdate(float dt) override;
    void OnShutdown() override;
};

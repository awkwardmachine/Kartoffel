//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "ComponentRegistry.hpp"

#include <ranges>

void ComponentRegistry::EntityDestroyed(const Entity entity) {
    for (const auto &array: arrays_ | std::views::values)
        array->EntityDestroyed(entity);
}

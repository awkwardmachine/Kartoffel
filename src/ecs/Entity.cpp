//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "Entity.hpp"

#include <algorithm>

Entity EntityRegistry::Create() {
    if (!free_list_.empty()) {
        const Entity id = free_list_.back();
        free_list_.pop_back();
        alive_.push_back(id);
        return id;
    }
    const Entity id = next_id_++;
    alive_.push_back(id);
    return id;
}

void EntityRegistry::Destroy(const Entity entity) {
    std::erase(alive_, entity);
    free_list_.push_back(entity);
}

bool EntityRegistry::IsAlive(const Entity entity) const { return std::ranges::find(alive_, entity) != alive_.end(); }

const std::vector<Entity> &EntityRegistry::All() const { return alive_; }

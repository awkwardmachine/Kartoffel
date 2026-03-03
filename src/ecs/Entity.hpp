//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_ENTITY_HPP
#define KARTOFFEL_ENTITY_HPP

#include <cstdint>
#include <vector>

using Entity = uint32_t;

static constexpr Entity NULL_ENTITY  = 0;
static constexpr size_t MAX_ENTITIES = 5000;

class EntityRegistry {
public:
    Entity Create();
    void   Destroy(Entity entity);

    [[nodiscard]] bool                       IsAlive(Entity entity) const;
    [[nodiscard]] const std::vector<Entity> &All() const;

private:
    Entity              next_id_{1};
    std::vector<Entity> alive_{};
    std::vector<Entity> free_list_{};
};

#endif // KARTOFFEL_ENTITY_HPP

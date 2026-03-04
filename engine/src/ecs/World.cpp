//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/ecs/World.hpp"
#include "kartoffel/ecs/Components.hpp"

Entity World::CreateEntity() { return entities_.Create(); }

void World::DestroyEntity(const Entity entity) {
    entities_.Destroy(entity);
    components_.EntityDestroyed(entity);
}

bool World::IsAlive(const Entity entity) const { return entities_.IsAlive(entity); }

const std::vector<Entity> &World::Entities() const { return entities_.All(); }

void World::UpdateSystems(const float dt) {
    for (const auto &system: systems_)
        system->Update(*this, dt);
}

void World::ShutdownSystems() {
    for (const auto &system: systems_)
        system->Shutdown(*this);
}

std::vector<Entity> World::FindByTag(const std::string &tag) const {
    std::vector<Entity> result;
    for (const Entity e : entities_.All()) {
        if (HasComponent<TagComponent>(e) && GetComponent<TagComponent>(e).HasTag(tag))
            result.push_back(e);
    }
    return result;
}

Entity World::FindByName(const std::string &name) const {
    for (const Entity e : entities_.All()) {
        if (HasComponent<TagComponent>(e) && GetComponent<TagComponent>(e).name == name)
            return e;
    }
    return Entity{};
}

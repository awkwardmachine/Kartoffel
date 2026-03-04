//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_WORLD_HPP
#define KARTOFFEL_WORLD_HPP

#include <memory>
#include <string>
#include <vector>

#include "ComponentRegistry.hpp"
#include "Entity.hpp"
#include "System.hpp"

class World {
public:
    // Entity management
    Entity CreateEntity();
    void   DestroyEntity(Entity entity);

    [[nodiscard]] bool                       IsAlive(Entity entity) const;
    [[nodiscard]] const std::vector<Entity> &Entities() const;

    // Component registration
    template<typename T>
    void RegisterComponent() {
        components_.Register<T>();
    }

    // Component operations
    template<typename T>
    void AddComponent(const Entity entity, T component) {
        components_.Add<T>(entity, std::move(component));
    }

    template<typename T>
    void RemoveComponent(const Entity entity) {
        components_.Remove<T>(entity);
    }

    template<typename T>
    T &GetComponent(const Entity entity) {
        return components_.Get<T>(entity);
    }

    template<typename T>
    const T &GetComponent(const Entity entity) const {
        return components_.Get<T>(entity);
    }

    template<typename T>
    [[nodiscard]] bool HasComponent(const Entity entity) const {
        return components_.Has<T>(entity);
    }

    // System registration
    template<typename T, typename... Args>
    T *RegisterSystem(Args &&...args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T   *ptr    = system.get();
        ptr->Init(*this);
        systems_.push_back(std::move(system));
        return ptr;
    }

    void UpdateSystems(float dt);
    void ShutdownSystems();

    // Query
    template<typename... Components>
    std::vector<Entity> Query() const {
        std::vector<Entity> result;
        for (Entity e: entities_.All()) {
            if ((HasComponent<Components>(e) && ...))
                result.push_back(e);
        }
        return result;
    }

    // Tag / name look-up helpers (require TagComponent to be registered)
    [[nodiscard]] std::vector<Entity> FindByTag(const std::string &tag) const;
    [[nodiscard]] Entity              FindByName(const std::string &name) const; // returns invalid entity if not found

private:
    EntityRegistry                       entities_;
    ComponentRegistry                    components_;
    std::vector<std::unique_ptr<System>> systems_;
};

#endif // KARTOFFEL_WORLD_HPP

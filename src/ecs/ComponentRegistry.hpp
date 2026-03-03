//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_COMPONENTREGISTRY_HPP
#define KARTOFFEL_COMPONENTREGISTRY_HPP

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ComponentArray.hpp"

class ComponentRegistry {
public:
    template<typename T>
    void Register();

    template<typename T>
    void Add(Entity entity, T component);

    template<typename T>
    void Remove(Entity entity);

    template<typename T>
    T &Get(Entity entity);

    template<typename T>
    [[nodiscard]] bool Has(Entity entity) const;

    void EntityDestroyed(Entity entity);

private:
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> arrays_;

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetArray();
};

#include "ComponentRegistry.inl"

#endif // KARTOFFEL_COMPONENTREGISTRY_HPP

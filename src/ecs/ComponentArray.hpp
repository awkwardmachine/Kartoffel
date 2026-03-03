//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_COMPONENTARRAY_HPP
#define KARTOFFEL_COMPONENTARRAY_HPP

#include <array>
#include <unordered_map>

#include "Entity.hpp"

class IComponentArray {
public:
    virtual ~IComponentArray()                  = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
    void Insert(Entity entity, T component);
    void Remove(Entity entity);

    T &Get(Entity entity);

    [[nodiscard]] bool Has(Entity entity) const;

    void EntityDestroyed(Entity entity) override;

private:
    std::array<T, MAX_ENTITIES>        components_{};
    std::unordered_map<Entity, size_t> entity_to_index_{};
    std::unordered_map<size_t, Entity> index_to_entity_{};
    size_t                             size_{0};
};

#include "ComponentArray.inl"

#endif // KARTOFFEL_COMPONENTARRAY_HPP

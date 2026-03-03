//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_COMPONENTARRAY_HPP
#define KARTOFFEL_COMPONENTARRAY_HPP

#include <cassert>
#include <unordered_map>
#include <vector>

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

    T       &Get(Entity entity);
    const T &Get(Entity entity) const;

    [[nodiscard]] bool Has(Entity entity) const;

    void EntityDestroyed(Entity entity) override;

private:
    std::vector<T>                     components_{};
    std::unordered_map<Entity, size_t> entity_to_index_{};
    std::unordered_map<size_t, Entity> index_to_entity_{};
    size_t                             size_{0};
};

#include "ComponentArray.inl"

#endif // KARTOFFEL_COMPONENTARRAY_HPP

//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#pragma once

#include <cassert>

template<typename T>
void ComponentArray<T>::Insert(const Entity entity, T component) {
    assert(!entity_to_index_.contains(entity) && "Component added to same entity twice");

    size_t index             = size_++;
    entity_to_index_[entity] = index;
    index_to_entity_[index]  = entity;
    components_[index]       = std::move(component);
}

template<typename T>
void ComponentArray<T>::Remove(const Entity entity) {
    assert(entity_to_index_.contains(entity) && "Removing non-existent component");

    size_t removed_index       = entity_to_index_[entity];
    size_t last_index          = --size_;
    components_[removed_index] = std::move(components_[last_index]);

    const Entity last_entity        = index_to_entity_[last_index];
    entity_to_index_[last_entity]   = removed_index;
    index_to_entity_[removed_index] = last_entity;

    entity_to_index_.erase(entity);
    index_to_entity_.erase(last_index);
}

template<typename T>
T &ComponentArray<T>::Get(const Entity entity) {
    assert(entity_to_index_.contains(entity) && "Component not found for entity");
    return components_[entity_to_index_[entity]];
}

template<typename T>
bool ComponentArray<T>::Has(const Entity entity) const {
    return entity_to_index_.contains(entity);
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity) {
    if (Has(entity))
        Remove(entity);
}

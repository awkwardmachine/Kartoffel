//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#pragma once

#include <cassert>

template<typename T>
void ComponentRegistry::Register() {
    const auto type = std::type_index(typeid(T));
    assert(!arrays_.contains(type) && "Component type already registered");
    arrays_[type] = std::make_shared<ComponentArray<T>>();
}

template<typename T>
void ComponentRegistry::Add(Entity entity, T component) {
    GetArray<T>()->Insert(entity, std::move(component));
}

template<typename T>
void ComponentRegistry::Remove(Entity entity) {
    GetArray<T>()->Remove(entity);
}

template<typename T>
T &ComponentRegistry::Get(Entity entity) {
    return GetArray<T>()->Get(entity);
}

template<typename T>
bool ComponentRegistry::Has(Entity entity) const {
    const auto it = arrays_.find(std::type_index(typeid(T)));
    if (it == arrays_.end())
        return false;
    return std::static_pointer_cast<ComponentArray<T>>(it->second)->Has(entity);
}

template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentRegistry::GetArray() {
    const auto type = std::type_index(typeid(T));
    assert(arrays_.contains(type) && "Component type not registered");
    return std::static_pointer_cast<ComponentArray<T>>(arrays_[type]);
}

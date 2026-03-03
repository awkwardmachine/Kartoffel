//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include <algorithm>
#include <filesystem>
#include <iostream>

#include "ecs/MeshFactory.hpp"

namespace fs = std::filesystem;

int main(const int argc, char *argv[]) {
#ifdef KARTOFFEL_SOURCE_DIR
    const fs::path src_dir = (argc > 1) ? fs::path(argv[1]) : fs::path(KARTOFFEL_SOURCE_DIR) / "models";
    const fs::path dst_dir = (argc > 2) ? fs::path(argv[2]) : fs::path(KARTOFFEL_BUILD_DIR) / "models";
#else
    const fs::path src_dir = (argc > 1) ? fs::path(argv[1]) : fs::path("models");
    const fs::path dst_dir = (argc > 2) ? fs::path(argv[2]) : fs::path("models");
#endif

    if (!fs::exists(src_dir)) {
        std::cerr << "[BakeMeshes] Source directory not found: " << src_dir << "\n";
        return 1;
    }

    std::cout << "[BakeMeshes] Source : " << fs::absolute(src_dir) << "\n";
    std::cout << "[BakeMeshes] Output : " << fs::absolute(dst_dir) << "\n";

    int baked = 0, failed = 0;

    for (const fs::directory_entry &entry: fs::recursive_directory_iterator(src_dir)) {
        if (!entry.is_regular_file())
            continue;

        const std::string ext = [&] {
            std::string e = entry.path().extension().string();
            std::ranges::transform(e, e.begin(), ::tolower);
            return e;
        }();

        if (ext != ".glb" && ext != ".gltf" && ext != ".obj")
            continue;

        const fs::path rel     = fs::relative(entry.path(), src_dir);
        const fs::path out_dir = dst_dir / rel.parent_path();
        const fs::path dst     = out_dir / (entry.path().stem().string() + ".kasset");

        std::error_code ec;
        fs::create_directories(out_dir, ec);

        std::cout << "[BakeMeshes] " << rel << " -> " << dst << " ... ";
        std::cout.flush();

        if (MeshFactory::BakeToKAsset(entry.path().string(), dst.string())) {
            std::cout << "ok\n";
            ++baked;
        } else {
            std::cout << "FAILED\n";
            ++failed;
        }
    }

    std::cout << "[BakeMeshes] " << baked << " baked, " << failed << " failed.\n";
    return failed > 0 ? 1 : 0;
}

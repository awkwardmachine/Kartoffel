##
## Created by Noah Belton (awkwardmachine) on 03/03/2026.
## Copyright (C) 2026 Noah Belton (awkwardmachine)
## SPDX-License-Identifier: GPL-3.0-only
##

add_executable(BakeMeshes bake_main.cpp)

target_compile_definitions(BakeMeshes PRIVATE
    KARTOFFEL_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    KARTOFFEL_BUILD_DIR="$<TARGET_FILE_DIR:BakeMeshes>"
)

target_link_libraries(BakeMeshes PRIVATE KartoffelEngine)

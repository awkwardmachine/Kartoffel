##
## Created by Noah Belton (awkwardmachine) on 03/03/2026.
## Copyright (C) 2026 Noah Belton (awkwardmachine)
## SPDX-License-Identifier: GPL-3.0-only
##

add_executable(KartoffelGame
        src/main.cpp
        src/MyGame.cpp
        src/MyGame.hpp
)

target_link_libraries(KartoffelGame PRIVATE KartoffelEngine)

add_dependencies(KartoffelGame BakeMeshes)

set(GAME_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_custom_command(TARGET KartoffelGame PRE_BUILD
        COMMAND $<TARGET_FILE:BakeMeshes>
        ${GAME_ASSETS_DIR}/models
        $<TARGET_FILE_DIR:KartoffelGame>/assets/models
        COMMENT "Baking mesh assets..."
)

foreach(ASSET_DIR shaders textures scenes)
    add_custom_command(TARGET KartoffelGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${GAME_ASSETS_DIR}/${ASSET_DIR}
            $<TARGET_FILE_DIR:KartoffelGame>/assets/${ASSET_DIR}
            COMMENT "Copying ${ASSET_DIR}/ to output"
    )
endforeach()

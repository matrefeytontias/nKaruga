cmake_minimum_required(VERSION 3.13)

project(nKarugaProject LANGUAGES C CXX)

set(PLATFORM "Windows" CACHE STRING "Platform to build nKaruga for")
set_property(CACHE PLATFORM PROPERTY STRINGS Windows)

set(SOURCES
# Game source - headers
    src/GameSystems.hpp
    
    src/entities/BossEnemy.hpp
    src/entities/Bullet.hpp
    src/entities/Enemy.hpp
    src/entities/Entity.hpp
    src/entities/Homing.hpp
    src/entities/Laser.hpp
    src/entities/Player.hpp
    src/entities/PowerFragment.hpp

    src/graphics/BackgroundScroller.hpp
    src/graphics/Camera.hpp
    src/graphics/ChainNotif.hpp
    src/graphics/DrawingCandidate.hpp
    src/graphics/ExplosionAnim.hpp
    src/graphics/ExplosionEffect.hpp
    src/graphics/Particles.hpp

    src/handlers/BulletArray.hpp
    src/handlers/DestroyedEnemies.hpp
    src/handlers/DrawingCandidates.hpp
    src/handlers/EnemiesArray.hpp
    src/handlers/SoundHandler.hpp

    src/level/Level.hpp

    src/helpers/Constants.hpp
    src/helpers/Joint.hpp

    src/menu/Menu.hpp

# Game source - sources
    src/GameSystems.cpp

    src/entities/BossEnemy.cpp
    src/entities/Bullet.cpp
    src/entities/Enemy.cpp
    src/entities/Entity.cpp
    src/entities/Homing.cpp
    src/entities/Laser.cpp
    src/entities/Player.cpp
    src/entities/PowerFragment.cpp

    src/graphics/BackgroundScroller.cpp
    src/graphics/Camera.cpp
    src/graphics/ChainNotif.cpp
    src/graphics/DrawingCandidate.cpp
    src/graphics/ExplosionAnim.cpp
    src/graphics/ExplosionEffect.cpp
    src/graphics/Particles.cpp

    src/handlers/BulletArray.cpp
    src/handlers/DestroyedEnemies.cpp
    src/handlers/DrawingCandidates.cpp
    src/handlers/EnemiesArray.cpp
    src/handlers/SoundHandler.cpp

    src/level/Level.cpp

    src/helpers/Constants.cpp
    src/helpers/Joint.cpp

    src/menu/Menu.cpp

# TODO : refactor - Ad-hoc and data
    src/level/bossData.h
    src/level/cameras.h
    src/level/level_cmds.h
    src/level/levels.h
    src/level/patterns.h
    src/main.cpp
    src/types.h
    src/utils.hpp
    src/utils.cpp

# TODO : get rid of these - gfx
    src/gfx/bg.h
    src/gfx/bossgfx.h
    src/gfx/bossWarning.h
    src/gfx/end_of_demo.h
    src/gfx/gfx.h
    src/gfx/kanji.h
    src/gfx/titleScreen.h
# sfx
    src/sfx/list.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})

add_executable(nKaruga ${SOURCES} ${BACKEND_SOURCES} icon.rc)

target_compile_features(nKaruga PRIVATE cxx_std_11)

target_include_directories(nKaruga PRIVATE src)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT nKaruga)

add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/sfx ${CMAKE_BINARY_DIR}/sfx
    DEPENDS nKaruga)

# Gather backend files depending on platform cache variable

set(BACKEND_SOURCES src/backend/backends.h)

if(${PLATFORM} STREQUAL "Windows")
    set(BACKEND_SOURCES ${BACKEND_SOURCES} src/backend/backend_win.cpp)
endif()
target_sources(nKaruga PRIVATE ${BACKEND_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/backend PREFIX "Backend Files" FILES ${BACKEND_SOURCES})

### Third-party libraries

# SDL2 dev binaries

set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR}/SDL2-2.24.0/cmake)
find_package(SDL2 REQUIRED)

# SDL2_mixer dev binaries

set(SDL2_mixer_DIR ${CMAKE_CURRENT_LIST_DIR}/SDL2_mixer-2.6.2/cmake)
find_package(SDL2_mixer REQUIRED)

# n2DLib

set(N2DLIB_SOURCES
    n2DLib-PC/n2DLib/n2DLib.c
    n2DLib-PC/n2DLib/n2DLib.h
    n2DLib-PC/n2DLib/n2DLib_font.h
    n2DLib-PC/n2DLib/n2DLib_math.c
    n2DLib-PC/n2DLib/n2DLib_math.h
    n2DLib-PC/n2DLib/n2DLib_types.h
)

add_library(n2DLib STATIC ${N2DLIB_SOURCES})

target_include_directories(n2DLib PUBLIC n2DLib-PC)

if(${PLATFORM} STREQUAL "Windows")
    target_compile_definitions(n2DLib PUBLIC PIXEL_TYPE_RGB565)
endif()

# Link it all

target_link_libraries(nKaruga n2DLib SDL2::SDL2 SDL2::SDL2main SDL2_mixer::SDL2_mixer)

# n2DLib needs SDL2 currently

target_link_libraries(n2DLib SDL2::SDL2)

cmake_minimum_required(VERSION 3.13)

project(nKarugaProject LANGUAGES C CXX)

set(SOURCES ${SOURCES}
# Game source - headers
    src/bossData.h
    src/common.h
    src/ExplosionEffect.h
    src/levels.h
    src/misc_data.h
    src/patterns.h
# Game source - source files
    src/BackgroundScroller.cpp
    src/BossEnemy.cpp
    src/Bullet.cpp
    src/BulletArray.cpp
    src/ChainNotif.cpp
    src/DestroyedEnemies.cpp
    src/DrawingCandidate.cpp
    src/DrawingCandidates.cpp
    src/EnemiesArray.cpp
    src/Enemy.cpp
    src/Entity.cpp
    src/ExplosionAnim.cpp
    src/ExplosionEffect.cpp
    src/Homing.cpp
    src/Joint.cpp
    src/Laser.cpp
    src/Level.cpp
    src/Menu.cpp
    src/Particles.cpp
    src/Player.cpp
    src/PowerFragment.cpp
    src/SoundHandle.cpp
# Ad-hoc and miscellaneous
    src/buildGameLUTs.cpp
    src/main.cpp
    src/utils.cpp
# n2DLib
    src/n2DLib.h
    src/n2DLib.c
    src/n2DLib_font.h
# gfx
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

add_executable(nKaruga ${SOURCES} icon.rc)

target_compile_features(nKaruga PRIVATE cxx_std_11)

target_include_directories(nKaruga PRIVATE src)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT nKaruga)

add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/sfx ${CMAKE_BINARY_DIR}/sfx
    DEPENDS nKaruga)

### Third-party libraries

set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR}/SDL2-2.24.0/cmake)
find_package(SDL2 REQUIRED)

set(SDL2_mixer_DIR ${CMAKE_CURRENT_LIST_DIR}/SDL2_mixer-2.6.2/cmake)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(nKaruga SDL2::SDL2 SDL2::SDL2main SDL2_mixer::SDL2_mixer)
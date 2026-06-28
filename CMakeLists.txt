cmake_minimum_required(VERSION 3.21)

project(simple-sampler)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

set(SOURCES
    source/main.cpp
    source/dr_wav.c
    source/sampler.cpp
    source/sampler_engine.cpp
    source/sample_registry.cpp
    source/sample.cpp
    source/voice.cpp
    source/reverb.cpp
    source/runtime.cpp
    source/legacy_driver_runtime.cpp

    source/sampler_c.cpp

    source/driver/player.c
    source/driver/effects.c
    source/driver/external.c
    source/driver/stubs.c
)

set(HEADERS
    source/sampler.hpp
    source/sampler_engine.hpp
    source/sample_registry.hpp
    source/audio_types.hpp
    source/reverb_presets.hpp
    source/sample.hpp
    source/voice.hpp
    source/reverb.hpp
    source/runtime.hpp
    source/legacy_driver_runtime.hpp

    source/sampler_c.h

    source/driver/driver.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE miniaudio)

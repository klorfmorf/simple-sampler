cmake_minimum_required(VERSION 3.21)

project(miniaudio)

set(SOURCES
    source/miniaudio.c
)

set(HEADERS
    include/miniaudio.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC include)
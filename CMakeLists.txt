cmake_minimum_required(VERSION 3.12.4)

# Project Setup
project(demo VERSION 0.0.0.1 LANGUAGES CXX)

# Set directories for sandbox package
set(SANDBOX_DIR "${PROJECT_SOURCE_DIR}/sandbox")

# Configure Alkahest
set(ALKAHEST_DIR "${PROJECT_SOURCE_DIR}/alkahest")
add_subdirectory(${ALKAHEST_DIR})

# For threading support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Sandbox package setup
set(SANDBOX_SOURCE_DIR "${SANDBOX_DIR}/src")
file(GLOB sandbox_source_files "${SANDBOX_SOURCE_DIR}/main.cpp")

# Create sandbox executable
add_executable(sandbox ${sandbox_source_files})
target_include_directories(sandbox PRIVATE "${SANDBOX_SOURCE_DIR}")
target_compile_features(sandbox PRIVATE cxx_std_17)

# Link Alkahest files to client
target_link_libraries(sandbox "alkahest" Threads::Threads)
target_include_directories(sandbox PRIVATE "${ALKAHEST_DIR}/include")

cmake_minimum_required(VERSION 3.12.4)

# Set up project name and default settings
project(alkahest VERSION 0.0.0.1)
include(cmake/DefaultProjectSettings.cmake)

# Create variable for source files
set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(
    GLOB
    source_files
    "${source_dir}/main.cpp"
)

# Set include dir for self-reference
set(include_dir "${PROJECT_SOURCE_DIR}/include")

# "Library" for project options / C++ standard
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# "Library" for compiler warning flags
add_library(project_warnings INTERFACE)

# Add project warnings module
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Add sanitizer module
include(cmake/CompilerSanitizers.cmake)
enable_sanitizers(project_options)

# Add doxygen
include(cmake/Doxygen.cmake)
enable_doxygen(${include_dir})

# Enable static analyzers
include(cmake/StaticAnalysis.cmake)

# Various extra options go here
option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable test builds" OFF)

# Configure pre-compiled headers if desired
option(ENABLE_PCH "Enable pre-compiled headers" ON)
if(ENABLE_PCH)
    target_precompile_headers(project_options INTERFACE
    "$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/include/alkahest/pch.h>"
    )
endif()

# Testing setup
if(ENABLE_TESTING)
    enable_testing()
    message(STATUS "Building tests")
    add_subdirectory(test)
endif()

# For threading support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Add executable to build
add_library(alkahest)
target_sources(alkahest PRIVATE ${source_files})
target_link_libraries(alkahest PRIVATE project_options project_warnings Threads::Threads)
target_include_directories(alkahest PRIVATE ${include_dir})

# Library versioning
set_target_properties(alkahest
PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
)

# Add API flags
include(cmake/APIExportFlags.cmake)
set_api_flags(alkahest)

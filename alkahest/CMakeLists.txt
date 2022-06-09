cmake_minimum_required(VERSION 3.12.4)

# Set up project name and default settings
project(alkahest VERSION 0.0.0.1)
include(cmake/DefaultProjectSettings.cmake)

# Set platform variables for path resolution
include(cmake/PlatformDetection.cmake)
set_platform_detection_vars()

# Create variable for source files
set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(
    GLOB
    source_files
    "${source_dir}/init.cpp"
    "${source_dir}/log.cpp"
    "${source_dir}/platform/${target_platform}/sample.cpp"
    "${source_dir}/platform/${target_platform}/systems/window.cpp"
    "${source_dir}/platform/${target_platform}/systems/input.cpp"
    "${source_dir}/util/allocator.cpp"
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

# For threading support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# TODO: Refactor for Platform Independence
# Check for OpenGL
find_package(OpenGL)

if (OPENGL_FOUND)
    message("OpenGL found!")
    include_directories(${OPENGL_INCLUDE_DIR})
else(OPENGL_FOUND)
    message("OpenGL not found!")
endif(OPENGL_FOUND)

# Set up third-party libraries
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(FMT_DIR "${LIB_DIR}/fmt")
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GL3W_DIR "${LIB_DIR}/gl3w")
set(GLM_DIR "${LIB_DIR}/glm")

# Disable unwanted GLFW features
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

# Check for X11/Wayland (*nix only)
if(UNIX)
    execute_process(
        COMMAND /bin/sh -c "loginctl show-session $(loginctl | grep $(whoami) | awk '{print $1}') -p Type"
        OUTPUT_VARIABLE result_display_server
    )
    if("${result_display_server}" MATCHES "wayland")
        message("Wayland session detected!")
        set(GLFW_USE_WAYLAND ON CACHE INTERNAL "Build GLFW for Wayland")
    endif()
endif()

# Set GL_PREFERENCE
set(OpenGL_GL_PREFERENCE GLVND)

# Generate OpenGL profile in build dir
execute_process(COMMAND python3 gl3w_gen.py --root ${PROJECT_BINARY_DIR}/lib/gl3w WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/gl3w)

# Set PIC for using fmt as a shared lib
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Add subdirectories for third-party libs
add_subdirectory(${FMT_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GL3W_DIR})
add_subdirectory(${GLM_DIR})

# Testing setup
if(ENABLE_TESTING)
    enable_testing()

    # Fetch release of googletest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
    )
    FetchContent_MakeAvailable(googletest)

    # Include GTest
    include(GoogleTest)
    
    # Notify user of test building and create new target for tests
    message(STATUS "Building tests")
    set(test_dir "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    file(
        GLOB
        test_files
        "${test_dir}/main.cpp"
    )
    add_executable(alkahest_tests ${test_files})

    # Link gtest to the test target and find tests to add to CTest queue
    target_link_libraries(alkahest_tests gtest_main)
    gtest_discover_tests(alkahest_tests)
endif()

# Add executable to build
add_library(alkahest)
target_sources(alkahest PRIVATE ${source_files})
target_link_libraries(alkahest PRIVATE project_options project_warnings Threads::Threads)
target_include_directories(alkahest PRIVATE ${include_dir})
target_compile_definitions(alkahest PRIVATE ${target_platform_include_macro} LOGGING_LEVEL_ALL)

# Library versioning
set_target_properties(alkahest
PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
)

# Add API flags
include(cmake/APIExportFlags.cmake)
set_api_flags(alkahest)

# Link fmt
target_link_libraries(alkahest PUBLIC "fmt")
target_include_directories(alkahest PUBLIC "${FMT_DIR}/include")

# Link GLFW
target_link_libraries(alkahest PUBLIC "glfw")
target_include_directories(alkahest PUBLIC "${GLFW_DIR}/include")
target_compile_definitions(alkahest PUBLIC "GLFW_INCLUDE_NONE")

# Link gl3w
target_link_libraries(alkahest PRIVATE "gl3w")
target_include_directories(alkahest PRIVATE "${GL3W_DIR}/include")

# Link GLM
target_link_libraries(alkahest PUBLIC "glm")
target_include_directories(alkahest PUBLIC "${GLM_DIR}/include")

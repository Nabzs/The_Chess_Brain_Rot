cmake_minimum_required(VERSION 3.20)

project(ChessBrainrot)
add_executable(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    CXX_EXTENSIONS OFF)

# ── STATIC build ──
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

include(FetchContent)

# SFML 2.6 (static)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.2
    GIT_SHALLOW    TRUE)
set(SFML_BUILD_AUDIO TRUE)
set(SFML_BUILD_NETWORK FALSE)
FetchContent_MakeAvailable(SFML)

# TGUI 1.x (static)
FetchContent_Declare(TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG        v1.7.0
    GIT_SHALLOW    TRUE)
set(TGUI_BACKEND "SFML_GRAPHICS")
FetchContent_MakeAvailable(TGUI)

# Sources
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${MY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include src)

# Tell the compiler SFML is static
target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)

# Link everything
target_link_libraries(${PROJECT_NAME} PRIVATE
    TGUI::TGUI
    sfml-audio sfml-graphics sfml-window sfml-system
    opengl32 winmm gdi32)

# Static MinGW runtime (but NOT system DLLs like opengl32)
if(MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE
        -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
endif()

# Copy assets
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_SOURCE_DIR}/bin)

cmake_minimum_required(VERSION 3.20)

project(TensorForge VERSION 0.1.0 LANGUAGES CXX)

add_executable(tensorforge
    src/main.cpp
)

target_compile_features(tensorforge PRIVATE cxx_std_20)
set_target_properties(tensorforge PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(tensorforge PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

if(MSVC)
    target_compile_options(tensorforge PRIVATE /W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tensorforge PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
endif()

option(TENSORFORGE_ENABLE_SANITIZERS
    "Enable AddressSanitizer and UndefinedBehaviorSanitizer"
    OFF
)

if(TENSORFORGE_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(tensorforge PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
        target_link_options(tensorforge PRIVATE
            -fsanitize=address,undefined
        )
    else()
        message(WARNING
            "TENSORFORGE_ENABLE_SANITIZERS is supported only with GCC or Clang"
        )
    endif()
endif()

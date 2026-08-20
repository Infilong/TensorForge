cmake_minimum_required(VERSION 3.20)

project(TensorForge VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

option(TENSORFORGE_ENABLE_SANITIZERS
    "Enable AddressSanitizer and UndefinedBehaviorSanitizer"
    OFF
)

if(TENSORFORGE_ENABLE_SANITIZERS
   AND NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(WARNING
        "TENSORFORGE_ENABLE_SANITIZERS is supported only with GCC or Clang"
    )
endif()

function(tensorforge_configure_target target_name)
    target_compile_features(${target_name} PRIVATE cxx_std_20)
    set_target_properties(${target_name} PROPERTIES
        CXX_EXTENSIONS OFF
    )

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive-)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
        )
    endif()

    if(TENSORFORGE_ENABLE_SANITIZERS
       AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
        target_link_options(${target_name} PRIVATE
            -fsanitize=address,undefined
        )
    endif()
endfunction()

add_library(tensorforge_core
    src/cpu_storage.cpp
)
target_include_directories(tensorforge_core PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)
tensorforge_configure_target(tensorforge_core)

add_executable(tensorforge
    src/main.cpp
)
target_include_directories(tensorforge PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
tensorforge_configure_target(tensorforge)

add_executable(tensorforge_compiler_optimization
    experiments/compiler_optimization.cpp
)
tensorforge_configure_target(tensorforge_compiler_optimization)

if(BUILD_TESTING)
    add_executable(tensorforge_cpu_storage_tests
        tests/cpu_storage_tests.cpp
    )
    target_link_libraries(tensorforge_cpu_storage_tests PRIVATE
        tensorforge_core
    )
    tensorforge_configure_target(tensorforge_cpu_storage_tests)

    add_test(
        NAME cpu_storage
        COMMAND tensorforge_cpu_storage_tests
    )
endif()

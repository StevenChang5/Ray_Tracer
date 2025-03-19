cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CUDA_ARCHITECTURES 75)
set(CMAKE_CUDA_COMPILER /usr/local/cuda-12.8/bin/nvcc)

project(RayTracing LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD  11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(EXTERNAL src/external/stb_image.h)

set(CMAKE_CUDA_ARCHITECTURES "native")

set(SOURCE
    src/main.cpp
    src/camera.h
    src/color.h
    src/hittable_list.h
    src/hittable.h
    src/interval.h
    src/material.h
    src/ray.h
    src/sphere.h
    src/utils.h
    src/vec3.h
    
)

set(SOURCE_CUDA
    src/cuda/main.cu
)

include_directories(src)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # /wd #### - Disable warning
    # /we #### - treat warning as error
    add_compile_options("/W4")      # Enable level-4 warnings
    add_compile_options("/we 4265") # Class has virtual functions, but its non-trivial destructor is not virtual
    add_compile_options("/we 5204") # Class has virtual functions, but its trivial destructor is not virtual
    add_compile_options("/wd 4100") # unreferenced formal parameter
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wmaybe-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wsometimes-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
endif()

add_executable(rt ${SOURCE})

add_executable(rt_cuda ${SOURCE_CUDA})
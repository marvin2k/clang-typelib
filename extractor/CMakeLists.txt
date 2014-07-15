cmake_minimum_required(VERSION 2.8.3)

project(extractor)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules"
    )

find_package(LibClang REQUIRED)

add_definitions(${LibClang_DEFINITIONS})

include_directories(
    ${LibClang_INCLUDE_DIRS}
    )

link_directories("/home/mzenzes/llvm.install/lib")
add_executable(${PROJECT_NAME}
    main.cpp
    )

target_link_libraries(${PROJECT_NAME}
    clangASTMatchers clangTooling clangBasic LLVMSupport
    )

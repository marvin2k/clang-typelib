# prevent loud warnings from gcc-4.9 caused by a macro-expansion the
# AST-headers... this only clutters up the compiler output with nonsense
add_definitions("-Wno-unused-local-typedefs")

add_clang_executable(extractor
    Extractor.cpp
    TypelibBuilder.cpp
    )

target_link_libraries(extractor
    clangEdit clangTooling clangBasic clangAST clangASTMatchers
    )

add_clang_executable(extractor
    Extractor.cpp
    )

target_link_libraries(extractor
    clangEdit clangTooling clangBasic clangAST clangASTMatchers
    )

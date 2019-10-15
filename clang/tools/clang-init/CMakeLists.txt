
set( LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  )
set(CMAKE_CXX_STANDARD 17)
add_clang_executable(find-class-decls FindClassDecls.cpp)

clang_target_link_libraries(find-class-decls
  PRIVATE
  clangAST
  clangBasic
  clangDriver
  clangFrontend
  clangRewriteFrontend
  clangSerialization
  clangStaticAnalyzerFrontend
  clangTooling
  )

install(TARGETS find-class-decls
  RUNTIME DESTINATION bin)

add_subdirectory(loop-convert)

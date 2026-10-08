# SPDX-FileCopyrightText: 2026 Edward Kmett <ekmett@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause OR Apache-2.0
cmake_minimum_required(VERSION 3.23)
project(hint VERSION 0.1.0 LANGUAGES NONE)

# A header-only configure does not need a compiler or a target architecture.
if(NOT CMAKE_CXX_COMPILER_LOADED AND NOT CMAKE_C_COMPILER_LOADED)
  set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Library installation directory")
endif()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
add_library(hint INTERFACE)
add_library(hint::hint ALIAS hint)
target_sources(hint INTERFACE FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" FILES include/hint.h)
target_include_directories(hint INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
install(TARGETS hint EXPORT hintTargets FILE_SET HEADERS)
install(EXPORT hintTargets NAMESPACE hint:: DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/hint")
export(EXPORT hintTargets NAMESPACE hint:: FILE "${CMAKE_CURRENT_BINARY_DIR}/hintTargets.cmake")
configure_package_config_file(cmake/hintConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/hintConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/hint")
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/hintConfigVersion.cmake"
  VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/hintConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/hintConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/hint")
install(FILES LICENSE.md DESTINATION "${CMAKE_INSTALL_DATADIR}/hint")

option(HINT_BUILD_TESTS "Build annotation smoke tests" OFF)
if(HINT_BUILD_TESTS)
  enable_language(CXX)
  enable_testing()
  add_subdirectory(tests)
endif()

option(HINT_BUILD_DOCS "Build the attribute reference with Doxygen" OFF)
if(HINT_BUILD_DOCS)
  find_package(Doxygen 1.9 REQUIRED)
  set(DOXYGEN_PROJECT_NAME Hint)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")
  set(DOXYGEN_GENERATE_LATEX NO)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
  set(DOXYGEN_QUIET YES)
  set(DOXYGEN_WARN_AS_ERROR YES)
  set(DOXYGEN_PREDEFINED "__cplusplus=201703L")
  doxygen_add_docs(hint_docs include/hint.h README.md LICENSE.md ALL)
endif()

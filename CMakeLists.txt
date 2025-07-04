# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.16)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
  set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()
project(cominit VERSION 0.9.2)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS NO)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")
include(GNUInstallDirs)

option(UNIT_TESTS "Build unit tests" ON)
option(FAKE_HSM "Emulate a HSM for development" OFF)
option(USE_TPM "Add TPM functionality for development" OFF)
set(FAKE_HSM_KEY_DESCS
    "dm-integrity-hmac-secret dm-integrity-jmac-secret dm-integrity-jcrypt-secret"
    CACHE STRING
    "The description fields of the keys to enroll if HSM emulation is compiled in.")
set(FAKE_HSM_KEY_DIR
    "/etc/fake_hsm"
    CACHE STRING
    "The directory in initramfs where the keyfiles to enroll are located.")

set(COMINIT_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(COMINIT_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(COMINIT_VERSION_MICRO ${PROJECT_VERSION_PATCH})
set(PKG_VERSION ${COMINIT_VERSION_MAJOR}.${COMINIT_VERSION_MINOR}.${COMINIT_VERSION_MICRO})

find_package(Git)
if (GIT_FOUND)
  macro(FetchGitRevision)
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE exit_code
      OUTPUT_VARIABLE output
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    if(NOT exit_code EQUAL 0)
      message("failed to retrieve git short commit hash")
    else()
      set(COMINIT_VERSION_GIT ${output})
      message("version ${PKG_VERSION}.${COMINIT_VERSION_GIT}")
    endif()
  endmacro()
  FetchGitRevision()
endif()

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_C_FLAGS_RELEASE "-Os -DNODEBUG")

find_package(MbedTLS 2.28 REQUIRED)

add_compile_options(
  -Wall -Wextra -Werror -pedantic
  -fstack-protector-strong
  -ffunction-sections -fdata-sections
  -D_DEFAULT_SOURCE
  -D_FORTIFY_SOURCE=2
)

if (CMAKE_BUILD_TYPE STREQUAL "Analyzer")
  add_compile_options(-fanalyzer)
elseif (CMAKE_BUILD_TYPE STREQUAL "Asan")
  link_libraries(asan)
  add_compile_options(-fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Ubsan")
  link_libraries(ubsan)
  add_compile_options(-fsanitize=undefined)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG -g)
endif()

add_subdirectory(src/)
if(UNIT_TESTS)
  enable_testing()
  add_subdirectory(test/)
endif(UNIT_TESTS)

find_package(Doxygen)
add_custom_target(
  doxygen 
  COMMAND doxygen ${PROJECT_SOURCE_DIR}/Doxyfile
  WORKING_DIRECTORY ${DOC_BUILD_DIR}
)

add_custom_target(
  rpmbuild
  COMMAND
    mkdir -p packaging/rpmbuild/SOURCES &&
    tar -czf packaging/rpmbuild/SOURCES/cominit-${PKG_VERSION}.tar.gz --transform "s,^,cominit-${PKG_VERSION}/," src/ inc/ test/ CMakeLists.txt CMakeModules Doxyfile README.md &&
    cd packaging/rpmbuild &&
    rpmbuild
      --target ${CMAKE_SYSTEM_PROCESSOR}
      --nodeps
      --define "_topdir ${PROJECT_SOURCE_DIR}/packaging/rpmbuild"
      -v -ba SPECS/cominit-git.spec
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

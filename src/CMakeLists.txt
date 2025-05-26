# SPDX-License-Identifier: MIT
configure_file(version.c.in version.c)

if(FAKE_HSM)
    add_compile_options(
        -DCOMINIT_FAKE_HSM
        -DCOMINIT_FAKE_HSM_KEY_DESCS="${FAKE_HSM_KEY_DESCS}"
        -DCOMINIT_FAKE_HSM_KEY_DIR="${FAKE_HSM_KEY_DIR}"
    )
endif()

add_executable(
  cominit
  cominit.c
  keyring.c
  minsetup.c
  meta.c
  dmctl.c
  output.c
  ${CMAKE_CURRENT_BINARY_DIR}/version.c
)

target_include_directories(
  cominit
  PRIVATE
    ${PROJECT_SOURCE_DIR}/inc/
    ${MBEDTLS_INCLUDE_DIR}
)
target_link_libraries(
  cominit
  PRIVATE
    ${MBEDTLS_CRYPTO_LIBRARY}
)

if(USE_TPM)
  target_compile_definitions(cominit PRIVATE COMINIT_USE_TPM)

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(TSS2_ESYS REQUIRED tss2-esys)

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(TSS2_TCTILDR REQUIRED tss2-tctildr)

  target_sources(cominit PRIVATE tpm.c)

  target_include_directories(
    cominit
    PRIVATE
      ${TSS2_ESYS_INCLUDE_DIRS}
      ${TSS2_TCTILDR_INCLUDE_DIRS}
  )
  target_link_libraries(
    cominit
    PRIVATE
      ${TSS2_ESYS_LIBRARIES}
      ${TSS2_TCTILDR_LIBRARIES}
  )
endif()

# install

install(TARGETS cominit DESTINATION ${CMAKE_INSTALL_BINDIR})

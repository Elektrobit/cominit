# FindTss2-esys.cmake

# Search for tss2-esys library
find_path(TSS2_ESYS_INCLUDE_DIR
  NAMES tss2/tss2_esys.h
  PATHS /usr/local/aarch64-musl/include /usr/include
)

find_library(TSS2_ESYS_LIBRARY
  NAMES tss2-esys
  PATHS /usr/local/aarch64-musl/lib /usr/lib
)

if(TSS2_ESYS_INCLUDE_DIR AND TSS2_ESYS_LIBRARY)
  set(Tss2-esys_FOUND TRUE)
  set(Tss2-esys_INCLUDE_DIRS ${TSS2_ESYS_INCLUDE_DIR})
  set(Tss2-esys_LIBRARIES ${TSS2_ESYS_LIBRARY})
else()
  set(Tss2-esys_FOUND FALSE)
endif()

mark_as_advanced(TSS2_ESYS_INCLUDE_DIR TSS2_ESYS_LIBRARY)


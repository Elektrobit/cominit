# FindTss2-tctildr.cmake

# Search for tss2-tctildr library
find_path(TSS2_TCTILDR_INCLUDE_DIR
  NAMES tss2/tss2_tctildr.h
  PATHS /usr/local/aarch64-musl/include /usr/include
)

find_library(TSS2_TCTILDR_LIBRARY
  NAMES tss2-tctildr
  PATHS /usr/local/aarch64-musl/lib /usr/lib
)

if(TSS2_TCTILDR_INCLUDE_DIR AND TSS2_TCTILDR_LIBRARY)
  set(Tss2-tctildr_FOUND TRUE)
  set(Tss2-tctildr_INCLUDE_DIRS ${TSS2_TCTILDR_INCLUDE_DIR})
  set(Tss2-tctildr_LIBRARIES ${TSS2_TCTILDR_LIBRARY})
else()
  set(Tss2-tctildr_FOUND FALSE)
endif()

mark_as_advanced(TSS2_TCTILDR_INCLUDE_DIR TSS2_TCTILDR_LIBRARY)


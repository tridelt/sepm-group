set(SOURCES "crypt_blowfish.c" "crypt_gensalt.c" "wrapper.c")

set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
add_definitions(-D__SKIP_GNU)

add_library(ow-crypt STATIC ${SOURCES} "x86.S")

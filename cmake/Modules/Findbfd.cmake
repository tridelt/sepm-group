# - Try to find bfd
# Once done, this will define
#
#  bfd_FOUND - system has binutils
#  bfd_INCLUDE_DIRS - the binutils include directories
#  bfd_LIBRARIES - link these to use binutils

include(LibFindMacros)

# Include dir
find_path(bfd_INCLUDE_DIR
  NAMES bfd.h
  PATHS /usr/include
)

# Finally the library itself
find_library(bfd_LIBRARY
  NAMES bfd
  PATHS /usr/lib
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(bfd_PROCESS_INCLUDES bfd_INCLUDE_DIR)
set(bfd_PROCESS_LIBS bfd_LIBRARY)
libfind_process(bfd)

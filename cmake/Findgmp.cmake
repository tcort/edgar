
find_path(GMP_INCLUDE_DIR gmp.h)
find_library(GMP_LIBRARY NAMES gmp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gmp REQUIRED_VARS GMP_INCLUDE_DIR GMP_LIBRARY)
mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARY)

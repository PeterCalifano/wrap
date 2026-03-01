include_guard(GLOBAL)
# Configure time configuration for wrap
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# Set the path to the Python package directory so we can add it to the PYTHONPATH.
# Used in the *Wrap.cmake files.
set_and_check( GTWRAP_PYTHON_PACKAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Set the variables for the wrapping scripts to be used in the build.
set(PYBIND_WRAP_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../scripts/pybind_wrap.py" CACHE PATH "Path to pybind11 wrapping script.")
set(MATLAB_WRAP_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../scripts/matlab_wrap.py" CACHE PATH "Path to MATLAB wrapping script.")

# Load all the CMake scripts from the standard location
include(${CMAKE_CURRENT_LIST_DIR}/PybindWrap.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MatlabWrap.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/GtwrapUtils.cmake)

# Load the pybind11 code from the library installation path
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../pybind11)

check_required_components(gtwrap)

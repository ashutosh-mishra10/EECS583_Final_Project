cmake_minimum_required(VERSION 3.4.3)
set(CMAKE_PREFIX_PATH "~/llvm-project/build/")
set(LLVM_DIR "~/llvm-project/build/lib/cmake/llvm/")
set(Polly_DIR "~/llvm-project/build/tools/polly/lib/cmake/polly/")
set(Polly_INCLUDE_DIRS "~/llvm-project/polly/include/polly/")
set (CMAKE_CXX_STANDARD 17)
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
   message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
find_package(LLVM REQUIRED CONFIG)                        # This will find the shared LLVM build.
find_package(Polly REQUIRED CONFIG)                        # This will find the shared LLVM build.
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")        # You don't need to change ${LLVM_CMAKE_DIR} since it is already defined.
include(AddLLVM)
add_definitions(${LLVM_DEFINITIONS})                      # You don't need to change ${LLVM_DEFINITIONS} since it is already defined.
include_directories(${LLVM_INCLUDE_DIRS})                 # You don't need to change ${LLVM_INCLUDE_DIRS} since it is already defined.
include_directories(${Polly_INCLUDE_DIRS})                 # You don't need to change ${LLVM_INCLUDE_DIRS} since it is already defined.
add_subdirectory(mypass)                                 # Add the directory which your pass lives.

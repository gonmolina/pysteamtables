cmake_minimum_required(VERSION 3.11.0)

if (CMAKE_VERSION VERSION_GREATER "3.11.99")
  cmake_policy(SET CMP0074 NEW)
endif()

project(pySteamTables LANGUAGES NONE)

set(PROJECT_BRIEF "Cython wrappers for Freesteam: an implementation of IAPWS-IF97 thermophysical properties for water and steam.")

enable_language(C)

set(PACKAGE_VERSION_NAME ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR})
set(VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR})

find_package (Python COMPONENTS Interpreter Development)
find_package(GSL REQUIRED)
set(DIRS ${GSL_INCLUDE_DIRS} ${GSLCBLAS_INCLUDE_DIRS})

message("Libraries: ${GSL_LIBRARIES} ${GSLCBLAS_LIBRARIES}")
message("Includes: ${GSL_INCLUDE_DIRS} ${GSLCBLAS_INCLUDE_DIRS}")

message("Python executable:  ${Python_EXECUTABLE}")
message("site path:  ${Python_SITEARCH}")

list(APPEND CMAKE_MODULE_PATH ${Python_SITEARCH}/skbuild/resources/cmake/)

message(${CMAKE_MODULE_PATH})

find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)


include_directories("./pysteamtables/lib")

add_subdirectory("pysteamtables")


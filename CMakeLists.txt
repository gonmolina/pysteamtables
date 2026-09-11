cmake_minimum_required(VERSION 3.11.0)

if (CMAKE_VERSION VERSION_GREATER "3.11.99")
  cmake_policy(SET CMP0074 NEW)
endif()

project(pySteamTables LANGUAGES C)

set(PROJECT_BRIEF "Cython wrappers for Freesteam: an implementation of IAPWS-IF97 thermophysical properties for water and steam.")

find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)
find_package(GSL REQUIRED)


include_directories("./pysteamtables/lib")

add_subdirectory("pysteamtables")


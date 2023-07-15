cmake_minimum_required(VERSION 3.25)
add_subdirectory(CS1605-Project2)

project(MisakaVan-Stuff)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")
set_target_properties(Project2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# message("${CMAKE_PROJECT_NAME}")

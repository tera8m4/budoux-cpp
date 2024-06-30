cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Budoux LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME utfcpp
  GITHUB_REPOSITORY nemtrif/utfcpp
  GIT_TAG v4.0.5  # Use the desired version tag here
  DOWNLOAD_ONLY True
)

if(utfcpp_ADDED)
  add_library(utfcpp INTERFACE)
  target_include_directories(utfcpp SYSTEM INTERFACE ${utfcpp_SOURCE_DIR}/source)
endif()

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/**/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_library(${PROJECT_NAME} SHARED ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE utfcpp)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

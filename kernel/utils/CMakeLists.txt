file(GLOB CPP_SRCS *.cpp)
add_library(utils STATIC ${CPP_SRCS})
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
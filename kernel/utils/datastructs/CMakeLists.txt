
file(GLOB CPP_SRCS CONFIGURE_DEPENDS *.h *.cpp)
add_library(datastructs STATIC ${CPP_SRCS})
target_include_directories(datastructs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
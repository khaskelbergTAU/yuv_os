
file(GLOB CPP_SRCS CONFIGURE_DEPENDS *.h *.cpp)
add_library(multiboot STATIC ${CPP_SRCS})
target_link_libraries(multiboot utils)
target_include_directories(multiboot INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
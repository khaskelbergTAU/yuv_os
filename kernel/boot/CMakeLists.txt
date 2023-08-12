file(GLOB CPP_SRCS CONFIGURE_DEPENDS *.h *.cpp)
add_library(boot STATIC ${CPP_SRCS} boot.s)
target_include_directories(boot INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
set_property(SOURCE boot.s PROPERTY LANGUAGE ASM_NASM)
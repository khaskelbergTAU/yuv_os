file(GLOB CPP_SRCS *.cpp)
add_library(interrupts STATIC ${CPP_SRCS})
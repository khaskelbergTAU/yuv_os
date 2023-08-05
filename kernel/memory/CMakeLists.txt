file(GLOB CPP_SRCS CONFIGURE_DEPENDS *.h *.cpp)
add_library(memory STATIC ${CPP_SRCS})
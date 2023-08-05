
file(GLOB CPP_SRCS CONFIGURE_DEPENDS *.h *.cpp)
add_library(irq_handlers STATIC ${CPP_SRCS})
target_link_libraries(irq_handlers utils)
target_compile_options(irq_handlers PRIVATE -mgeneral-regs-only)
target_include_directories(irq_handlers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
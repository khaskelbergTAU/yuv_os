add_executable(kernel.elf crti.s boot.s kernel.cpp paging.cpp serial.cpp video.cpp crtn.s)

add_subdirectory(utils)

set_property(SOURCE crti.s PROPERTY LANGUAGE ASM_NASM)

set_property(SOURCE crtn.s PROPERTY LANGUAGE ASM_NASM)

set_property(SOURCE boot.s PROPERTY LANGUAGE ASM_NASM)

target_include_directories(kernel.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(kernel.elf PUBLIC utils)


execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-libgcc-file-name OUTPUT_VARIABLE libgcc OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN_O OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND_O OUTPUT_STRIP_TRAILING_WHITESPACE)


file(GLOB CPP_SRCS CONFIGURE_DEPENDS *.h *.cpp)
add_executable(kernel.elf ${CPP_SRCS} ${libgcc})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(utils)
add_subdirectory(interrupts)
add_subdirectory(memory)
add_subdirectory(boot)
add_subdirectory(multiboot)

set_target_properties(kernel.elf PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)


target_include_directories(kernel.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties(${libgcc}
PROPERTIES
EXTERNAL_OBJECT true
GENERATED true
)

set_source_files_properties(${CRTBEGIN_O}
PROPERTIES
EXTERNAL_OBJECT true
GENERATED true
)

set_source_files_properties(${CRTEND_O}
PROPERTIES
EXTERNAL_OBJECT true
GENERATED true
)

set(crti ${CMAKE_BINARY_DIR}/crts/crti.o)
set(crtn ${CMAKE_BINARY_DIR}/crts/crtn.o)

add_custom_target(crts WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
COMMAND mkdir -p ${CMAKE_BINARY_DIR}/crts
COMMAND ${CMAKE_ASM_NASM_COMPILER} crti.s -f elf64 -o ${crti}
COMMAND ${CMAKE_ASM_NASM_COMPILER} crtn.s -f elf64 -o ${crtn}
COMMENT "compiling crt[i|n].s" VERBATIM)


add_dependencies(kernel.elf crts)
target_link_libraries(kernel.elf PUBLIC utils ${libgcc} interrupts memory boot multiboot)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <CMAKE_CXX_LINK_FLAGS> <FLAGS> <LINK_FLAGS> ${crti} ${CRTBEGIN_O} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> ${CRTEND_O} ${crtn}" )

add_custom_command(OUTPUT myos.iso
COMMAND mkdir -p isodir/boot/grub
COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/grub.cfg isodir/boot/grub/grub.cfg
COMMAND cp kernel.elf isodir/boot/kernel.elf
COMMAND ${CMAKE_GRUB_COMMAND} -o myos.iso isodir
DEPENDS kernel.elf
BYPRODUCTS isodir/boot/grub/grub.cfg isodir/boot/kernel.elf)
add_custom_target(iso ALL DEPENDS myos.iso)
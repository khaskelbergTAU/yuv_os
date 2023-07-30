set(libgcc ${TOOLS_DIR}/lib/gcc/i686-elf/12.2.0/libgcc.a)
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN_O OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND_O OUTPUT_STRIP_TRAILING_WHITESPACE)


add_executable(kernel.elf boot.s kernel.cpp paging.cpp serial.cpp video.cpp ${libgcc})

add_subdirectory(utils)

set_property(SOURCE boot.s PROPERTY LANGUAGE ASM_NASM)

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
COMMAND ${CMAKE_ASM_NASM_COMPILER} crti.s ${CMAKE_ASM_NASM_COMPILER_FLAGS} -o ${crti}
COMMAND ${CMAKE_ASM_NASM_COMPILER} crtn.s ${CMAKE_ASM_NASM_COMPILER_FLAGS} -o ${crtn}
COMMENT "compiling crt[i|n].s" VERBATIM)


add_dependencies(kernel.elf crts)
target_link_libraries(kernel.elf PUBLIC utils ${libgcc})

set(CMAKE_CXX_LINKER ${TOOLS_DIR}/bin/i686-elf-gcc)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINKER} <CMAKE_CXX_LINK_FLAGS> <FLAGS> <LINK_FLAGS> ${crti} ${CRTBEGIN_O} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> ${CRTEND_O} ${crtn}" )
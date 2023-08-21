cmake_minimum_required( VERSION 3.6 )
project( gb-emulator )

set(CMAKE_CXX_STANDARD 17)

find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)
find_package(pugixml REQUIRED)
find_package(inja REQUIRED)

######################################################
### Libgbe

add_library( libgbe
    src/gbmemory.cpp
    src/gbcpu.cpp
    src/bit_utils.cpp
    src/instruction_trait.cpp
    #src/call_switch-autogen.cpp
    src/registers.cpp
    src/opcode.cpp
)

target_include_directories(libgbe PRIVATE
    third-party/magic_enum/include
    include)

target_link_libraries( libgbe
    fmt::fmt
    pugixml::static)

target_compile_options(libgbe PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

######################################################
### Opcodes

add_executable( gbopcodes
    src/opcodes_main.cpp
)
#message(${inja_INCLUDE_DIRS})
target_include_directories(gbopcodes PRIVATE
    #${inja_INCLUDE_DIRS}
    third-party/magic_enum/include
    include)

target_link_libraries( gbopcodes PRIVATE libgbe pantor::inja )

#### Main executable

#add_executable( gbe src/main.cpp )

#target_include_directories(gbe PRIVATE
#    third-party/magic_enum/include
#    include)

#target_link_libraries( gbe PRIVATE libgbe )

#### Tests

add_executable( tests tests/test.cpp )
add_compile_definitions( ROOT_DIR="${CMAKE_SOURCE_DIR}" )
target_include_directories(tests PRIVATE
    third-party/magic_enum/include
    include)

target_link_libraries( tests PRIVATE Catch2::Catch2WithMain libgbe )

project( gb-emulator )

cmake_minimum_required( VERSION 2.8 )

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third-party/Catch2)

#### Libgbe

add_library( libgbe
    src/gbmemory.cpp
    src/gbcpu.cpp
    src/bit_utils.cpp
    src/instruction_trait.cpp
    src/call_switch-autogen.cpp
    src/registers.cpp)

target_include_directories(libgbe PRIVATE
    third-party/magic_enum/include
    include)

#### Add warning configurations

target_compile_options(libgbe PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (MSVC)
    #find_package(unofficial-sqlite3 CONFIG REQUIRED)

    set(CAPSTONE_LIBRARY "C:/vcpkg-master/installed/x64-windows/debug/lib")
    set(CAPSTONE_INCLUDE_DIR "C:/vcpkg-master/installed/x64-windows/include/capstone")

    # searching for include directory
    find_path(CAPSTONE_INCLUDE_DIR capstone.h)

    # searching for library file
    find_library(CAPSTONE_LIBRARY capstone_dll PATHS "C:/vcpkg-master/installed/x64-windows/debug/lib")

    if (CAPSTONE_INCLUDE_DIR AND CAPSTONE_LIBRARY)
        # you may need that if further action in your CMakeLists.txt depends
        # on detecting your library
        set(CAPSTONE_FOUND TRUE)

        # you may need that if you want to conditionally compile some parts
        # of your code depending on library availability
        add_definitions(-DHAVE_CAPSTONE=1)

        # those two, you really need
        include_directories(${CAPSTONE_INCLUDE_DIR})
    endif ()


    #if (${CMAKE_BUILD_TYPE} STREQUAL "Debug") 
    #    set(CAPSTONE_dll_dir "C:/vcpkg-master/installed/x64-windows/bin")
    #else()
    #    set(CAPSTONE_dll_dir "C:/vcpkg-master/installed/x64-windows/debug/bin")
    #endif()

    #add_library(CAPSTONE "${CAPSTONE_dll_dir}/capstone.dll" "${CAPSTONE_dll_dir}/../lib/capstone_dll.lib" )
    #set_target_properties(CAPSTONE PROPERTIES LINKER_LANGUAGE C)

    find_package(fmt 7.1.3 CONFIG REQUIRED)
    target_link_libraries(libgbe PRIVATE fmt::fmt CAPSTONE_LIBRARY)
else ()
    find_package(fmt CONFIG REQUIRED)

    target_link_libraries(libgbe -lfmt)
endif (MSVC)

#### Main executable

add_executable( gbe src/main.cpp )

target_include_directories(gbe PRIVATE
    third-party/magic_enum/include
    include)

target_link_libraries( gbe PRIVATE libgbe )

#### Tests

add_executable( tests tests/test.cpp )
add_compile_definitions( ROOT_DIR="${CMAKE_SOURCE_DIR}" )
target_include_directories(tests PRIVATE
    third-party/magic_enum/include
    include)

target_link_libraries( tests PRIVATE Catch2::Catch2WithMain libgbe )

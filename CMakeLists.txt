# http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects
# lot's of cmake modules: https://github.com/rpavlik/cmake-modules

cmake_minimum_required(VERSION 2.6)
project(chat CXX C)

set(CMAKE_C_COMPILER "gcc-4.6")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-4.6")

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++0x")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(ZeroCIce REQUIRED)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


include_directories(${ZeroCIce_INCLUDE_DIR})
set(LIBS ${LIBS} ${ZeroCIceCore_LIBRARY})
set(LIBS ${LIBS} ${ZeroCIceUtil_LIBRARY})
set(LIBS ${LIBS} ${ZeroCIceSSL_LIBRARY})
set(LIBS ${LIBS} ${Boost_LIBRARIES})
set(LIBS ${LIBS} ${QT_LIBRARIES})
set(LIBS ${LIBS} ${QT_QTDECLARATIVE_LIBRARY})


# only enable pretty backtraces if libbfd is available
find_package(bfd)
if(bfd_FOUND)
  add_definitions(-DBACKWARD_HAS_BFD)
  set(LIBS ${LIBS} ${bfd_LIBRARIES})   # for backward pretty stacktracing
endif()


# only enable gccfilter if necessary perl modules are present
find_package(PerlModules COMPONENTS Regexp::Common Getopt::ArgvFile Getopt::Long Term::ANSIColor)
if(Regexp::Common_FOUND AND Getopt::ArgvFile_FOUND AND Getopt::Long_FOUND AND Term::ANSIColor_FOUND)
  if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCPP)
    option(COLOR_GCC "Use GCCFilter to color compiler output messages" ON)
    set(COLOR_GCC_OPTIONS "-c -r -w" CACHE STRING "Arguments that are passed to gccfilter when output coloring is switchend on. Defaults to -c -r -w.")
    if(COLOR_GCC)
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${PROJECT_SOURCE_DIR}/cmake/gccfilter ${COLOR_GCC_OPTIONS}")
    endif()
  endif()
endif()


#begin snippet
#next lines are for creating .h & .cpp files out of .ice Files
set(out_dir "${CMAKE_CURRENT_SOURCE_DIR}/common")
set(out_file "${out_dir}/SecureDistributedChat.cpp")

add_custom_command(OUTPUT
"${out_file}"
COMMAND "${ZeroCIce_slice2cpp_BIN}"
  --add-header=cstddef
  -I /usr/share/Ice/slice/
  -I /usr/share/Ice-3.4.2/slice/
  -I /usr/share/slice/
  --stream
  --output-dir "${out_dir}"
  "${CMAKE_CURRENT_SOURCE_DIR}/common/SecureDistributedChat.ice"
)

#end snippet

# hack to show files in Qt Creator
FILE(GLOB QML_FILES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.qml")
add_custom_target( QML_Resources ALL SOURCES ${QML_FILES_LIST} )

FILE(GLOB HEADER_FILES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_custom_target( HEADER_Resources ALL SOURCES ${HEADER_FILES_LIST} )


# copy resources to build dir
add_custom_command(OUTPUT ui_files
    POST_BUILD
    COMMAND cmake -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/ui" "${CMAKE_CURRENT_BINARY_DIR}/ui")

add_custom_command(OUTPUT cert
    POST_BUILD
    COMMAND cmake -E copy "${CMAKE_CURRENT_SOURCE_DIR}/ca.crt" "${CMAKE_CURRENT_BINARY_DIR}/ca.crt")


# copy executables to top of build dir
add_custom_command(OUTPUT client_binary
    POST_BUILD
    COMMAND cmake -E copy "${CMAKE_BINARY_DIR}/client/client" "${CMAKE_BINARY_DIR}/chat_client")

add_custom_command(OUTPUT server_binary
    POST_BUILD
    COMMAND cmake -E copy "${CMAKE_BINARY_DIR}/server/server" "${CMAKE_BINARY_DIR}/chat_server")



include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

add_subdirectory(dependencies)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

add_custom_target(post_build_steps ALL DEPENDS server client ui_files cert client_binary server_binary)

add_subdirectory(tests)

cmake_minimum_required(VERSION 3.16)
project(sfml LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FILE(GLOB Srcs
  source/*.cc
  source/dev/*.cc
  source/lib/*.cc
)
add_executable(sfml ${Srcs})
target_link_libraries(sfml
  PRIVATE sfml-graphics
  PRIVATE sfml-system
  PRIVATE sfml-audio
  PRIVATE sfml-network
)
target_compile_features(sfml PRIVATE cxx_std_17)

if(WIN32)
  add_custom_command(
        TARGET sfml
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sfml>
        VERBATIM)
endif()

install(TARGETS sfml)

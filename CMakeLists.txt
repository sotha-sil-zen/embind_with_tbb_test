cmake_minimum_required(VERSION 3.10)
project(TBBEmscriptenExample)

set(BUILD_TYPE "library")
set(BUILD_TARGET "wasm")
# Set the C++ standard
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_STANDARD 17)
# Include oneTBB
add_subdirectory(oneTBB)

# Emscripten specific settings
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")

    # Add the main executable
    add_executable(lib lib.cpp)
    #add_library(lib main.cpp)
    # Link oneTBB to the main executable
    target_link_libraries(lib PRIVATE TBB::tbb)

    #set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -lembind -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1 -sALLOW_MEMORY_GROWTH -fPIC")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Os -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1 -sALLOW_MEMORY_GROWTH")
    #set_target_properties(lib PROPERTIES LINK_FLAGS "-s ENVIRONMENT=web,worker -s SINGLE_FILE=1 -s MODULARIZE -s 'EXPORT_NAME=DummyCppLibrary' -s WASM=1 -s SIDE_MODULE=1 -s STANDALONE_WASM --no-entry")
    #set_target_properties(lib PROPERTIES LINK_FLAGS "-s ENVIRONMENT=web,worker -s MODULARIZE -s 'EXPORT_NAME=DummyCppLibrary' -s WASM=2 -s SIDE_MODULE=1 -s STANDALONE_WASM --no-entry")
    set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS}\
    -lembind\
    -O3\
    -sASSERTIONS\
    -Wno-unused-command-line-argument\
    -sMODULARIZE\
    -sUSE_PTHREADS=1\
    -sPTHREAD_POOL_SIZE=4\
    -sEXPORT_NAME=GetLyOralCPPCore\
    -sEXPORT_ES6=1\
    -sSINGLE_FILE=1\
    -sEXCEPTION_CATCHING_ALLOWED=[..]\
    -sEXPORTED_RUNTIME_METHODS=[addFunction]\
    -sALLOW_MEMORY_GROWTH\
    -sSTACK_SIZE=400MB\
    -sINITIAL_MEMORY=800MB\
    -sMAXIMUM_MEMORY=2GB\
    --no-entry")

endif()
#emcc -lembind -s WASM=1 -s MODULARIZE=1 -s USE_PTHREADS=1 -sEXPORT_NAME=ChohoMesh -s PROXY_TO_PTHREAD=1 -sALLOW_MEMORY_GROWTH -I../oneTBB/include -L../oneTBB/build/clang_18.0_cxx11_32_relwithdebinfo -ltbb -s EXPORTED_FUNCTIONS='["_main","_parallel_task"]' --no-entry -o main.js ../main.cpp

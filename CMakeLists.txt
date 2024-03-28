cmake_minimum_required(VERSION 3.20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

message(STATUS "Building!")

project(cweb)

if(EMSCRIPTEN)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1"
  )
  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your
                                       # executable to build with the emscripten
                                       # html template so that you can directly
                                       # open it.
  add_compile_definitions(PLATFORM_WEB on)
endif()

add_executable(${PROJECT_NAME} src/main.c)

add_subdirectory(include/${PROJECT_NAME})
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC include)

if(EMSCRIPTEN)
  target_include_directories(
    ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib/web/include)
  target_link_libraries(${PROJECT_NAME}
                        ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib/web/libraylib.a)

endif()

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if(APPLE)
  # RAYLIB
  target_include_directories(
    ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib/macos/include)
  target_link_libraries(
    ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib/macos/libraylib.a)

  target_link_libraries(${PROJECT_NAME} "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

cmake_minimum_required(VERSION 3.0)

project(fty-service-status-systemd-example)

#get dependencies

#get dependencies using pkgconfig
find_package(PkgConfig REQUIRED) 
pkg_check_modules(FTY_SERVICE_STATUS fty-service-status REQUIRED IMPORTED_TARGET )

add_executable(${PROJECT_NAME} src/example.cpp)

target_link_libraries(${PROJECT_NAME}
  PkgConfig::FTY_SERVICE_STATUS
)

if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else ()
  target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
endif()

target_compile_options(${PROJECT_NAME} PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
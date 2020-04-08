cmake_minimum_required(VERSION 3.0)

project(fty-service-status-systemd)

include(GNUInstallDirs)

#options and default value
option(BUILD_DOC "Build documentation" ON)

#create the plugin (library)
add_library(${PROJECT_NAME} SHARED src/systemd.cpp)

target_link_libraries(${PROJECT_NAME}
  #fty-service-status => header only (no cmake package for now)
  systemd
)

target_include_directories(${PROJECT_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else ()
  target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
endif()

target_compile_options(${PROJECT_NAME} PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

#Setup instructions for install
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/service_status_plugins/)

#if build documentation
if(BUILD_DOC)
        #create documentation with doxygen
        find_package(Doxygen)
        if (DOXYGEN_FOUND)
        add_custom_target( doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} doxygen.conf
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Generating API documentation with Doxygen")
        else (DOXYGEN_FOUND)
                message("Doxygen need to be installed to generate the doxygen documentation")
        endif (DOXYGEN_FOUND)
endif()
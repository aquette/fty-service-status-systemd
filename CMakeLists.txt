cmake_minimum_required(VERSION 3.0)

project(fty-service-status-systemd)

include(GNUInstallDirs)

#options and default value
option(BUILD_DOC "Build documentation" ON)
option(BUILD_EXAMPLE "Build example" OFF)

#create the plugin (library)
add_library(${PROJECT_NAME} SHARED src/systemd.cpp)

#set dependencies

#get dependencies using pkgconfig
find_package(PkgConfig REQUIRED)

# keep this because of debian 8...
if(CMAKE_VERSION VERSION_LESS "3.7")
  pkg_check_modules(FTY_SERVICE_STATUS REQUIRED fty-service-status)
  pkg_check_modules(SYSTEM_D REQUIRED libsystemd)

  target_link_libraries(${PROJECT_NAME}
    systemd
  )
else ()
  pkg_check_modules(FTY_SERVICE_STATUS REQUIRED IMPORTED_TARGET fty-service-status)
  pkg_check_modules(SYSTEM_D REQUIRED IMPORTED_TARGET libsystemd)

  target_link_libraries(${PROJECT_NAME}
    PkgConfig::FTY_SERVICE_STATUS
    PkgConfig::SYSTEM_D
  )
endif()


target_include_directories(${PROJECT_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# keep this because of debian 8...
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

#if build example
if(BUILD_EXAMPLE)
        #build example
        add_subdirectory(example)
endif()
cmake_minimum_required(VERSION 3.1)

project(cpphash VERSION 0.0.1 LANGUAGES CXX)


option(CPPHASH_BUILD_TESTS "Build the unit tests when BUILD_TESTING is enabled." ON)
option(CPPHASH_USE_SSE_AVX "Use SSE and AVX instructions" ON)

##
## CONFIGURATION
##
set(CPPHASH_TARGET_NAME               ${PROJECT_NAME})
set(CPPHASH_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include")

##
## TARGET
##
message("Target name: ${CPPHASH_TARGET_NAME}")
add_library(${CPPHASH_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${CPPHASH_TARGET_NAME} ALIAS ${CPPHASH_TARGET_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if (CPPHAS_USE_SSE_AVX)
    target_compile_definitions(
        ${CPPHASH_TARGET_NAME}
        INTERFACE -DCPPHASH_USE_SSE_AVX
    )
endif()

target_include_directories(
    ${CPPHASH_TARGET_NAME}
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(
    ${CPPHASH_TARGET_NAME}
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/external>
)

##
## TESTS
##
include(CTest) #adds option BUILD_TESTING (default ON)

if(BUILD_TESTING AND CPPHASH_BUILD_TESTS)
    enable_testing()
    add_subdirectory(unit_tests)
endif()

##
## INSTALL
##
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

if(DEFINED CMAKE_INSTALL_DATAROOTDIR)
    configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)


    install(EXPORT ${PROJECT_NAME}_Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
endif()

install(DIRECTORY include DESTINATION .)
install(FILES external/wyhash/wyhash.h DESTINATION include/wyhash)

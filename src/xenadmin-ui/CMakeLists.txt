file(GLOB_RECURSE XENADMIN_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.ui" "*.qrc")

qt_add_executable(xenadmin-qt ${XENADMIN_SOURCES})

# Include directories are inherited transitively from xenlib's PUBLIC
# target_include_directories (src/xenlib and src/).
# OpenSSL is also inherited transitively from xenlib's PUBLIC target_link_libraries.

target_link_libraries(xenadmin-qt
    PRIVATE
        xenlib
        Qt6::Widgets
        Qt6::Network
        Qt6::Xml
        Qt6::Charts
)

if(UNIX AND NOT APPLE)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        # Probe freerdp3 first (Ubuntu 24.04+ / Debian 12+), fall back to freerdp2.
        # The qmake build links freerdp-client2 and winpr2 explicitly; pkg-config
        # Libs metadata for both freerdp2 and freerdp3 includes those same libraries,
        # so ${FREERDP_LIBRARIES} is equivalent.
        pkg_check_modules(FREERDP QUIET freerdp3)
        if(NOT FREERDP_FOUND)
            pkg_check_modules(FREERDP QUIET freerdp2)
        endif()
        if(FREERDP_FOUND)
            message(STATUS "FreeRDP found - enabling RDP support")
            target_compile_definitions(xenadmin-qt PRIVATE HAVE_FREERDP)
            target_link_libraries(xenadmin-qt PRIVATE ${FREERDP_LIBRARIES})
            target_include_directories(xenadmin-qt PRIVATE ${FREERDP_INCLUDE_DIRS})
        else()
            message(STATUS "FreeRDP not found - RDP support disabled (VNC-only)")
        endif()
    endif()
endif()

install(TARGETS xenadmin-qt)

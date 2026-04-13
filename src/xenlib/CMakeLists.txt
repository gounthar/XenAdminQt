file(GLOB_RECURSE XENLIB_SOURCES CONFIGURE_DEPENDS "*.cpp")

qt_add_library(xenlib STATIC ${XENLIB_SOURCES})

target_include_directories(xenlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)

# XENLIB_LIBRARY (PRIVATE) enables Q_DECL_EXPORT when building the .a.
# XENLIB_STATIC  (PUBLIC)  tells consumers the library is static so the
# XENLIB_EXPORT macro stays empty — without it, MSVC consumers see
# Q_DECL_IMPORT and fail to link against the static archive.
target_compile_definitions(xenlib
    PRIVATE XENLIB_LIBRARY
    PUBLIC  XENLIB_STATIC
)

target_link_libraries(xenlib
    PUBLIC
        Qt6::Core
        Qt6::Network
        Qt6::Xml
)

option(XENADMIN_NO_CRYPTO "Disable crypto/OpenSSL support" OFF)
if(NOT XENADMIN_NO_CRYPTO)
    if(UNIX AND NOT APPLE)
        find_package(OpenSSL REQUIRED)
        target_link_libraries(xenlib PUBLIC OpenSSL::SSL OpenSSL::Crypto)
    elseif(APPLE)
        # CommonCrypto is part of the macOS system frameworks - no extra linking needed
    elseif(WIN32)
        target_link_libraries(xenlib PUBLIC bcrypt)
    endif()
else()
    target_compile_definitions(xenlib PUBLIC XENADMIN_NO_CRYPTO)
endif()

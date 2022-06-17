INCLUDE(ExternalProject)
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_BINARY_DIR}/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)
SET(ENV{PATH} ${CMAKE_BINARY_DIR}/bin:$ENV{PATH})
SET(ENV{PKG_CONFIG_PATH} ${CMAKE_BINARY_DIR}/lib/pkgconfig)

message(STATUS "libcsv - CMAKE Binary dir: ${CMAKE_BINARY_DIR}")

EXTERNALPROJECT_ADD(
    csv
    # https://sourceforge.net/projects/libcsv/
    URL ${CMAKE_SOURCE_DIR}/vendor/libcsv-3.0.3.tar.gz
    CONFIGURE_COMMAND autoreconf --install > /dev/null 2>&1 || autoreconf && ./configure --prefix=${CMAKE_BINARY_DIR}
    BUILD_IN_SOURCE 1
    URL_MD5 d3307a7bd41d417da798cd80c80aa42a
    )

SET(THIRDPARTY_LIBRARIES libcsv)

file(GLOB_RECURSE GENERIC_SOURCES ./ */*.c)
add_library(asn ${GENERIC_SOURCES})

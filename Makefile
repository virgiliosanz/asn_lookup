##############################################################
#               CMake Project Wrapper Makefile               #
##############################################################

SHELL := /bin/bash
RM    := rm -rfv

all: ./build/Makefile
	@ $(MAKE) -C build

tests:
	@ $(MAKE) test

check: tests

./build/Makefile:
	@ (cd build && cmake ..)

distclean:
	@- (cd build >/dev/null 2>&1 && cmake .. >/dev/null 2>&1)
	@- $(MAKE) --silent -C build clean || true
	@- $(RM) ./build/* CMakeLists.txt.user ./bin/*.dSYM
	@- $(RM) GPATH GRTAGS GTAGS compile_commands.json


ifeq ($(findstring distclean,$(MAKECMDGOALS)),)

    $(MAKECMDGOALS): ./build/Makefile
	@ $(MAKE) -C build $(MAKECMDGOALS)

endif

EPOCH      := $(shell /bin/date +%s)
ORIG_EPOCH := 1

# ----------------------------------------------------------------------
# adjust the following to the location of your Lua include file

INCFLAGS   := -I/usr/include/lua5.1

# ----------------------------------------------------------------------

CC= gcc
AR= ar

CFLAGS= -Os -fPIC $(INCFLAGS)
LDFLAGS= -fPIC

LUAEC25519_O= luaec25519.o randombytes.o tweetnacl.o

luaec25519.so: *.c *.h
	$(CC) -c $(CFLAGS) *.c
	$(CC) -shared $(LDFLAGS) -o luaec25519.so $(LUAEC25519_O)

clean:
	rm -f *.o *.a *.so *.rock *.deb *.changes *.buildinfo

.PHONY: clean

.PHONY: help
help:
	@echo "Usage:"
	@echo "make rock - build Lua rock package"
	@echo "make deb - build Ubuntu package"
	@echo "make clean - remove built packages"

.PHONY: fix-rock
fix-rock:
	mv luaec25519-0.5-*.rockspec luaec25519-0.5-$(EPOCH).rockspec
	sed -E -i "2s/([0-9.]+\-)[0-9]+(.*)$$/\1$(EPOCH)\2/" luaec25519-0.5-*.rockspec

.PHONY: build-rock
build-rock: fix-rock
	luarocks pack *.rockspec

.PHONY: rock
rock: build-rock
	sed -E -i "2s/([0-9.]+\-)[0-9]+(.*)$$/\1$(ORIG_EPOCH)\2/" luaec25519-0.5-*.rockspec
	mv luaec25519-0.5-*.rockspec luaec25519-0.5-$(ORIG_EPOCH).rockspec

.PHONY: fix-deb
fix-deb:
	sed -E -i "1s/([0-9.]+\-)[0-9]+(.*)$$/\1$(EPOCH)\2/" debian/changelog

.PHONY: build-deb
build-deb: fix-deb
	dpkg-buildpackage -D -b -us -uc -tc
	mv ../lua-ec25519_*.* ./

.PHONY: deb
deb: build-deb
	sed -E -i "1s/([0-9.]+\-)[0-9]+(.*)$$/\1$(ORIG_EPOCH)\2/" debian/changelog

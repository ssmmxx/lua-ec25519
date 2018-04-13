# ----------------------------------------------------------------------
# adjust the following to the location of your Lua include file

INCFLAGS= -I../lua/include

# ----------------------------------------------------------------------

CC= gcc
AR= ar

CFLAGS= -Os -fPIC $(INCFLAGS)
LDFLAGS= -fPIC

LUAEC25519_O= luaec25519.o randombytes.o tweetnacl.o

tweetnacl.so:  *.c *.h
	$(CC) -c $(CFLAGS) *.c
	$(CC) -shared $(LDFLAGS) -o luaec25519.so $(LUAEC25519_O)

clean:
	rm -f *.o *.a *.so

.PHONY: clean

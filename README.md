# lua-ec25519

## How to use to generate curve25519 keypairs
Let 'ask' and 'bsk' be repectively Alice and Bob private keys (random 32-byte strings)

For generate use: 
```lua
local curve25519 = require "luaec25519"
ask = curve25519.randombytes(32)
bsk = curve25519.randombytes(32)
```
Alice public key 'apk' is obtained by:
```lua
apk = curve25519.public_key(ask)
```
Similarly for Bob public key 'bpk':
```lua
bpk = curve25519.public_key(bsk)
```
Alice and Bob each have the public key of the other party.

Alice compute a secret sk:
```lua
sk = curve25519.shared_key(ask, bpk)
```

Bob is able to compute the same secret sk as:
```lua
sk = curve25519.shared_key(bsk, apk)
```
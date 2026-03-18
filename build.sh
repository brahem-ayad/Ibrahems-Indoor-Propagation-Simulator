#!/bin/bash

# 1. Compile the library to a TEMPORARY file
cc -o library_tmp.so library.c -shared -fPIC -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# 2. Move it atomically to the final name (prevents partial reads by the Host)
mv library_tmp.so library.so

# 3. Compile the Host (only needs to be done once, usually)
cc -o HotReloading HotReloading.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -rdynamic

echo "Build Complete."


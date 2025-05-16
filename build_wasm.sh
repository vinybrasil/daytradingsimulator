emcc -o build/index.html main.c \
 -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces \
  -Wunused-result -Os -I. -I $RAYLIB_SRC \
   -I $RAYLIB_SRC/external -L. -L $RAYLIB_SRC \
    -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 \
    --shell-file customshell.html $RAYLIB_SRC/web/libraylib.web.a \
    -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]' -s EXPORTED_RUNTIME_METHODS=ccall
emcc -o build/index.html main.c \
 -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces \
  -Wunused-result -Os -I. -I /home/vinybrasil/projects/raylib/raylib/src \
   -I /home/vinybrasil/projects/raylib/raylib/src/external -L. -L /home/vinybrasil/projects/raylib/raylib/src \
    -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 \
    --shell-file /home/vinybrasil/projects/raylib/raylib/src/minshell.html /home/vinybrasil/projects/raylib/raylib/src/web/libraylib.web.a \
    -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]' -s EXPORTED_RUNTIME_METHODS=ccall
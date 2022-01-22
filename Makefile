EMCC := /usr/lib/emscripten/emcc
EMAR := /usr/lib/emscripten/emar

GAME_HEADERS := $(shell find src -name "*.h")
GAME_SOURCES := $(shell find src -name "*.c")
CONTENT_SOURCES := $(shell find src/resources)

$(VERBOSE).SILENT:

.PHONY: all
all: clean release
	@echo "Done"

build:
	mkdir $@

raylib:
	git clone https://github.com/raysan5/raylib.git
	@echo ""

raylib/src/libraylib.a: raylib
	cd raylib/src; $(EMCC) -c rcore.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	cd raylib/src; $(EMCC) -c rshapes.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	cd raylib/src; $(EMCC) -c rtextures.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	cd raylib/src; $(EMCC) -c rtext.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	cd raylib/src; $(EMCC) -c rmodels.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	cd raylib/src; $(EMCC) -c utils.c -Os -Wall -DPLATFORM_WEB
	cd raylib/src; $(EMCC) -c raudio.c -Os -Wall -DPLATFORM_WEB

	cd raylib/src; $(EMAR) rcs libraylib.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o
	@echo ""

build/index.html: raylib/src/libraylib.a $(GAME_HEADERS) $(GAME_SOURCES) $(CONTENT_SOURCES) | build
	$(EMCC) -o build/index.html $(GAME_SOURCES) -Os -Wall raylib/src/libraylib.a -I. -Iraylib/src -L. -Lraylib/src -s USE_GLFW=3 --shell-file src/minshell.html -DPLATFORM_WEB

.PHONY: release
release: build/index.html
	@echo Done

.PHONY: clean
clean:
	if [ -d "raylib" ]; then rm -rf raylib; fi
	if [ -d "build" ]; then rm -rf build; fi
	@echo "Done"

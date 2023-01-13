GCC=x86_64-w64-mingw32-gcc
GPP=x86_64-w64-mingw32-g++

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
ASSETS_DIR = assets

DLL_PATH = C:\cygwin64\usr\x86_64-w64-mingw32\sys-root\mingw\bin
INCLUDE_PATH = /usr/x86_64-w64-mingw32/sys-root/mingw/include

forward-to-backward = $(subst /,\,$1)

subdirs := $(wildcard */)
VPATH = $(subdirs)

INCLUDES = src \
	 src/debugger \
	 src/devices \
	 modules/tms9918/src \
	 modules/65c02/src \
	 modules/lcd/src \
	 thirdparty\imgui \
	 thirdparty\imgui\backends \
	 thirdparty\emu2149	\
	 $(INCLUDE_PATH)/SDL2

INCLUDES := $(INCLUDES:%=-I%)

CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) \
	thirdparty/imgui/imgui.cpp			\
	thirdparty/imgui/imgui_draw.cpp		\
	thirdparty/imgui/imgui_tables.cpp	\
	thirdparty/imgui/imgui_widgets.cpp	\
	thirdparty/imgui/backends/imgui_impl_sdl.cpp	\
	thirdparty/imgui/backends/imgui_impl_sdlrenderer.cpp	
CPP_OBJECTS := $(CPP_SOURCES:%.cpp=$(OBJ_DIR)/%.o)

C_SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/**/**/*.c) \
	thirdparty/emu2149/emu2149.c \
	modules/65c02/src/vrEmu6502.c \
	modules/lcd/src/vrEmuLcd.c \
	modules/tms9918/src/vrEmuTms9918.c \
	modules/tms9918/src/vrEmuTms9918Util.c 

C_OBJECTS := $(C_SOURCES:%.c=$(OBJ_DIR)/%.o)

OUT = hbc56emu

MODULE_CFLAGS=-DVR_LCD_EMU_STATIC=1 \
	      -DVR_TMS9918_EMU_STATIC=1	\
		  -DVR_6502_EMU_STATIC=1

CFLAGS_W = -Wall -Wl,-subsystem,console

build: $(BIN_DIR)/$(OUT).exe

-include $(OBJECTS:.o=.d)
$(C_OBJECTS) : obj/%.o : %.c 
	mkdir -p $(dir $@)
	$(info Compiling $<)
	$(GCC) -g $(MODULE_CFLAGS) $(CFLAGS_W) $(INCLUDES) -I$(INCLUDE_PATH) -c $< -o $@

$(CPP_OBJECTS) : obj/%.o : %.cpp
	mkdir -p $(dir $@)
	$(info Compiling $<)
	$(GPP) -g $(MODULE_CFLAGS) $(CFLAGS_W) $(INCLUDES) -I$(INCLUDE_PATH) -c $< -o $@

OBJECTS = $(C_OBJECTS) $(CPP_OBJECTS)

$(BIN_DIR)/$(OUT).exe: $(OBJECTS) 
	mkdir -p $(BIN_DIR)
	cp $(DLL_PATH)\SDL2.dll bin\SDL2.dll
	cp $(DLL_PATH)\libgcc_s_seh-1.dll bin\libgcc_s_seh-1.dll
	cp $(DLL_PATH)\libstdc++-6.dll bin\libstdc++-6.dll
	cp $(ASSETS_DIR)\imgui.ini $(BIN_DIR)\imgui.ini
	$(GPP) $(CFLAGS_W) $(OBJECTS) -o $(BIN_DIR)/$(OUT) -lmingw32 -lsdl2main -lsdl2

clean:
	$(info Cleaning...)
	rm $(OBJ_DIR) $(BIN_DIR) -r

ifdef FILE
INCLUDES_TEST = $(INCLUDES) -Ithirdparty/acutest/include

TEST_SOURCE_FILE = $(subst \,/,$(FILE))
TEST_INCLUDES_FILE = $(addsuffix .inc, $(basename $(TEST_SOURCE_FILE)))
TEST_OBJECT_FILE = $(addprefix obj\, $(addsuffix .o, $(basename $(TEST_SOURCE_FILE))))

TEST_INCLUDES_SOURCES = $(file < $(TEST_INCLUDES_FILE))
TEST_INCLUDES_OBJECTS := $(TEST_INCLUDES_SOURCES:%.c=$(OBJ_DIR)/%.o)

C_OBJECTS = $(TEST_OBJECT_FILE) $(TEST_INCLUDES_OBJECTS)

$(C_OBJECTS) : obj/%.o : %.c
	mkdir -p $(dir $@)
	$(info Compiling $<)
	$(GCC) -g $(MODULE_CFLAGS) $(CFLAGS_W) $(INCLUDES) -I$(INCLUDE_PATH) -c $(subst \,/,$<) -o $@

test: $(TEST_OBJECT_FILE) $(TEST_INCLUDES_OBJECTS)
	mkdir -p $(dir $(TEST_OBJECT_FILE))
	mkdir -p $(BIN_DIR)
	$(GCC) -g $(MODULE_CFLAGS) $(CFLAGS_W) $(INCLUDES_TEST) -I$(INCLUDE_PATH) -c $(TEST_SOURCE_FILE) -o $(TEST_OBJECT_FILE)
	$(GPP) -g $(CFLAGS_W) $(TEST_OBJECT_FILE) $(TEST_INCLUDES_OBJECTS) -o $(BIN_DIR)/run_tests -lmingw32 -lsdl2main -lsdl2
	$(BIN_DIR)/run_tests.exe
endif

run:
	.\bin\hbc56emu.exe --rom H:\src\my-6502\bin\bios.bin --brk --cfcard H:\src\my-6502\bin\cf_card.bin
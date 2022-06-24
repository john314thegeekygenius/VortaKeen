
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-Wno-incompatible-pointer-types -Wall -O2  \
		-mcpu=arm7tdmi -mtune=arm7tdmi\
		$(ARCH)

CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	$(ARCH)
LDFLAGS	=	$(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lmm -lgba

#---------------------------------------------
# files
#---------------------------------------------

BIN_NAME = VORTKEEN

GB_SRC = src/GBA_Defs.c src/VK_Main.c src/VK_SoundEngine.c src/VK_GraphicsEngine.c src/VK_LevelEngine.c src/VK_InputEngine.c src/VK_GUIEngine.c src/VK_SpriteEngine.c src/VK_ObjectsEngine.c src/VK_MainEngine.c 

GB_GBA = bin/$(BIN_NAME).gba

#-o $(GB_OBJ)

#https://gist.github.com/JShorthouse/bfe49cdfad126e9163d9cb30fd3bf3c2

build:
	arm-none-eabi-gcc crt0.s -O2 $(GB_SRC) -w -mcpu=arm7tdmi -nostartfiles -Tlnkscript
	#mv a.out $(GB_GBA)
	arm-none-eabi-objcopy -v -O binary a.out $(GB_GBA)
	gbafix $(GB_GBA) -t $(BIN_NAME)

all: build


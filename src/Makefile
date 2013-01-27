TARGET = brechen.elf

SRCS_MAIN	= main/main.cpp main/Level.cpp main/SceneManager.cpp main/Game.cpp

SRCS_DRAWABLES	= drawables/Paddle.cpp drawables/Brick.cpp drawables/Ball.cpp drawables/UILabel.cpp \
	drawables/UI.cpp drawables/Border.cpp drawables/Powerup.cpp drawables/LaserBeam.cpp

SRCS_MENUS	= menus/TitleScreen.cpp
			
SRCS = $(SRCS_MAIN) $(SRCS_DRAWABLES) $(SRCS_MENUS)

OBJS = $(SRCS:%.cpp=%.o)

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(TARGET) $(OBJS) romdisk.*

rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS) romdisk.o
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) \
		$(OBJS) romdisk.o $(OBJEXTRA) -ltsunami -lk++ -lparallax -lpng -ljpeg -lkmg -lz -lkosutils -lmp3 -lm $(KOS_LIBS)

romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d romdisk -v

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

bin:
	rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)
	sh-elf-objcopy -O binary $(TARGET) 1ST_READ.BIN
	cp ./1ST_READ.BIN ../release/
	rm -f ./1ST_READ.BIN
		
dist:
	rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)


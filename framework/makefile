################################################################################
#
# Makefile for FATE Framework (Palm OS)
# Wolfgang Ziegler
# August, 2004
#
################################################################################

CCFLAGS = -fno-exceptions

#  Target ALL
all : ./src/PalmDebug/libFATED.a
	@cp  ./src/PalmDebug/libFATED.a ./lib/PalmOS/libFATED.a

#  Target Clean
clean : 
	cd ./src/PalmDebug/ && \
	rm -f *.bin bin.res *~ *.grc *.prc *.o *.ld *.s *.d *.a FATE_APP.prc FATE_APP

#  Rule for building sourcefile FATE_PALMOS.cpp
./src/PalmDebug/FATE_PALMOS.o : src/FATE_PALMOS.cpp include/FMsgPanel.h
	@echo "Compiling FATE_PALMOS.cpp..." && \
	m68k-palmos-g++  -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FATE_PALMOS.o -c src/FATE_PALMOS.cpp -MD $(CCFLAGS)
	@mv  FATE_PALMOS.d ./src/PalmDebug/FATE_PALMOS.d

#  Rule for building sourcefile FSystem.cpp
./src/PalmDebug/FSystem.o : src/FSystem.cpp
	@echo "Compiling FSystem.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FSystem.o -c src/FSystem.cpp -MD $(CCFLAGS)
	@mv  FSystem.d ./src/PalmDebug/FSystem.d

#  Rule for building sourcefile FSystemPalmOS.cpp
./src/PalmDebug/FSystemPalmOS.o : src/FSystemPalmOS.cpp
	@echo "Compiling FSystemPalmOS.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FSystemPalmOS.o -c src/FSystemPalmOS.cpp -MD $(CCFLAGS)
	@mv  FSystemPalmOS.d ./src/PalmDebug/FSystemPalmOS.d

#  Rule for building sourcefile FateApp.cpp
./src/PalmDebug/FateApp.o : src/FateApp.cpp include/FateApp.h
	@echo "Compiling FateApp.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FateApp.o -c src/FateApp.cpp -MD $(CCFLAGS)
	@mv  FateApp.d ./src/PalmDebug/FateApp.d

#  Rule for building sourcefile IFateContainer.cpp
./src/PalmDebug/IFateContainer.o : src/IFateContainer.cpp
	@echo "Compiling IFateContainer.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/IFateContainer.o -c src/IFateContainer.cpp -MD $(CCFLAGS)
	@mv  IFateContainer.d ./src/PalmDebug/IFateContainer.d

#  Rule for building sourcefile FBitmap_PalmOS.cpp
CFBitmap : ./src/PalmDebug/FBitmap_PalmOS.o 
./src/PalmDebug/FBitmap_PalmOS.o : src/FBitmap_PalmOS.cpp
	@echo "Compiling FBitmap_PalmOS.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FBitmap_PalmOS.o -c src/FBitmap_PalmOS.cpp -MD $(CCFLAGS)
	@mv  FBitmap_PalmOS.d ./src/PalmDebug/FBitmap_PalmOS.d

#  Rule for building sourcefile IFSocket_PalmOS.cpp
./src/PalmDebug/IFSocket_PalmOS.o : src/IFSocket_PalmOS.cpp
	@echo "Compiling IFSocket_PalmOS.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/IFSocket_PalmOS.o -c src/IFSocket_PalmOS.cpp -MD $(CCFLAGS)
	@mv  IFSocket_PalmOS.d ./src/PalmDebug/IFSocket_PalmOS.d

#  Rule for building sourcefile FUDPSocket_PalmOS.cpp
CFUDPSocket : ./src/PalmDebug/FUDPSocket_PalmOS.o ./include/FUDPSocket_PalmOS.h
./src/PalmDebug/FUDPSocket_PalmOS.o : src/FUDPSocket_PalmOS.cpp
	@echo "Compiling FUDPSocket_PalmOS.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FUDPSocket_PalmOS.o -c src/FUDPSocket_PalmOS.cpp -MD $(CCFLAGS)
	@mv  FUDPSocket_PalmOS.d ./src/PalmDebug/FUDPSocket_PalmOS.d

#  Rule for building sourcefile FSocket_PalmOS.cpp
./src/PalmDebug/FSocket_PalmOS.o : src/FSocket_PalmOS.cpp
	@echo "Compiling FSocket_PalmOS.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FSocket_PalmOS.o -c src/FSocket_PalmOS.cpp -MD $(CCFLAGS)
	@mv  FSocket_PalmOS.d ./src/PalmDebug/FSocket_PalmOS.d

#  Rule for building sourcefile FServer.cpp
./src/PalmDebug/FServer.o : src/FServer.cpp
	@echo "Compiling FServer.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FServer.o -c src/FServer.cpp -MD $(CCFLAGS)
	@mv  FServer.d ./src/PalmDebug/FServer.d

#  Rule for building sourcefile FLabel.cpp
CFLabel : ./src/PalmDebug/FLabel.o 
./src/PalmDebug/FLabel.o : src/FLabel.cpp ./include/FLabel.h
	@echo "Compiling FLabel.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FLabel.o -c src/FLabel.cpp -MD $(CCFLAGS)
	@mv  FLabel.d ./src/PalmDebug/FLabel.d

#  Rule for building sourcefile FLabel.cpp
CFButton : ./src/PalmDebug/FButton.o 
./src/PalmDebug/FButton.o : src/FButton.cpp ./include/FButton.h
	@echo "Compiling FButton.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FButton.o -c src/FButton.cpp -MD $(CCFLAGS)
	@mv  FButton.d ./src/PalmDebug/FButton.d

#  Rule for building sourcefile FLabel.cpp
CFOffscreenRenderer : ./src/PalmDebug/FOffscreenRenderer.o 
./src/PalmDebug/FOffscreenRenderer.o : src/FOffscreenRenderer.cpp ./include/FOffscreenRenderer.h
	@echo "Compiling FOffscreenRenderer.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -Iinclude -o./src/PalmDebug/FOffscreenRenderer.o -c src/FOffscreenRenderer.cpp -MD $(CCFLAGS)
	@mv  FOffscreenRenderer.d ./src/PalmDebug/FOffscreenRenderer.d

#  Rule for Linking the project
./src/PalmDebug/libFATED.a : ./src/PalmDebug/FATE_PALMOS.o \
./src/PalmDebug/FateApp.o ./src/PalmDebug/FSystem.o \
./src/PalmDebug/FSystemPalmOS.o ./src/PalmDebug/FBitmap_PalmOS.o ./src/PalmDebug/IFSocket_PalmOS.o \
./src/PalmDebug/IFateContainer.o ./src/PalmDebug/FSocket_PalmOS.o ./src/PalmDebug/FUDPSocket_PalmOS.o \
./src/PalmDebug/FServer.o \
./src/PalmDebug/FLabel.o \
./src/PalmDebug/FButton.o
	@echo "Linking..." && \
	m68k-palmos-ar rcs ./src/PalmDebug/libFATED.a \
./src/PalmDebug/FSystemPalmOS.o \
./src/PalmDebug/FSystem.o \
./src/PalmDebug/FBitmap_PalmOS.o \
./src/PalmDebug/IFSocket_PalmOS.o \
./src/PalmDebug/IFateContainer.o \
./src/PalmDebug/FUDPSocket_PalmOS.o \
./src/PalmDebug/FSocket_PalmOS.o \
./src/PalmDebug/FServer.o \
./src/PalmDebug/FLabel.o \
./src/PalmDebug/FButton.o \
./src/PalmDebug/FateApp.o \
./src/PalmDebug/FATE_PALMOS.o \
#m68k-palmos-objdump --section-headers ./src/PalmDebug/libFATED.a




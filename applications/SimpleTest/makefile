################################################################################
#
# Makefile for "SimpleApplication" (Palm OS)
#
################################################################################

CCFLAGS = -fno-exceptions

#  Target ALL
all : ./PalmDebug/SIMPLE_APP.prc

#  Target Clean
clean : 
	cd ./PalmDebug/ && \
	rm -f *.bin bin.res *~ *.grc *.prc *.o *.ld *.s *.d SIMPLE_APP.prc SIMPLE_APP

#  Rule for building sourcefile Simple.cpp
./PalmDebug/Simple.o : Simple.cpp Simple.h
	@echo "Compiling Simple.cpp..." && \
	m68k-palmos-g++ -palmos5r4 -g  -o./PalmDebug/Simple.o -c Simple.cpp -MD $(CCFLAGS)
	@mv  Simple.d ./PalmDebug/Simple.d


#  Rule for building assembler stub
./PalmDebug/SimpleApp-sections.o : ./PalmDebug/SimpleApp-sections.s
	@echo "Building Assembler Stub..." && \
	m68k-palmos-gcc -palmos5r4 -o ./PalmDebug/SimpleApp-sections.o -c ./PalmDebug/SimpleApp-sections.s

#  Rule for building linker script
./PalmDebug/SimpleApp-sections.s : SimpleApp.def
	@echo "Building Linker Script..." && \
	cd ./PalmDebug/ && multigen ../SimpleApp.def


#  Rule for Linking the project
./PalmDebug/SIMPLE_APP : ../../framework/lib/PalmOS/libFATED.a ./PalmDebug/Simple.o ./PalmDebug/SimpleApp-sections.o
	@echo "Linking..." && \
	m68k-palmos-gcc -palmos5r4 -o ./PalmDebug/SIMPLE_APP ./PalmDebug/Simple.o ../../framework/lib/PalmOS/libFATED.a \
./PalmDebug/SimpleApp-sections.o \
./PalmDebug/SimpleApp-sections.ld  && \
m68k-palmos-objdump --section-headers ./PalmDebug/SIMPLE_APP


#  Rule for building PRCFile
./PalmDebug/SIMPLE_APP.prc : ./PalmDebug/Simple.o ./PalmDebug/SIMPLE_APP ./PalmDebug/SimpleApp-sections.o
	@echo "Building program file ./PalmDebug/SIMPLE_APP.prc..." && \
	cd ./PalmDebug/ && \
	build-prc -t appl -o SIMPLE_APP.prc ../SimpleApp.def SIMPLE_APP

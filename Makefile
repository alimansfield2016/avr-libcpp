NAME = c++

PROGRAM = ${NAME}

ASMDIR = asm
OBJDIR = obj
SRCDIR = src

SRCEXT = cpp
CSRCEXT = c
ASMEXT = S
OBJEXT = o
LIBEXT = a
BINEXT = elf
HEXEXT = hex


SRC += ${wildcard ${SRCDIR}/*.${SRCEXT}}
SRC += ${wildcard ${SRCDIR}/**/*.${SRCEXT}}
SRC += ${wildcard ${SRCDIR}/**/**/*.${SRCEXT}}
SRC += ${wildcard ${SRCDIR}/**/**/**/*.${SRCEXT}}
CSRC += ${wildcard ${SRCDIR}/*.${CSRCEXT}}
CSRC += ${wildcard ${SRCDIR}/**/*.${CSRCEXT}}
CSRC += ${wildcard ${SRCDIR}/**/**/*.${CSRCEXT}}
CSRC += ${wildcard ${SRCDIR}/**/**/**/*.${CSRCEXT}}
ASM += ${wildcard ${SRCDIR}/*.${ASMEXT}}
ASM += ${wildcard ${SRCDIR}/**/*.${ASMEXT}}
ASM += ${wildcard ${SRCDIR}/**/**/*.${ASMEXT}}
ASM += ${wildcard ${SRCDIR}/**/**/**/*.${ASMEXT}}

OBJ += ${SRC:${SRCDIR}/%.${SRCEXT}=${OBJDIR}/%.${OBJEXT}}
OBJ += ${CSRC:${SRCDIR}/%.${CSRCEXT}=${OBJDIR}/%.${OBJEXT}}
OBJ += ${ASM:${SRCDIR}/%.${ASMEXT}=${OBJDIR}/%.${OBJEXT}}

WARNINGS += all pedantic extra shadow no-overflow no-volatile
STANDARD = c++20
OPTIMISATION = 2
DEFINES += __AVR_ATmega644P__ F_CPU=12000000 #ENABLE_INTERRUPTS
LIBRARIES += #atmega644p c c++ 
WHOLE_LIBRARIES += 

INCLUDEPATH += ${PRJDIR}/avr-libc++/hdr/ /usr/lib/avr/include/ ${PRJDIR}/libusb++/hdr/
INC = ${INCLUDEPATH:%=-I%}
LIBDIR += . /usr/local/avr/lib/avr5/ ${PRJDIR}/avr-libc++/ ${PRJDIR}/libusb++/
LIB = ${LIBDIR:%=-L%} ${LIBRARIES:%=-l%} -Wl,--whole-archive ${WHOLE_LIBRARIES:%=-l%} -Wl,--no-whole-archive

CXX_COMPILE_FLAGS = -c -std=${STANDARD} ${WARNINGS:%=-W%} -O${OPTIMISATION} ${INC} ${DEFINES:%=-D%} -ggdb
C_COMPILE_FLAGS = -c ${WARNINGS:%=-W%} -O${OPTIMISATION} ${INC} ${DEFINES:%=-D%} -ggdb
ASSEMBLE_FLAGS = -c -mmcu=atmega644p ${INC} ${DEFINES:%=-D%}
ARCHIVE_FLAGS = rcs
LINK_FLAGS = ${LIB} -Xlinker --defsym=__DATA_REGION_ORIGIN__=0x800100

COMPILE = avr-gcc-10.1.0
ASSEMBLE = avr-as
ARCHIVE = avr-ar
LINK = ${COMPILE}
MKDIR = mkdir -p



all: ${PROGRAM}


debug:
	@echo "SRC:"
	@echo ${SRC}
	@echo "OBJ:"
	@echo ${OBJ}



obj: ${OBJ}
${OBJDIR}/%.${OBJEXT}: ${SRCDIR}/%.${SRCEXT}
	${MKDIR} ${@D}
	${COMPILE} ${CXX_COMPILE_FLAGS} -o $@ $<

${OBJDIR}/%.${OBJEXT}: ${SRCDIR}/%.${CSRCEXT}
	${MKDIR} ${@D}
	${COMPILE} ${C_COMPILE_FLAGS} -o $@ $<
${OBJDIR}/%.${OBJEXT}: ${SRCDIR}/%.${ASMEXT}
	${MKDIR} ${@D}
	${COMPILE} ${ASSEMBLE_FLAGS} -o $@ $<

#make assembly files
asm: ${ASM}
${ASMDIR}/%.${ASMEXT}: ${SRCDIR}/%.${SRCEXT}
	${MKDIR} ${@D}
	${COMPILE} -S ${COMPILE_FLAGS} -o $@ $<


#make executeable file
${PROGRAM}.${BINEXT}: ${OBJ}
	${LINK}  -o $@ $< /usr/lib/avr/lib/avr5/crtatmega644p.o ${LINK_FLAGS}

lib${PROGRAM}.${LIBEXT} : ${OBJ}
	${ARCHIVE} ${ARCHIVE_FLAGS} $@ $^


${PROGRAM}.${HEXEXT} : ${PROGRAM}.${BINEXT}
	avr-objcopy -O ihex $< $@

${PROGRAM}: ${PROGRAM}.${BINEXT}
lib: lib${PROGRAM}.${LIBEXT}

hex: ${PROGRAM}.${HEXEXT}

upload: ${PROGRAM}.${HEXEXT}
	avrdude -c usbasp -p m644p -U flash:w:$<


clean:
	rm -rf ${ASMDIR}/* ${OBJ} ${PROGRAM}*

size: ${PROGRAM}.${BINEXT}
	avr-size $<

nm: ${PROGRAM}.${BINEXT}
	avr-nm -C -S --size-sort -t d $<
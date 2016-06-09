TOP       = .
INCLUDE   = -I. 


CC=		gcc

# Options to be passed to your C++ compiler:
CCOPTIONS=	-fpic -D_HPUX -O2


# Compile flags:
CPPFLAGS= 	$(CCOPTIONS) $(INCLUDE)
CCFLAGS = 	-G

ALIB    =       libMibParse.a
SLIB	= 	libMibParse.so



OBJS	=	EnumNode.o\
		MibLoad.o\
		MibNode.o\
		MibParser.o\
		MibTree.o\
		Node.o\
		ValueRangeNode.o\
		count.o\
		doImports.o\
		doMibName.o\
		doModuleIdentity.o\
		doNotificationTypes.o\
		doObjectIdentity.o\
		doObjectIds.o\
		doObjectTypes.o\
		doTrapTypes.o

.SUFFIXES: .cpp .C .cc .cxx .o

default: all

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<


all:	mibparsea mibparses

mibparsea:	$(OBJS)
	ld -r -o $(ALIB) $(OBJS)

mibparses:	$(OBJS)
	ld -b -o $(SLIB) $(OBJS)

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) $(ALIB)
	$(RM) $(SLIB)


CFLAGS = -L/opt/FSFlibg++/lib 
CPPFLAGS= -fPIC -D_USE_PACKETLIB -D_IRIX
CXX=/usr/freeware/bin/gcc
CXXFLAGS= 

# The command to create a shared library.  This command will be used
# in this manner : $(LDSHARED) <libname> <objects> so make sure that
# whatever argument is needed to set the resulting library name comes
# last and that the command takes a series of objects as the final
# argument.
LDSHARED = $(CXX) $(CFLAGS) -shared -o

LDSTATIC = ar rv

# Command to build the dependencies file.  This command must be structured
# in a manner to take a series of source files and write the data to
# standard output.
MKDEPEND = /usr/freeware/bin/g++ -M $(CPPFLAGS)

SRCS = EnumNode.cpp \
MibLoad.cpp\
MibNode.cpp\
MibParser.cpp\
MibTree.cpp\
Node.cpp\
ValueRangeNode.cpp\
count.cpp\
doImports.cpp\
doMibName.cpp\
doModuleIdentity.cpp\
doNotificationTypes.cpp\
doObjectIdentity.cpp\
doObjectIds.cpp\
doObjectTypes.cpp\
doTrapTypes.cpp


OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .cpp .o
.cpp.o: 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

lib: objs
	$(LDSTATIC) libMibParse.a $(OBJS)
	$(LDSHARED) libMibParse.so $(OBJS)

objs: $(OBJS)

clean:
	$(RM) *.o *~ *.so *.a

.depend : $(SRCS)
	$(MKDEPEND) $(SRCS) > .depend
include .depend



TOP       = .
INCLUDE   = -I. 


# CCC=		CC
CCC=		gcc

# Options to be passed to your C++ compiler:
CCOPTIONS=	-g -D_USE_PACKETLIB -O2


# Compile flags:
CPPFLAGS= 	$(CCOPTIONS) $(INCLUDE)
CCFLAGS = 	-G

ALIB    =       libPktLib.a
SLIB	= 	libPktLib.so



OBJS	=	CounterVarbind.o\
		GaugeVarbind.o\
		GenericPacket.o\
		IntVarbind.o\
		IpAddrVarbind.o\
		OidVarbind.o\
		OpaqueVarbind.o\
		Packet.o\
		ScalarVarbind.o\
		SnmpException.o\
                Scalar64Varbind.o\
                Counter64Varbind.o\
		SnmpParser.o\
		StringVarbind.o\
		TimeTickVarbind.o\
		V1GetNextPdu.o\
		V1GetPdu.o\
		V1Pdu.o\
		V1ResponsePdu.o\
		V1SetPdu.o\
		V1TrapPdu.o\
		V2TrapPdu.o\
		Varbind.o\
		VarbindList.o\
		VbPair.o\
		V2Notification.o\
		InformPdu.o\
		Udp.o\
		UdpClient.o\
		UdpServer.o

.SUFFIXES: .cpp .C .cc .cxx .o

default: all

%.o: %.cpp
	$(CCC) $(CPPFLAGS) -c -o $@ $<


all:	pktliba pktlibs

pktliba:	$(OBJS)
	ld -r -s -o $(ALIB) $(OBJS)

pktlibs:	$(OBJS)
	ld -dy -G -s -o $(SLIB) $(OBJS)

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) $(ALIB)
	$(RM) $(SLIB)


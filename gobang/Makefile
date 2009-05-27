CC	= gcc
CFLAGS =	-g -Wall -fmessage-length=0

OBJS =		globalconst.o \
			basetypes.o \
			hashsrv.o \
			basestat.o \
			advstat.o \
			timer.o \
			enginetypes.o \
			transposition.o \
			historyheuristic.o \
			moveheuristic.o \
			forbid.o \
			evaluator.o \
			expansion.o \
			abengine.o \
			mtdfengine.o \
			engine.o \
			simpleshell.o
			
DLLOBJS	=	$(OBJS) \
			gobang_JNIAdapter.c

LIBS =

TARGET =	gobang

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

evaltest:
	$(CC) globalconst.c basetypes.c basestat.c advstat.c evaluator.c  advevaltest.c -o evaltest

clean:
	rm -f $(OBJS) $(TARGET)

dll:
	gcc -Wl,--kill-at -I "c:\program files\java\jdk1.6.0_03\include" \
			-I "c:\program files\java\jdk1.6.0_03\include\win32" \
			-shared -o JNIAdapter.dll $(DLLOBJS)
			
so:
	gcc -I "/usr/lib/jvm/java-6-sun-1.6.0.10/include" \
			-I "/usr/lib/jvm/java-6-sun-1.6.0.10/include/linux" \
			-shared -o libJNIAdapter.so $(DLLOBJS)			

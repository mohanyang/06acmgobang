CFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		globalconst.o \
			basetypes.o \
			basestat.o \
			timer.o \
			enginetypes.o \
			transposition.o \
			evaluator.o \
			expansion.o \
			abengine.o \
			mtdfengine.o \
			engine.o \
			simpleshell.o

LIBS =

TARGET =	gobang

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

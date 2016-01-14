LIBEVENT=/opt/libevent
CFLAGS =	-I$(LIBEVENT)/include -Wall
LIBS =		$(LIBEVENT)/lib/libevent.a -lrt

chat-server: chat-server.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f chat-server *~


OBJS = blackjack.o auth.o account.o misc.o deck.o fdutil.o md5/md5.o
CFLAGS = -Wall -O2

all: blackjack

blackjack: $(OBJS)
	$(CC) $(OBJS) -o blackjack

md5/md5.o: md5/md5.c md5/md5.h
	$(CC) $(CFLAGS) -c md5/md5.c -o md5/md5.o

deck.o: deck.h
fdutil.o: fdutil.h
account.o: account.h
auth.o: auth.h
misc.o: misc.h

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f blackjack *.o md5/md5.o


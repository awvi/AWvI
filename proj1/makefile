################################################################################
#                                                                              #
# Makefile                                                                     #
#                                                                              #
################################################################################

################################################################################

CC	=	gcc
SRC	=	main.c nextion.c dht.c weat.c jsmn.c
CFLAGS	= -Wall -L ../../librequests/build -std=gnu99 -I ../../librequests/include
LDLIBS    = -lwiringPi -lpthread -lm -lrequests -lcurl
OBJS	=	$(SRC:.c=.o)

################################################################################

BIN	=	main

all:		 $(BIN)

$(BIN):	$(OBJS)
		$(CC) -o $(BIN) $(OBJS) $(LDLIBS) $(CFLAGS)
clean:
		rm -f $(OBJS)
fclean:		clean
		rm -f $(BIN)
re: fclean all

################################################################################

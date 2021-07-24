EXEC = a.out
CC = g++
CFLAGS = -c -Wall
# $(EXEC) has the value of shell variable EXEC, which is run.
# run depends on the files inputbuf.o, parser.o, and lexer.o
$(EXEC) :lexer.o inputbuf.o parser.o
# run is created by the command g++ -o run lexer.o inputbuf.o
# note that the TAB before $(CC) is REQUIRED...
	$(CC) -o $(EXEC) lexer.o inputbuf.o parser.o

# lexer.o depends on the files lexer.h lexer.cc
lexer.o: lexer.cc
# lexer.o is created by the command g++ -c -Wall lexer.cc
# note that the TAB before $(CC) is REQUIRED...
	$(CC) $(CFLAGS) lexer.cc
	
inputbuf.o  :inputbuf.h inputbuf.cc
	$(CC) $(CFLAGS) inputbuf.cc

parser.o  :parser.h parser.cc
	$(CC) $(CFLAGS) parser.cc

clean	:
	:rm *.o
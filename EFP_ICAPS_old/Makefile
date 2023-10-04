# Makefile

OBJS	= bison.o lex.o main.o reader.o action.o planner.o kstate.o kedge.o kripke.o planninggraph.o

CC	= clang++
CFLAGS	= -g -Wall -ansi -Wno-c++11-extensions --std=c++11

cpa+:		$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o cpa+ -ll

lex.o:		lex.c
		$(CC) $(CFLAGS) -c lex.c -o lex.o

lex.c:		lcp.lex 
		flex lcp.lex
		cp lex.yy.c lex.c

bison.o:	bison.c
		$(CC) $(CFLAGS) -c bison.c -o bison.o

bison.c:	lcp.y
		bison -d -v lcp.y
		cp lcp.tab.c bison.c
		cmp -s lcp.tab.h tok.h || cp lcp.tab.h tok.h

main.o:	main.cpp planner.h define.h 
	echo "#define BUILT_DATE \"`date`\"" > built_date
	cat built_date main.cpp > main.temp.cpp
	$(CC) $(CFLAGS) -c main.temp.cpp -o main.o
	rm main.temp.cpp

reader.o: reader.cpp reader.h  define.h
	$(CC) $(CFLAGS) -c reader.cpp -o reader.o

action.o: action.cpp action.h  define.h
	$(CC) $(CFLAGS) -c action.cpp -o action.o

kstate.o: kstate.cpp kstate.h define.h kedge.h
	$(CC) $(CFLAGS) -c kstate.cpp -o kstate.o

kedge.o: kedge.cpp kedge.h define.h kstate.h
	$(CC) $(CFLAGS) -c kedge.cpp -o kedge.o

kripke.o: kripke.cpp kripke.h define.h 
	$(CC) $(CFLAGS) -c kripke.cpp -o kripke.o

planninggraph.o: define.h planninggraph.cpp planninggraph.h
	$(CC) $(CFLAGS) -c planninggraph.cpp -o planninggraph.o
	
planner.o: planner.cpp planner.h  state.h define.h reader.h planninggraph.h
	$(CC) $(CFLAGS) -c planner.cpp -o planner.o

lex.o yac.o main.o	: 
lex.o main.o		: 

clean:
	rm -f *.o *~ lex.c lex.yy.c bison.c tok.h lcp.tab.c lcp.tab.h lcp.output

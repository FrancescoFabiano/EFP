# Makefile
OBJS	=	$(BUILD_DIR)/bison.o $(BUILD_DIR)/lex.o $(BUILD_DIR)/main.o \
			$(BUILD_DIR)/reader.o $(BUILD_DIR)/action.o $(BUILD_DIR)/planner.o \
			$(BUILD_DIR)/kstate.o $(BUILD_DIR)/kedge.o $(BUILD_DIR)/kripke.o \
			$(BUILD_DIR)/planninggraph.o

CC	= g++
CFLAGS	= -g -Wall -ansi -std=c++11

dir_guard = @mkdir -p $(@D)
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib
INCLUDE_DIR = include
PARSE_DIR = parse

$(BIN_DIR)/efp.out:		$(OBJS)
		$(dir_guard)
		$(CC) $(CFLAGS) $(OBJS) -o $(BIN_DIR)/efp.out -ll

$(BUILD_DIR)/lex.o:		$(BUILD_DIR)/lex.c
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(BUILD_DIR)/lex.c -o $(BUILD_DIR)/lex.o

$(BUILD_DIR)/lex.c:		$(INCLUDE_DIR)/$(PARSE_DIR)/lcp.lex
		$(dir_guard)
		flex $(INCLUDE_DIR)/$(PARSE_DIR)/lcp.lex
		cp lex.yy.c $(BUILD_DIR)/lex.c
		mv lex.yy.c $(BUILD_DIR)/

$(BUILD_DIR)/bison.o:	$(BUILD_DIR)/bison.c
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(BUILD_DIR)/bison.c -o $(BUILD_DIR)/bison.o

$(BUILD_DIR)/bison.c:	$(INCLUDE_DIR)/$(PARSE_DIR)/lcp.y
		$(dir_guard)
		bison -d -v $(INCLUDE_DIR)/$(PARSE_DIR)/lcp.y
		cp lcp.tab.c $(BUILD_DIR)/bison.c
		cmp -s lcp.tab.h $(BUILD_DIR)/tok.h || cp lcp.tab.h $(BUILD_DIR)/tok.h
		mv lcp.* $(BUILD_DIR)/

$(BUILD_DIR)/main.o:	src/main.cpp $(INCLUDE_DIR)/planner.h $(INCLUDE_DIR)/define.h
		$(dir_guard)
		echo "#define BUILT_DATE \"`date`\"" > $(BUILD_DIR)/built_date
		cat $(BUILD_DIR)/built_date src/main.cpp > $(BUILD_DIR)/main.temp.cpp
		$(CC) $(CFLAGS) -c $(BUILD_DIR)/main.temp.cpp -o $(BUILD_DIR)/main.o
		rm $(BUILD_DIR)/main.temp.cpp

$(BUILD_DIR)/reader.o: $(INCLUDE_DIR)/reader.cpp $(INCLUDE_DIR)/reader.h  $(INCLUDE_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/reader.cpp -o $(BUILD_DIR)/reader.o

$(BUILD_DIR)/action.o: $(INCLUDE_DIR)/action.cpp $(INCLUDE_DIR)/action.h  $(INCLUDE_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/action.cpp -o $(BUILD_DIR)/action.o

$(BUILD_DIR)/kstate.o: $(INCLUDE_DIR)/kstate.cpp $(INCLUDE_DIR)/kstate.h $(INCLUDE_DIR)/define.h $(INCLUDE_DIR)/kedge.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/kstate.cpp -o $(BUILD_DIR)/kstate.o

$(BUILD_DIR)/kedge.o: $(INCLUDE_DIR)/kedge.cpp $(INCLUDE_DIR)/kedge.h $(INCLUDE_DIR)/define.h $(INCLUDE_DIR)/kstate.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/kedge.cpp -o $(BUILD_DIR)/kedge.o

$(BUILD_DIR)/kripke.o: $(INCLUDE_DIR)/kripke.cpp $(INCLUDE_DIR)/kripke.h $(INCLUDE_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/kripke.cpp -o $(BUILD_DIR)/kripke.o

$(BUILD_DIR)/planninggraph.o: $(INCLUDE_DIR)/planninggraph.cpp $(INCLUDE_DIR)/define.h $(INCLUDE_DIR)/planninggraph.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/planninggraph.cpp -o $(BUILD_DIR)/planninggraph.o
	
$(BUILD_DIR)/planner.o: $(INCLUDE_DIR)/planner.cpp $(INCLUDE_DIR)/planner.h  $(INCLUDE_DIR)/state.h $(INCLUDE_DIR)/define.h $(INCLUDE_DIR)/reader.h $(INCLUDE_DIR)/planninggraph.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/planner.cpp -o $(BUILD_DIR)/planner.o

lex.o yac.o main.o	: 
lex.o main.o		: 

clean:
	rm -f $(BIN_DIR)/*.out \
	$(BUILD_DIR)/*.o \
	*~ $(BUILD_DIR)/lex.c $(BUILD_DIR)/lex.yy.c $(BUILD_DIR)/bison.c $(BUILD_DIR)/tok.h $(BUILD_DIR)/lcp.tab.c $(BUILD_DIR)/lcp.tab.h $(BUILD_DIR)/lcp.efp.output
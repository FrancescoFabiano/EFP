# Makefile
OBJS	=	$(BUILD_DIR)/bison.o $(BUILD_DIR)/lex.o $(BUILD_DIR)/main.o \
			$(BUILD_DIR)/belief_formula.o $(BUILD_DIR)/proposition.o $(BUILD_DIR)/domain.o \
			$(BUILD_DIR)/grounder.o $(BUILD_DIR)/printer.o \
			$(BUILD_DIR)/action.o \
			$(BUILD_DIR)/reader.o

CC	= g++
CFLAGS	= -g -Wall -ansi -std=c++14

dir_guard = @mkdir -p $(@D)
BUILD_DIR = build
BIN_DIR = bin

LIB_DIR = lib
SRC_DIR = src
INCLUDE_DIR = include

HEURISTIC_DIR = heuristic
INTERFACES_DIR = interfaces
PARSE_DIR = parse
STATES_DIR = states
UTILITIES_DIR = utilities


#-----------------------------------PARSE FILES-----------------------------------#

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

#-----------------------------------MAIN FILE-----------------------------------#
$(BUILD_DIR)/main.o:	$(SRC_DIR)/main.cpp $(INCLUDE_DIR)/$(UTILITIES_DIR)/reader.h $(INCLUDE_DIR)/$(UTILITIES_DIR)/domain.h
		$(dir_guard)
		echo "#define BUILT_DATE \"`date`\"" > $(BUILD_DIR)/built_date
		cat $(BUILD_DIR)/built_date $(SRC_DIR)/main.cpp > $(BUILD_DIR)/main.temp.cpp
		$(CC) $(CFLAGS) -c $(BUILD_DIR)/main.temp.cpp -o $(BUILD_DIR)/main.o
		rm $(BUILD_DIR)/main.temp.cpp
		
		
#-----------------------------------INTERFACES-----------------------------------#
$(BUILD_DIR)/action.o: $(INCLUDE_DIR)/$(INTERFACES_DIR)/action.cpp $(INCLUDE_DIR)/$(INTERFACES_DIR)/action.h \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/grounder.h \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/proposition.h $(INCLUDE_DIR)/$(UTILITIES_DIR)/belief_formula.h \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/$(INTERFACES_DIR)/action.cpp -o $(BUILD_DIR)/action.o
		
		
#-----------------------------------INCLUDE FILES-----------------------------------#
$(BUILD_DIR)/printer.o: $(INCLUDE_DIR)/$(UTILITIES_DIR)/printer.cpp $(INCLUDE_DIR)/$(UTILITIES_DIR)/printer.h \
						 $(INCLUDE_DIR)/$(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/$(UTILITIES_DIR)/printer.cpp -o $(BUILD_DIR)/printer.o


$(BUILD_DIR)/grounder.o: $(INCLUDE_DIR)/$(UTILITIES_DIR)/grounder.cpp $(INCLUDE_DIR)/$(UTILITIES_DIR)/grounder.h \
#						 $(INCLUDE_DIR)/$(INTERFACES_DIR)/action.h \
						 $(INCLUDE_DIR)/$(UTILITIES_DIR)/define.h $(INCLUDE_DIR)/$(UTILITIES_DIR)/printer.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/$(UTILITIES_DIR)/grounder.cpp -o $(BUILD_DIR)/grounder.o

$(BUILD_DIR)/proposition.o: $(INCLUDE_DIR)/$(UTILITIES_DIR)/proposition.cpp $(INCLUDE_DIR)/$(UTILITIES_DIR)/proposition.h \
							$(INCLUDE_DIR)/$(UTILITIES_DIR)/belief_formula.h \
							$(INCLUDE_DIR)/$(UTILITIES_DIR)/define.h $(INCLUDE_DIR)/$(UTILITIES_DIR)/printer.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/$(UTILITIES_DIR)/proposition.cpp -o $(BUILD_DIR)/proposition.o
		
$(BUILD_DIR)/belief_formula.o: $(INCLUDE_DIR)/$(UTILITIES_DIR)/belief_formula.cpp $(INCLUDE_DIR)/$(UTILITIES_DIR)/belief_formula.h \
							   $(INCLUDE_DIR)/$(UTILITIES_DIR)/grounder.h \
							   $(INCLUDE_DIR)/$(UTILITIES_DIR)/define.h $(INCLUDE_DIR)/$(UTILITIES_DIR)/printer.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/$(UTILITIES_DIR)/belief_formula.cpp -o $(BUILD_DIR)/belief_formula.o

$(BUILD_DIR)/reader.o: $(INCLUDE_DIR)/$(UTILITIES_DIR)/reader.cpp $(INCLUDE_DIR)/$(UTILITIES_DIR)/reader.h \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/belief_formula.h  \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/define.h $(INCLUDE_DIR)/$(UTILITIES_DIR)/printer.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/$(UTILITIES_DIR)/reader.cpp -o $(BUILD_DIR)/reader.o
		
$(BUILD_DIR)/domain.o: $(INCLUDE_DIR)/$(UTILITIES_DIR)/domain.cpp $(INCLUDE_DIR)/$(UTILITIES_DIR)/domain.h \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/reader.h $(INCLUDE_DIR)/$(INTERFACES_DIR)/action.h \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/grounder.h \
					   $(INCLUDE_DIR)/$(UTILITIES_DIR)/define.h 
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(INCLUDE_DIR)/$(UTILITIES_DIR)/domain.cpp -o $(BUILD_DIR)/domain.o


lex.o yac.o main.o	: 
lex.o main.o		: 

clean:
	rm -f $(BIN_DIR)/*.out \
	$(BUILD_DIR)/*.o \
	*~ $(BUILD_DIR)/lex.c $(BUILD_DIR)/lex.yy.c $(BUILD_DIR)/bison.c $(BUILD_DIR)/tok.h $(BUILD_DIR)/lcp.tab.c $(BUILD_DIR)/lcp.tab.h $(BUILD_DIR)/lcp.efp.output
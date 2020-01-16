# Makefile
OBJS	=	$(BUILD_DIR)/bison.o $(BUILD_DIR)/lex.o $(BUILD_DIR)/main.o \
			$(BUILD_DIR)/belief_formula.o $(BUILD_DIR)/proposition.o $(BUILD_DIR)/domain.o \
			$(BUILD_DIR)/grounder.o $(BUILD_DIR)/printer.o \
			$(BUILD_DIR)/action.o $(BUILD_DIR)/helper.o $(BUILD_DIR)/initially.o \
			$(BUILD_DIR)/kstore.o \
			$(BUILD_DIR)/kedge.o $(BUILD_DIR)/kworld.o $(BUILD_DIR)/kstate.o \
			$(BUILD_DIR)/pstore.o \
			$(BUILD_DIR)/pworld.o $(BUILD_DIR)/pstate.o \
			$(BUILD_DIR)/pstore_opt.o $(BUILD_DIR)/pstate_opt.o \
			$(BUILD_DIR)/reader.o \
			$(BUILD_DIR)/heuristics_manager.o $(BUILD_DIR)/satisfied_goals.o \
			
CC	= g++
CFLAGS	= -g -Wall -ansi -Wfatal-errors -std=c++14 
MAKEFLAGS += "-j -l $(shell grep -c ^processor /proc/cpuinfo)"

dir_guard = @mkdir -p $(@D)
BUILD_DIR = build
BIN_DIR = bin

LIB_DIR = lib
SRC_DIR = src

INCLUDE_DIR = include
ACTION_DIR = $(INCLUDE_DIR)/actions
DOMAIN_DIR = $(INCLUDE_DIR)/domain
FORMULA_DIR = $(INCLUDE_DIR)/formulae
HEURISTIC_DIR = $(INCLUDE_DIR)/heuristics
PARSE_DIR = $(INCLUDE_DIR)/parse
STATES_DIR = $(INCLUDE_DIR)/states
S_KRIPE_DIR = $(STATES_DIR)/kripke
S_POSSIBILITY_DIR = $(STATES_DIR)/possibilities
S_POSSIBILITY_OPT_DIR = $(STATES_DIR)/possibilities_opt
UTILITIES_DIR = $(INCLUDE_DIR)/utilities
SEARCH_DIR = $(INCLUDE_DIR)/search

DOXYGEN_DIR = doxygen
OUT_DIR = out


#-----------------------------------PARSE FILES-----------------------------------#

$(BIN_DIR)/efp.out: $(OBJS)
		$(dir_guard)
		$(CC) $(CFLAGS) $(OBJS) -o $(BIN_DIR)/efp.out -ll

$(BUILD_DIR)/lex.o:		$(BUILD_DIR)/lex.c
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(BUILD_DIR)/lex.c -o $(BUILD_DIR)/lex.o

$(BUILD_DIR)/lex.c:		$(PARSE_DIR)/lcp.lex
		$(dir_guard)
		flex $(PARSE_DIR)/lcp.lex
		cp lex.yy.c $(BUILD_DIR)/lex.c
		mv lex.yy.c $(BUILD_DIR)/

$(BUILD_DIR)/bison.o:	$(BUILD_DIR)/bison.c
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(BUILD_DIR)/bison.c -o $(BUILD_DIR)/bison.o

$(BUILD_DIR)/bison.c:	$(PARSE_DIR)/lcp.y
		$(dir_guard)
		bison -d -v $(PARSE_DIR)/lcp.y
		cp lcp.tab.c $(BUILD_DIR)/bison.c
		cmp -s lcp.tab.h $(BUILD_DIR)/tok.h || cp lcp.tab.h $(BUILD_DIR)/tok.h
		mv lcp.* $(BUILD_DIR)/

#-----------------------------------MAIN FILE-----------------------------------#
$(BUILD_DIR)/main.o:	$(SRC_DIR)/main.cpp \
						$(UTILITIES_DIR)/reader.h \
						$(DOMAIN_DIR)/domain.h \
						$(SEARCH_DIR)/planner.h $(SEARCH_DIR)/planner.ipp \
						$(HEURISTIC_DIR)/heuristics_manager.h $(HEURISTIC_DIR)/heuristics_template.ipp\
						$(STATES_DIR)/state_T.h $(STATES_DIR)/state_T.ipp 
		$(dir_guard)
		echo "#define BUILT_DATE \"`date`\"" > $(BUILD_DIR)/built_date
		cat $(BUILD_DIR)/built_date $(SRC_DIR)/main.cpp > $(BUILD_DIR)/main.temp.cpp
		$(CC) $(CFLAGS) -c $(BUILD_DIR)/main.temp.cpp -o $(BUILD_DIR)/main.o
		rm $(BUILD_DIR)/main.temp.cpp
		
#-----------------------------------INCLUDE FILES-----------------------------------#
####UTILITIES
$(BUILD_DIR)/printer.o: $(UTILITIES_DIR)/printer.cpp $(UTILITIES_DIR)/printer.h \
						$(UTILITIES_DIR)/define.h \
						$(FORMULA_DIR)/belief_formula.h \
						$(DOMAIN_DIR)/grounder.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(UTILITIES_DIR)/printer.cpp -o $(BUILD_DIR)/printer.o
		
$(BUILD_DIR)/reader.o: $(UTILITIES_DIR)/reader.cpp $(UTILITIES_DIR)/reader.h \
					   $(UTILITIES_DIR)/define.h $(UTILITIES_DIR)/printer.h \
					   $(FORMULA_DIR)/belief_formula.h \
					   $(ACTION_DIR)/proposition.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(UTILITIES_DIR)/reader.cpp -o $(BUILD_DIR)/reader.o

$(BUILD_DIR)/helper.o: $(UTILITIES_DIR)/helper.cpp $(UTILITIES_DIR)/helper.h \
					   $(UTILITIES_DIR)/define.h $(UTILITIES_DIR)/printer.h \
					   $(FORMULA_DIR)/belief_formula.h 
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(UTILITIES_DIR)/helper.cpp -o $(BUILD_DIR)/helper.o

####FORMULAE			
$(BUILD_DIR)/belief_formula.o: $(FORMULA_DIR)/belief_formula.cpp $(FORMULA_DIR)/belief_formula.h \
							   $(DOMAIN_DIR)/grounder.h \
							   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(FORMULA_DIR)/belief_formula.cpp -o $(BUILD_DIR)/belief_formula.o

####SEARCH		
#$(BUILD_DIR)/planner.o: $(SEARCH_DIR)/planner.cpp $(SEARCH_DIR)/planner.h \
#					    $(STATES_DIR)/state_T.h $(STATES_DIR)/state_T.ipp \
#						$(DOMAIN_DIR)/domain.h
#		$(dir_guard)
#		$(CC) $(CFLAGS) -c $(SEARCH_DIR)/planner.cpp -o $(BUILD_DIR)/planner.o

####ACTIONS
$(BUILD_DIR)/action.o: $(ACTION_DIR)/action.cpp $(ACTION_DIR)/action.h \
					   $(DOMAIN_DIR)/grounder.h \
					   $(ACTION_DIR)/proposition.h \
					   $(FORMULA_DIR)/belief_formula.h \
					   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(ACTION_DIR)/action.cpp -o $(BUILD_DIR)/action.o
		
$(BUILD_DIR)/proposition.o: $(ACTION_DIR)/proposition.cpp $(ACTION_DIR)/proposition.h \
							$(FORMULA_DIR)/belief_formula.h \
							$(UTILITIES_DIR)/define.h $(UTILITIES_DIR)/printer.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(ACTION_DIR)/proposition.cpp -o $(BUILD_DIR)/proposition.o
			
####STATES
#$(BUILD_DIR)/state_T.o: $(STATES_DIR)/state_T.cpp $(STATES_DIR)/state_T.h \
#					    $(ACTION_DIR)/action.h \
#					    $(DOMAIN_DIR)/initially.h \
#   					    $(UTILITIES_DIR)/define.h
#		$(dir_guard)
#		$(CC) $(CFLAGS) -c $(STATES_DIR)/state_T.cpp -o $(BUILD_DIR)/state_T.o
#
##HEURISTICS
$(BUILD_DIR)/heuristics_manager.o: $(HEURISTIC_DIR)/heuristics_manager.cpp $(HEURISTIC_DIR)/heuristics_manager.h \
								   $(UTILITIES_DIR)/define.h \
								   $(HEURISTIC_DIR)/satisfied_goals.h \
								   $(HEURISTIC_DIR)/planning_graph.h $(HEURISTIC_DIR)/planning_graph.ipp
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(HEURISTIC_DIR)/heuristics_manager.cpp -o $(BUILD_DIR)/heuristics_manager.o 

$(BUILD_DIR)/satisfied_goals.o: $(HEURISTIC_DIR)/satisfied_goals.cpp $(HEURISTIC_DIR)/satisfied_goals.h \
								$(UTILITIES_DIR)/define.h \
								$(FORMULA_DIR)/belief_formula.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(HEURISTIC_DIR)/satisfied_goals.cpp -o $(BUILD_DIR)/satisfied_goals.o
#$(BUILD_DIR)/planning_graph.o: $(HEURISTIC_DIR)/planning_graph.cpp $(HEURISTIC_DIR)/planning_graph.h \
#					   $(S_KRIPE_DIR)/kstate \
#					   $(ACTION_DIR)/action.h \
#					   $(FORMULA_DIR)/belief_formula.h \
#					   $(UTILITIES_DIR)/define.h				   
#		$(dir_guard)
#		$(CC) $(CFLAGS) -c $(S_KRIPE_DIR)/planning_graph.cpp -o $(BUILD_DIR)/planning_graph.o


##KRIPKE STATES		
$(BUILD_DIR)/kstate.o: $(S_KRIPE_DIR)/kstate.cpp $(S_KRIPE_DIR)/kstate.h \
					   $(S_KRIPE_DIR)/kworld.h $(S_KRIPE_DIR)/kedge.h $(S_KRIPE_DIR)/kstore.h \
					   $(ACTION_DIR)/action.h \
					   $(DOMAIN_DIR)/initially.h \
					   $(UTILITIES_DIR)/helper.h \
					   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_KRIPE_DIR)/kstate.cpp -o $(BUILD_DIR)/kstate.o

$(BUILD_DIR)/kworld.o: $(S_KRIPE_DIR)/kworld.cpp $(S_KRIPE_DIR)/kworld.h \
					   $(UTILITIES_DIR)/printer.h $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_KRIPE_DIR)/kworld.cpp -o $(BUILD_DIR)/kworld.o
				
$(BUILD_DIR)/kedge.o: $(S_KRIPE_DIR)/kedge.cpp $(S_KRIPE_DIR)/kedge.h \
					  $(S_KRIPE_DIR)/kworld.h \
					  $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_KRIPE_DIR)/kedge.cpp -o $(BUILD_DIR)/kedge.o
				
$(BUILD_DIR)/kstore.o: $(S_KRIPE_DIR)/kstore.cpp $(S_KRIPE_DIR)/kstore.h \
					   $(S_KRIPE_DIR)/kworld.h $(S_KRIPE_DIR)/kedge.h \
					   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_KRIPE_DIR)/kstore.cpp -o $(BUILD_DIR)/kstore.o

##POSSIBILITIES STATES
$(BUILD_DIR)/pstate.o: $(S_POSSIBILITY_DIR)/pstate.cpp $(S_POSSIBILITY_DIR)/pstate.h \
					   $(S_POSSIBILITY_DIR)/pworld.h $(S_POSSIBILITY_DIR)/pstore.h \
					   $(ACTION_DIR)/action.h \
					   $(DOMAIN_DIR)/initially.h \
					   $(UTILITIES_DIR)/helper.h \
					   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_POSSIBILITY_DIR)/pstate.cpp -o $(BUILD_DIR)/pstate.o

$(BUILD_DIR)/pworld.o: $(S_POSSIBILITY_DIR)/pworld.cpp $(S_POSSIBILITY_DIR)/pworld.h \
					   $(UTILITIES_DIR)/printer.h $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_POSSIBILITY_DIR)/pworld.cpp -o $(BUILD_DIR)/pworld.o

$(BUILD_DIR)/pstore.o: $(S_POSSIBILITY_DIR)/pstore.cpp $(S_POSSIBILITY_DIR)/pstore.h \
					   $(S_POSSIBILITY_DIR)/pworld.h \
					   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_POSSIBILITY_DIR)/pstore.cpp -o $(BUILD_DIR)/pstore.o
		
#The optimized version		
$(BUILD_DIR)/pstate_opt.o: $(S_POSSIBILITY_OPT_DIR)/pstate_opt.cpp $(S_POSSIBILITY_OPT_DIR)/pstate_opt.h \
					   $(S_POSSIBILITY_OPT_DIR)/pstore_opt.h \
					   $(ACTION_DIR)/action.h \
					   $(DOMAIN_DIR)/initially.h \
					   $(UTILITIES_DIR)/helper.h \
					   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_POSSIBILITY_OPT_DIR)/pstate_opt.cpp -o $(BUILD_DIR)/pstate_opt.o


$(BUILD_DIR)/pstore_opt.o: $(S_POSSIBILITY_OPT_DIR)/pstore_opt.cpp $(S_POSSIBILITY_OPT_DIR)/pstore_opt.h \
					   $(S_POSSIBILITY_OPT_DIR)/pstate_opt.cpp\
					   $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(S_POSSIBILITY_OPT_DIR)/pstore_opt.cpp -o $(BUILD_DIR)/pstore_opt.o

####DOMAIN
$(BUILD_DIR)/initially.o: $(DOMAIN_DIR)/initially.cpp $(DOMAIN_DIR)/initially.h \
						  $(FORMULA_DIR)/belief_formula.h  \
						  $(UTILITIES_DIR)/define.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(DOMAIN_DIR)/initially.cpp -o $(BUILD_DIR)/initially.o
			
$(BUILD_DIR)/grounder.o: $(DOMAIN_DIR)/grounder.cpp $(DOMAIN_DIR)/grounder.h \
						 $(UTILITIES_DIR)/define.h $(UTILITIES_DIR)/printer.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(DOMAIN_DIR)/grounder.cpp -o $(BUILD_DIR)/grounder.o
			
$(BUILD_DIR)/domain.o: $(DOMAIN_DIR)/domain.cpp $(DOMAIN_DIR)/domain.h \
					   $(DOMAIN_DIR)/initially.h $(DOMAIN_DIR)/grounder.h \
					   $(UTILITIES_DIR)/reader.h $(UTILITIES_DIR)/define.h \
					   $(ACTION_DIR)/action.h
		$(dir_guard)
		$(CC) $(CFLAGS) -c $(DOMAIN_DIR)/domain.cpp -o $(BUILD_DIR)/domain.o


.PHONY: clean_build clean_out doxygen doc old all fresh clear

doxygen:
	$(MAKE)
	mkdir -p $(DOXYGEN_DIR)/Docs
	cd $(DOXYGEN_DIR)/ && doxygen efp-doxygen
	
doc:
	$(MAKE) doxygen

clean_build:
	rm -f -r $(BIN_DIR) $(BUILD_DIR)
	
clean_out:
	rm -f -r $(OUT_DIR) *tmp

clear:
	$(MAKE) clean_build
	$(MAKE) clean_out

fresh:
	$(MAKE) clear
	rm -f -r $(DOXYGEN_DIR)/Docs
	
old:
	cd EFP_ICAPS_old/ && $(MAKE) clean
	cd EFP_ICAPS_old/ && $(MAKE)
	
all:
	$(MAKE) 
	$(MAKE) doxygen
	$(MAKE) old
	
clean:
	$(MAKE) clean_build

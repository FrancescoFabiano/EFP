### MAKEFILE
- **make**: to compile the planner.
- **make doxygen**: to compile the planner and the documentation (to check it open doxygen/Docs/html/index.html).
- **make clean**: removes all compilation-generated files.
- **make clean_build**: removes all compilation-generated files.
- **make clean_out**: removes all the file in out/ (the pdf visualization of the states).
- **make clear**: executes both **clean_build** and **clean_out**.
- **make fresh**: executes **clear** and also removes doxygen documentation.
- **make old**: cleans and compile the old version (1.0) of EFP.
- **make all**: executes **make doxygen** and **make_old**.
	
	
### Example
Following some examples of execution (we assume to be in the main project folder).
The folder that contains the domains is "exp/"

- Print of all the possible parameters
	- bin/efp.out
	
- Execution of Grapevine (example for any domain) with 4 agents and plan of length 3 on Possibilities without check for already visited state
	- bin/efp.out exp/Grapevine/Grapevine_4/Grapevine_4__pl_3.txt -st POSS
	
- Execution of Grapevine (example for any domain) with 4 agents and plan of length 3 on Possibilities with check for already visited state (FASTER CONFIG.)
	- bin/efp.out exp/Grapevine/Grapevine_4/Grapevine_4__pl_3.txt -st POSS -check_visited
	
- Test of sequential actions execution (-e parameter):
	- bin/efp.out exp/Grapevine/Grapevine_4/Grapevine_4__pl_3.txt -st POSS -e share_b_sb_1, right_a, share_c_sc_1
	
- Print of the e-states after a sequential actions execution (-debug and -e parameters):
	- bin/efp.out exp/Grapevine/Grapevine_4/Grapevine_4__pl_3.txt -st POSS -debug -e share_b_sb_1, right_a, share_c_sc_1

- Execution of CC domain (example for any domain) on Kripke Structure with check for already visited state
	- bin/efp.out exp/CC/CC_2_2_4/CC_2_2_4__pl_5.txt -check_visited
	
- Execution of Coin_In_The_Box domain (example for any domain) on Kripke Structure with check for already visited state and bisimulation reduction
	- bin/efp.out exp/Coin_In_The_Box/Coin_in_the_Box__pl_3.txt -check_visited -bis PT


### Requirements
- flex
- bison


### Extra Files
- The folder "scripts" contains some useful scripts---they may need to be slightly adapted.
- The folder "EFP_ICAPS_old" contains EFP 1.0

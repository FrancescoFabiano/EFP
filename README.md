# EFP

## Goal
Realize an Epistemic Forward Planner that bases its concept on different states representations such as:
- [x] *Kripke structure*
- [ ] *possibilities*
- [ ] *OBDDs*

## The situation:
- We have PG-EFP, an epistemic forward planner based on Kripke structure that also uses epistemic planning graph.

## Problems:
- Transition function is ok? Do we need to check the executability on every word?
- What happen if an action is not executable accordingly to one of the fully observant agent but it is for others
- Ontic generates duplicates?
- Is announcement with false beliefs an ontic? It creates the world if it didn't exist. old semantic + ontic update for fully observant (sensing the same)
  
## Future works
- Encode the concept of possibility;
- Prepare for dynamic programming exploiment.

- Think about OBDDs.

- Allow the planner to work with every the representation.

- Heuristics.

## Theorical ideas to study
- If **ag_i** sees as partial **ag_y** announcing **phi** and **ag_i** thought that **ag_y** did not know **phi** how should **ag_i** react?
- If **ag_i** sees as partial **ag_y** announcing **f** and **ag_i** knows **f** then
	- **ag_i** believes that **ag_y** knows **f**; or
	- **ag_i** believes that **ag_y** knows (**f OR -f**).
- Objective vs subjective common knowledge;
- announcement/sensing of Belief formulae;
- ontic that remove uncertanty;
- static laws (the same as biased info?);
- intial state;
- false beliefs correction.


- Lies and biased info;
- ethic constraints.

###Reqiurements
- flex
- bison
- texlive-font-utils
- graphviz

###Usage
- **make**: to compile the planner.
- **make doxygen**: to compile the planner and the documentation (to check it open doxygen/Docs/html/index.html).
- **make clean_build**: removes all compiletion-generated files.
- **make clean_out**: removes all the file in out/ (the pdf visualization of the states).
- **make clear**: executes both **clean_build** and **clean_out**.
- **make fresh**: executes **clear** and also removes doxygen documentation.
- **make old**: cleans and compile the old version (1.0) of EFP.
- **make all**: executes **make doxygen** and **make_old**.
	
# EFP

## Goal
Realize an Epistemic Forward Planner that bases its concept on different states representations such as:
- [x] *Kripke structure*
- [ ] *possibilities* and exploits *dynamic programming*.
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
- Objective vs subjective common knowledge;
- announcement/sensing of Belief formulae;
- ontic that remove uncertanty;
- static laws (the same as biased info?);
- intial state;
- false beliefs correction.


- Lies and biased info;
- ethic constraints.
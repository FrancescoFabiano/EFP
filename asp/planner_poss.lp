%
% EMP encoding in ASP
%

% ********** REACHABILITY **********

reaches(T1, P1, T2, P2, AGS) :- believes(T1, P1, T2, P2, AG), contains_ag(AGS, AG).
reaches(T1, P1, T2, P2, AGS) :- believes(T1, P1, T3, P3, AG), contains_ag(AGS, AG), reaches(T3, P3, T2, P2, AGS).


% ********** ENTAILMENT **********

formula(F) :- fluent(F).        % A fluent is a formula

holds(W, true) :- world(W).

entails(T, P, true  )      :-                                           possible_world(T, P, _).
entails(T, P,  F    )      :-     holds(W, F),                          possible_world(T, P, W), world(W), fluent(F).
entails(T, P, -F    )      :- not holds(W, F),                          possible_world(T, P, W), world(W), fluent(F).

entails(T, P, neg(F))      :- not entails(T, P, F ),                    possible_world(T, P, _), formula(F).
entails(T, P, and(F1, F2)) :-     entails(T, P, F1), entails(T, P, F2), possible_world(T, P, _), formula(F1), formula(F2).
entails(T, P,  or(F1, F2)) :-     entails(T, P, F1),                    possible_world(T, P, _), formula(F1), formula(F2).
entails(T, P,  or(F1, F2)) :-                        entails(T, P, F2), possible_world(T, P, _), formula(F1), formula(F2).

% b operator
not_entails(T1, P1, b(AG , F)) :- not     entails(T2, P2, F        ), believes(T1, P1, T2, P2, AG), possible_world(T2, P2, _), formula(F), agent(AG).
entails    (T , P , b(AG , F)) :- not not_entails(T , P , b(AG , F)),                               possible_world(T , P , _), formula(b(AG , F)).

% c operator
not_entails(T1, P1, c(AGS, F)) :- not     entails(T2, P2, F        ), reaches(T1, P1, T2, P2, AGS), possible_world(T2, P2, _), formula(F).
entails    (T , P , c(AGS, F)) :- not not_entails(T , P , c(AGS, F)),                               possible_world(T , P , _), formula(c(AGS, F)).



% ********** INITIAL STATE **********

% Since we assume to deal with FINITARY S5-states, we only allow the initial conditions initially(F), where F has one of the following forms:
%  1. Phi (propositional formula)
%  2. C_all( B_i(Phi))
%  3. C_all( B_i(Phi) \/  B_i(-Phi))
%  4. C_all(-B_i(Phi) /\ -B_i(-Phi)) (this case doesn't add knowledge)

% The initial conditions of form 1. determine the pointed possibility of the initial state
% A possibility is pointed if it entails exactly those fluents that hold initially

no_good_initial_pointed(W) :- not holds(W, F), world(W), initially( F), fluent(F).
no_good_initial_pointed(W) :-     holds(W, F), world(W), initially(-F), fluent(F).

possible_initial_world(1, W) :- not no_good_initial_pointed(W), world(W).
pointed(0, 1).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
no_good_initial_world(W) :- not holds(W, F), world(W), initially(c(AGS,  F)), agent_set(AGS), fluent(F).
no_good_initial_world(W) :-     holds(W, F), world(W), initially(c(AGS, -F)), agent_set(AGS), fluent(F).

good_initial_world(W) :- not no_good_initial_world(W), world(W).

% We force the initial possibilities to be numbered from 1 to K, where K is the number of the initial possible worlds
1 { possible_initial_world(P, W) : world(P), P <= K } 1 :- good_initial_world(W), K = { good_initial_world(W1) : world(W1) }.
:- possible_initial_world(P, W1), possible_initial_world(P, W2), W1 != W2.
% TODO: forse possible_initial_world si può scrivere meglio...
possible_world(0, P, W) :- possible_initial_world(P, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails(0, P1, F),     entails(0, P2, F), world(P1), world(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).
not_believes_initially(P1, P2, AG) :-     entails(0, P1, F), not entails(0, P2, F), world(P1), world(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).

believes(0, P1, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_initial_world(P1, W1), possible_initial_world(P2, W2), agent(AG).


% ********** TRANSITION FUNCTION **********

action(ACT) :- ontic_action(ACT).
% action(ACT) :- sensing_action(ACT).
% action(ACT) :- announcement_action(ACT).

is_executable(T, ACT) :- plan(T, ACT), pointed(T, P), entails(T, P, EXEC), executable(ACT, EXEC).

is_fully_obs(T ,AG) :- is_executable(T, ACT), ontic_action(ACT), fully_obs(ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, PS), possible_world(T, PS, WS).
is_oblivious(T, AG) :- is_executable(T, ACT), ontic_action(ACT), fully_obs(ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, PS), possible_world(T, PS, WS).
is_oblivious(T, AG) :- is_executable(T, ACT), ontic_action(ACT), oblivious(ACT, AG).

% ONTIC ACTIONS

apply_ontic(W1, W2, T+1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
                            world(W1), world(W2),     holds(W1, CAUSED),
                            0 { fluent(F) : F != CAUSED,     holds(W1, F), not holds(W2, F) } 0,
                            0 { fluent(F) : F != CAUSED, not holds(W1, F),     holds(W2, F) } 0.

apply_ontic(W1, W2, T+1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED),
                            world(W1), world(W2), not holds(W1, CAUSED),
                            0 { fluent(F) : F != CAUSED,     holds(W1, F), not holds(W2, F) } 0,
                            0 { fluent(F) : F != CAUSED, not holds(W1, F),     holds(W2, F) } 0.

pointed(T+1, phi(P)) :- is_executable(T, ACT), ontic_action(ACT), pointed(T, P), possible_world(T+1, phi(P), _).

possible_world(T+1, phi(P), W1) :- is_executable(T, ACT), ontic_action(ACT), apply_ontic(W1, W2, T+1), pointed(T, P), possible_world(T, P, W2).

possible_world(T+1, phi(P), W1) :- is_executable(T, ACT), ontic_action(ACT), apply_ontic(W1, W2, T+1), pointed(T, PP),
                                   believes(T, PP, T2, P, AG), possible_world(T2, P, _), is_fully_obs(T, AG).

believes(T+1, phi(P1), T+1, phi(P2), AG) :- is_executable(T, ACT), ontic_action(ACT), believes(T1, P1, T2, P2, AG), T1 <= T, T2 <= T,
                                            is_fully_obs(T, AG), possible_world(T1, P1, _), possible_world(T2, P2, _), possible_world(T+1, phi(P1), _), possible_world(T+1, phi(P2), _).

believes(T+1, phi(P1), T2,      P2 , AG) :- is_executable(T, ACT), ontic_action(ACT), believes(T1, P1, T2, P2, AG), T1 <= T, T2 <= T,
                                            is_oblivious(T, AG), possible_world(T1, P1, _), possible_world(T2, P2, _), possible_world(T+1, phi(P1), _).

% SENSING ACTIONS
% ANNOUNCEMENT ACTIONS


% Generate
0 { plan(T, ACT) : action(ACT) } 1 :- time(T), T < maxT.

% Define
not_reached_goal(0) :- not entails(0, P, F), pointed(0, P), goal(F).
not_reached_goal(T) :- not entails(T, P, F), pointed(T, P), time(T), T > 0, goal(F).

reached_goal(0) :- not not_reached_goal(0).
reached_goal(T) :- plan(T-1, ACT), is_executable(T-1, ACT), not not_reached_goal(T), time(T), T > 0.

% Test
:- 0 { reached_goal(T) : time(T) } 0.

#minimize{ T : reached_goal(T) }.

% Display

% #show is_fully_obs/2.
% #show is_oblivious/2.

% #show pointed/2.
% #show possibility/2.
% #show possible_world/3.
% #show believes/5.

#show plan/2.
% #show reached_goal/1.
% #show not_reached_goal/1.

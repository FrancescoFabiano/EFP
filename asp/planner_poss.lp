%
% EMP encoding in ASP
%

% ********** REACHABILITY **********

reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T2, R2, P2, AG), contains_ag(AGS, AG).
reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T3, R3, P3, AG), contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS).


% ********** ENTAILMENT **********

formula(F) :- fluent(F).        % A fluent is a formula
% formula(F) :- initially(F).
% formula(F) :- goal(F).

% formula(F) :- formula(neg(F)).
% formula(F) :- formula(and(F, _)).
% formula(F) :- formula(and(_, F)).
% formula(F) :- formula(or(F, _)).
% formula(F) :- formula(or(_, F)).
% formula(F) :- formula(b(_, F)).
% formula(F) :- formula(c(_, F)).

holds(W, true) :- world(W).

entails(T, R, P, true  )      :-                                                 possible_world(T, R, P, _).
entails(T, R, P,  F    )      :-     holds(W, F),                                possible_world(T, R, P, W), world(W), fluent(F).
entails(T, R, P, -F    )      :- not holds(W, F),                                possible_world(T, R, P, W), world(W), fluent(F).

entails(T, R, P, neg(F))      :- not entails(T, R, P, F ),                       possible_world(T, R, P, _), formula(F).
entails(T, R, P, and(F1, F2)) :-     entails(T, R, P, F1), entails(T, R, P, F2), possible_world(T, R, P, _), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :-     entails(T, R, P, F1),                       possible_world(T, R, P, _), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :-                           entails(T, R, P, F2), possible_world(T, R, P, _), formula(F1), formula(F2).

% b operator
not_entails(T1, R1, P1, b(AG , F)) :- not     entails(T2, R2, P2, F        ), believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T2, R2, P2, _), formula(F), agent(AG).
entails    (T , R , P , b(AG , F)) :- not not_entails(T , R , P , b(AG , F)),                                       possible_world(T , R , P , _), formula(b(AG , F)).

% c operator
not_entails(T1, R1, P1, c(AGS, F)) :- not     entails(T2, R2, P2, F        ), reaches(T1, R1, P1, T2, R2, P2, AGS), possible_world(T2, R2, P2, _), formula(F).
entails    (T , R , P , c(AGS, F)) :- not not_entails(T , R , P , c(AGS, F)),                                       possible_world(T , R , P , _), formula(c(AGS, F)).



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
pointed(0, 0, 1).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
no_good_initial_world(W) :- not holds(W, F), world(W), initially(c(AGS,  F)), agent_set(AGS), fluent(F).
no_good_initial_world(W) :-     holds(W, F), world(W), initially(c(AGS, -F)), agent_set(AGS), fluent(F).

good_initial_world(W) :- not no_good_initial_world(W), world(W).

% We force the initial possibilities to be numbered from 1 to K, where K is the number of the initial possible worlds
1 { possible_initial_world(P, W) : world(P), P <= K } 1 :- good_initial_world(W), K = { good_initial_world(W1) : world(W1) }.
:- possible_initial_world(P, W1), possible_initial_world(P, W2), W1 != W2.
% TODO: forse possible_initial_world si può scrivere meglio...
possible_world(0, 0, P, W) :- possible_initial_world(P, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F),     entails(0, 0, P2, F), world(P1), world(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).
not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F), not entails(0, 0, P2, F), world(P1), world(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).

believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_initial_world(P1, W1), possible_initial_world(P2, W2), agent(AG).


% ********** TRANSITION FUNCTION **********

action(ACT) :- ontic(ACT).
action(ACT) :- sensing(ACT).
% action(ACT) :- announcement(ACT).

is_executable(T, ACT) :- plan(T, ACT), pointed(T, R, P), entails(T, R, P, EXEC), executable(ACT, EXEC).

is_fully_obs  (T, AG) :- is_executable(T, ACT), ontic(ACT),        observes (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).
is_oblivious  (T, AG) :- is_executable(T, ACT), ontic(ACT),        observes (ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).

is_fully_obs  (T, AG) :- is_executable(T, ACT), sensing(ACT),      observes (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).
is_partial_obs(T, AG) :- is_executable(T, ACT), sensing(ACT),      aware_of (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).
is_oblivious  (T, AG) :- is_executable(T, ACT), sensing(ACT),      observes (ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).
is_oblivious  (T, AG) :- is_executable(T, ACT), sensing(ACT),      aware_of (ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).

% is_fully_obs  (T, AG) :- is_executable(T, ACT), announcement(ACT), observes (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, PS), possible_world(T, PS, WS).
% is_partial_obs(T, AG) :- is_executable(T, ACT), announcement(ACT), aware_of (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, PS), possible_world(T, PS, WS).
% is_oblivious  (T, AG) :- is_executable(T, ACT), announcement(ACT), observes (ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, PS), possible_world(T, PS, WS).
% is_oblivious  (T, AG) :- is_executable(T, ACT), announcement(ACT), aware_of (ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, PS), possible_world(T, PS, WS).

is_oblivious  (T, AG) :- is_executable(T, ACT), action(ACT),       oblivious(ACT, AG).

% ONTIC ACTIONS

apply_ontic(W1, W2, T+1) :- is_executable(T, ACT), ontic(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
                            world(W1), world(W2),     holds(W1, CAUSED),
                            0 { fluent(F) : F != CAUSED,     holds(W1, F), not holds(W2, F) } 0,
                            0 { fluent(F) : F != CAUSED, not holds(W1, F),     holds(W2, F) } 0.

apply_ontic(W1, W2, T+1) :- is_executable(T, ACT), ontic(ACT), causes(ACT, -CAUSED), fluent(CAUSED),
                            world(W1), world(W2), not holds(W1, CAUSED),
                            0 { fluent(F) : F != CAUSED,     holds(W1, F), not holds(W2, F) } 0,
                            0 { fluent(F) : F != CAUSED, not holds(W1, F),     holds(W2, F) } 0.

% The repetition of the pointed world is incremented every time that an ontic action is made, since the pointed is always updated
pointed(T+1, T+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, T, P), possible_world(T+1, T+1, phi(P), _).

% possible_world(T, R, P, W): time T, repetition R, index P, world W
possible_world(T+1, T +1, phi(P), W1) :- is_executable(T, ACT), ontic(ACT), apply_ontic(W1, W2, T+1), pointed(T, T, P), possible_world(T, _, P, W2).

possible_world(T+1, T2+1, phi(P), W1) :- is_executable(T, ACT), ontic(ACT), apply_ontic(W1, W2, T+1), pointed(T, T, PP),
                                         believes(T, T, PP, T2, R2, P, AG), possible_world(T2, R2, P, W2), is_fully_obs(T, AG).

% Fully observant
believes(T+1, R3, phi(P1), T+1, R4, phi(P2), AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                    is_fully_obs(T, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                    possible_world(T+1, R3, phi(P1), _), possible_world(T+1, R4, phi(P2), _), R1 < R3, R2 < R4.

% Oblivious
believes(T+1, R3, phi(P1), T2 , R2,  P2 , AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                 is_oblivious(T, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                 possible_world(T+1, R3, phi(P1), _), R1 < R3.

% SENSING ACTIONS

% pointed(T+1, phi(P)) :- is_executable(T, ACT), sensing(ACT), pointed(T, P), possible_world(T+1, phi(P), _).

% possible_world(T+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED), pointed(T, PP), possible_world(T2, P, W),
%                                    believes(T, PP, T2, P, AG),     entails(T, PP, SENSED),     entails(T2, P, SENSED), is_fully_obs(T, AG).

% possible_world(T+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED), pointed(T, PP), possible_world(T2, P, W),
%                                    believes(T, PP, T2, P, AG), not entails(T, PP, SENSED), not entails(T2, P, SENSED), is_fully_obs(T, AG).

% possible_world(T+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), reaches(T1, P, T2, P2, all_agents), T1 <= T, T2 <= T,
%                                    possible_world(T+1, phi(P2), W2), possible_world(T2, P2, W2), possible_world(T1, P, W).

% believes(T+1, phi(P1), T+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), believes(T1, P1, T2, P2, AG), T1 <= T, T2 <= T,
%                                             is_fully_obs(T, AG), possible_world(T1, P1, _), possible_world(T2, P2, _), possible_world(T+1, phi(P1), _), possible_world(T+1, phi(P2), _).

% believes(T+1, phi(P1), T2,      P2 , AG) :- is_executable(T, ACT), sensing(ACT), believes(T1, P1, T2, P2, AG), T1 <= T, T2 <= T,
%                                             is_partial_obs(T, AG), possible_world(T1, P1, _), possible_world(T2, P2, _), possible_world(T+1, phi(P1), _).

% believes(T+1,     P1 , T2,  phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), believes(T1, P1, T2, P2, AG), T1 <= T, T2 <= T,
%                                             not is_oblivious(T, AG), possible_world(T1, P1, _), possible_world(T2, P2, _), possible_world(T+1, phi(P2), _).

% believes(T+1, phi(P1), T2,      P2 , AG) :- is_executable(T, ACT), sensing(ACT), believes(T1, P1, T2, P2, AG), T1 <= T, T2 <= T,
%                                             is_oblivious(T, AG), possible_world(T1, P1, _), possible_world(T2, P2, _), possible_world(T+1, phi(P1), _).

% TODO: fai i primi test per le sensing

% ANNOUNCEMENT ACTIONS


% Generate
0 { plan(T, ACT) : action(ACT) } 1 :- time(T), T < maxT.

% plan(0, open_a).
% plan(1, close_a).
% plan(2, open_c).
% plan(3, peek_c).

% Define
not_reached_goal(0) :- not entails(0, 0, P, F), pointed(0, 0, P), goal(F).
not_reached_goal(T) :- not entails(T, R, P, F), pointed(T, R, P), time(T), T > 0, goal(F).

reached_goal(0) :- not not_reached_goal(0).
reached_goal(T) :- plan(T-1, ACT), is_executable(T-1, ACT), not not_reached_goal(T), time(T), T > 0.

% Test
:- plan(T, ACT), pointed(T, R, P), not entails(T, R, P, EXEC), executable(ACT, EXEC).
:- 0 { reached_goal(T) : time(T) } 0.
:- reached_goal(T1), plan(T2, ACT), T2 > T1.

#minimize{ T : reached_goal(T) }.

% Display

% #show is_fully_obs/2.
% #show is_oblivious/2.

#show pointed/3.
#show possible_world/4.
#show believes/7.

#show plan/2.
% #show reached_goal/1.

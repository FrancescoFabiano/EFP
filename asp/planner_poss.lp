%
% EMP encoding in ASP
%

% ********** REACHABILITY **********

reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T2, R2, P2, AG), contains_ag(AGS, AG).
reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T3, R3, P3, AG), contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS).


% ********** ENTAILMENT **********

holds(W, true) :- fluent_set(W).

entails(T, R, P, true  )      :-                                                 possible_world(T, R, P, _).
entails(T, R, P,  F    )      :-     holds(W, F),                                possible_world(T, R, P, W), fluent_set(W), fluent(F).
entails(T, R, P, -F    )      :- not holds(W, F),                                possible_world(T, R, P, W), fluent_set(W), fluent(F).

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

no_good_initial_pointed(W) :- not holds(W, F), fluent_set(W), initially( F), fluent(F).
no_good_initial_pointed(W) :-     holds(W, F), fluent_set(W), initially(-F), fluent(F).

possible_initial_world(1, W) :- not no_good_initial_pointed(W), fluent_set(W).
pointed(0, 0, 1).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
no_good_initial_world(W) :- not holds(W, F), fluent_set(W), initially(c(AGS,  F)), agent_set(AGS), fluent(F).
no_good_initial_world(W) :-     holds(W, F), fluent_set(W), initially(c(AGS, -F)), agent_set(AGS), fluent(F).

good_initial_world(W) :- not no_good_initial_world(W), fluent_set(W).

% We force the initial possibilities to be numbered from 1 to K, where K is the number of the initial possible worlds
1 { possible_initial_world(P, W) : fluent_set(P), P <= K } 1 :- good_initial_world(W), K = { good_initial_world(W1) : fluent_set(W1) }.
:- possible_initial_world(P, W1), possible_initial_world(P, W2), W1 != W2.
% TODO: forse possible_initial_world si può scrivere meglio...
possible_world(0, 0, P, W) :- possible_initial_world(P, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F),     entails(0, 0, P2, F), fluent_set(P1), fluent_set(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).
not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F), not entails(0, 0, P2, F), fluent_set(P1), fluent_set(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).

believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_initial_world(P1, W1), possible_initial_world(P2, W2), agent(AG).



% ********** TRANSITION FUNCTION **********

action(ACT) :- ontic(ACT).
action(ACT) :- sensing(ACT).
action(ACT) :- announcement(ACT).

is_executable(T, ACT) :- plan(T, ACT), action(ACT), pointed(T, T, P), entails(T, T, P, EXEC), executable(ACT, EXEC).

is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- plan(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2, _), entails(T2, R2, P2, EEXEC),
                                                    pointed(T, T, PP), is_fully_obs(T, AG), believes(T, T, PP, T2, R2, P2, AG) : not pointed(T, T, P2).

is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- plan(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), possible_world(T2, R2, P2, _), entails(T2, R2, P2, EEXEC),
                                                    pointed(T, T, PP), is_fully_obs(T, AG), believes(T, T, PP, T2, R2, P2, AG) : not pointed(T, T, P2).

is_fully_obs  (T, AG) :- is_executable(T, ACT), action(ACT),       observes (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).
is_oblivious  (T, AG) :- is_executable(T, ACT), action(ACT),       observes (ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).
is_oblivious  (T, AG) :- is_executable(T, ACT), action(ACT),       oblivious(ACT, AG).

is_partial_obs(T, AG) :- is_executable(T, ACT), sensing(ACT),      aware_of (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).
is_partial_obs(T, AG) :- is_executable(T, ACT), announcement(ACT), aware_of (ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, R, PS), possible_world(T, R, PS, WS).


% ONTIC ACTIONS

% When we apply ACT to P2, the possibility phi(P2) must entail CAUSED if it holds causes(T, ACT, CAUSED)
apply_ontic_fluent(T2, R2, P2,  CAUSED, T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2),
                                              is_executable_effect(T, ACT, T2, R2, P2, CAUSED), T2 <= T, R2 <= T,
                                              pointed(T, T, PP), is_fully_obs(T, AG), believes(T, T, PP, T2, R2, P2, AG) : not pointed(T, T, P2).

% When we apply ACT to P2, the possibility phi(P2) must entail F if it does not hold nor causes(T, ACT, F), nor causes(T, ACT, -F)
apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), holds(W2, F),
                                              not is_executable_effect(T, ACT, T2, R2, P2, F), not is_executable_effect(T, ACT, T2, R2, P2, -F), fluent(F), T2 <= T, R2 <= T,
                                              pointed(T, T, PP), is_fully_obs(T, AG), believes(T, T, PP, T2, R2, P2, AG) : not pointed(T, T, P2).

% The fluent_set W1 is the one resulting from W2 when we apply ACT. W1 must entail all the fluents that are caused by ACT and must agree with W2 w.r.t. the other fluents
apply_ontic(W1, T2, R2, P2, T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), fluent_set(W1), possible_world(T2, R2, P2, W2), T2 <= T, R2 <= T,
                                  0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2,  F, T), not holds(W1, F) } 0,
                                  0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2, -F, T),     holds(W1, F) } 0,
                                  0 { fluent(F) : not apply_ontic_fluent(T2, R2, P2,  F, T),     holds(W1, F) } 0,
                                  pointed(T, T, PP), is_fully_obs(T, AG), believes(T, T, PP, T2, R2, P2, AG) : not pointed(T, T, P2).

% The repetition of the pointed possibility is incremented every time that an ontic action is made, since the pointed is always updated
pointed(T+1, T+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, T, P), possible_world(T+1, T+1, phi(P), _).

% possible_world(T, R, P, W): time T, repetition R, index P, fluent_set W
possible_world(T+1, T +1, phi(P), W1) :- is_executable(T, ACT), ontic(ACT), apply_ontic(W1, T, T, P, T), pointed(T, T, P), possible_world(T, T, P, W2).

possible_world(T+1, T2+1, phi(P), W1) :- is_executable(T, ACT), ontic(ACT), apply_ontic(W1, T2, R2, P, T), pointed(T, T, PP),
                                         believes(T, T, PP, T2, R2, P, AG), possible_world(T2, R2, P, W2), is_fully_obs(T, AG).

% Fully observant agents
believes(T+1, T1+1, phi(P1), T+1, T2+1, phi(P2), AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                        is_fully_obs(T, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _), possible_world(T+1, T2+1, phi(P2), _).

% Oblivious agents
believes(T+1, T1+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                        is_oblivious(T, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _).

% SENSING ACTIONS

% The possibility P2 respects the SENSED fluents if it agrees with the pointed possibility on such fluents
hold_sensed_effects(T2, R2, P2, T) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), possible_world(T, T, PP, WP), possible_world(T2, R2, P2, W2), T2 <= T,
                                      0 { fluent(SENSED) : is_executable_effect(T, ACT, T2, R2, P2, SENSED),     holds(W2, SENSED), not holds(WP, SENSED) } 0,
                                      0 { fluent(SENSED) : is_executable_effect(T, ACT, T2, R2, P2, SENSED), not holds(W2, SENSED),     holds(WP, SENSED) } 0,
                                      pointed(T, T, PP), is_fully_obs(T, AG), believes(T, T, PP, T2, R2, P2, AG) : not pointed(T, T, P2).

pointed(T+1, T+1, phi(P)) :- is_executable(T, ACT), sensing(ACT), pointed(T, T, P), possible_world(T+1, T+1, phi(P), _).

% When a sensing action is performed, some posibilities will not be believed from the fully observant agents (namely, all the possibilities
% that do not entail the SENSED fluents). Those possibilities must be updated alongside with all the possibilities that reach the updated ones.
% We must consider also these other possibilities in order to correctly update the believes of the fully observant agents.

possible_world(T+1, T +1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, T, P), possible_world(T, T, P, W).

possible_world(T+1, T2+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, T, PP), possible_world(T2, R2, P, W),
                                         believes(T, T, PP, T2, R2, P, AG), hold_sensed_effects(T2, R2, P, T), is_fully_obs(T, AG).

possible_world(T+1, T2+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, T, PP), possible_world(T2, R2, P, W),
                                         believes(T, T, PP, T2, R2, P, AG), is_partial_obs(T, AG).

% This rule makes sure that if there exists an updated possibility phi(P2) such that P2 is reached by P, then also P must be updated
possible_world(T+1, T1+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, T, PP), reaches(T, T, PP, T1, R1, P, all_agents),
                                         possible_world(T+1, T2+1, phi(P2), W2), possible_world(T2, R2, P2, W2), possible_world(T1, R1, P, W),
                                         believes(T1, R1, P, T2, R2, P2, AG), R1 < T, R2 <= T.

% Fully observant agents (sigma, sigma)
believes(T+1, T1+1, phi(P1), T+1, T2+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_fully_obs(T, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _), possible_world(T+1, T2+1, phi(P2), _),
                                                        hold_sensed_effects(T1, R1, P1, T), hold_sensed_effects(T2, R2, P2, T).

% Fully observant agents (tau, tau)
believes(T+1, T1+1, phi(P1), T+1, T2+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_fully_obs(T, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _), possible_world(T+1, T2+1, phi(P2), _),
                                                        not hold_sensed_effects(T1, R1, P1, T), not hold_sensed_effects(T2, R2, P2, T).

% Partially observant agents
believes(T+1, T1+1, phi(P1), T+1, T2+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_partial_obs(T, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _), possible_world(T+1, T2+1, phi(P2), _).

% Oblivious agents
believes(T+1, T1+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), sensing(ACT), is_oblivious(T, AG), T1 <= T, T2 <= T,
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _).

% ANNOUNCEMENT ACTIONS

pointed(T+1, T+1, phi(P)) :- is_executable(T, ACT), announcement(ACT), pointed(T, T, P), possible_world(T+1, T+1, phi(P), _).

% When a sensing action is performed, some posibilities will not be believed from the fully observant agents (namely, all the possibilities
% that do not entail the ANN formula). Those possibilities must be updated alongside with all the possibilities that reach the updated ones.
% We must consider also these other possibilities in order to correctly update the believes of the fully observant agents.

possible_world(T+1, T +1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, T, P), possible_world(T, T, P, W).

possible_world(T+1, T2+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true), pointed(T, T, PP), possible_world(T2, R2, P, W),
                                         believes(T, T, PP, T2, R2, P, AG),     entails(T, T, PP, ANN),     entails(T2, R2, P, ANN), is_fully_obs(T, AG).

possible_world(T+1, T2+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true), pointed(T, T, PP), possible_world(T2, R2, P, W),
                                         believes(T, T, PP, T2, R2, P, AG), not entails(T, T, PP, ANN), not entails(T2, R2, P, ANN), is_fully_obs(T, AG).

possible_world(T+1, T2+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, T, PP), possible_world(T2, R2, P, W),
                                         believes(T, T, PP, T2, R2, P, AG), is_partial_obs(T, AG).

% This rule makes sure that if there exists an updated possibility phi(P2) such that P2 is reached by P, then also P must be updated
possible_world(T+1, T1+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, T, PP), reaches(T, T, PP, T1, R1, P, all_agents),
                                         possible_world(T+1, T2+1, phi(P2), W2), possible_world(T2, R2, P2, W2), possible_world(T1, R1, P, W),
                                         believes(T1, R1, P, T2, R2, P2, AG), R1 < T, R2 <= T.

% Fully observant agents (sigma, sigma)
believes(T+1, T1+1, phi(P1), T+1, T2+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true), T1 <= T, T2 <= T, is_fully_obs(T, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _), possible_world(T+1, T2+1, phi(P2), _),
                                                        entails(T1, R1, P1, ANN), entails(T2, R2, P2, ANN).

% Fully observant agents (tau, tau)
believes(T+1, T1+1, phi(P1), T+1, T2+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true), T1 <= T, T2 <= T, is_fully_obs(T, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _), possible_world(T+1, T2+1, phi(P2), _),
                                                        not entails(T1, R1, P1, ANN), not entails(T2, R2, P2, ANN).

% Partially observant agents
believes(T+1, T1+1, phi(P1), T+1, T2+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_partial_obs(T, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _), possible_world(T+1, T2+1, phi(P2), _).

% Oblivious agents
believes(T+1, T1+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), announcement(ACT), is_oblivious(T, AG), T1 <= T, T2 <= T,
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _).

% Generate
0 { plan(T, ACT) : action(ACT) } 1 :- time(T), T < maxT.

% plan(0,open_ac).
% plan(1,peek_c).

% plan(0,open_a).
% plan(1,close_a).
% plan(2,open_c).
% plan(3,peek_c).

% Define
not_reached_goal(0) :- not entails(0, 0, P, F), pointed(0, 0, P), goal(F).
not_reached_goal(T) :- not entails(T, T, P, F), pointed(T, T, P), time(T), T > 0, goal(F).

reached_goal(0) :- not not_reached_goal(0).
% reached_goal(T) :- plan(T-1, ACT), is_executable(T-1, ACT), not not_reached_goal(T), time(T), T > 0.

reached_goal(T) :- plan(T-1, ACT), is_executable(T-1, ACT), time(T), T > 0,
                   0 { goal(F) : pointed(T, T, P), not entails(T, T, P, F) } 0.

% Test
:- plan(T, ACT), pointed(T, T, P), not entails(T, T, P, EXEC), action(ACT), executable(ACT, EXEC).
:- plan(T, ACT), pointed(T, T, P), 0 { is_executable_effect(T, ACT, T, T, P, CAUSED) :     causes(ACT, CAUSED, _) } 0, ontic(ACT).
:- plan(T, ACT), pointed(T, T, P), 0 { is_executable_effect(T, ACT, T, T, P, SENSED) : determines(ACT, SENSED, _) } 0, sensing(ACT).
:- plan(T, ACT), pointed(T, T, P), not entails(T, T, P, ANN),                           announces(ACT, ANN   , _)    , announcement(ACT).

:- 0 { reached_goal(T) : time(T) } 0.
:- reached_goal(T1), plan(T2, ACT),    T2 >= T1.
:- reached_goal(T1), reached_goal(T2), T2 >  T1.

% #minimize{ T : reached_goal(T) }.

% Display

% #show pointed/3.
% #show possible_world/4.
% #show believes/7.

#show plan/2.
#show reached_goal/1.

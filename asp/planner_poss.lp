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

unknown_initially(F)            :- not initially(c(all_agents, F)), not initially(c(all_agents, -F)), fluent(F).
initial_state_dimension(2 ** K) :- K = { fluent(F) : unknown_initially(F) }.
initial_fluent_set(1..K)        :- initial_state_dimension(K).

% The initial conditions of form 1. determine the pointed possibility of the initial state
% A possibility is pointed if it entails exactly those fluents that hold initially

    holds(1, F) :- initially (F), fluent(F).
not holds(1, F) :- initially(-F), fluent(F).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
    holds(W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F).
not holds(W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F).

% We impose that all the initial possibilities have a different fluent set
K/2 {     holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
K/2 { not holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).

:- K1 = { holds(W1, F) : not holds(W2, F) }, K2 = { holds(W2, F) : not holds(W1, F), fluent(F) }, % FN = { fluent(F) },
   initial_fluent_set(W1), initial_fluent_set(W2), W1 != W2, 0 = K1 + K2.

pointed(0, 0, 1).
possible_world(0, 0, W, W) :- initial_fluent_set(W).
fluent_set(W) :- possible_world(0, 0, W, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F),     entails(0, 0, P2, F), initial_fluent_set(P1), initial_fluent_set(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).
not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F), not entails(0, 0, P2, F), initial_fluent_set(P1), initial_fluent_set(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).

believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), agent(AG).


% ********** TRANSITION FUNCTION **********

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

pointed(T+1, T+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, T, P), possible_world(T+1, T+1, phi(P), _).

% possible_world(T, R, P, W): time T, repetition R, index P, fluent_set W
possible_world(T+1, T +1, phi(P), W2) :- is_executable(T, ACT), ontic(ACT), pointed(T, T, P ), possible_world(T , T , P, W1), possible_world(T2, R2, P2, W2), T2 <= T,
                                         0 { holds(W1, F) : not holds(W2, F), fluent(F) } 0, 0 { holds(W2, F) : not holds(W1, F), fluent(F) } 0.

possible_world(T+1, T +1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, T, P ), possible_world(T , T , P, W).

possible_world(T+1, T2+1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, T, PP), possible_world(T2, R2, P, W),
                                             believes(T, T, PP, T2, R2, P, AG), is_fully_obs(T, AG).

fluent_set(W) :- possible_world(T, R, P, W).

holds(phi(W), F) :- is_executable(T, ACT), ontic(ACT), possible_world(T+1, T2+1, phi(P), phi(W)), possible_world(T2, R2, P, W),
                    causes(ACT, F, EEXEC),     entails(T2, R2, P, EEXEC),               fluent(F), fluent_set(W), fluent_set(phi(W)).

holds(phi(W), F) :- is_executable(T, ACT), ontic(ACT), possible_world(T+1, T2+1, phi(P), phi(W)), possible_world(T2, R2, P, W),
                    causes(ACT, F, EEXEC), not entails(T2, R2, P, EEXEC),  holds(W, F), fluent(F), fluent_set(W), fluent_set(phi(W)).

holds(phi(W), F) :- is_executable(T, ACT), ontic(ACT), possible_world(T+1, T2+1, phi(P), phi(W)), possible_world(T2, R2, P, W),
                    0 { causes(ACT, F, _) } 0, 0 { causes(ACT, -F, _) } 0, holds(W, F), fluent(F), fluent_set(W), fluent_set(phi(W)).

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
believes(T+1, T1+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_oblivious(T, AG),
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
believes(T+1, T1+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_oblivious(T, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, T1+1, phi(P1), _).

% Generate
0 { plan(T, ACT) : action(ACT) } 1 :- time(T), T < maxT.

% plan(0,signal_a_b).
% plan(1,distract_a_b).
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

% :- possible_world(T1, R1, P1, _), agent(AG), 0 { believes(T1, R1, P1, T2, R2, P2, AG) : possible_world(T2, R2, P2, _) } 0.

:- 0 { reached_goal(T) : time(T) } 0.
:- reached_goal(T1), plan(T2, ACT),    T2 >= T1.
:- reached_goal(T1), reached_goal(T2), T2 >  T1.

#minimize{ T : reached_goal(T) }.

% Display

% #show pointed/3.
% #show possible_world/4.
% #show believes/7.

#show plan/2.
#show reached_goal/1.

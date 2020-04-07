%
% EMP encoding in ASP
%

% ********** REACHABILITY **********

% reaches/7: P1 reaches P2 with a path that contains only agents inside AGS
reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T2, R2, P2, AG), contains_ag(AGS, AG).
reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T3, R3, P3, AG), contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS).


% ********** ENTAILMENT **********

% entails/4: P entails the formula F
entails(T, R, P,  F    )      :-     holds(T, R, P, W, F),                       possible_world(T, R, P, W), fluent_set(W), fluent(F).
entails(T, R, P, -F    )      :- not holds(T, R, P, W, F),                       possible_world(T, R, P, W), fluent_set(W), fluent(F).

entails(T, R, P, neg(F))      :- not entails(T, R, P, F ),                       possible_world(T, R, P, _), formula(neg(F)),      formula(F).
entails(T, R, P, and(F1, F2)) :-     entails(T, R, P, F1), entails(T, R, P, F2), possible_world(T, R, P, _), formula(and(F1, F2)), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :-     entails(T, R, P, F1),                       possible_world(T, R, P, _), formula( or(F1, F2)), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :-                           entails(T, R, P, F2), possible_world(T, R, P, _), formula( or(F1, F2)), formula(F1), formula(F2).

not_entails(T1, R1, P1, b(AG , F)) :- not     entails(T2, R2, P2, F        ), believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T2, R2, P2, _), formula(b(AG , F)), formula(F), agent(AG).
entails    (T , R , P , b(AG , F)) :- not not_entails(T , R , P , b(AG , F)),                                       possible_world(T , R , P , _), formula(b(AG , F)).

not_entails(T1, R1, P1, c(AGS, F)) :- not     entails(T2, R2, P2, F        ), reaches(T1, R1, P1, T2, R2, P2, AGS), possible_world(T2, R2, P2, _), formula(c(AGS, F)), formula(F).
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

%     holds(1, F) :- initially (F), fluent(F).
% not holds(1, F) :- initially(-F), fluent(F).

% holds/5: the fluent set W associated to P contains the fluent F
    holds(0, 0, 1, 1, F) :- initially (F), fluent(F).
not holds(0, 0, 1, 1, F) :- initially(-F), fluent(F).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
%     holds(W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F).
% not holds(W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F).
    holds(0, 0, W, W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F).
not holds(0, 0, W, W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F).

% We impose that all the initial possibilities have a different fluent set
% K/2 {     holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
% K/2 { not holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
K/2 {     holds(0, 0, W, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
K/2 { not holds(0, 0, W, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).

:- K1 = { holds(0, 0, W1, W1, F) : not holds(0, 0, W2, W2, F) }, K2 = { holds(0, 0, W2, W2, F) : not holds(0, 0, W1, W1, F), fluent(F) },
% :- K1 = { holds(W1, F) : not holds(W2, F) }, K2 = { holds(W2, F) : not holds(W1, F), fluent(F) },
   initial_fluent_set(W1), initial_fluent_set(W2), W1 != W2, 0 = K1 + K2.

pointed(0, 0, 1).
possible_world(0, 0, W, W) :- initial_fluent_set(W).
fluent_set(W) :- possible_world(0, 0, W, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F),     entails(0, 0, P2, F), initial_fluent_set(P1), initial_fluent_set(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).
not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F), not entails(0, 0, P2, F), initial_fluent_set(P1), initial_fluent_set(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).

% believes/7: the agent AG, inside P1, believes P2 to be possible
believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), agent(AG).


% ********** TRANSITION FUNCTION **********

% is_executable/2: the action ACT is executable at time T
is_executable(T, ACT) :- plan(T, ACT), action(ACT), pointed(T, R, P), executable(ACT, EXEC), entails(T, R, P, EXEC), formula(EEXEC).
is_executable(T, ACT) :- plan(T, ACT), action(ACT), pointed(T, R, P), executable(ACT, true).

% is_executable_effect/6: the effect CAUSED/SENSED/ANN of the action ACT is executable at time T
is_executable_effect(T, ACT, T , R , P , CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, true ), pointed(T, R, P).
is_executable_effect(T, ACT, T , R , P , CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), entails(T, R, P, EEXEC), pointed(T, R, P).

is_executable_effect(T, ACT, T , R , P , SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, true ), pointed(T, R, P).
is_executable_effect(T, ACT, T , R , P , SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), entails(T, R, P, EEXEC), pointed(T, R, P).

is_executable_effect(T, ACT, T , R , P , ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true ), pointed(T, R, P).
is_executable_effect(T, ACT, T , R , P , ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, EEXEC), entails(T, R, P, EEXEC), pointed(T, R, P).

is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, true ), possible_world(T2, R2, P2, _), T2 <= T,
                                                    pointed(T, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2, _), T2 <= T, formula(EEXEC),
                                                    pointed(T, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, true), possible_world(T2, R2, P2, _), T2 <= T, fluent(SENSED),
                                                    pointed(T, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), possible_world(T2, R2, P2, _), T2 <= T, formula(EEXEC), fluent(SENSED),
                                                    pointed(T, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true), possible_world(T2, R2, P2, _), T2 <= T,
                                                    pointed(T, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, EEXEC), possible_world(T2, R2, P2, _), T2 <= T, formula(EEXEC),
                                                    pointed(T, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

% is_fully_obs/2: the agent AG is fully observant
% is_partially_obs/2: the agent AG is partially observant
% is_oblivious/2: the agent AG is oblivious

is_fully_obs  (0,   AG) :- agent(AG).

is_fully_obs  (T+1, AG) :- is_executable(T, ACT), action(ACT),       observes (ACT, AG, true), agent(AG).
is_partial_obs(T+1, AG) :- is_executable(T, ACT), action(ACT),       aware_of (ACT, AG, true), agent(AG).
is_oblivious  (T+1, AG) :- is_executable(T, ACT), action(ACT),       oblivious(ACT, AG),       agent(AG).

is_fully_obs  (T+1, AG) :- is_executable(T, ACT), action(ACT),       observes (ACT, AG, OBS ), agent(AG),     entails(T, R, PS, OBS), formula(OBS), pointed(T, R, PS).

is_partial_obs(T+1, AG) :- is_executable(T, ACT), sensing(ACT),      aware_of (ACT, AG, OBS ), agent(AG),     entails(T, R, PS, OBS), formula(OBS), pointed(T, R, PS).
is_partial_obs(T+1, AG) :- is_executable(T, ACT), announcement(ACT), aware_of (ACT, AG, OBS ), agent(AG),     entails(T, R, PS, OBS), formula(OBS), pointed(T, R, PS).

is_oblivious  (T+1, AG) :- is_executable(T, ACT), action(ACT),       observes (ACT, AG, OBS ), agent(AG), not entails(T, R, PS, OBS), formula(OBS), pointed(T, R, PS).
is_oblivious  (T+1, AG) :- is_executable(T, ACT), sensing(ACT),      aware_of (ACT, AG, OBS ), agent(AG), not entails(T, R, PS, OBS), formula(OBS), pointed(T, R, PS).
is_oblivious  (T+1, AG) :- is_executable(T, ACT), announcement(ACT), aware_of (ACT, AG, OBS ), agent(AG), not entails(T, R, PS, OBS), formula(OBS), pointed(T, R, PS).

max_rep(T, R) :- plan(T, ACT), pointed(T, R, P), not reached_goal(T).

% ONTIC ACTIONS

% pointed/3: the possibility P is pointed at time T
pointed(T+1, R+MR+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, P), possible_world(T+1, R+MR+1, phi(P), _), max_rep(T, MR).


% possible_world/4: at time T the possibility P with repetition R is associated with the fluent set W
% A triple (T, R, P) univocally identifies a possibility. When we update such a triple when an action occurres, we obtain the triple (T+1, R+MR, phi(P)),
% where MR is the maximum repetition that can be found at time T and phi(P) is a new possibility
possible_world(T+1, R +MR+1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, P ), possible_world(T, R , P, W), max_rep(T, MR). %,
                                                % 0 { fluent_set(W1) : T1 <= T, possible_world(T1, _, _, W1), apply_ontic(W1, T, R, P, T) } 0.

% possible_world(T+1, R +MR+1, phi(P),     W1) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, P ), possible_world(T, R , P, W), max_rep(T, MR),
%                                                 possible_world(T1, _, _, W1), T1 <= T, apply_ontic(W1, T, R, P, T), W1 != W.

possible_world(T+1, R2+MR+1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                                believes(T, R, PP, T2, R2, P, AG), is_fully_obs(T+1, AG). %,
                                                % 0 { fluent_set(W1): T1 <= T, possible_world(T1, _, _, W1), apply_ontic(W1, T2, R2, P, T) } 0.

% possible_world(T+1, R2+MR+1, phi(P),     W1) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                                 believes(T, R, PP, T2, R2, P, AG), is_fully_obs(T+1, AG),
%                                                 possible_world(T1, _, _, W1), T1 <= T, apply_ontic(W1, T2, R2, P, T).

possible_world(T+1, R2+MR+1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                                reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0. %,
                                                % 0 { fluent_set(W1): T1 <= T, possible_world(T1, _, _, W1), apply_ontic(W1, T2, R2, P, T) } 0.

% possible_world(T+1, R2+MR+1, phi(P),     W1) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                                 reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0,
%                                                 possible_world(T1, _, _, W1), T1 <= T, apply_ontic(W1, T2, R2, P, T).


fluent_set(W) :- possible_world(T, R, P, W).

holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
% holds(phi(W), F) :- is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                       is_executable_effect(T, ACT, T2, R2, P, F), fluent(F), fluent_set(W), fluent_set(phi(W)).% ,
                                    %    0 { time(T1): T1 <= T, possible_world(T1, _, _, phi(W)), apply_ontic(phi(W), T2, R2, P, T) } 0.

not holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                       is_executable_effect(T, ACT, T2, R2, P, -F), fluent(F), fluent_set(W), fluent_set(phi(W)).% ,


holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                       not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F),
                                       holds(T2, R2, P, W, F), fluent(F), fluent_set(W), fluent_set(phi(W)).

not holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                       not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F),
                                       not holds(T2, R2, P, W, F), fluent(F), fluent_set(W), fluent_set(phi(W)).

% holds(phi(W), F) :- is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                        not is_executable_effect(T, ACT, T2, R2, P, F), holds(W, F), fluent(F), fluent_set(W), fluent_set(phi(W)),
                                    %    0 { time(T1): T1 <= T, possible_world(T1, _, _, phi(W)), apply_ontic(phi(W), T2, R2, P, T) } 0.


% Fully observant agents
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                        is_fully_obs(T+1, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _), max_rep(T, MR).

% Oblivious agents
believes(T+1, R1+MR+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                        is_oblivious(T+1, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), max_rep(T, MR).

% SENSING ACTIONS

% hold_sensed_effects/4: the possibility P2 agrees with the pointed possibility on the SENSED fluents of the sensing action ACT performed at time T
hold_sensed_effects(T , R , P , T) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), pointed(T, R, P).
hold_sensed_effects(T2, R2, P2, T) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), possible_world(T, R, PP, WP), possible_world(T2, R2, P2, W2), T2 <= T,
                                    %   0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _),     holds(W2, SENSED), not holds(WP, SENSED) } 0,
                                    %   0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _), not holds(W2, SENSED),     holds(WP, SENSED) } 0,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _),     holds(T2, R2, P2, W2, SENSED), not holds(T, R, PP, WP, SENSED) } 0,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _), not holds(T2, R2, P2, W2, SENSED),     holds(T, R, PP, WP, SENSED) } 0,
                                      pointed(T, R, PP), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).


pointed(T+1, R+MR+1, phi(P)) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, P), possible_world(T+1, R+MR+1, phi(P), _), max_rep(T, MR).


possible_world(T+1, R+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, P), possible_world(T, R, P, W), max_rep(T, MR).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                         believes(T, R, PP, T2, R2, P, AG), hold_sensed_effects(T2, R2, P, T), is_fully_obs(T+1, AG).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                         reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0,
                                         hold_sensed_effects(T2, R2, P, T).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                         believes(T, R, PP, T2, R2, P, AG), is_partial_obs(T+1, AG).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                         reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0.%,
                                        %  1 <= { agent(AG) : contains_ag(AGS, AG), is_partial_obs(T+1, AG) }, not hold_sensed_effects(T2, R2, P, T).


holds(T+1, R2+MR+1, phi(P), W, F) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), possible_world(T+1, R2+MR+1, phi(P), W), possible_world(T2, R2, P, W),
                                  holds(T2, R2, P, W, F), fluent(F), fluent_set(W), max_rep(T, MR).


% Fully observant agents (sigma, sigma)
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                        hold_sensed_effects(T1, R1, P1, T), hold_sensed_effects(T2, R2, P2, T), max_rep(T, MR).

% Fully observant agents (tau, tau)
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                        not hold_sensed_effects(T1, R1, P1, T), not hold_sensed_effects(T2, R2, P2, T), max_rep(T, MR).

% Partially observant agents
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_partial_obs(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _), max_rep(T, MR).

% Oblivious agents
believes(T+1, R1+MR+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_oblivious(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), max_rep(T, MR).

% ANNOUNCEMENT ACTIONS

% hold_announced_effects/4: the possibility P2 agrees with the pointed possibility on the ANN formulas of the announcement action ACT performed at time T
hold_announced_effects(T , R , P , T) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), pointed(T, R, P).
hold_announced_effects(T2, R2, P2, T) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), possible_world(T, R, PP, WP), possible_world(T2, R2, P2, W2), T2 <= T,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _),     entails(T2, R2, P2, ANN), not entails(T, R, PP, ANN) } 0,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _), not entails(T2, R2, P2, ANN),     entails(T, R, PP, ANN) } 0,
                                      pointed(T, R, PP), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).


pointed(T+1, R+MR+1, phi(P)) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, P), possible_world(T+1, R+MR+1, phi(P), _), max_rep(T, MR).


possible_world(T+1, R+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, P), possible_world(T, R, P, W), max_rep(T, MR).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                         believes(T, R, PP, T2, R2, P, AG), hold_announced_effects(T2, R2, P, T), is_fully_obs(T+1, AG).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                         reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0,
                                         hold_announced_effects(T2, R2, P, T).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                         believes(T, R, PP, T2, R2, P, AG), is_partial_obs(T+1, AG).

possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR), not pointed(T, R, P),
                                         reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_partial_obs(T+1, AG) } 0.

holds(T+1, R2+MR+1, phi(P), W, F) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), possible_world(T+1, R2+MR+1, phi(P), W), possible_world(T2, R2, P, W),
                                  holds(T2, R2, P, W, F), fluent(F), fluent_set(W), max_rep(T, MR).


% Fully observant agents (sigma, sigma)
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                        hold_announced_effects(T1, R1, P1, T), hold_announced_effects(T2, R2, P2, T), max_rep(T, MR).

% Fully observant agents (tau, tau)
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                        not hold_announced_effects(T1, R1, P1, T), not hold_announced_effects(T2, R2, P2, T), max_rep(T, MR).

% Partially observant agents
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_partial_obs(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _), max_rep(T, MR).

% Oblivious agents
believes(T+1, R1+MR+1, phi(P1), T2 , R2  ,     P2 , AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_oblivious(T+1, AG),
                                                        believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                        possible_world(T+1, R1+MR+1, phi(P1), _), max_rep(T, MR).


% Generate
0 { plan(T, ACT) : action(ACT) } 1 :- time(T), T < maxT.

% Define
not_reached_goal(0) :- not entails(0, 0, P, F), pointed(0, 0, P), goal(F).
not_reached_goal(T) :- not entails(T, R, P, F), pointed(T, R, P), time(T), T > 0, goal(F).

reached_goal(0) :- not not_reached_goal(0).
reached_goal(T) :- plan(T-1, ACT), is_executable(T-1, ACT), not not_reached_goal(T), time(T), T > 0.

% Test
:- is_executable(T, ACT), 0 { pointed(T, R, P) } 0.
:- plan(T, ACT), not is_executable(T, ACT).
:- plan(T, ACT), pointed(T, R, P), not entails(T, R, P, EXEC), action(ACT), executable(ACT, EXEC), formula(EXEC).

:- plan(T, ACT), pointed(T, R, P), 0 { is_executable_effect(T, ACT, T, R, P, CAUSED) :     causes(ACT, CAUSED, _) } 0, ontic(ACT).
:- plan(T, ACT), pointed(T, R, P), 0 { is_executable_effect(T, ACT, T, R, P, SENSED) : determines(ACT, SENSED, _) } 0, sensing(ACT).
:- plan(T, ACT), pointed(T, R, P), 0 { is_executable_effect(T, ACT, T, R, P, ANN   ) :  announces(ACT, ANN   , _) } 0, announcement(ACT).

:- possible_world(T, R, P, W1), possible_world(T, R, P, W2), W1 != W2.
:- possible_world(T1, R1, P1, _), agent(AG), 0 { believes(T1, R1, P1, T2, R2, P2, AG) : possible_world(T2, R2, P2, _) } 0.
:- possible_world(T1, R1, P1, _), agent(AG), 0 { possible_world(T2, R2, P2, _) : believes(T1, R1, P1, T2, R2, P2, AG) } 0.

:- 0 { reached_goal(T) : time(T) } 0.
:- reached_goal(T1), plan(T2, ACT),    T2 >= T1.
:- reached_goal(T1), reached_goal(T2), T2 >  T1.

#minimize{ T : reached_goal(T) }.

% Display

#show pointed/3.
#show possible_world/4.
#show believes/7.

#show plan/2.
#show reached_goal/1.

% OTHER STUFF...

% When we apply ACT to P2, the possibility phi(P2) must entail CAUSED if it holds causes(T, ACT, CAUSED)
% apply_ontic_fluent(T2, R2, P2,  CAUSED, T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2),
%                                               is_executable_effect(T, ACT, T2, R2, P2,  CAUSED), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% When we apply ACT to P2, the possibility phi(P2) must entail F if it does not hold nor causes(T, ACT, F), nor causes(T, ACT, -F) and if P2 entails F
% apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), holds(T2, R2, P2, W2, F),
% apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), holds(W2, F),
%                                               not is_executable_effect(T, ACT, T2, R2, P2, F), not is_executable_effect(T, ACT, T2, R2, P2, -F), fluent(F), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% not apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), not holds(T2, R2, P2, W2, F),
% not apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), not holds(W2, F),
%                                               not is_executable_effect(T, ACT, T2, R2, P2, F), not is_executable_effect(T, ACT, T2, R2, P2, -F), fluent(F), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% The fluent_set W1 is the one resulting from W2 when we apply ACT. W1 must entail all the fluents that are caused by ACT and must agree with W2 w.r.t. the other fluents
% apply_ontic(W1, T2, R2, P2, T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), fluent_set(W1), possible_world(T1, R1, P1, W1), possible_world(T2, R2, P2, W2), T1 <= T, T2 <= T,
%                                   0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2,  F, T), not holds(W1, F) } 0, %not holds(T1, R1, P1, W1, F) } 0,
%                                   0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2, -F, T),     holds(W1, F) } 0, %not holds(T1, R1, P1, W1, F) } 0,
%                                   0 { fluent(F) : not apply_ontic_fluent(T2, R2, P2,  F, T),     holds(W1, F), not holds(W2, F) } 0, %holds(T1, R1, P1, W1, F), not holds(T2, R2, P2, W2, F) } 0,
%                                   pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% 0 { apply_ontic(W1, T2, R2, P2, T) : possible_world(T1, R1, P1, W1), T1 <= T } 1 :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2),
%                                                                                     pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% #show apply_ontic_fluent/5.
% #show apply_ontic/5.

% :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), fluent_set(W1), possible_world(T1, R1, P1, W1), possible_world(T2, R2, P2, W2), T1 <= T, T2 <= T,
%    apply_ontic(W1, T2, R2, P2, T),     apply_ontic_fluent(T2, R2, P2,  F, T), fluent(F), not holds(W1, F).

% :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), fluent_set(W1), possible_world(T1, R1, P1, W1), possible_world(T2, R2, P2, W2), T1 <= T, T2 <= T,
%    apply_ontic(W1, T2, R2, P2, T),     apply_ontic_fluent(T2, R2, P2, -F, T), fluent(F),     holds(W1, F).

% :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), fluent_set(W1), possible_world(T1, R1, P1, W1), possible_world(T2, R2, P2, W2), T1 <= T, T2 <= T,
%    apply_ontic(W1, T2, R2, P2, T), not apply_ontic_fluent(T2, R2, P2,  F, T), fluent(F),     holds(W1, F), not holds(W2, F).

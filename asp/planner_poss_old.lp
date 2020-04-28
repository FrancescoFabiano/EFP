%
% EMP encoding in ASP
%

% ########## READ ME ##########
% 
% Possibili ottimizzazioni da realizzare:
%  1. Fare in modo che lo stato iniziale sia univoco rimuovendo le varie permutazioni.
% 
%  2. Limitare i predicati entails(T, R, P, F) nelle sole possibility P in cui effettivamente è necessario controllare F.
%     E.g.: formule come executability conditions, goals, observability conditions sono da controllare solo sulla possibility puntata.
% 
%  3. Riutilizzare gli indici dei fluent set già presenti a tempo T' <= T quando eseguo un'azione ontic. Le possibility a tempo T+1
%     dovranno utilizzare "vecchi" fluent set se sono già stati calcolati precedentemente. Altrimenti, si genera il fluent set phi(W).
% 
%  4. Eventualmente aggiungere altri constraint.
% 
% Esempio di utilizzo da terminale:
%  1. Per eseguire il modello su un'istanza già generata:
%       >> clingo planner_poss.lp path/to/domain --configuration=jumpy
%     Ad esempio:
%       >> clingo planner_poss.lp ./domains/Coin_in_the_Box__pl_5.lp --configuration=jumpy
% 
%  2. Per generare una nuova istanza a partire da un .txt in mA*:
%       >> ./bin/efp.out path/to/domain -generate_asp
% 
%  3. Per generare una rappresentazione grafica degli stati (ricordarsi di decommentare gli statement #show per i predicati pointed, possible_world, believes):
%       >> ./asp_print path/to/domain
%
% ##########   END   ##########


% ********** REACHABILITY **********

% reaches/7: P1 reaches P2 with a path that contains only agents inside AGS
reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T2, R2, P2, AG), contains_ag(AGS, AG).
reaches(T1, R1, P1, T2, R2, P2, AGS) :- believes(T1, R1, P1, T3, R3, P3, AG), contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS).


% ********** ENTAILMENT **********

literal( F) :- fluent(F).
literal(-F) :- fluent(F).

entails_init(W,  F    )      :-     holds(0, 0, W, W, F),                     initial_fluent_set(W), fluent(F), init_sf(F).
entails_init(W, -F    )      :- not holds(0, 0, W, W, F),                     initial_fluent_set(W), fluent(F), init_sf(F).

entails_init(W, neg(F))      :- not entails_init(W, F ),                      initial_fluent_set(W), init_sf(neg(F)),      init_sf(F).
entails_init(W, and(F1, F2)) :-     entails_init(W, F1), entails_init(W, F2), initial_fluent_set(W), init_sf(and(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2)) :-     entails_init(W, F1),                      initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2)) :-                          entails_init(W, F2), initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).

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
pointed(0, 0, W) :- good_initial_fluent_set(W), 0 { fluent(F) : initially(F), not holds(0, 0, W, W, F) } 0, 0 { fluent(F) : initially(-F), holds(0, 0, W, W, F) } 0.

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
    holds(0, 0, W, W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F).
not holds(0, 0, W, W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F).

%     holds(W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F).
% not holds(W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F).

% We impose that all the initial possibilities have a different fluent set
K/2 {     holds(0, 0, W, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
K/2 { not holds(0, 0, W, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).

% K/2 {     holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
% K/2 { not holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).

% All the initial fluent sets must be different
:- K1 = { holds(0, 0, W1, W1, F) : not holds(0, 0, W2, W2, F) }, K2 = { holds(0, 0, W2, W2, F) : not holds(0, 0, W1, W1, F), fluent(F) }, % :- K1 = { holds(W1, F) : not holds(W2, F) }, K2 = { holds(W2, F) : not holds(W1, F), fluent(F) },
   initial_fluent_set(W1), initial_fluent_set(W2), W1 != W2, K1 + K2 = 0.

% *** Working on optimization #3... ***

% We impose a complete order on the fluent sets indices W_i w.r.t. their fluents
:- MF1 = #max { F : fluent(F), unknown_initially(F),     holds(0, 0, W1, W1, F), not holds(0, 0, W2, W2, F) },
   MF2 = #max { F : fluent(F), unknown_initially(F), not holds(0, 0, W1, W1, F),     holds(0, 0, W2, W2, F) },
   good_initial_fluent_set(W1), good_initial_fluent_set(W2), W1 < W2, MF1 > MF2.

% ***              End              ***

% So far we dealt with (commonly known) fluents. An initial fluent set is "good" if it entails all the fluent formulas F that are initial common knowledge
good_initial_fluent_set(W) :- initial_fluent_set(W), 0 { initially(c(all_agents, F)) : not entails_init(W, F), initially(c(all_agents, F)), init_sf(F), not literal(F) } 0.

% For each good initial fluent set we create a correspoinding possible world
possible_world(0, 0, W, W) :- good_initial_fluent_set(W).
fluent_set(W) :- possible_world(0, 0, W, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails_init(P1, F),     entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).
not_believes_initially(P1, P2, AG) :-     entails_init(P1, F), not entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).

% believes/7: the agent AG, inside P1, believes P2 to be possible
believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), agent(AG).


% ********** TRANSITION FUNCTION **********

% is_executable/2: the action ACT is executable at time T
is_executable(T, ACT) :- plan(T, ACT), action(ACT), pointed(T, R, P), executable(ACT, EXEC), entails(T, R, P, EXEC), formula(EXEC).
is_executable(T, ACT) :- plan(T, ACT), action(ACT), pointed(T, R, P), executable(ACT, true).

% is_executable_effect/6: the effect CAUSED/SENSED/ANN of the action ACT is executable at time T
is_executable_effect(T, ACT, T , R , P , CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, true ), pointed(T, R, P).
is_executable_effect(T, ACT, T , R , P , CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), entails(T, R, P, EEXEC), pointed(T, R, P).

is_executable_effect(T, ACT, T , R , P , SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, true ), pointed(T, R, P).
is_executable_effect(T, ACT, T , R , P , SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), entails(T, R, P, EEXEC), pointed(T, R, P).

is_executable_effect(T, ACT, T , R , P , ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true ), pointed(T, R, P).
is_executable_effect(T, ACT, T , R , P , ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, EEXEC), entails(T, R, P, EEXEC), pointed(T, R, P).

% For ontic actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully observant agents.
% Then the effect CAUSED is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, true ), possible_world(T2, R2, P2, _), T2 <= T,
                                                    pointed(T, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2, _), T2 <= T, formula(EEXEC),
                                                    pointed(T, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

% For sensing/announcement actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully or partially observant agents.
% Then the effect SENSED/ANN is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, true), possible_world(T2, R2, P2, _), T2 <= T, fluent(SENSED),
                                                    pointed(T, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), possible_world(T2, R2, P2, _), T2 <= T, formula(EEXEC), fluent(SENSED),
                                                    pointed(T, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, true), possible_world(T2, R2, P2, _), T2 <= T,
                                                    pointed(T, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), announces(ACT, ANN, EEXEC), possible_world(T2, R2, P2, _), T2 <= T, formula(EEXEC),
                                                    pointed(T, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).

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

% The maximum repetition at time T is the repetition of the pointed possibility
max_rep(T, R) :- plan(T, ACT), pointed(T, R, P), not reached_goal(T).

% ONTIC ACTIONS

% pointed/3: the possibility P is pointed at time T
pointed(T+1, R+MR+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, P), possible_world(T+1, R+MR+1, phi(P), _), max_rep(T, MR).


% possible_world/4: at time T the possibility P with repetition R is associated with the fluent set W
% A triple (T, R, P) univocally identifies a possibility. When we update such a triple when an action occurres, we obtain the triple (T+1, R+MR, phi(P)),
% where MR is the maximum repetition that can be found at time T and phi(P) is a new possibility

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T
possible_world(T+1, R +MR+1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, P ), possible_world(T, R , P, W), max_rep(T, MR). %,
                                                % 0 { fluent_set(W1) : T1 <= T, possible_world(T1, _, _, W1), apply_ontic(W1, T, R, P, T) } 0.

% If a fully observant agent AG believes P2, then we create the new possibility phi(P2)
% possible_world(T+1, R2+MR+1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                                 believes(T, R, PP, T2, R2, P, AG), is_fully_obs(T+1, AG). %,
                                                % 0 { fluent_set(W1): T1 <= T, possible_world(T1, _, _, W1), apply_ontic(W1, T2, R2, P, T) } 0.

% Let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully observant agents.
% Then we create the new possibility phi(P2). We need this rule since we assume that each fully observant agent knows which agent is fully observant as well.
% Hence we must update the fluent set of all the possibilities such as P2.
% [To see a pratical example print the representation of the states using Coin in the Box with the actions: plan(0, singal_a_b). plan(1, singal_a_c). plan(2, open_a). The rule generates the possibilities "3_4_p1" and "3_4_p2" ]
possible_world(T+1, R2+MR+1, phi(P), phi(W)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                                reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0. %,
                                                % 0 { fluent_set(W1): T1 <= T, possible_world(T1, _, _, W1), apply_ontic(W1, T2, R2, P, T) } 0.

% We keep track of the newly generated fluent sets
fluent_set(W) :- possible_world(T, R, P, W).

% holds/5: the fluent set W associated to P contains the fluent F

% A fluent F holds in the fluent set phi(W) associated to the possibility phi(P) if it is caused by the ontic action ACT and if the effect is executable w.r.t. P
    holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                              is_executable_effect(T, ACT, T2, R2, P, F), fluent(F), fluent_set(W), fluent_set(phi(W)).

% A fluent F must not hold in the fluent set phi(W) associated to the possibility phi(P) if -F is caused by the ontic action ACT and if the effect is executable w.r.t. P
not holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                              is_executable_effect(T, ACT, T2, R2, P, -F), fluent(F), fluent_set(W), fluent_set(phi(W)).

% Inertia: if a fluent F is not modified by the ontic action ACT, then it holds in phi(W) if it was true in W...
    holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                              not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F),
                                              holds(T2, R2, P, W, F), fluent(F), fluent_set(W), fluent_set(phi(W)).

% ... and it must not hold in phi(W) if it was false in W.
not holds(T+1, R2+MR+1, phi(P), phi(W), F) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
                                              not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F),
                                              not holds(T2, R2, P, W, F), fluent(F), fluent_set(W), fluent_set(phi(W)).

% !!!
% PER PROF: ho lasciato not holds nelle teste delle regole qui sopra perché trasformandoli in vincoli diventa meno efficiente (test condotto su Coin_in_the_Box__pl_5.lp)
% !!!

% *** Working on optimization #3... ***
% possible_world(T+1, R +MR+1, phi(P),     W1) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, P ), possible_world(T, R , P, W), max_rep(T, MR),
%                                                 possible_world(T1, _, _, W1), T1 <= T, apply_ontic(W1, T, R, P, T), W1 != W.

% possible_world(T+1, R2+MR+1, phi(P),     W1) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                                 believes(T, R, PP, T2, R2, P, AG), is_fully_obs(T+1, AG),
%                                                 possible_world(T1, _, _, W1), T1 <= T, apply_ontic(W1, T2, R2, P, T).

% possible_world(T+1, R2+MR+1, phi(P),     W1) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                                 reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0,
%                                                 possible_world(T1, _, _, W1), T1 <= T, apply_ontic(W1, T2, R2, P, T).
%
% holds(phi(W), F) :- is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                        0 { time(T1): T1 <= T, possible_world(T1, _, _, phi(W)), apply_ontic(phi(W), T2, R2, P, T) } 0.

% holds(phi(W), F) :- is_executable(T, ACT), ontic(ACT), possible_world(T+1, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                        not is_executable_effect(T, ACT, T2, R2, P, F), holds(W, F), fluent(F), fluent_set(W), fluent_set(phi(W)),
%                                        0 { time(T1): T1 <= T, possible_world(T1, _, _, phi(W)), apply_ontic(phi(W), T2, R2, P, T) } 0.
% ***              End              ***

% Fully observant agents: if P1 -- AG --> P2 at time T and AG is fully observant, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                              is_fully_obs(T+1, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _), max_rep(T, MR).

% Oblivious agents: if P1 -- AG --> P2 at time T and AG is oblivious, then phi(P1) -- AG --> P2 at time T+1
believes(T+1, R1+MR+1, phi(P1), T2 , R2     ,     P2 , AG) :- is_executable(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T,
                                                              is_oblivious(T+1, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), max_rep(T, MR).

% SENSING ACTIONS

% hold_sensed_effects/4: the possibility P2 agrees with the pointed possibility on the SENSED fluents of the sensing action ACT performed at time T
hold_sensed_effects(T , R , P , T) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), pointed(T, R, P).
hold_sensed_effects(T2, R2, P2, T) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), possible_world(T, R, PP, WP), possible_world(T2, R2, P2, W2), T2 <= T,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _),     holds(T2, R2, P2, W2, SENSED), not holds(T, R, PP, WP, SENSED) } 0,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _), not holds(T2, R2, P2, W2, SENSED),     holds(T, R, PP, WP, SENSED) } 0,
                                      pointed(T, R, PP), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).


pointed(T+1, R+MR+1, phi(P)) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, P), possible_world(T+1, R+MR+1, phi(P), _), max_rep(T, MR).


% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T
possible_world(T+1, R +MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, P), possible_world(T, R, P, W), max_rep(T, MR).

% If a fully observant agent AG believes P2 and P2 agrees with the pointed possibility on the SENSED fluents, then we create the new possibility phi(P2)
% possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                             believes(T, R, PP, T2, R2, P, AG), hold_sensed_effects(T2, R2, P, T), is_fully_obs(T+1, AG).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                            reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0,
                                            hold_sensed_effects(T2, R2, P, T).

% If a partially observant agent AG believes P2, then we create the new possibility phi(P2)
possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                            believes(T, R, PP, T2, R2, P, AG), is_partial_obs(T+1, AG).

% !!!
% PER PROF: la regola qui sotto può generare mondi superflui (ovvero non raggiungibili dal mondo puntato). Si provi a stampare il grafico dell'istanza
% Coin_in_the_Box__pl_6.lp con piano: plan(0, signal_a_b). plan(1, distract_b_a). plan(2, signal_b_c). plan(3, open_a). plan(4, peek_b). plan(5, peek_c).
% Le ultime due azioni generano due nuove possibility ridondanti: "5_31_ppppp2" e "6_60_pppp1".
% !!!
possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), sensing(ACT), pointed(T, R, PP), possible_world(T1, R1, P1, W1), possible_world(T2, R2, P, W), max_rep(T, MR),
                                            believes(T, R, PP, T1, R1, P1, AG), is_partial_obs(T, AG), reaches(T1, R1, P1, T2, R2, P, AGS),0 { agent(AG1) : contains_ag(AGS, AG1), is_oblivious(T+1, AG1) } 0.
                                        %  1 <= { agent(AG) : contains_ag(AGS, AG), is_partial_obs(T+1, AG) }, not hold_sensed_effects(T2, R2, P, T).

% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in phi(P) the fluents that hold in W
holds(T+1, R2+MR+1, phi(P), W, F) :- plan(T, ACT), is_executable(T, ACT), sensing(ACT), possible_world(T+1, R2+MR+1, phi(P), W), possible_world(T2, R2, P, W),
                                     holds(T2, R2, P, W, F), fluent(F), fluent_set(W), max_rep(T, MR).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time T and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                              believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                              hold_sensed_effects(T1, R1, P1, T), hold_sensed_effects(T2, R2, P2, T), max_rep(T, MR).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time T and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                              believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                              not hold_sensed_effects(T1, R1, P1, T), not hold_sensed_effects(T2, R2, P2, T), max_rep(T, MR).

% Partially observant agents: if P1 -- AG --> P2 at time T and AG is partially observant, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_partial_obs(T+1, AG),
                                                              believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _), max_rep(T, MR).

% Oblivious agents: if P1 -- AG --> P2 at time T and AG is oblivious, then phi(P1) -- AG --> P2 at time T+1
believes(T+1, R1+MR+1, phi(P1), T2 , R2     ,     P2 , AG) :- is_executable(T, ACT), sensing(ACT), T1 <= T, T2 <= T, is_oblivious(T+1, AG),
                                                              believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), max_rep(T, MR).

% ANNOUNCEMENT ACTIONS

% hold_announced_effects/4: the possibility P2 agrees with the pointed possibility on the ANN formulas of the announcement action ACT performed at time T
hold_announced_effects(T , R , P , T) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), pointed(T, R, P).
hold_announced_effects(T2, R2, P2, T) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), possible_world(T, R, PP, WP), possible_world(T2, R2, P2, W2), T2 <= T,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _),     entails(T2, R2, P2, ANN), not entails(T, R, PP, ANN) } 0,
                                      0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _), not entails(T2, R2, P2, ANN),     entails(T, R, PP, ANN) } 0,
                                      pointed(T, R, PP), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0, reaches(T, R, PP, T2, R2, P2, AGS).


pointed(T+1, R+MR+1, phi(P)) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, P), possible_world(T+1, R+MR+1, phi(P), _), max_rep(T, MR).


% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T
possible_world(T+1, R+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, P), possible_world(T, R, P, W), max_rep(T, MR).

% If a fully observant agent AG believes P2 and P2 agrees with the pointed possibility on the ANN formulas, then we create the new possibility phi(P2)
% possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
%                                             believes(T, R, PP, T2, R2, P, AG), hold_announced_effects(T2, R2, P, T), is_fully_obs(T+1, AG).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                            reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0,
                                            hold_announced_effects(T2, R2, P, T).

% If a partially observant agent AG believes P2, then we create the new possibility phi(P2)
possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T2, R2, P, W), max_rep(T, MR),
                                            believes(T, R, PP, T2, R2, P, AG), is_partial_obs(T+1, AG).

% !!!
% PER PROF: stessa cosa delle sensing...
% !!!
possible_world(T+1, R2+MR+1, phi(P), W ) :- is_executable(T, ACT), announcement(ACT), pointed(T, R, PP), possible_world(T1, R1, P1, W1), possible_world(T2, R2, P, W), max_rep(T, MR), not pointed(T, R, P),
                                            believes(T, R, PP, T1, R1, P1, AG), is_partial_obs(T, AG), reaches(T1, R1, P1, T2, R2, P, AGS), 0 { agent(AG1) : contains_ag(AGS, AG1), is_oblivious(T+1, AG1) } 0.

% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in phi(P) the fluents that hold in W
holds(T+1, R2+MR+1, phi(P), W, F) :- plan(T, ACT), is_executable(T, ACT), announcement(ACT), possible_world(T+1, R2+MR+1, phi(P), W), possible_world(T2, R2, P, W),
                                     holds(T2, R2, P, W, F), fluent(F), fluent_set(W), max_rep(T, MR).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time T and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                              believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                              hold_announced_effects(T1, R1, P1, T), hold_announced_effects(T2, R2, P2, T), max_rep(T, MR).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time T and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_fully_obs(T+1, AG),
                                                              believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _),
                                                              not hold_announced_effects(T1, R1, P1, T), not hold_announced_effects(T2, R2, P2, T), max_rep(T, MR).

% Partially observant agents: if P1 -- AG --> P2 at time T and AG is partially observant, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T+1, R1+MR+1, phi(P1), T+1, R2+MR+1, phi(P2), AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_partial_obs(T+1, AG),
                                                              believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                              possible_world(T+1, R1+MR+1, phi(P1), _), possible_world(T+1, R2+MR+1, phi(P2), _), max_rep(T, MR).

% Oblivious agents: if P1 -- AG --> P2 at time T and AG is oblivious, then phi(P1) -- AG --> P2 at time T+1
believes(T+1, R1+MR+1, phi(P1), T2 , R2     ,     P2 , AG) :- is_executable(T, ACT), announcement(ACT), T1 <= T, T2 <= T, is_oblivious(T+1, AG),
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
% :- plan(T, ACT), pointed(T, R, P), 0 { is_executable_effect(T, ACT, T, R, P, SENSED) : determines(ACT, SENSED, _) } 0, sensing(ACT).
% :- plan(T, ACT), pointed(T, R, P), 0 { is_executable_effect(T, ACT, T, R, P, ANN   ) :  announces(ACT, ANN   , _) } 0, announcement(ACT).

:- possible_world(T, R, P, W1), possible_world(T, R, P, W2), W1 != W2.
% :- possible_world(T1, R1, P1, _), agent(AG), 0 { believes(T1, R1, P1, T2, R2, P2, AG) : possible_world(T2, R2, P2, _) } 0.
% :- possible_world(T1, R1, P1, _), agent(AG), 0 { possible_world(T2, R2, P2, _) : believes(T1, R1, P1, T2, R2, P2, AG) } 0.

:- 0 { reached_goal(T) : time(T) } 0.
:- reached_goal(T1), plan(T2, ACT),    T2 >= T1.
:- reached_goal(T1), reached_goal(T2), T2 >  T1.

% #minimize{ T : reached_goal(T) }.

% Display

% !!! Uncomment while printing the results !!!
% #show pointed/3.
% #show possible_world/4.
% #show believes/7.

#show plan/2.
#show reached_goal/1.



% *** Working on optimization #3... ***

% When we apply ACT to P2, the possibility phi(P2) must entail CAUSED if it holds causes(T, ACT, CAUSED)
% apply_ontic_fluent(T2, R2, P2,  CAUSED, T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2),
%                                               is_executable_effect(T, ACT, T2, R2, P2,  CAUSED), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% When we apply ACT to P2, the possibility phi(P2) must entail F if it does not hold nor causes(T, ACT, F), nor causes(T, ACT, -F) and if P2 entails F
% apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), holds(W2, F),
% apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), holds(T2, R2, P2, W2, F),
%                                               not is_executable_effect(T, ACT, T2, R2, P2, F), not is_executable_effect(T, ACT, T2, R2, P2, -F), fluent(F), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% not apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), not holds(W2, F),
% not apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), is_executable(T, ACT), ontic(ACT), possible_world(T2, R2, P2, W2), not holds(T2, R2, P2, W2, F),
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

% ***              End              ***

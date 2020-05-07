%
% EMP encoding in ASP
%

% ########## READ ME ##########
%
% Possibili ottimizzazioni da realizzare:
%  1. Fare in modo che lo stato iniziale sia univoco rimuovendo le varie permutazioni.
%
%  2. Limitare i predicati entails(t, R, P, F) nelle sole possibility P in cui effettivamente è necessario controllare F.
%     E.g.: formule come executability conditions, goals, observability conditions sono da controllare solo sulla possibility puntata.
%
%  3. Riutilizzare gli indici dei fluent set già presenti a tempo t' <= t quando eseguo un'azione ontic. Le possibility a tempo t+1
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

% #include <incmode>.

% ********** REACHABILITY **********

#include <incmode>.

#program base.
time(0).
reaches(0, 0, P1, 0, 0, P2, AGS) :- believes(0, 0, P1, 0, 0, P2, AG), contains_ag(AGS, AG).
reaches(0, 0, P1, 0, 0, P2, AGS) :- believes(0, 0, P1, 0, 0, P3, AG), contains_ag(AGS, AG), reaches(0, 0, P3, 0, 0, P2, AGS).

#program step(t).

time(t).
% reaches/7: P1 reaches P2 with a path that contains only agents inside AGS
reaches(t, R1, P1, T2, R2, P2, AGS) :- t > 0, time(t), time(T2),           believes(t, R1, P1, T2, R2, P2, AG), T2 <= t, contains_ag(AGS, AG).
reaches(t, R1, P1, T2, R2, P2, AGS) :- t > 0, time(t), time(T2), time(T3), believes(t, R1, P1, T3, R3, P3, AG), T3 <= t, T2 <= T3, contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS).


% ********** ENTAILMENT **********

#program base.

literal( F) :- fluent(F).
literal(-F) :- fluent(F).

entails_init(W,  F    )          :-     holds(0, 0, W, F),                     initial_fluent_set(W), fluent(F), init_sf(F).
entails_init(W, -F    )          :- not holds(0, 0, W, F),                     initial_fluent_set(W), fluent(F), init_sf(F).

entails_init(W, neg(F))          :- not entails_init(W, F ),                      initial_fluent_set(W), init_sf(neg(F)),      init_sf(F).
entails_init(W, and(F1, F2))     :-     entails_init(W, F1), entails_init(W, F2), initial_fluent_set(W), init_sf(and(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2))     :-     entails_init(W, F1),                      initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2))     :-                          entails_init(W, F2), initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails(0, 0, W,  F    )         :-     holds(0, 0, W, F),                     initial_fluent_set(W), fluent(F).
entails(0, 0, W, -F    )         :- not holds(0, 0, W, F),                     initial_fluent_set(W), fluent(F).


entails(0, 0, W, neg(F))         :- not entails(0, 0, W, F ),                       initial_fluent_set(W), formula(neg(F)),      formula(F).
entails(0, 0, W, and(F1, F2))    :-     entails(0, 0, W, F1), entails(0, 0, W, F2), initial_fluent_set(W), formula(and(F1, F2)), formula(F1), formula(F2).
entails(0, 0, W,  or(F1, F2))    :-     entails(0, 0, W, F1),                       initial_fluent_set(W), formula( or(F1, F2)), formula(F1), formula(F2).
entails(0, 0, W,  or(F1, F2))    :-                           entails(0, 0, W, F2), initial_fluent_set(W), formula( or(F1, F2)), formula(F1), formula(F2).

not_entails(0, 0, P1, b(AG , F)) :- not     entails(0, 0, P2, F        ), believes(0, 0, P1, 0, 0, P2, AG), possible_world(0, 0, P2), formula(b(AG , F)), formula(F), agent(AG).
entails    (0, 0, P , b(AG , F)) :- not not_entails(0, 0, P , b(AG , F)),                                   possible_world(0, 0, P ), formula(b(AG , F)).

not_entails(0, 0, P1, c(AGS, F)) :- not     entails(0, 0, P2, F        ), reaches(0, 0, P1, 0, 0, P2, AGS), possible_world(0, 0, P2), formula(c(AGS, F)), formula(F).
entails    (0, 0, P , c(AGS, F)) :- not not_entails(0, 0, P , c(AGS, F)),                                   possible_world(0, 0, P ), formula(c(AGS, F)).

#program step(t).

% entails/4: P entails the formula F
entails(t, R, P,  F    )      :- t > 0,     holds(t, R, P, F),                       possible_world(t, R, P),%*fluent_set(W)*% time(t), fluent(F).
entails(t, R, P, -F    )      :- t > 0, not holds(t, R, P, F),                       possible_world(t, R, P),%*fluent_set(W)*% time(t), fluent(F).

entails(t, R, P, neg(F))      :- t > 0, not entails(t, R, P, F ),                       possible_world(t, R, P), formula(neg(F)),      formula(F), time(t).
entails(t, R, P, and(F1, F2)) :- t > 0,     entails(t, R, P, F1), entails(t, R, P, F2), possible_world(t, R, P), formula(and(F1, F2)), formula(F1), formula(F2), time(t).
entails(t, R, P,  or(F1, F2)) :- t > 0,     entails(t, R, P, F1),                       possible_world(t, R, P), formula( or(F1, F2)), formula(F1), formula(F2), time(t).
entails(t, R, P,  or(F1, F2)) :- t > 0,                           entails(t, R, P, F2), possible_world(t, R, P), formula( or(F1, F2)), formula(F1), formula(F2), time(t).

not_entails(t, R1, P1, b(AG , F)) :- t > 0, not     entails(T2, R2, P2, F        ), believes(t, R1, P1, T2, R2, P2, AG), possible_world(T2, R2, P2), formula(b(AG , F)), formula(F), agent(AG), time(t), time(T2).
entails    (t, R , P , b(AG , F)) :- t > 0, not not_entails(t , R , P , b(AG , F)),                                      possible_world(t , R , P ), formula(b(AG , F)), time(t).

not_entails(t, R1, P1, c(AGS, F)) :- t > 0, not     entails(T2, R2, P2, F        ), reaches(t, R1, P1, T2, R2, P2, AGS), possible_world(T2, R2, P2), formula(c(AGS, F)), formula(F), time(t), time(T2).
entails    (t, R , P , c(AGS, F)) :- t > 0, not not_entails(t , R , P , c(AGS, F)),                                      possible_world(t , R , P ), formula(c(AGS, F)), time(t).


#program base.

% ********** INITIAL STATE **********

% Since we assume to deal with FINITARY S5-states, we only allow the initial conditions initially(F), where F has one of the following forms:
%  1. Phi (propositional formula)
%  2. C_all( B_i(Phi))
%  3. C_all( B_i(Phi) \/  B_i(-Phi))
%  4. C_all(-B_i(Phi) /\ -B_i(-Phi)) (this case doesn't add knowledge)

unknown_initially(F)            :- not initially(c(all_agents, F)), not initially(c(all_agents, -F)), fluent(F).
initial_state_dimension(2 ** K) :- K = #count{ F : fluent(F), unknown_initially(F) }.
initial_fluent_set(1..K)        :- initial_state_dimension(K).

% The initial conditions of form 1. determine the pointed possibility of the initial state
% A possibility is pointed if it entails exactly those fluents that hold initially
pointed(0, 0, W) :- good_initial_fluent_set(W), 0 { fluent(F) : initially(F), not holds(0, 0, W, F) } 0, 0 { fluent(F) : initially(-F), holds(0, 0, W, F) } 0.

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
    holds(0, 0, W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F).
not holds(0, 0, W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F).

%   holds(W, F) :- initially(c(all_agents,  F)), fluent(F).
%not holds(W, F) :- initially(c(all_agents, -F)), fluent(F).

% We impose that all the initial possibilities have a different fluent set
K/2 {     holds(0, 0, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
K/2 { not holds(0, 0, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).

% K/2 {     holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
% K/2 { not holds(W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).

% All the initial fluent sets must be different
:- K1 = { holds(0, 0, W1, F) : not holds(0, 0, W2, F) }, K2 = { holds(0, 0, W2, F) : not holds(0, 0, W1, F), fluent(F) }, % :- K1 = { holds(W1, F) : not holds(W2, F) }, F) : not holds(W1, F), fluent(F) },
   initial_fluent_set(W1), initial_fluent_set(W2), W1 != W2, K1 + K2 = 0.

% *** Working on optimization #1... ***

% We impose a complete order on the fluent sets indices W_i w.r.0. their fluents
:- MF1 = #max { F : fluent(F), unknown_initially(F),     holds(0, 0, W1, F), not holds(0, 0, W2, F) },
   MF2 = #max { F : fluent(F), unknown_initially(F), not holds(0, 0, W1, F),     holds(0, 0, W2, F) },
   good_initial_fluent_set(W1), good_initial_fluent_set(W2), W1 < W2, MF1 > MF2.

% ***              End              ***

% So far we dealt with (commonly known) fluents. An initial fluent set is "good" if it entails all the fluent formulas F that are initial common knowledge
good_initial_fluent_set(W) :- initial_fluent_set(W), not bad_ini_w(W).%, 0 { initially(c(all_agents, F)) : not entails_init(W, F), initially(c(all_agents, F)), init_sf(F), not literal(F) } 0.
bad_ini_w(W):- not entails_init(W, F), initially(c(all_agents, F)), init_sf(F), not literal(F), initial_fluent_set(W).
% For each good initial fluent set we create a correspoinding possible world
possible_world(0, 0, W) :- good_initial_fluent_set(W).

%%*fluent_set(W)*% :- possible_world(0, 0, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
% not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F),     entails(0, 0, P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).
% not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F), not entails(0, 0, P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).
not_believes_initially(P1, P2, AG) :- not entails_init(P1, F),     entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).
not_believes_initially(P1, P2, AG) :-     entails_init(P1, F), not entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).

% The initial conditions of form 4. filter out some of the agents' beliefs inside the initial state
% not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F),     entails(0, 0, P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, neg(and(neg(b(AG, F)), neg(b(AG, -F)))))), formula(F).
% not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F), not entails(0, 0, P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, neg(and(neg(b(AG, F)), neg(b(AG, -F)))))), formula(F).

% TODO: TEST^^

% believes/7: the agent AG, inside P1, believes P2 to be possible
believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1), possible_world(0, 0, P2), agent(AG).


% ********** TRANSITION FUNCTION **********

#program step(t).

% is_executable/2: the action ACT is executable at time t
is_executable(t, ACT) :- t > 0, plan(t, ACT), action(ACT), pointed(t-1, R, P), executable(ACT, EXEC), entails(t-1, R, P, EXEC), formula(EXEC), time(t).
is_executable(t, ACT) :- t > 0, plan(t, ACT), action(ACT), pointed(t-1, R, P), executable(ACT, true), time(t).

% is_executable_effect/6: the effect CAUSED/SENSED/ANN of the action ACT is executable at time t
is_executable_effect(t, ACT, t-1, R , P , CAUSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), causes(ACT, CAUSED, true ), pointed(t-1, R, P), time(t).
is_executable_effect(t, ACT, t-1, R , P , CAUSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), causes(ACT, CAUSED, EEXEC), entails(t-1, R, P, EEXEC), pointed(t-1, R, P), formula(EEXEC), time(t).

is_executable_effect(t, ACT, t-1, R , P , SENSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), determines(ACT, SENSED, true ), pointed(t-1, R, P), time(t).
is_executable_effect(t, ACT, t-1, R , P , SENSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), determines(ACT, SENSED, EEXEC), entails(t-1, R, P, EEXEC), pointed(t-1, R, P), formula(EEXEC), time(t).

is_executable_effect(t, ACT, t-1, R , P , ANN   ) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), announces(ACT, ANN, true ), pointed(t-1, R, P), time(t).
is_executable_effect(t, ACT, t-1, R , P , ANN   ) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), announces(ACT, ANN, EEXEC), entails(t-1, R, P, EEXEC), pointed(t-1, R, P), formula(EEXEC), time(t).

% For ontic actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully observant agents.
% Then the effect CAUSED is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(t, ACT, T2, R2, P2, CAUSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), causes(ACT, CAUSED, true ), possible_world(T2, R2, P2), T2 < t,
                                                    pointed(t-1, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).

is_executable_effect(t, ACT, T2, R2, P2, CAUSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2), T2 < t, formula(EEXEC),
                                                    pointed(t-1, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).

% For sensing/announcement actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully or partially observant agents.
% Then the effect SENSED/ANN is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(t, ACT, T2, R2, P2, SENSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), determines(ACT, SENSED, true), possible_world(T2, R2, P2), T2 < t, fluent(SENSED),
                                                    pointed(t-1, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).

is_executable_effect(t, ACT, T2, R2, P2, SENSED) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), determines(ACT, SENSED, EEXEC), possible_world(T2, R2, P2), T2 < t, formula(EEXEC), fluent(SENSED),
                                                    pointed(t-1, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).

is_executable_effect(t, ACT, T2, R2, P2, ANN   ) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), announces(ACT, ANN, true), possible_world(T2, R2, P2), T2 < t, formula(ANN),
                                                    pointed(t-1, R, PP),                             0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).

is_executable_effect(t, ACT, T2, R2, P2, ANN   ) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), announces(ACT, ANN, EEXEC), possible_world(T2, R2, P2), T2 < t, formula(ANN),formula(EEXEC),
                                                    pointed(t-1, R, PP), entails(T2, R2, P2, EEXEC), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).

is_fully_obs  (t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), action(ACT),       observes (ACT, AG, true), agent(AG).
is_partial_obs(t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), action(ACT),       aware_of (ACT, AG, true), agent(AG).
is_oblivious  (t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), action(ACT),       oblivious(ACT, AG),       agent(AG).

is_fully_obs  (t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), action(ACT),       observes (ACT, AG, OBS ), agent(AG),     entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).

is_partial_obs(t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), sensing(ACT),      aware_of (ACT, AG, OBS ), agent(AG),     entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).
is_partial_obs(t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), announcement(ACT), aware_of (ACT, AG, OBS ), agent(AG),     entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).

is_oblivious  (t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), action(ACT),       observes (ACT, AG, OBS ), agent(AG), not entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).
is_oblivious  (t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), sensing(ACT),      aware_of (ACT, AG, OBS ), agent(AG), not entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).
is_oblivious  (t, AG) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% time(t), announcement(ACT), aware_of (ACT, AG, OBS ), agent(AG), not entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).

#program base.
% The maximum repetition at time t is the repetition of the pointed possibility
max_rep(0, 0).

#program step(t).
max_rep(t, R) :- t > 0, pointed(t, R, P), time(t), plan(t, ACT).


% ONTIC ACTIONS

% pointed/3: the possibility P is pointed at time t
pointed(t, R+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), pointed(t-1, R, P), possible_world(t, R+MR+1, phi(P)), max_rep(t-1, MR), time(t).

% possible_world/4: at time t the possibility P with repetition R is associated with the fluent set W
% A triple (t, R, P) univocally identifies a possibility. When we update such a triple when an action occurres, we obtain the triple (t+1, R+MR, phi(P)),
% where MR is the maximum repetition that can be found at time t and phi(P) is a new possibility

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time t
possible_world(t, R+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), pointed(t-1, R, P ), possible_world(t-1, R , P), max_rep(t-1, MR), time(t). %,
                                                % 0 {%*fluent_set(W1)*% : T1 <= t, possible_world(T1, _, _), apply_ontic(W1, t, R, P, t) } 0.

% If a fully observant agent AG believes P2, then we create the new possibility phi(P2)
% possible_world(t+1, R2+MR+1, phi(P))) :- is_executable(t, ACT), ontic(ACT), pointed(t, R, PP), possible_world(T2, R2, P), max_rep(t, MR),
%                                                 believes(t, R, PP, T2, R2, P, AG), is_fully_obs(t+1, AG). %,
                                                % 0 {%*fluent_set(W1)*%: T1 <= t, possible_world(T1, _, _), apply_ontic(W1, T2, R2, P, t) } 0.

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully observant agents.
% Then we create the new possibility phi(P). We need this rule since we assume that each fully observant agent knows which agent is fully observant as well.
% Hence we must update the fluent set of all the possibilities such as P.
% [To see a pratical example print the representation of the states using Coin in the Box with the actions: plan(0, signal_a_b). plan(1, signal_a_c). plan(2, open_a). The rule generates the possibilities "3_4_p1" and "3_4_p2" ]
possible_world(t, R2+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), pointed(t-1, R, PP), possible_world(T2, R2, P), max_rep(t-1, MR), T2 < t,
                                                reaches(t-1, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, time(t), time(T2). %,
                                                % 0 {%*fluent_set(W1)*%: T1 <= t, possible_world(T1, _, _), apply_ontic(W1, T2, R2, P, t) } 0.

% We keep track of the newly generated fluent sets
%%*fluent_set(W)*% :- t > 0, possible_world(t, R, P), time(t).

% holds/5: the fluent set W associated to P contains the fluent F

% A fluent F holds in the fluent set phi(W) associated to the possibility phi(P) if it is caused by the ontic action ACT and if the effect is executable w.r.t. P
    holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), max_rep(t-1, MR),
                                            is_executable_effect(t, ACT, T2, R2, P,  F), fluent(F),%*fluent_set(W)*%%*fluent_set(phi(W)*% T2 < t, time(t), time(T2).

% A fluent F must not hold in the fluent set phi(W) associated to the possibility phi(P) if -F is caused by the ontic action ACT and if the effect is executable w.r.t. P
not holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), max_rep(t-1, MR),
                                            is_executable_effect(t, ACT, T2, R2, P, -F), fluent(F),%*fluent_set(W)*%%*fluent_set(phi(W)*% T2 < t, time(t), time(T2).

% Inertia: if a fluent F is not modified by the ontic action ACT, then it holds in phi(W) if it was true in W...
    holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), max_rep(t-1, MR),
                                            not is_executable_effect(t, ACT, T2, R2, P, F), not is_executable_effect(t, ACT, T2, R2, P, -F),
                                            holds(T2, R2, P, F), fluent(F),%*fluent_set(W)*%%*fluent_set(phi(W)*% T2 < t, time(t), time(T2).

% ... and it must not hold in phi(W) if it was false in W.
not holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), max_rep(t-1, MR),
                                            not is_executable_effect(t, ACT, T2, R2, P, F), not is_executable_effect(t, ACT, T2, R2, P, -F),
                                            not holds(T2, R2, P, F), fluent(F),%*fluent_set(W)*%%*fluent_set(phi(W)*% T2 < t, time(t), time(T2).

% !!!
% PER PROF: ho lasciato not holds nelle teste delle regole qui sopra perché trasformandoli in vincoli diventa meno efficiente (test condotto su Coin_in_the_Box__pl_5.lp)
% !!!

% *** Working on optimization #3... ***
% possible_world(t+1, R +MR+1, phi(P)) :- is_executable(t, ACT), ontic(ACT), pointed(t, R, P ), possible_world(t, R , P), max_rep(t, MR),
%                                                 possible_world(T1, _, _), T1 <= t, apply_ontic(W1, t, R, P, t), W1 != W.

% possible_world(t+1, R2+MR+1, phi(P)) :- is_executable(t, ACT), ontic(ACT), pointed(t, R, PP), possible_world(T2, R2, P), max_rep(t, MR),
%                                                 believes(t, R, PP, T2, R2, P, AG), is_fully_obs(t+1, AG),
%                                                 possible_world(T1, _, _), T1 <= t, apply_ontic(W1, T2, R2, P, t).

% possible_world(t+1, R2+MR+1, phi(P)) :- is_executable(t, ACT), ontic(ACT), pointed(t, R, PP), possible_world(T2, R2, P), max_rep(t, MR),
%                                                 reaches(t, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(t+1, AG) } 0,
%                                                 possible_world(T1, _, _), T1 <= t, apply_ontic(W1, T2, R2, P, t).
%
% holds(phi(W), F) :- is_executable(t, ACT), possible_world(t+1, R2+MR+1, phi(P))), possible_world(T2, R2, P), max_rep(t, MR),
%                                        0 { time(T1): T1 <= t, possible_world(T1, _, _)), apply_ontic(phi(W), T2, R2, P, t) } 0.

% holds(phi(W), F) :- is_executable(t, ACT), possible_world(t+1, R2+MR+1, phi(P))), possible_world(T2, R2, P), max_rep(t, MR),
%                                        not is_executable_effect(t, ACT, T2, R2, P, F), holds(W, F), fluent(F),%*fluent_set(phi(W)*%),
%                                        0 { time(T1): T1 <= t, possible_world(T1, _, _)), apply_ontic(phi(W), T2, R2, P, t) } 0.
% ***              End              ***

% Fully observant agents: if P1 -- AG --> P2 at time t and AG is fully observant, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0, plan(t,ACT), %*is_executable(t, ACT),*% ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= t, T2 <= t, T2 <= T1,
                                                           is_fully_obs(t, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)), max_rep(t-1, MR), time(t), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time t and AG is oblivious, then phi(P1) -- AG --> P2 at time t+1
believes(t, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= t, T2 <= t, T2 <= T1,
                                                           is_oblivious(t, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), max_rep(t-1, MR),time(t), time(T1), time(T2).

% SENSING ACTIONS

% hold_sensed_effects/4: the possibility P2 agrees with the pointed possibility on the SENSED fluents of the sensing action ACT performed at time t
hold_sensed_effects(t-1, R , P , t) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), pointed(t-1, R, P), time(t).
hold_sensed_effects(T2 , R2, P2, t) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), possible_world(t-1, R, PP), possible_world(T2, R2, P2), T2 < t,
                                       0 { is_executable_effect(t, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _),     holds(T2, R2, P2, SENSED), not holds(t-1, R, PP, SENSED) } 0,
                                       0 { is_executable_effect(t, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _), not holds(T2, R2, P2, SENSED),     holds(t-1, R, PP, SENSED) } 0,
                                       pointed(t-1, R, PP), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).


pointed(t, R+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), pointed(t-1, R, P), possible_world(t, R+MR+1, phi(P)), max_rep(t-1, MR), time(t).

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time t
possible_world(t, R +MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), pointed(t-1, R, P), possible_world(t-1, R, P), max_rep(t-1, MR), time(t).

% good :- is_executable(2, peek_a), sensing(peek_a), pointed(1, 1, phi(2)).
% #show good/0.

% If a fully observant agent AG believes P2 and P2 agrees with the pointed possibility on the SENSED fluents, then we create the new possibility phi(P2)
% possible_world(t+1, R2+MR+1, phi(P)) :- is_executable(t, ACT), sensing(ACT), pointed(t, R, PP), possible_world(T2, R2, P), max_rep(t, MR),
%                                             believes(t, R, PP, T2, R2, P, AG), hold_sensed_effects(T2, R2, P, t), is_fully_obs(t+1, AG).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(t, R2+MR+1, phi(P)) :- t > 0, is_executable(t, ACT), sensing(ACT), pointed(t-1, R, PP), possible_world(T2, R2, P), max_rep(t-1, MR), T2 < t,
                                          reaches(t-1, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(t, AG) } 0,
                                          hold_sensed_effects(T2, R2, P, t), time(t), time(T2).

% If a partially observant agent AG believes P2, then we create the new possibility phi(P2)
possible_world(t, R2+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), pointed(t-1, R, PP), possible_world(T2, R2, P), max_rep(t-1, MR), T2 < t,
                                          believes(t-1, R, PP, T2, R2, P, AG), is_partial_obs(t, AG), time(t), time(T2).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully/partially observant agents.
% Then we create the new possibility phi(P).

% !!!
% PER PROF: la regola qui sotto può generare mondi superflui (ovvero non raggiungibili dal mondo puntato). Si provi a stampare il grafico dell'istanza
% Coin_in_the_Box__pl_6.lp con piano: plan(0, signal_a_b). plan(1, distract_b_a). plan(2, signal_b_c). plan(3, open_a). plan(4, peek_b). plan(5, peek_c).
% Le ultime due azioni generano due nuove possibility ridondanti: "5_31_ppppp2" e "6_60_pppp1".
% !!!
possible_world(t, R2+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), pointed(t-1, R, PP), possible_world(T1, R1, P1), possible_world(T2, R2, P), max_rep(t-1, MR), T1 < t, T2 <= T1,
                                          believes(t-1, R, PP, T1, R1, P1, AG), is_partial_obs(t, AG), reaches(T1, R1, P1, T2, R2, P, AGS), 0 { agent(AG1) : contains_ag(AGS, AG1), is_oblivious(t, AG1) } 0, time(t), time(T1), time(T2).

% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in phi(P) the fluents that hold in W
holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), %* plan(t, ACT), %*is_executable(t, ACT),*%*% sensing(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), T2 < t,
                                   holds(T2, R2, P, F), fluent(F),%*fluent_set(W)*% max_rep(t-1, MR), time(t), time(T2).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time t and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), T1 <= t, T2 <= t, T2 <= T1, is_fully_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)),
                                                           hold_sensed_effects(T1, R1, P1, t), hold_sensed_effects(T2, R2, P2, t), max_rep(t-1, MR), time(t), time(T1), time(T2).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time t and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), T1 <= t, T2 <= t, T2 <= T1, is_fully_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)),
                                                           not hold_sensed_effects(T1, R1, P1, t), not hold_sensed_effects(T2, R2, P2, t), max_rep(t-1, MR), time(t), time(T1), time(T2).

% Partially observant agents: if P1 -- AG --> P2 at time t and AG is partially observant, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), T1 <= t, T2 <= t, T2 <= T1, is_partial_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)), max_rep(t-1, MR), time(t), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time t and AG is oblivious, then phi(P1) -- AG --> P2 at time t+1
believes(t, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% sensing(ACT), T1 <= t, T2 <= t, T2 <= T1, is_oblivious(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), max_rep(t-1, MR), time(t), time(T1), time(T2).

% ANNOUNCEMENT ACTIONS

% hold_announced_effects/4: the possibility P2 agrees with the pointed possibility on the ANN formulas of the announcement action ACT performed at time t
hold_announced_effects(t-1, R , P , t) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), pointed(t-1, R, P), time(t).
hold_announced_effects(T2 , R2, P2, t) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), possible_world(t-1, R, PP), possible_world(T2, R2, P2), T2 < t,
                                          0 { is_executable_effect(t, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _),     entails(T2, R2, P2, ANN), not entails(t-1, R, PP, ANN) } 0,
                                          0 { is_executable_effect(t, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _), not entails(T2, R2, P2, ANN),     entails(t-1, R, PP, ANN) } 0,
                                          pointed(t-1, R, PP), 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0, reaches(t-1, R, PP, T2, R2, P2, AGS), time(t), time(T2).


pointed(t, R+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), pointed(t-1, R, P), possible_world(t, R+MR+1, phi(P)), max_rep(t-1, MR), time(t).


% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time t
possible_world(t, R+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), pointed(t-1, R, P), possible_world(t-1, R, P), max_rep(t-1, MR), time(t).

% If a fully observant agent AG believes P2 and P2 agrees with the pointed possibility on the ANN formulas, then we create the new possibility phi(P2)
% possible_world(t+1, R2+MR+1, phi(P)) :- is_executable(t, ACT), announcement(ACT), pointed(t, R, PP), possible_world(T2, R2, P), max_rep(t, MR),
%                                             believes(t, R, PP, T2, R2, P, AG), hold_announced_effects(T2, R2, P, t), is_fully_obs(t+1, AG).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(t, R2+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), pointed(t-1, R, PP), possible_world(T2, R2, P), max_rep(t-1, MR), T2 < t,
                                          reaches(t-1, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(t, AG) } 0,
                                          hold_announced_effects(T2, R2, P, t), time(t), time(T2).

% If a partially observant agent AG believes P2, then we create the new possibility phi(P2)
possible_world(t, R2+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), pointed(t-1, R, PP), possible_world(T2, R2, P), max_rep(t-1, MR), T2 < t,
                                          believes(t-1, R, PP, T2, R2, P, AG), is_partial_obs(t, AG), time(t), time(T2).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully/partially observant agents.
% Then we create the new possibility phi(P).
possible_world(t, R2+MR+1, phi(P)) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), pointed(t-1, R, PP), possible_world(T1, R1, P1), possible_world(T2, R2, P), max_rep(t-1, MR), T1 < t, T2 <= T1,
                                          believes(t-1, R, PP, T1, R1, P1, AG), is_partial_obs(t, AG), reaches(T1, R1, P1, T2, R2, P, AGS), 0 { agent(AG1) : contains_ag(AGS, AG1), is_oblivious(t, AG1) } 0, time(t), time(T1), time(T2).

% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in phi(P) the fluents that hold in W
holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), T2 < t,
                                   holds(T2, R2, P, F), fluent(F),%*fluent_set(W)*% max_rep(t-1, MR), time(t), time(T2).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time t and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), T1 <= t, T2 <= t, T2 <= T1, is_fully_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)),
                                                           hold_announced_effects(T1, R1, P1, t), hold_announced_effects(T2, R2, P2, t), max_rep(t-1, MR), time(t), time(T1), time(T2).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time t and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), T1 <= t, T2 <= t, T2 <= T1, is_fully_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)),
                                                           not hold_announced_effects(T1, R1, P1, t), not hold_announced_effects(T2, R2, P2, t), max_rep(t-1, MR), time(t), time(T1), time(T2).

% Partially observant agents: if P1 -- AG --> P2 at time t and AG is partially observant, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), T1 <= t, T2 <= t, T2 <= T1, is_partial_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)), max_rep(t-1, MR), time(t), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time t and AG is oblivious, then phi(P1) -- AG --> P2 at time t+1
believes(t, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- t > 0,  plan(t, ACT), %*is_executable(t, ACT),*% announcement(ACT), T1 <= t, T2 <= t, T2 <= T1, is_oblivious(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), max_rep(t-1, MR), time(t), time(T1), time(T2).


% Generate
%
#program step(t).
1 { plan(t, ACT) : action(ACT) } 1 :- t > 0, time(t).
%0 { is_executable(t, ACT) : action(ACT) } 1.



% plan(1,signal_a_c).

% plan(1,open_a).
% plan(2,peek_a).
% plan(3,signal_a_b).
% plan(4,signal_a_c).
% plan(5,shout_tail_a).


% plan(1,open_a).
% plan(2,signal_a_b).
% plan(3,signal_a_c).
% plan(4,peek_a).

%plan(1,signal_a_b).
%plan(2,open_a).
%plan(3,peek_b).

%is_executable(1,signal_a_b).
%is_executable(2,open_a).


%plan(T,action1) :- time(T), not plan(T, action2), not plan(T, action3),....

%plan(t,null) :- time(t), t > 0, not plan(t, open_a), not plan(t, peek_b), not plan(t,signal_a_b).
%
%
%plan(t,signal_a_b) :- time(t), t > 0, not plan(t, open_a), not plan(t, peek_b), not plan(t,null).
%                      %not plan(t,distract_a_b), not plan(t,distract_a_c),
%                      %not plan(t,distract_b_a), not plan(t,distract_b_c), not plan(t,distract_c_a),
%                      %not plan(t,distract_c_b), not plan(t,open_a), not plan(t,open_b), not plan(t,open_c),
%                      %not plan(t,peek_a), not plan(t,peek_b), not plan(t,peek_c), not plan(t,shout_tail_a),
%                      %not plan(t,shout_tail_b), not plan(t,shout_tail_c), not plan(t,signal_a_c),
%                      %not plan(t,signal_b_a), not plan(t,signal_c_a), not plan(t,signal_c_b), not plan(t,null).
%
%plan(t,open_a) :- time(t), t > 0, not plan(t, signal_a_b), not plan(t, peek_b), not plan(t,null).
%                      %not plan(t,distract_a_b), not plan(t,distract_a_c),
%                      %not plan(t,distract_b_a), not plan(t,distract_b_c), not plan(t,distract_c_a),
%                      %not plan(t,distract_c_b), not plan(t,signal_a_b), not plan(t,open_b), not plan(t,open_c),
%                      %not plan(t,peek_a), not plan(t,peek_b), not plan(t,peek_c), not plan(t,shout_tail_a),
%                      %not plan(t,shout_tail_b), not plan(t,shout_tail_c), not plan(t,signal_a_c),
%                      %not plan(t,signal_b_a), not plan(t,signal_c_a), not plan(t,signal_c_b), not plan(t,null).
%
%plan(t,peek_b) :- time(t), t > 0, not plan(t, signal_a_b), not plan(t, open_a), not plan(t,null).
                      %not plan(t,distract_a_b), not plan(t,distract_a_c),
                      %not plan(t,distract_b_a), not plan(t,distract_b_c), not plan(t,distract_c_a),
                      %not plan(t,distract_c_b), not plan(t,signal_a_b), not plan(t,open_b), not plan(t,open_c),
                      %not plan(t,peek_a), not plan(t,open_a), not plan(t,peek_c), not plan(t,shout_tail_a),
                      %not plan(t,shout_tail_b), not plan(t,shout_tail_c), not plan(t,signal_a_c),
                      %not plan(t,signal_b_a), not plan(t,signal_c_a), not plan(t,signal_c_b), not plan(t,null).

%plan(1,signal_a_b).
%plan(2,open_a).
%plan(3,peek_b).

%plan(1,open_a).
%plan(2,signal_a_b).
%plan(3,signal_a_c).
%plan(4,open_a).
%plan(5,peek_a).
%plan(6,peek_b).

% Define

#program base.

%plan(0, null).

not_reached_goal(0) :- not entails(0, 0, P, F), pointed(0, 0, P), goal(F).
reached_goal(0)     :- not not_reached_goal(0).


#program step(t).
:- plan(t,null), t > 0.
%:- 0{ plan(t-1, _) } 0.

not_reached_goal(t) :- t > 0, not entails(t, R, P, F), pointed(t, R, P), goal(F), time(t), formula(F).
reached_goal(t)     :- t > 0, plan(t, ACT),  plan(t, ACT), %*is_executable(t, ACT),*% not not_reached_goal(t), time(t).
% Test
:- t > 0, plan(t, ACT), not is_executable(t, ACT), time(t).
%:- t > 0, plan(t, ACT), pointed(t-1, R, P), not entails(t-1, R, P, EXEC), action(ACT), executable(ACT, EXEC), formula(EXEC), time(t).
%%
%:- t > 0, plan(t, ACT), pointed(t-1, R, P), 0 { is_executable_effect(t, ACT, t-1, R, P, CAUSED) : causes(ACT, CAUSED, _) } 0, ontic(ACT), time(t).
%
%:- t > 0, reached_goal(t), plan(T2, ACT),    T2 > t.
%:- pointed(t, R, P), pointed(T1, RR, PP), T1 < t, t > 0, time(t), time(T1),
%  0 { formula(F): entails(t, R, P, F), not entails(T1, RR, PP, F)} 0,
%  0 { formula(F): not entails(t, R, P, F), entails(T1, RR, PP, F)} 0.


#program check(t).

:- not reached_goal(t), query(t).


% Display

% !!! Uncomment while printing the results !!!
%#show pointed/3.
%#show possible_world/4.
%#show believes/7.
%#show holds/5.
%#show fluent/1.

%#show is_executable/2.
%#show entails/4.

%#show is_fully_obs/2.
#show plan/2.
#show reached_goal/1.
%#show not_reached_goal/1.

% #show not_reached_goal/1.





% ********** BISIMULATION ********** (Dovier, ICLP 2015)

% Reflexivity and Symmetry
% bis(T1, R1, P1, T1, R1, P1) :- possible_world(T1, R1, P1).
% bis(T1, R1, P1, T2, R2, P2) :- possible_world(T1, R1, P1), possible_world(T2, R2, P2), bis(T2, R2, P2, T1, R1, P1).

% Nondeterministic choice
% { bis(T1, R1, P1, T2, R2, P2) } :- possible_world(T1, R1, P1), possible_world(T2, R2, P2).

% :- possible_world(T1, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3),
%    bis(T1, R1, P1, T2, R2, P2), believes(T1, R1, P1, T3, R3, P3, AG), 0 { possible_world(T4, R4, P4) : bis(T3, R3, P3, T4, R4, P4), believes(T2, R2, P2, T4, R4, P4, AG) } 0.

% :- possible_world(T1, R1, P1), possible_world(T2, R2, P2),
%    bis(T1, R1, P1, T2, R2, P2), holds(T1, R1, P1, F), not holds(T2, R2, P2, F), fluent(F).

% % Morphism requirement (1’)
% one_son_bis(T1, R1, P1, T2, R2, P2, AG) :- possible_world(T1, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3),
%                                            believes(T2, R2, P2, T3, R3, P3, AG), agent(AG), bis(T1, R1, P1, T3, R3, P3).

% :- possible_world(T1, R1, P1), possible_world(T2, R2, P2), possible_world(T4, R4, P4), agent(AG),
%    bis(T4, R4, P4, T2, R2, P2), believes(T4, R4, P4, T1, R1, P1, AG), not one_son_bis(T1, R1, P1, T2, R2, P2, AG).


% Visited states
% :- pointed(T1, R1, P1), pointed(T2, R2, P2), T1 < T2, bis(T1, R1, P1, T2, R2, P2).

% #show bis/6.
% #show holds_exactly/8.

% one_son_bis(V1,U2) :- node(V1;U2;V2), edge(U2,V2), bis(V1,V2).
% :- node(U1;U2;V1), bis(U1,U2), edge(U1,V1), not one_son_bis(V1,U2).







% ag_list(F, dummy)                 :- literal(F).

% ag_list(neg(F),      l(dummy, L)) :- ag_list(F , L), cp_formula(neg(F)),      cp_formula(F).
% ag_list(and(F1, F2), l(dummy, L)) :- ag_list(F1, L), cp_formula(and(F1, F2)), cp_formula(F1), cp_formula(F2).
% ag_list(and(F1, F2), l(dummy, L)) :- ag_list(F2, L), cp_formula(and(F1, F2)), cp_formula(F1), cp_formula(F2).
% ag_list( or(F1, F2), l(dummy, L)) :- ag_list(F1, L), cp_formula( or(F1, F2)), cp_formula(F1), cp_formula(F2).
% ag_list( or(F1, F2), l(dummy, L)) :- ag_list(F2, L), cp_formula( or(F1, F2)), cp_formula(F1), cp_formula(F2).

% ag_list(b(AG , F),   l(AG,    L)) :- ag_list(F , L), cp_formula(b(AG , F)),   cp_formula(F), agent(AG).
% ag_list(c(AGS, F),   l(AG,    L)) :- ag_list(F , L), cp_formula(c(AGS, F)),   cp_formula(F), agent(AG), contains_ag(AGS, AG).

% reaches_ag_list(T1, R1, P1, T1, R1, P1,   dummy    ) :- possible_world(T1, R1, P1).
% reaches_ag_list(T1, R1, P1, T2, R2, P2, l(dummy, L)) :- reaches_ag_list(T1, R1, P1, T2, R2, P2, L), possible_world(T1, R1, P1), possible_world(T2, R2, P2).
% reaches_ag_list(T1, R1, P1, T2, R2, P2, l(AG   , L)) :- believes(T1, R1, P1, T3, R3, P3, AG), reaches_ag_list(T3, R3, P3, T2, R2, P2, L), possible_world(T1, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3), agent(AG).

% entails_cp(t, T1, R1, P1, MF,  F) :-     holds(T1, R1, P1, F), ag_list(MF, L), reaches_ag_list(t, R, P, T1, R1, P1, L), pointed(t, R, P), possible_world(T1, R1, P1),%*fluent_set(W1)*%, cp_formula(MF), fluent(F).
% entails_cp(t, T1, R1, P1, MF, -F) :- not holds(T1, R1, P1, F), ag_list(MF, L), reaches_ag_list(t, R, P, T1, R1, P1, L), pointed(t, R, P), possible_world(T1, R1, P1),%*fluent_set(W1)*%, cp_formula(MF), fluent(F).

% entails_cp(t, T1, R1, P1, neg(F)) :-

% reaches_formula(T1, R1, P1, T1, R1, P1, F)           :- possible_world(T1, R1, P1), literal(F).

% reaches_formula(T1, R1, P1, T2, R2, P2, neg(F))      :- reaches_formula(T1, R1, P1, T2, R2, P2, F ), possible_world(T1, R1, P1), possible_world(T2, R2, P2), cp_formula(neg(F)), cp_formula(F).

% reaches_formula(T1, R1, P1, T2, R2, P2, and(F1, F2)) :- reaches_formula(T1, R1, P1, T2, R2, P2, F2), possible_world(T1, R1, P1), possible_world(T2, R2, P2), cp_formula(and(F1, F2)), cp_formula(F1), cp_formula(F2).
% reaches_formula(T1, R1, P1, T2, R2, P2, and(F1, F2)) :- reaches_formula(T1, R1, P1, T2, R2, P2, F1), possible_world(T1, R1, P1), possible_world(T2, R2, P2), cp_formula(and(F1, F2)), cp_formula(F1), cp_formula(F2).

% reaches_formula(T1, R1, P1, T2, R2, P2,  or(F1, F2)) :- reaches_formula(T1, R1, P1, T2, R2, P2, F1), possible_world(T1, R1, P1), possible_world(T2, R2, P2), cp_formula( or(F1, F2)), cp_formula(F1), cp_formula(F2).
% reaches_formula(T1, R1, P1, T2, R2, P2,  or(F1, F2)) :- reaches_formula(T1, R1, P1, T2, R2, P2, F2), possible_world(T1, R1, P1), possible_world(T2, R2, P2), cp_formula( or(F1, F2)), cp_formula(F1), cp_formula(F2).

% reaches_formula(T1, R1, P1, T2, R2, P2, b(AG , F))   :- believes(T1, R1, P1, T3, R3, P3, AG), reaches_formula(T3, R3, P3, T2, R2, P2, F), possible_world(T1, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3), cp_formula(b(AG , F)), cp_formula(F), agent(AG).
% reaches_formula(T1, R1, P1, T2, R2, P2, c(AGS, F))   :- believes(T1, R1, P1, T3, R3, P3, AG), reaches_formula(T3, R3, P3, T2, R2, P2, F), possible_world(T1, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3), cp_formula(c(AGS, F)), cp_formula(F), agent(AG), contains_ag(AGS, AG).

% #show entails/4.
% #show reaches_ag_list/7.

% entails(t, F) :- pointed(t, R, P), ...







% *** Working on optimization #3... ***

% When we apply ACT to P2, the possibility phi(P2) must entail CAUSED if it holds causes(t, ACT, CAUSED)
% apply_ontic_fluent(T2, R2, P2,  CAUSED, t) :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(T2, R2, P2),
%                                               is_executable_effect(t, ACT, T2, R2, P2,  CAUSED), T2 <= t,
%                                               pointed(t, R, PP), is_fully_obs(t+1, AG), believes(t, R, PP, T2, R2, P2, AG) : not pointed(t, R, P2).

% When we apply ACT to P2, the possibility phi(P2) must entail F if it does not hold nor causes(t, ACT, F), nor causes(t, ACT, -F) and if P2 entails F
% apply_ontic_fluent(T2, R2, P2,  F     , t) :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(T2, R2, P2), holds(W2, F),
% apply_ontic_fluent(T2, R2, P2,  F     , t) :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(T2, R2, P2), holds(T2, R2, P2, F),
%                                               not is_executable_effect(t, ACT, T2, R2, P2, F), not is_executable_effect(t, ACT, T2, R2, P2, -F), fluent(F), T2 <= t,
%                                               pointed(t, R, PP), is_fully_obs(t+1, AG), believes(t, R, PP, T2, R2, P2, AG) : not pointed(t, R, P2).

% not apply_ontic_fluent(T2, R2, P2,  F     , t) :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(T2, R2, P2), not holds(W2, F),
% not apply_ontic_fluent(T2, R2, P2,  F     , t) :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(T2, R2, P2), not holds(T2, R2, P2, F),
%                                               not is_executable_effect(t, ACT, T2, R2, P2, F), not is_executable_effect(t, ACT, T2, R2, P2, -F), fluent(F), T2 <= t,
%                                               pointed(t, R, PP), is_fully_obs(t+1, AG), believes(t, R, PP, T2, R2, P2, AG) : not pointed(t, R, P2).

% The fluent_set W1 is the one resulting from W2 when we apply ACT. W1 must entail all the fluents that are caused by ACT and must agree with W2 w.r.t. the other fluents
% apply_ontic(W1, T2, R2, P2, t) :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= t, T2 <= t,
%                                   0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2,  F, t), not holds(W1, F) } 0, %not holds(T1, P1, W1, F) } 0,
%                                   0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2, -F, t),     holds(W1, F) } 0, %not holds(T1, P1, W1, F) } 0,
%                                   0 { fluent(F) : not apply_ontic_fluent(T2, R2, P2,  F, t),     holds(W1, F), not holds(W2, %holds(T1, R1, P1, W1, F), not holds(T2, R2, P2, F) } 0,
%                                   pointed(t, R, PP), is_fully_obs(t+1, AG), believes(t, R, PP, T2, R2, P2, AG) : not pointed(t, R, P2).




% 0 { apply_ontic(W1, T2, R2, P2, t) : possible_world(T1, R1, P1), T1 <= t } 1 :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT), possible_world(T2, R2, P2),
%                                                                                     pointed(t, R, PP), is_fully_obs(t+1, AG), believes(t, R, PP, T2, R2, P2, AG) : not pointed(t, R, P2).

% #show apply_ontic_fluent/5.
% #show apply_ontic/5.

% :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= t, T2 <= t,
%    apply_ontic(W1, T2, R2, P2, t),     apply_ontic_fluent(T2, R2, P2,  F, t), fluent(F), not holds(W1, F).

% :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= t, T2 <= t,
%    apply_ontic(W1, T2, R2, P2, t),     apply_ontic_fluent(T2, R2, P2, -F, t), fluent(F),     holds(W1, F).

% :- plan(t, ACT), %*is_executable(t, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= t, T2 <= t,
%    apply_ontic(W1, T2, R2, P2, t), not apply_ontic_fluent(T2, R2, P2,  F, t), fluent(F),     holds(W1, F), not holds(W2, F).

% ***              End              ***

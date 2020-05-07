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

% #include <incmode>.

% ********** REACHABILITY **********

%#include <incmode>.

%#program base.
time(0..maxT).
reaches(0, 0, P1, 0, 0, P2, AGS) :- believes(0, 0, P1, 0, 0, P2, AG), contains_ag(AGS, AG),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), agent(AG), agent_set(AGS).

reaches(0, 0, P1, 0, 0, P2, AGS) :- believes(0, 0, P1, 0, 0, P3, AG), contains_ag(AGS, AG), reaches(0, 0, P3, 0, 0, P2, AGS),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), possible_world(0, 0, P3), agent(AG), agent_set(AGS).

%#program step(T).

% reaches/7: P1 reaches P2 with a path that contains only agents inside AGS
reaches(T, R1, P1, T2, R2, P2, AGS) :- believes(T, R1, P1, T2, R2, P2, AG), time(T), time(T2), T2 <= T,
                                       contains_ag(AGS, AG), agent(AG), agent_set(AGS), possible_world(T, R1, P1), possible_world(T2, R2, P2).

reaches(T, R1, P1, T2, R2, P2, AGS) :- believes(T, R1, P1, T3, R3, P3, AG), time(T), time(T2), time(T3), T3 <= T, T2 <= T3,
                                       contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS), agent(AG), agent_set(AGS),
                                       possible_world(T, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3).


% ********** ENTAILMENT **********

%#program base.

% Sensing or Announcement
soa(ACT) :- sensing(ACT).
soa(ACT) :- announcement(ACT).

literal( F) :- fluent(F).
literal(-F) :- fluent(F).

% Entailment of initially conditions. We use it to generate the initial state
entails_init(P,  F    )          :-     holds(0, 0, P, F),                          good_initial_fluent_set(P), fluent(F), init_sf(F).
entails_init(P, -F    )          :- not holds(0, 0, P, F),                          good_initial_fluent_set(P), fluent(F), init_sf(F).

entails_init(P, neg(F))          :- not entails_init(P, F ),                        good_initial_fluent_set(P), init_sf(neg(F)),      init_sf(F).
entails_init(P, and(F1, F2))     :-     entails_init(P, F1), entails_init(P, F2),   good_initial_fluent_set(P), init_sf(and(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(P,  or(F1, F2))     :-     entails_init(P, F1),                        good_initial_fluent_set(P), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(P,  or(F1, F2))     :-                          entails_init(P, F2),   good_initial_fluent_set(P), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).

% Entailment of all the other formulas w.r.T. the calculated initial state
entails(0, 0, P,  F    )         :-     holds(0, 0, P, F),                          good_initial_fluent_set(P), possible_world(0, 0, P), fluent(F).
entails(0, 0, P, -F    )         :- not holds(0, 0, P, F),                          good_initial_fluent_set(P), possible_world(0, 0, P), fluent(F).

entails(0, 0, P, neg(F))         :- not entails(0, 0, P, F ),                       good_initial_fluent_set(P), possible_world(0, 0, P), formula(neg(F)),      formula(F).
entails(0, 0, P, and(F1, F2))    :-     entails(0, 0, P, F1), entails(0, 0, P, F2), good_initial_fluent_set(P), possible_world(0, 0, P), formula(and(F1, F2)), formula(F1), formula(F2).
entails(0, 0, P,  or(F1, F2))    :-     entails(0, 0, P, F1),                       good_initial_fluent_set(P), possible_world(0, 0, P), formula( or(F1, F2)), formula(F1), formula(F2).
entails(0, 0, P,  or(F1, F2))    :-                           entails(0, 0, P, F2), good_initial_fluent_set(P), possible_world(0, 0, P), formula( or(F1, F2)), formula(F1), formula(F2).

entails(0, 0, P , b(AG , F))     :- not not_entails(0, 0, P , b(AG , F)),           good_initial_fluent_set(P), possible_world(0, 0, P), formula(b(AG , F)), formula(F), agent(AG).
entails(0, 0, P , c(AGS, F))     :- not not_entails(0, 0, P , c(AGS, F)),           good_initial_fluent_set(P), possible_world(0, 0, P), formula(c(AGS, F)), formula(F), agent_set(AGS).

not_entails(0, 0, P1, b(AG , F)) :- not entails(0, 0, P2, F ), believes(0, 0, P1, 0, 0, P2, AG), good_initial_fluent_set(P1), good_initial_fluent_set(P2),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), formula(b(AG , F)), formula(F), agent(AG).

not_entails(0, 0, P1, c(AGS, F)) :- not entails(0, 0, P2, F), reaches(0, 0, P1, 0, 0, P2, AGS), good_initial_fluent_set(P1), good_initial_fluent_set(P2),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), formula(c(AGS, F)), formula(F), agent_set(AGS).


%#program step(T).

% entails/4: P entails the formula F
entails(T, R, P,  F    )      :- T > 0, time(T),     holds(T, R, P, F),                          possible_world(T, R, P), fluent(F).
entails(T, R, P, -F    )      :- T > 0, time(T), not holds(T, R, P, F),                          possible_world(T, R, P), fluent(F).

entails(T, R, P, neg(F))      :- T > 0, time(T), not entails(T, R, P, F ),                       possible_world(T, R, P), formula(neg(F)),      formula(F).
entails(T, R, P, and(F1, F2)) :- T > 0, time(T),     entails(T, R, P, F1), entails(T, R, P, F2), possible_world(T, R, P), formula(and(F1, F2)), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :- T > 0, time(T),     entails(T, R, P, F1),                       possible_world(T, R, P), formula( or(F1, F2)), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :- T > 0, time(T),                           entails(T, R, P, F2), possible_world(T, R, P), formula( or(F1, F2)), formula(F1), formula(F2).

entails(T, R , P , b(AG , F)) :- T > 0, time(T), not not_entails(T , R , P , b(AG , F)),         possible_world(T , R , P ), formula(b(AG , F)), formula(F), agent(AG).
entails(T, R , P , c(AGS, F)) :- T > 0, time(T), not not_entails(T , R , P , c(AGS, F)),         possible_world(T , R , P ), formula(c(AGS, F)), formula(F), agent_set(AGS).

% Aggiungendo possible_world(T, R1, P1) rallenta
not_entails(T, R1, P1, b(AG , F)) :- T > 0, time(T), time(T2), T2 <= T, not entails(T2, R2, P2, F), believes(T, R1, P1, T2, R2, P2, AG),
                                     possible_world(T2, R2, P2), formula(b(AG , F)), formula(F), agent(AG).

not_entails(T, R1, P1, c(AGS, F)) :- T > 0, time(T), time(T2), T2 <= T, not entails(T2, R2, P2, F), reaches(T, R1, P1, T2, R2, P2, AGS),
                                     possible_world(T2, R2, P2), formula(c(AGS, F)), formula(F), agent_set(AGS).

%#program base.

% ********** INITIAL STATE **********

% Since we assume to deal with FINITARY S5-states, we only allow the initial conditions initially(F), where F has one of the following forms:
%  1. Phi (propositional formula)
%  2. C_all( B_i(Phi))
%  3. C_all( B_i(Phi) \/  B_i(-Phi))
%  4. C_all(-B_i(Phi) /\ -B_i(-Phi)) (this case doesn'T add knowledge)

known_initially(F) :- fluent(F), initially(c(all_agents,  F)).
known_initially(F) :- fluent(F), initially(c(all_agents, -F)).

unknown_initially(F)            :- fluent(F), not known_initially(F).
initial_state_dimension(2 ** K) :- K = #count { F : fluent(F), unknown_initially(F) }.
initial_fluent_set(1..K)        :- initial_state_dimension(K).

% The initial conditions of form 1. determine the pointed possibility of the initial state
% A possibility is pointed if it entails exactly those fluents that hold initially
not_initial_pointed(P) :- good_initial_fluent_set(P), initially( F), not holds(0, 0, P, F), fluent(F).
not_initial_pointed(P) :- good_initial_fluent_set(P), initially(-F),     holds(0, 0, P, F), fluent(F).

pointed(0, 0, P) :- good_initial_fluent_set(P), not not_initial_pointed(P).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
    holds(0, 0, P, F) :- initially(c(all_agents,  F)), initial_fluent_set(P), fluent(F).
not holds(0, 0, P, F) :- initially(c(all_agents, -F)), initial_fluent_set(P), fluent(F).

% We impose that all the initial possibilities have a different fluent set
K/2 {     holds(0, 0, P, F) : initial_fluent_set(P) } K/2 :- unknown_initially(F), initial_state_dimension(K).
% K/2 { not holds(0, 0, P, F) : initial_fluent_set(P) } K/2 :- unknown_initially(F), initial_state_dimension(K).

% K/2 {     holds(P, F) : initial_fluent_set(P) } K/2 :- unknown_initially(F), initial_state_dimension(K).
% K/2 { not holds(P, F) : initial_fluent_set(P) } K/2 :- unknown_initially(F), initial_state_dimension(K).

% All the initial fluent sets must be different
diffzero(P1, P2) :- good_initial_fluent_set(P1), good_initial_fluent_set(P2), holds(0, 0, P1, F), not holds(0, 0, P2, F),
                    fluent(F), P1 < P2, possible_world(0, 0, P1), possible_world(0, 0, P2).

diffzero(P1, P2) :- good_initial_fluent_set(P1), good_initial_fluent_set(P2), holds(0, 0, P2, F), not holds(0, 0, P1, F),
                    fluent(F), P1 < P2, possible_world(0, 0, P1), possible_world(0, 0, P2).

:- not diffzero(P1, P2), initial_fluent_set(P1), initial_fluent_set(P2), P1 < P2.

% *** Working on optimization #1... ***

% We impose a complete order on the fluent sets indices W_i w.r.T. their fluents
:- MF1 = #max { F : fluent(F), unknown_initially(F),     holds(0, 0, P1, F), not holds(0, 0, P2, F) },
   MF2 = #max { F : fluent(F), unknown_initially(F), not holds(0, 0, P1, F),     holds(0, 0, P2, F) },
   good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 < P2, MF1 > MF2.

% !!!
% PER PROF: togliendo il vincolo qui sopra rallenta
% !!!

% ***              End              ***

% So far we dealt with (commonly known) fluents. An initial fluent set is "good" if it entails all the fluent formulas F that are initial common knowledge
no_good_initial_fluent_set(P):- not entails_init(P, F), initially(c(all_agents, F)), init_sf(F), not literal(F), initial_fluent_set(P).
good_initial_fluent_set(P)   :- initial_fluent_set(P), not no_good_initial_fluent_set(P).

% For each good initial fluent set we create a correspoinding possible world
possible_world(0, 0, P) :- good_initial_fluent_set(P).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails_init(P1, F),     entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).
not_believes_initially(P1, P2, AG) :-     entails_init(P1, F), not entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F).

% The initial conditions of form 4. filter out some of the agents' beliefs inside the initial state
% not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F),     entails(0, 0, P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, neg(and(neg(b(AG, F)), neg(b(AG, -F)))))), formula(F).
% not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F), not entails(0, 0, P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, neg(and(neg(b(AG, F)), neg(b(AG, -F)))))), formula(F).

% TODO: TEST^^

% believes/7: the agent AG, inside P1, believes P2 to be possible
believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1), possible_world(0, 0, P2),
                                    good_initial_fluent_set(P1), good_initial_fluent_set(P2), agent(AG).


% ********** TRANSITION FUNCTION **********

%#program step(T).

% is_executable/2: the action ACT is executable at time T
is_executable(T, ACT) :- T > 0, time(T), plan(T, ACT), action(ACT), pointed(T-1, R, P), executable(ACT, EXEC), entails(T-1, R, P, EXEC), formula(EXEC).
is_executable(T, ACT) :- T > 0, time(T), plan(T, ACT), action(ACT), pointed(T-1, R, P), executable(ACT, true).

% is_executable_effect/6: the effect CAUSED/SENSED/ANN of the action ACT is executable at time T
is_executable_effect(T, ACT, T-1, R , P , CAUSED) :- T > 0, time(T), plan(T, ACT), ontic(ACT), causes(ACT, CAUSED, true ), pointed(T-1, R, P).
is_executable_effect(T, ACT, T-1, R , P , CAUSED) :- T > 0, time(T), plan(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), entails(T-1, R, P, EEXEC), pointed(T-1, R, P), formula(EEXEC).

is_executable_effect(T, ACT, T-1, R , P , SENSED) :- T > 0, time(T), plan(T, ACT), sensing(ACT), determines(ACT, SENSED, true ), pointed(T-1, R, P).
is_executable_effect(T, ACT, T-1, R , P , SENSED) :- T > 0, time(T), plan(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), entails(T-1, R, P, EEXEC), pointed(T-1, R, P), formula(EEXEC).

is_executable_effect(T, ACT, T-1, R , P , ANN   ) :- T > 0, time(T), plan(T, ACT), announcement(ACT), announces(ACT, ANN, true ), pointed(T-1, R, P).
is_executable_effect(T, ACT, T-1, R , P , ANN   ) :- T > 0, time(T), plan(T, ACT), announcement(ACT), announces(ACT, ANN, EEXEC), entails(T-1, R, P, EEXEC), pointed(T-1, R, P), formula(EEXEC).

% For ontic actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully observant agents.
% Then the effect CAUSED is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), causes(ACT, CAUSED, true ), possible_world(T2, R2, P2),
                                                    pointed(T-1, R, PP),                             not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2), formula(EEXEC),
                                                    pointed(T-1, R, PP), entails(T2, R2, P2, EEXEC), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

% For sensing/announcement actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully or partially observant agents.
% Then the effect SENSED/ANN is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), sensing(ACT), determines(ACT, SENSED, true), possible_world(T2, R2, P2), fluent(SENSED),
                                                    pointed(T-1, R, PP),                             not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), possible_world(T2, R2, P2), formula(EEXEC), fluent(SENSED),
                                                    pointed(T-1, R, PP), entails(T2, R2, P2, EEXEC), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), announcement(ACT), announces(ACT, ANN, true), possible_world(T2, R2, P2), formula(ANN),
                                                    pointed(T-1, R, PP),                             not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), announcement(ACT), announces(ACT, ANN, EEXEC), possible_world(T2, R2, P2), formula(ANN),formula(EEXEC),
                                                    pointed(T-1, R, PP), entails(T2, R2, P2, EEXEC), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

is_fully_obs  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), observes (ACT, AG, true), agent(AG).
is_partial_obs(T, AG) :- T > 0, time(T), plan(T, ACT),    soa(ACT), aware_of (ACT, AG, true), agent(AG).
is_oblivious  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), oblivious(ACT, AG),       agent(AG).

is_fully_obs  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), observes (ACT, AG, OBS ), agent(AG),     entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).
is_partial_obs(T, AG) :- T > 0, time(T), plan(T, ACT),    soa(ACT), aware_of (ACT, AG, OBS ), agent(AG),     entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).
is_oblivious  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), observes (ACT, AG, OBS ), agent(AG), not entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).
is_oblivious  (T, AG) :- T > 0, time(T), plan(T, ACT),    soa(ACT), aware_of (ACT, AG, OBS ), agent(AG), not entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).

oblivious_ags(T, AGS)         :- T > 0, time(T), plan(T, ACT), action(ACT), agent(AG), agent_set(AGS), contains_ag(AGS, AG), is_oblivious(T, AG).
not_oblivious_ags(T, AGS)     :- T > 0, time(T), plan(T, ACT), action(ACT), not oblivious_ags(T, AGS), agent_set(AGS), time(T).

not_all_fully_obs_ags(T, AGS) :- T > 0, time(T), plan(T, ACT), action(ACT), agent(AG), agent_set(AGS), contains_ag(AGS, AG), not is_fully_obs(T, AG).
all_fully_obs_ags(T, AGS)     :- T > 0, time(T), plan(T, ACT), action(ACT), not not_all_fully_obs_ags(T, AGS), agent_set(AGS), time(T).


% ONTIC ACTIONS

% pointed/3: the possibility P is pointed at time T
pointed(T, 2*MR+1, phi(P)) :- T > 0, time(T), plan(T, ACT), ontic(ACT), pointed(T-1, MR, P), possible_world(T, 2*MR+1, phi(P)).

% possible_world/3: at time T the possibility P with repetition R is associated with the fluent set W
% A triple (T, R, P) univocally identifies a possibility. When we update such a triple when an action occurres, we obtain the triple (T+1, R+MR, phi(P)),
% where MR is the maximum repetition that can be found at time T and phi(P) is a new possibility

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T
possible_world(T,  2*MR+1, phi(P)) :- T > 0, time(T), plan(T, ACT), ontic(ACT), pointed(T-1, MR, P), possible_world(T-1, MR, P).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully observant agents.
% Then we create the new possibility phi(P).
possible_world(T, R2+MR+1, phi(P)) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), possible_world(T2, R2, P), pointed(T-1, MR, PP),
                                      reaches(T-1, R, PP, T2, R2, P, AGS), not_oblivious_ags(T, AGS).

% holds/4: the fluent set W associated to P contains the fluent F

% A fluent F holds in the fluent set phi(W) associated to the possibility phi(P) if it is caused by the ontic action ACT and if the effect is executable w.r.T. P
    holds(T, R2+MR+1, phi(P), F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), pointed(T-1, MR, PP), fluent(F),
                                    possible_world(T, R2+MR+1, phi(P)), possible_world(T2, R2, P), is_executable_effect(T, ACT, T2, R2, P,  F).

% A fluent F must not hold in the fluent set phi(W) associated to the possibility phi(P) if -F is caused by the ontic action ACT and if the effect is executable w.r.T. P
not holds(T, R2+MR+1, phi(P), F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), pointed(T-1, MR, PP), fluent(F),
                                    possible_world(T, R2+MR+1, phi(P)), possible_world(T2, R2, P), is_executable_effect(T, ACT, T2, R2, P, -F).

% Inertia: if a fluent F is not modified by the ontic action ACT, then it holds in phi(W) if it was true in W...
    holds(T, R2+MR+1, phi(P), F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), possible_world(T, R2+MR+1, phi(P)), possible_world(T2, R2, P), pointed(T-1, MR, PP),
                                    not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F), holds(T2, R2, P, F), fluent(F).

% ... and it must not hold in phi(W) if it was false in W.
not holds(T, R2+MR+1, phi(P), F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), possible_world(T, R2+MR+1, phi(P)), possible_world(T2, R2, P), pointed(T-1, MR, PP),
                                    not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F), not holds(T2, R2, P, F), fluent(F).

% !!!
% PER PROF: ho lasciato not holds nelle teste delle regole qui sopra perché trasformandoli in vincoli diventa meno efficiente (test condotto su Coin_in_the_Box__pl_5.lp)
% !!!

% Fully observant agents: if P1 -- AG --> P2 at time T' < T and AG is fully observant, then phi(P1) -- AG --> phi(P2) at time T
believes(T, R1+MR+1, phi(P1), T , R2+MR+1, phi(P2), AG) :- T > 0, plan(T,ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T, T2 <= T1,
                                                           is_fully_obs(T, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), possible_world(T, R2+MR+1, phi(P2)), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time T' < T and AG is oblivious, then phi(P1) -- AG --> P2 at time T
believes(T, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- T > 0, plan(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T, T2 <= T1,
                                                           is_oblivious(T, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% SENSING ACTIONS

% hold_sensed_effects/4: the possibility P2 agrees with the pointed possibility on the SENSED fluents of the sensing action ACT performed at time T
hold_sensed_effects(T-1, R , P , T) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, R, P), time(T).
hold_sensed_effects(T2 , R2, P2, T) :- T > 0, plan(T, ACT), sensing(ACT), possible_world(T-1, R, PP), possible_world(T2, R2, P2), T2 < T,
                                       0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _),     holds(T2, R2, P2, SENSED), not holds(T-1, R, PP, SENSED) } 0,
                                       0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _), not holds(T2, R2, P2, SENSED),     holds(T-1, R, PP, SENSED) } 0,
                                       pointed(T-1, R, PP), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS), time(T), time(T2).

% !!!
% PER PROF: al momento hold_sensed_effects è più efficiente se lasciato nella forma qui sopra rispetto alla forma commentata qui sotto
% !!!

% not_hold_sensed_effects(T2 , R2, P2, T) :- T > 0, plan(T, ACT), time(T), time(T2), sensing(ACT), possible_world(T-1, R, PP), possible_world(T2, R2, P2), T2 < T,
%                                         is_executable_effect(T, ACT, T2, R2, P2, SENSED), determines(ACT, SENSED, _),     holds(T2, R2, P2, SENSED), not holds(T-1, R, PP, SENSED),
%                                         pointed(T-1, R, PP), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

% not_hold_sensed_effects(T2 , R2, P2, T) :- T > 0, plan(T, ACT), time(T), time(T2), sensing(ACT), possible_world(T-1, R, PP), possible_world(T2, R2, P2), T2 < T,
%                                         is_executable_effect(T, ACT, T2, R2, P2, SENSED), determines(ACT, SENSED, _), not holds(T2, R2, P2, SENSED),     holds(T-1, R, PP, SENSED),
%                                         pointed(T-1, R, PP), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

% hold_sensed_effects(T-1, R , P , T) :- T > 0, plan(T, ACT), time(T), sensing(ACT), pointed(T-1, R, P).

% hold_sensed_effects(T2 , R2, P2, T) :- T > 0, plan(T, ACT), time(T), time(T2), sensing(ACT), possible_world(T-1, R, PP), possible_world(T2, R2, P2), T2 < T,
%                                        not not_hold_sensed_effects(T2, R2, P2, T), pointed(T-1, R, PP), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS).

pointed(T, 2*MR+1, phi(P)) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, P), possible_world(T, 2*MR+1, phi(P)), time(T).

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T-1
possible_world(T,  2*MR+1, phi(P)) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, P), possible_world(T-1, MR, P), time(T).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(T, R2+MR+1, phi(P)) :- T > 0, is_executable(T, ACT), sensing(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P),T2 < T,
                                      reaches(T-1, R, PP, T2, R2, P, AGS), all_fully_obs_ags(T, AGS),
                                      hold_sensed_effects(T2, R2, P, T), time(T), time(T2).

% If a partially observant agent AG believes P2, then we create the new possibility phi(P2)
possible_world(T, R2+MR+1, phi(P)) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P), T2 < T,
                                      believes(T-1, R, PP, T2, R2, P, AG), is_partial_obs(T, AG), time(T), time(T2).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully/partially observant agents.
% Then we create the new possibility phi(P).
possible_world(T, R2+MR+1, phi(P)) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, PP), possible_world(T1, R1, P1), possible_world(T2, R2, P), T1 < T, T2 <= T1,
                                      believes(T-1, R, PP, T1, R1, P1, AG), is_partial_obs(T, AG), reaches(T1, R1, P1, T2, R2, P, AGS), not_oblivious_ags(T, AGS), time(T), time(T1), time(T2).

% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in phi(P) the fluents that hold in W
holds(T, R2+MR+1, phi(P), F) :- T > 0, plan(T, ACT), sensing(ACT), possible_world(T, R2+MR+1, phi(P)), possible_world(T2, R2, P), T2 < T,
                                holds(T2, R2, P, F), fluent(F), pointed(T-1, MR, PP), time(T), time(T2).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time T and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T, R1+MR+1, phi(P1), T , R2+MR+1, phi(P2), AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), possible_world(T, R2+MR+1, phi(P2)),
                                                           hold_sensed_effects(T1, R1, P1, T), hold_sensed_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time T and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T, R1+MR+1, phi(P1), T , R2+MR+1, phi(P2), AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), possible_world(T, R2+MR+1, phi(P2)),
                                                           not hold_sensed_effects(T1, R1, P1, T), not hold_sensed_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Partially observant agents: if P1 -- AG --> P2 at time T and AG is partially observant, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T, R1+MR+1, phi(P1), T , R2+MR+1, phi(P2), AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_partial_obs(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), possible_world(T, R2+MR+1, phi(P2)), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time T and AG is oblivious, then phi(P1) -- AG --> P2 at time T+1
believes(T, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_oblivious(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% ANNOUNCEMENT ACTIONS

% hold_announced_effects/4: the possibility P2 agrees with the pointed possibility on the ANN formulas of the announcement action ACT performed at time T
hold_announced_effects(T-1, R , P , T) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, R, P), time(T).
hold_announced_effects(T2 , R2, P2, T) :- T > 0, plan(T, ACT), announcement(ACT), possible_world(T-1, R, PP), possible_world(T2, R2, P2), T2 < T,
                                          0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _),     entails(T2, R2, P2, ANN), not entails(T-1, R, PP, ANN) } 0,
                                          0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _), not entails(T2, R2, P2, ANN),     entails(T-1, R, PP, ANN) } 0,
                                          pointed(T-1, R, PP), not_oblivious_ags(T, AGS), reaches(T-1, R, PP, T2, R2, P2, AGS), time(T), time(T2).


pointed(T, 2*MR+1, phi(P)) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, P), possible_world(T, 2*MR+1, phi(P)), time(T).

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T-1
possible_world(T,  2*MR+1, phi(P)) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, P), possible_world(T-1, MR, P), time(T).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths

possible_world(T, R2+MR+1, phi(P)) :- T > 0, is_executable(T, ACT), announcement(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P),T2 < T,
                                      reaches(T-1, R, PP, T2, R2, P, AGS), all_fully_obs_ags(T, AGS),
                                      hold_announced_effects(T2, R2, P, T), time(T), time(T2).

% If a partially observant agent AG believes P2, then we create the new possibility phi(P2)
possible_world(T, R2+MR+1, phi(P)) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P), T2 < T,
                                      believes(T-1, R, PP, T2, R2, P, AG), is_partial_obs(T, AG), time(T), time(T2).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully/partially observant agents.
% Then we create the new possibility phi(P).
possible_world(T, R2+MR+1, phi(P)) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, PP), possible_world(T1, R1, P1), possible_world(T2, R2, P), T1 < T, T2 <= T1,
                                      believes(T-1, R, PP, T1, R1, P1, AG), is_partial_obs(T, AG), reaches(T1, R1, P1, T2, R2, P, AGS), not_oblivious_ags(T, AGS), time(T), time(T1), time(T2).

% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in phi(P) the fluents that hold in W
holds(T, R2+MR+1, phi(P), F) :- T > 0, plan(T, ACT), announcement(ACT), possible_world(T, R2+MR+1, phi(P)), possible_world(T2, R2, P), T2 < T,
                                holds(T2, R2, P, F), fluent(F), pointed(T-1, MR, PP), time(T), time(T2).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time T and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T, R1+MR+1, phi(P1), T , R2+MR+1, phi(P2), AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), possible_world(T, R2+MR+1, phi(P2)),
                                                           hold_announced_effects(T1, R1, P1, T), hold_announced_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time T and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T, R1+MR+1, phi(P1), T , R2+MR+1, phi(P2), AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), possible_world(T, R2+MR+1, phi(P2)),
                                                           not hold_announced_effects(T1, R1, P1, T), not hold_announced_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Partially observant agents: if P1 -- AG --> P2 at time T and AG is partially observant, then phi(P1) -- AG --> phi(P2) at time T+1
believes(T, R1+MR+1, phi(P1), T , R2+MR+1, phi(P2), AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_partial_obs(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), possible_world(T, R2+MR+1, phi(P2)), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time T and AG is oblivious, then phi(P1) -- AG --> P2 at time T+1
believes(T, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_oblivious(T, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(T, R1+MR+1, phi(P1)), pointed(T-1, MR, PP), time(T), time(T1), time(T2).


%#program step(T).

1 { plan(T, ACT) : action(ACT) } 1 :- T > 0, time(T).
:- 0 { plan(T, _) } 0.

% Test
:- T > 0, plan(T, ACT), not is_executable(T, ACT), time(T).
% :- T > 0, plan(T, ACT), pointed(T-1, R, P), not entails(T-1, R, P, EXEC), action(ACT), executable(ACT, EXEC), formula(EXEC), time(T).
%%
%:- T > 0, plan(T, ACT), pointed(T-1, R, P), 0 { is_executable_effect(T, ACT, T-1, R, P, CAUSED) : causes(ACT, CAUSED, _) } 0, ontic(ACT), time(T).
%
%:- T > 0, reached_goal(T), plan(T2, ACT),    T2 > T.
%:- pointed(T, R, P), pointed(T1, RR, PP), T1 < T, T > 0, time(T), time(T1),
%  0 { formula(F): entails(T, R, P, F), not entails(T1, RR, PP, F)} 0,
%  0 { formula(F): not entails(T, R, P, F), entails(T1, RR, PP, F)} 0.


%#program check(T).
:- 0 { time(T): not entails(T, R, P, F), pointed(T, R, P), goal(F), formula(F)} 0.
% :- not_reached_goal(T), query(T).

% Display

% !!! Uncomment while printing the results !!!
%#show pointed/3.
%#show possible_world/3.
%#show believes/7.
%#show holds/4.
%#show fluent/1.

%#show is_executable/2.
%#show entails/4.

#show plan/2.
% #show reached_goal/1.





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

% entails_cp(T, T1, R1, P1, MF,  F) :-     holds(T1, R1, P1, F), ag_list(MF, L), reaches_ag_list(T, R, P, T1, R1, P1, L), pointed(T, R, P), possible_world(T1, R1, P1),%*fluent_set(W1)*%, cp_formula(MF), fluent(F).
% entails_cp(T, T1, R1, P1, MF, -F) :- not holds(T1, R1, P1, F), ag_list(MF, L), reaches_ag_list(T, R, P, T1, R1, P1, L), pointed(T, R, P), possible_world(T1, R1, P1),%*fluent_set(W1)*%, cp_formula(MF), fluent(F).

% entails_cp(T, T1, R1, P1, neg(F)) :-

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

% entails(T, F) :- pointed(T, R, P), ...





% *** Working on optimization #3... ***
% possible_world(T+1, R +MR+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, P ), possible_world(T, R , P), max_rep(T, MR),
%                                                 possible_world(T1, _, _), T1 <= T, apply_ontic(W1, T, R, P, T), W1 != W.

% possible_world(T+1, R2+MR+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P), max_rep(T, MR),
%                                                 believes(T, R, PP, T2, R2, P, AG), is_fully_obs(T+1, AG),
%                                                 possible_world(T1, _, _), T1 <= T, apply_ontic(W1, T2, R2, P, T).

% possible_world(T+1, R2+MR+1, phi(P)) :- is_executable(T, ACT), ontic(ACT), pointed(T, R, PP), possible_world(T2, R2, P), max_rep(T, MR),
%                                                 reaches(T, R, PP, T2, R2, P, AGS), 0 { agent(AG) : contains_ag(AGS, AG), not is_fully_obs(T+1, AG) } 0,
%                                                 possible_world(T1, _, _), T1 <= T, apply_ontic(W1, T2, R2, P, T).
%
% holds(phi(W), F) :- is_executable(T, ACT), possible_world(T+1, R2+MR+1, phi(P))), possible_world(T2, R2, P), max_rep(T, MR),
%                                        0 { time(T1): T1 <= T, possible_world(T1, _, _)), apply_ontic(phi(W), T2, R2, P, T) } 0.

% holds(phi(W), F) :- is_executable(T, ACT), possible_world(T+1, R2+MR+1, phi(P))), possible_world(T2, R2, P), max_rep(T, MR),
%                                        not is_executable_effect(T, ACT, T2, R2, P, F), holds(W, F), fluent(F),%*fluent_set(phi(W)*%),
%                                        0 { time(T1): T1 <= T, possible_world(T1, _, _)), apply_ontic(phi(W), T2, R2, P, T) } 0.

% When we apply ACT to P2, the possibility phi(P2) must entail CAUSED if it holds causes(T, ACT, CAUSED)
% apply_ontic_fluent(T2, R2, P2,  CAUSED, T) :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT), possible_world(T2, R2, P2),
%                                               is_executable_effect(T, ACT, T2, R2, P2,  CAUSED), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% When we apply ACT to P2, the possibility phi(P2) must entail F if it does not hold nor causes(T, ACT, F), nor causes(T, ACT, -F) and if P2 entails F
% apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT), possible_world(T2, R2, P2), holds(W2, F),
% apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT), possible_world(T2, R2, P2), holds(T2, R2, P2, F),
%                                               not is_executable_effect(T, ACT, T2, R2, P2, F), not is_executable_effect(T, ACT, T2, R2, P2, -F), fluent(F), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% not apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT), possible_world(T2, R2, P2), not holds(W2, F),
% not apply_ontic_fluent(T2, R2, P2,  F     , T) :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT), possible_world(T2, R2, P2), not holds(T2, R2, P2, F),
%                                               not is_executable_effect(T, ACT, T2, R2, P2, F), not is_executable_effect(T, ACT, T2, R2, P2, -F), fluent(F), T2 <= T,
%                                               pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% The fluent_set W1 is the one resulting from W2 when we apply ACT. W1 must entail all the fluents that are caused by ACT and must agree with W2 w.r.T. the other fluents
% apply_ontic(W1, T2, R2, P2, T) :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= T, T2 <= T,
%                                   0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2,  F, T), not holds(W1, F) } 0, %not holds(T1, P1, W1, F) } 0,
%                                   0 { fluent(F) :     apply_ontic_fluent(T2, R2, P2, -F, T),     holds(W1, F) } 0, %not holds(T1, P1, W1, F) } 0,
%                                   0 { fluent(F) : not apply_ontic_fluent(T2, R2, P2,  F, T),     holds(W1, F), not holds(W2, %holds(T1, R1, P1, W1, F), not holds(T2, R2, P2, F) } 0,
%                                   pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).




% 0 { apply_ontic(W1, T2, R2, P2, T) : possible_world(T1, R1, P1), T1 <= T } 1 :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT), possible_world(T2, R2, P2),
%                                                                                     pointed(T, R, PP), is_fully_obs(T+1, AG), believes(T, R, PP, T2, R2, P2, AG) : not pointed(T, R, P2).

% #show apply_ontic_fluent/5.
% #show apply_ontic/5.

% :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= T, T2 <= T,
%    apply_ontic(W1, T2, R2, P2, T),     apply_ontic_fluent(T2, R2, P2,  F, T), fluent(F), not holds(W1, F).

% :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= T, T2 <= T,
%    apply_ontic(W1, T2, R2, P2, T),     apply_ontic_fluent(T2, R2, P2, -F, T), fluent(F),     holds(W1, F).

% :- plan(T, ACT), %*is_executable(T, ACT),*% ontic(ACT),%*fluent_set(W1)*%, possible_world(T1, R1, P1), possible_world(T2, R2, P2), T1 <= T, T2 <= T,
%    apply_ontic(W1, T2, R2, P2, T), not apply_ontic_fluent(T2, R2, P2,  F, T), fluent(F),     holds(W1, F), not holds(W2, F).

% ***              End              ***

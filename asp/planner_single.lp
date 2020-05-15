%
% EMP encoding in ASP
%

% ********** REACHABILITY **********

time(0..maxT).

reaches(0, 0, P1, 0, 0, P2, AGS) :- believes(0, 0, P1, 0, 0, P2, AG), contains_ag(AGS, AG),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), agent(AG), agent_set(AGS).

reaches(0, 0, P1, 0, 0, P2, AGS) :- believes(0, 0, P1, 0, 0, P2, AG), pointed(0, 0, P1), main_formula(c(AGS, F)), contains_ag(AGS, AG),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), agent(AG), agent_set(AGS).

reaches(0, 0, P1, 0, 0, P2, AGS) :- believes(0, 0, P1, 0, 0, P3, AG), contains_ag(AGS, AG), reaches(0, 0, P3, 0, 0, P2, AGS),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), possible_world(0, 0, P3), agent(AG), agent_set(AGS).

% reaches/7: P1 reaches P2 with a path that contains only agents inside AGS
reaches(T, R1, P1, T2, R2, P2, AGS) :- believes(T, R1, P1, T2, R2, P2, AG), not initially(c(AGS, F)), formula(F), T > 0, time(T), time(T2), T2 <= T,
                                       contains_ag(AGS, AG), agent(AG), agent_set(AGS), possible_world(T, R1, P1), possible_world(T2, R2, P2).

reaches(T, R1, P1, T2, R2, P2, AGS) :- believes(T, R1, P1, T2, R2, P2, AG), pointed(T, R1, P1), main_formula(c(AGS, F)), not initially(c(AGS, F)), T > 0, time(T), time(T2), T2 <= T,
                                       contains_ag(AGS, AG), agent(AG), agent_set(AGS), possible_world(T, R1, P1), possible_world(T2, R2, P2).

reaches(T, R1, P1, T2, R2, P2, AGS) :- believes(T, R1, P1, T3, R3, P3, AG), T > 0, time(T), time(T2), time(T3), T3 <= T, T2 <= T3,
                                       contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS), agent(AG), agent_set(AGS),
                                       possible_world(T, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3).

reaches_no_oblivious(T, T2, R2, P2) :- T > 0, time(T), time(T2), T2 <= T, pointed(T-1, R, P), possible_world(T2, R2, P2),
                                       believes(T-1, R, P, T2, R2, P2, AG), not is_oblivious(T, AG).

reaches_no_oblivious(T, T2, R2, P2) :- T > 0, time(T), time(T1), time(T2), T1 <= T, T2 <= T, possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                       reaches_no_oblivious(T, T1, R1, P1), believes(T1, R1, P1, T2, R2, P2, AG), not is_oblivious(T, AG).

reaches_only_fully_obs(T, T2, R2, P2) :- T > 0, time(T), time(T2), T2 <= T, pointed(T-1, R, P), possible_world(T2, R2, P2),
                                         believes(T-1, R, P, T2, R2, P2, AG), is_fully_obs(T, AG).

reaches_only_fully_obs(T, T2, R2, P2) :- T > 0, time(T), time(T1), time(T2), T1 <= T, T2 <= T, possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                         reaches_only_fully_obs(T, T1, R1, P1), believes(T1, R1, P1, T2, R2, P2, AG), is_fully_obs(T, AG).

reaches_first_partially_obs(T, T2, R2, P2) :- T > 0, time(T),time(T2), T2 <= T, pointed(T-1, R, P), possible_world(T2, R2, P2),
                                              believes(T-1, R, P, T2, R2, P2, AG), is_partial_obs(T, AG).

reaches_first_partially_obs(T, T2, R2, P2) :- T > 0, time(T), time(T1), time(T2), T1 <= T, T2 <= T, possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                              reaches_first_partially_obs(T, T1, R1, P1), believes(T1, R1, P1, T2, R2, P2, AG), not is_oblivious(T, AG).


% ********** ENTAILMENT **********



% Sensing or Announcement
soa(ACT) :- sensing(ACT).
soa(ACT) :- announcement(ACT).

literal( F) :- fluent(F).
literal(-F) :- fluent(F).

main_formula(F) :- formula(F), executable(_,    F).
main_formula(F) :- formula(F),   observes(_, _, F).
main_formula(F) :- formula(F),   aware_of(_, _, F).
main_formula(F) :- formula(F),     causes(_, _, F).
main_formula(F) :- formula(F), determines(_, _, F).
main_formula(F) :- formula(F),  announces(_, _, F).
main_formula(F) :- formula(F), goal(F).

% Entailment of initially conditions. We use it to generate the initial state
entails_init(P,  F    )          :-     holds(0, 0, P, F),                          initial_possibility(P), fluent(F), init_sf(F).
entails_init(P, -F    )          :- not holds(0, 0, P, F),                          initial_possibility(P), fluent(F), init_sf(F).

entails_init(P, neg(F))          :- not entails_init(P, F ),                        initial_possibility(P), init_sf(neg(F)),      init_sf(F).
entails_init(P, and(F1, F2))     :-     entails_init(P, F1), entails_init(P, F2),   initial_possibility(P), init_sf(and(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(P,  or(F1, F2))     :-     entails_init(P, F1),                        initial_possibility(P), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(P,  or(F1, F2))     :-                          entails_init(P, F2),   initial_possibility(P), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).

% Entailment of all the other formulas w.r.T. the calculated initial state
entails(0, 0, P,  F    )         :-     holds(0, 0, P, F),                          good_initial_possibility(P), possible_world(0, 0, P), fluent(F).
entails(0, 0, P, -F    )         :- not holds(0, 0, P, F),                          good_initial_possibility(P), possible_world(0, 0, P), fluent(F).

entails(0, 0, P, neg(F))         :- not entails(0, 0, P, F ),                       good_initial_possibility(P), possible_world(0, 0, P), formula(neg(F)),      formula(F).
entails(0, 0, P, and(F1, F2))    :-     entails(0, 0, P, F1), entails(0, 0, P, F2), good_initial_possibility(P), possible_world(0, 0, P), formula(and(F1, F2)), formula(F1), formula(F2).
entails(0, 0, P,  or(F1, F2))    :-     entails(0, 0, P, F1),                       good_initial_possibility(P), possible_world(0, 0, P), formula( or(F1, F2)), formula(F1), formula(F2).
entails(0, 0, P,  or(F1, F2))    :-                           entails(0, 0, P, F2), good_initial_possibility(P), possible_world(0, 0, P), formula( or(F1, F2)), formula(F1), formula(F2).

entails(0, 0, P,   b(AG , F))    :- not not_entails(0, 0, P, b(AG , F)),            good_initial_possibility(P), possible_world(0, 0, P), formula(b(AG , F)), formula(F), agent(AG).
entails(0, 0, P,   c(AGS, F))    :- not not_entails(0, 0, P, c(AGS, F)),            good_initial_possibility(P), possible_world(0, 0, P), formula(c(AGS, F)), formula(F), agent_set(AGS).

not_entails(0, 0, P1, b(AG , F)) :- not entails(0, 0, P2, F ), believes(0, 0, P1, 0, 0, P2, AG), good_initial_possibility(P1), good_initial_possibility(P2),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), formula(b(AG , F)), formula(F), agent(AG).

not_entails(0, 0, P1, c(AGS, F)) :- not entails(0, 0, P2, F), reaches(0, 0, P1, 0, 0, P2, AGS), good_initial_possibility(P1), good_initial_possibility(P2),
                                    possible_world(0, 0, P1), possible_world(0, 0, P2), formula(c(AGS, F)), formula(F), agent_set(AGS).




% entails/4: P entails the formula F
entails(T, R, P,  F    )      :- T > 0, time(T),     holds(T, R, P, F),                          possible_world(T, R, P), fluent(F).
entails(T, R, P, -F    )      :- T > 0, time(T), not holds(T, R, P, F),                          possible_world(T, R, P), fluent(F).

entails(T, R, P, neg(F))      :- T > 0, time(T), not entails(T, R, P, F ),                       possible_world(T, R, P), formula(neg(F)),      formula(F).
entails(T, R, P, and(F1, F2)) :- T > 0, time(T),     entails(T, R, P, F1), entails(T, R, P, F2), possible_world(T, R, P), formula(and(F1, F2)), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :- T > 0, time(T),     entails(T, R, P, F1),                       possible_world(T, R, P), formula( or(F1, F2)), formula(F1), formula(F2).
entails(T, R, P,  or(F1, F2)) :- T > 0, time(T),                           entails(T, R, P, F2), possible_world(T, R, P), formula( or(F1, F2)), formula(F1), formula(F2).

entails(T, R, P,   b(AG , F)) :- T > 0, time(T), not not_entails(T, R, P, b(AG , F)),            possible_world(T, R, P), formula(b(AG , F)), formula(F), agent(AG).
entails(T, R, P,   c(AGS, F)) :- T > 0, time(T), not not_entails(T, R, P, c(AGS, F)),            possible_world(T, R, P), formula(c(AGS, F)), formula(F), agent_set(AGS).

not_entails(T, R1, P1, b(AG , F)) :- T > 0, time(T), time(T2), T2 <= T, not entails(T2, R2, P2, F), believes(T, R1, P1, T2, R2, P2, AG),
                                     possible_world(T1, R1, P1), possible_world(T2, R2, P2), formula(b(AG , F)), formula(F), agent(AG).

not_entails(T, R1, P1, c(AGS, F)) :- T > 0, time(T), time(T2), T2 <= T, not entails(T2, R2, P2, F), reaches(T, R1, P1, T2, R2, P2, AGS),
                                     possible_world(T1, R1, P1), possible_world(T2, R2, P2), formula(c(AGS, F)), formula(F), agent_set(AGS).




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
initial_possibility(1..K)        :- initial_state_dimension(K).

% The initial conditions of form 1. determine the pointed possibility of the initial state
% A possibility is pointed if it entails exactly those fluents that hold initially
not_initial_pointed(P) :- good_initial_possibility(P), initially( F), not holds(0, 0, P, F), fluent(F).
not_initial_pointed(P) :- good_initial_possibility(P), initially(-F),     holds(0, 0, P, F), fluent(F).

pointed(0, 0, P) :- good_initial_possibility(P), not not_initial_pointed(P).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
    holds(0, 0, P, F) :- initially(c(all_agents,  F)), initial_possibility(P), fluent(F).
not holds(0, 0, P, F) :- initially(c(all_agents, -F)), initial_possibility(P), fluent(F).

% We impose that all the initial possibilities have a different fluent set
K/2 { holds(0, 0, P, F) : initial_possibility(P) } K/2 :- unknown_initially(F), initial_state_dimension(K).

% All the initial fluent sets must be different
diffzero(P1, P2) :- initial_possibility(P1), initial_possibility(P2), holds(0, 0, P1, F), not holds(0, 0, P2, F),
                    fluent(F), unknown_initially(F), P1 < P2.

diffzero(P1, P2) :- initial_possibility(P1), initial_possibility(P2), holds(0, 0, P2, F), not holds(0, 0, P1, F),
                    fluent(F), unknown_initially(F), P1 < P2.

:- not diffzero(P1, P2), initial_possibility(P1), initial_possibility(P2), P1 < P2.

:- 0 { pointed(0, 0, P) : good_initial_possibility(P) } 0.

% We impose a complete order on the fluent sets indices W_i w.r.T. their fluents
:- MF1 = #max { F : fluent(F), unknown_initially(F),     holds(0, 0, P1, F), not holds(0, 0, P2, F) },
   MF2 = #max { F : fluent(F), unknown_initially(F), not holds(0, 0, P1, F),     holds(0, 0, P2, F) },
   initial_possibility(P1), initial_possibility(P2), P1 < P2, MF1 > MF2.

% So far we dealt with (commonly known) fluents. An initial fluent set is "good" if it entails all the fluent formulas F that are initial common knowledge
no_good_initial_possibility(P) :- not entails_init(P, F), initially(c(all_agents, F)), init_sf(F), not literal(F), initial_possibility(P).
good_initial_possibility(P)    :- initial_possibility(P), not no_good_initial_possibility(P).

% For each good initial fluent set we create a correspoinding possible world
possible_world(0, 0, P) :- good_initial_possibility(P).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F),     entails(0, 0, P2, F), good_initial_possibility(P1), good_initial_possibility(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, neg(F))))), fluent(F).
not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F), not entails(0, 0, P2, F), good_initial_possibility(P1), good_initial_possibility(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, neg(F))))), fluent(F).

% The initial conditions of form 4. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :-     entails(0, 0, P1, F),     entails(0, 0, P2, F), good_initial_possibility(P1), good_initial_possibility(P2), P1 != P2, initially(c(all_agents, and(neg(b(AG, F)), neg(b(AG, neg(F)))))), formula(F).
not_believes_initially(P1, P2, AG) :- not entails(0, 0, P1, F), not entails(0, 0, P2, F), good_initial_possibility(P1), good_initial_possibility(P2), P1 != P2, initially(c(all_agents, and(neg(b(AG, F)), neg(b(AG, neg(F)))))), formula(F).

% believes/7: the agent AG, inside P1, believes P2 to be possible
believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1), possible_world(0, 0, P2),
                                    good_initial_possibility(P1), good_initial_possibility(P2), agent(AG).


% ********** TRANSITION FUNCTION **********



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
                                                    reaches_no_oblivious(T, T2, R2, P2).

is_executable_effect(T, ACT, T2, R2, P2, CAUSED) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2), formula(EEXEC),
                                                    reaches_no_oblivious(T, T2, R2, P2), entails(T2, R2, P2, EEXEC).

% For sensing/announcement actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully or partially observant agents.
% Then the effect SENSED/ANN is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), sensing(ACT), determines(ACT, SENSED, true), possible_world(T2, R2, P2), fluent(SENSED),
                                                    reaches_no_oblivious(T, T2, R2, P2).

is_executable_effect(T, ACT, T2, R2, P2, SENSED) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), sensing(ACT), determines(ACT, SENSED, EEXEC), possible_world(T2, R2, P2), formula(EEXEC), fluent(SENSED),
                                                    reaches_no_oblivious(T, T2, R2, P2), entails(T2, R2, P2, EEXEC).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), announcement(ACT), announces(ACT, ANN, true), possible_world(T2, R2, P2), formula(ANN),
                                                    reaches_no_oblivious(T, T2, R2, P2).

is_executable_effect(T, ACT, T2, R2, P2, ANN   ) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), announcement(ACT), announces(ACT, ANN, EEXEC), possible_world(T2, R2, P2), formula(ANN),formula(EEXEC),
                                                    reaches_no_oblivious(T, T2, R2, P2), entails(T2, R2, P2, EEXEC).

is_fully_obs  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), observes (ACT, AG, true), agent(AG).
is_partial_obs(T, AG) :- T > 0, time(T), plan(T, ACT),    soa(ACT), aware_of (ACT, AG, true), agent(AG).
is_oblivious  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), oblivious(ACT, AG),       agent(AG).

is_fully_obs  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), observes (ACT, AG, OBS ), agent(AG),     entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).
is_partial_obs(T, AG) :- T > 0, time(T), plan(T, ACT),    soa(ACT), aware_of (ACT, AG, OBS ), agent(AG),     entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).
is_oblivious  (T, AG) :- T > 0, time(T), plan(T, ACT), action(ACT), observes (ACT, AG, OBS ), agent(AG), not entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).
is_oblivious  (T, AG) :- T > 0, time(T), plan(T, ACT),    soa(ACT), aware_of (ACT, AG, OBS ), agent(AG), not entails(T-1, R, PP, OBS), formula(OBS), pointed(T-1, R, PP).


% ONTIC ACTIONS

% pointed/3: the possibility P is pointed at time T
pointed(T, 2*MR+1, P) :- T > 0, time(T), plan(T, ACT), ontic(ACT), pointed(T-1, MR, P), possible_world(T, 2*MR+1, P).

% possible_world/3: at time T the possibility P with repetition R is associated with the fluent set W
% A triple (T, R, P) univocally identifies a possibility. When we update such a triple when an action occurres, we obtain the triple (T+1, R+MR, P),
% where MR is the maximum repetition that can be found at time T and P is a new possibility

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T
possible_world(T,  2*MR+1, P) :- T > 0, time(T), plan(T, ACT), ontic(ACT), pointed(T-1, MR, P), possible_world(T-1, MR, P).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully observant agents.
% Then we create the new possibility P.
possible_world(T, R2+MR+1, P) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), possible_world(T2, R2, P), pointed(T-1, MR, PP),
                                 reaches_no_oblivious(T, T2, R2, P).


% holds/4: the fluent set W associated to P contains the fluent F

% A fluent F holds in the fluent set phi(W) associated to the possibility P if it is caused by the ontic action ACT and if the effect is executable w.r.T. P
    holds(T, R2+MR+1, P, F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), pointed(T-1, MR, PP), fluent(F),
                               possible_world(T, R2+MR+1, P), possible_world(T2, R2, P), is_executable_effect(T, ACT, T2, R2, P,  F).

% A fluent F must not hold in the fluent set phi(W) associated to the possibility P if -F is caused by the ontic action ACT and if the effect is executable w.r.T. P
not holds(T, R2+MR+1, P, F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), pointed(T-1, MR, PP), fluent(F),
                               possible_world(T, R2+MR+1, P), possible_world(T2, R2, P), is_executable_effect(T, ACT, T2, R2, P, -F).

% Inertia: if a fluent F is not modified by the ontic action ACT, then it holds in phi(W) if it was true in W...
    holds(T, R2+MR+1, P, F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), possible_world(T, R2+MR+1, P), possible_world(T2, R2, P), pointed(T-1, MR, PP),
                               not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F), holds(T2, R2, P, F), fluent(F).

% ... and it must not hold in phi(W) if it was false in W.
not holds(T, R2+MR+1, P, F) :- T > 0, time(T), time(T2), T2 < T, plan(T, ACT), ontic(ACT), possible_world(T, R2+MR+1, P), possible_world(T2, R2, P), pointed(T-1, MR, PP),
                               not is_executable_effect(T, ACT, T2, R2, P, F), not is_executable_effect(T, ACT, T2, R2, P, -F), not holds(T2, R2, P, F), fluent(F).


% Fully observant agents: if P1 -- AG --> P2 at time T' < T and AG is fully observant, then P1 -- AG --> P2 at time T
believes(T, R1+MR+1, P1, T , R2+MR+1, P2, AG) :- T > 0, plan(T,ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T, T2 <= T1,
                                                 is_fully_obs(T, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), possible_world(T, R2+MR+1, P2), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time T' < T and AG is oblivious, then P1 -- AG --> P2 at time T
believes(T, R1+MR+1, P1, T2, R2     , P2, AG) :- T > 0, plan(T, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), T1 <= T, T2 <= T, T2 <= T1,
                                                 is_oblivious(T, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% SENSING ACTIONS

% hold_sensed_effects/4: the possibility P2 agrees with the pointed possibility on the SENSED fluents of the sensing action ACT performed at time T
hold_sensed_effects(T-1, R , P , T) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, R, P), time(T).
hold_sensed_effects(T2 , R2, P2, T) :- T > 0, plan(T, ACT), sensing(ACT), possible_world(T-1, R, PP), possible_world(T2, R2, P2), T2 < T,
                                       0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _),     holds(T2, R2, P2, SENSED), not holds(T-1, R, PP, SENSED) } 0,
                                       0 { is_executable_effect(T, ACT, T2, R2, P2, SENSED) : determines(ACT, SENSED, _), not holds(T2, R2, P2, SENSED),     holds(T-1, R, PP, SENSED) } 0,
                                       pointed(T-1, R, PP), reaches_no_oblivious(T, T2, R2, P2), time(T), time(T2).


pointed(T, 2*MR+1, P) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, P), possible_world(T, 2*MR+1, P), time(T).

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T-1
possible_world(T,  2*MR+1, P) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, P), possible_world(T-1, MR, P), time(T).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(T, R2+MR+1, P) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P),T2 < T,
                                 reaches_only_fully_obs(T, T2, R2, P), hold_sensed_effects(T2, R2, P, T), time(T), time(T2).

% If a partially observant agent AG believes P2, then we create the new possibility P2
possible_world(T, R2+MR+1, P) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P), T2 < T,
                                 believes(T-1, R, PP, T2, R2, P, AG), is_partial_obs(T, AG), time(T), time(T2).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully/partially observant agents.
% Then we create the new possibility P.
possible_world(T, R2+MR+1, P) :- T > 0, plan(T, ACT), sensing(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P), T2 < T,
                                 reaches_first_partially_obs(T, T2, R2, P2), time(T), time(T2).


% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in P the fluents that hold in W
holds(T, R2+MR+1, P, F) :- T > 0, plan(T, ACT), sensing(ACT), possible_world(T, R2+MR+1, P), possible_world(T2, R2, P), T2 < T,
                           holds(T2, R2, P, F), fluent(F), pointed(T-1, MR, PP), time(T), time(T2).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time T and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T , R2+MR+1, P2, AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), possible_world(T, R2+MR+1, P2),
                                                 hold_sensed_effects(T1, R1, P1, T), hold_sensed_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time T and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T , R2+MR+1, P2, AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), possible_world(T, R2+MR+1, P2),
                                                 not hold_sensed_effects(T1, R1, P1, T), not hold_sensed_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Partially observant agents: if P1 -- AG --> P2 at time T and AG is partially observant, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T , R2+MR+1, P2, AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_partial_obs(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), possible_world(T, R2+MR+1, P2), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time T and AG is oblivious, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T2, R2     , P2, AG) :- T > 0, plan(T, ACT), sensing(ACT), T1 <= T, T2 <= T, T2 <= T1, is_oblivious(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% ANNOUNCEMENT ACTIONS

% hold_announced_effects/4: the possibility P2 agrees with the pointed possibility on the ANN formulas of the announcement action ACT performed at time T
hold_announced_effects(T-1, R , P , T) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, R, P), time(T).
hold_announced_effects(T2 , R2, P2, T) :- T > 0, plan(T, ACT), announcement(ACT), possible_world(T-1, R, PP), possible_world(T2, R2, P2), T2 < T,
                                          0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _),     entails(T2, R2, P2, ANN), not entails(T-1, R, PP, ANN) } 0,
                                          0 { is_executable_effect(T, ACT, T2, R2, P2, ANN) : announces(ACT, ANN, _), not entails(T2, R2, P2, ANN),     entails(T-1, R, PP, ANN) } 0,
                                          pointed(T-1, R, PP), reaches_no_oblivious(T, T2, R2, P2), time(T), time(T2).


pointed(T, 2*MR+1, P) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, P), possible_world(T, 2*MR+1, P), time(T).

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time T-1
possible_world(T,  2*MR+1, P) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, P), possible_world(T-1, MR, P), time(T).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(T, R2+MR+1, P) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P),T2 < T,
                                 reaches_only_fully_obs(T, T2, R2, P), hold_announced_effects(T2, R2, P, T), time(T), time(T2).

% If a partially observant agent AG believes P2, then we create the new possibility P2
possible_world(T, R2+MR+1, P) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P), T2 < T,
                                 believes(T-1, R, PP, T2, R2, P, AG), is_partial_obs(T, AG), time(T), time(T2).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully/partially observant agents.
% Then we create the new possibility P.
possible_world(T, R2+MR+1, P) :- T > 0, plan(T, ACT), announcement(ACT), pointed(T-1, MR, PP), possible_world(T2, R2, P), T2 < T,
                                 reaches_first_partially_obs(T, T2, R2, P2), time(T), time(T2).


% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in P the fluents that hold in W
holds(T, R2+MR+1, P, F) :- T > 0, plan(T, ACT), announcement(ACT), possible_world(T, R2+MR+1, P), possible_world(T2, R2, P), T2 < T,
                           holds(T2, R2, P, F), fluent(F), pointed(T-1, MR, PP), time(T), time(T2).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time T and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T , R2+MR+1, P2, AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), possible_world(T, R2+MR+1, P2),
                                                 hold_announced_effects(T1, R1, P1, T), hold_announced_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time T and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T , R2+MR+1, P2, AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_fully_obs(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), possible_world(T, R2+MR+1, P2),
                                                 not hold_announced_effects(T1, R1, P1, T), not hold_announced_effects(T2, R2, P2, T), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Partially observant agents: if P1 -- AG --> P2 at time T and AG is partially observant, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T , R2+MR+1, P2, AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_partial_obs(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), possible_world(T, R2+MR+1, P2), pointed(T-1, MR, PP), time(T), time(T1), time(T2).

% Oblivious agents: if P1 -- AG --> P2 at time T and AG is oblivious, then P1 -- AG --> P2 at time T+1
believes(T, R1+MR+1, P1, T2, R2     , P2, AG) :- T > 0, plan(T, ACT), announcement(ACT), T1 <= T, T2 <= T, T2 <= T1, is_oblivious(T, AG),
                                                 believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                 possible_world(T, R1+MR+1, P1), pointed(T-1, MR, PP), time(T), time(T1), time(T2).




1 { plan(T, ACT) : action(ACT) } 1 :- T > 0, time(T).

% Test
:- 0 { plan(T, _) } 0.
:- T > 0, time(T), plan(T, ACT), not is_executable(T, ACT).
% :- T > 0, time(T), plan(T, ACT), 0 { pointed(T, _, _) } 0.
% :- T > 0, plan(T, ACT), pointed(T-1, R, P), 0 { is_executable_effect(T, ACT, T-1, R, P, CAUSED) : causes(ACT, CAUSED, _) } 0, ontic(ACT), time(T).
%
%:- T > 0, reached_goal(T), plan(T2, ACT),    T2 > T.
%:- pointed(T, R, P), pointed(T1, RR, PP), T1 < T, T > 0, time(T), time(T1),
%  0 { formula(F): entails(T, R, P, F), not entails(T1, RR, PP, F)} 0,
%  0 { formula(F): not entails(T, R, P, F), entails(T1, RR, PP, F)} 0.

:- not entails(maxT, R, P, F), pointed(maxT, R, P), goal(F), formula(F).

% Display

#show plan/2.

%To avoid showing the error "info: atom does not occur in any rule head"
%#defined init_sf/1.
%#defined agent_set/1.
%#defined contains_ag/2.
%
%#defined ontic/1.
%#defined causes/3.
%#defined sensing/1.
%#defined determines/3.
%#defined announcement/1.
%#defined announces/3.
%
%#defined observes/3.
%#defined aware_of/3.
%#defined oblivious/2.

% !!! Uncomment while printing the results !!!
% #show pointed/3.
% #show possible_world/3.
% #show believes/7.
% #show holds/4.
%#show fluent/1.



%

% % ********** BISIMULATION ********** (Dovier, ICLP 2015)

% % Reflexivity and Symmetry
% bis(T, R1, P1, T , R1, P1) :- T > 0, time(T), possible_world(T, R1, P1).
% bis(T, R1, P1, T2, R2, P2) :- T > 0, time(T), time(T2), T2 <= T, possible_world(T, R1, P1), possible_world(T2, R2, P2), bis(T2, R2, P2, T, R1, P1).

% % Nondeterministic choice
% { bis(T, R1, P1, T2, R2, P2) } :- T > 0, time(T), time(T2), T2 < T, possible_world(T, R1, P1), possible_world(T2, R2, P2).

% :- time(T), time(T2), time(T3), time(T4), T2 < T, possible_world(T, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3),
%    bis(T, R1, P1, T2, R2, P2), believes(T, R1, P1, T3, R3, P3, AG), 0 { possible_world(T4, R4, P4) : bis(T3, R3, P3, T4, R4, P4), believes(T2, R2, P2, T4, R4, P4, AG) } 0.

% :- time(T), time(T2), T2 < T, possible_world(T, R1, P1), possible_world(T2, R2, P2),
%    bis(T, R1, P1, T2, R2, P2), holds(T, R1, P1, F), not holds(T2, R2, P2, F), fluent(F).

% % Visited states
% :- plan(T, ACT), pointed(T, R1, P1), pointed(T2, R2, P2), T2 < T, bis(T, R1, P1, T2, R2, P2).

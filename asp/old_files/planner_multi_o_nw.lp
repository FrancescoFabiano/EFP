#include <incmode>.

#program base.
time(0).

reaches(0, 0, P1, 0, 0, P2, AGS) :-
    believes(0, 0, P1, 0, 0, P2, AG), contains_ag(AGS, AG),
    possible_world(0, 0, P1), possible_world(0, 0, P2), agent(AG), agent_set(AGS).

reaches(0, 0, P1, 0, 0, P2, AGS) :-
    believes(0, 0, P1, 0, 0, P3, AG), contains_ag(AGS, AG), reaches(0, 0, P3, 0, 0, P2, AGS),
    possible_world(0, 0, P1), possible_world(0, 0, P2), possible_world(0, 0, P3), agent(AG), agent_set(AGS).

#program step(t).

time(t).
% reaches/7: P1 reaches P2 with a path that contains only agents inside AGS
reaches(t, R1, P1, T2, R2, P2, AGS) :-
    t > 0, time(t), time(T2), T2 <= t, believes(t, R1, P1, T2, R2, P2, AG),
    contains_ag(AGS, AG), agent(AG), agent_set(AGS), possible_world(t, R1, P1), possible_world(T2, R2, P2).

reaches(t, R1, P1, T2, R2, P2, AGS) :-
    t > 0, time(t), time(T2), time(T3), T3 <= t, T2 <= T3, believes(t, R1, P1, T3, R3, P3, AG),
    contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS), agent(AG), agent_set(AGS),
    possible_world(t, R1, P1), possible_world(T2, R2, P2), possible_world(T3, R3, P3).

% ********** ENTAILMENT **********

#program base.

%#const b_test = 150.
%bound(0..b_test).

soa(ACT) :- sensing(ACT).
soa(ACT) :- announcement(ACT).

literal( F) :- fluent(F).
literal(-F) :- fluent(F).

entails_init(W,  F    )          :-    holds(0, 0, W, F), possible_world(0, 0, W), initial_fluent_set(W), fluent(F), init_sf(F).
entails_init(W, -F    )          :-not holds(0, 0, W, F), possible_world(0, 0, W), initial_fluent_set(W), fluent(F), init_sf(F).

entails_init(W, neg(F))          :-not entails_init(W, F ),                      initial_fluent_set(W), init_sf(neg(F)),      init_sf(F).
entails_init(W, and(F1, F2))     :-    entails_init(W, F1), entails_init(W, F2), initial_fluent_set(W), init_sf(and(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2))     :-    entails_init(W, F1),                      initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2))     :-                         entails_init(W, F2), initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails(0, 0, W,  F    )         :-    holds(0, 0, W, F), possible_world(0, 0, W), initial_fluent_set(W), fluent(F).
entails(0, 0, W, -F    )         :-not holds(0, 0, W, F), possible_world(0, 0, W), initial_fluent_set(W), fluent(F).


entails(0, 0, W, neg(F))         :-not entails(0, 0, W, F ),                       initial_fluent_set(W), formula(neg(F)),      formula(F)              , possible_world(0, 0, W).
entails(0, 0, W, and(F1, F2))    :-    entails(0, 0, W, F1), entails(0, 0, W, F2), initial_fluent_set(W), formula(and(F1, F2)), formula(F1), formula(F2), possible_world(0, 0, W).
entails(0, 0, W,  or(F1, F2))    :-    entails(0, 0, W, F1),                       initial_fluent_set(W), formula( or(F1, F2)), formula(F1), formula(F2), possible_world(0, 0, W).
entails(0, 0, W,  or(F1, F2))    :-                          entails(0, 0, W, F2), initial_fluent_set(W), formula( or(F1, F2)), formula(F1), formula(F2), possible_world(0, 0, W).

not_entails(0, 0, P1, b(AG , F)) :-not     entails(0, 0, P2, F        ), believes(0, 0, P1, 0, 0, P2, AG), possible_world(0, 0, P1), possible_world(0, 0, P2), initial_fluent_set(P1), initial_fluent_set(P2), formula(b(AG , F)), formula(F), agent(AG).
entails    (0, 0, P , b(AG , F)) :-not not_entails(0, 0, P , b(AG , F)),                                   possible_world(0, 0, P ), formula(b(AG , F)), possible_world(0, 0, P), initial_fluent_set(P), agent(AG).

not_entails(0, 0, P1, c(AGS, F)) :-not     entails(0, 0, P2, F        ), reaches(0, 0, P1, 0, 0, P2, AGS), possible_world(0, 0, P1), possible_world(0, 0, P2), initial_fluent_set(P1), initial_fluent_set(P2), formula(c(AGS, F)), formula(F), agent_set(AGS).
entails    (0, 0, P , c(AGS, F)) :-not not_entails(0, 0, P , c(AGS, F)),                                   possible_world(0, 0, P ), formula(c(AGS, F)), possible_world(0, 0, P), initial_fluent_set(P), agent_set(AGS).

#program step(t).

% entails/4: P entails the formula F
entails(t, R, P,  F    )      :- t > 0,     holds(t, R, P, F),                       possible_world(t, R, P), %*fluent_set(W),*% time(t), fluent(F).
entails(t, R, P, -F    )      :- t > 0, not holds(t, R, P, F),                       possible_world(t, R, P), %*fluent_set(W),*% time(t), fluent(F).

entails(t, R, P, neg(F))      :- t > 0, not entails(t, R, P, F ),                       possible_world(t, R, P), %*fluent_set(W),*% formula(neg(F)),      formula(F), time(t).
entails(t, R, P, and(F1, F2)) :- t > 0,     entails(t, R, P, F1), entails(t, R, P, F2), possible_world(t, R, P), %*fluent_set(W),*% formula(and(F1, F2)), formula(F1), formula(F2), time(t).
entails(t, R, P,  or(F1, F2)) :- t > 0,     entails(t, R, P, F1),                       possible_world(t, R, P), %*fluent_set(W),*% formula( or(F1, F2)), formula(F1), formula(F2), time(t).
entails(t, R, P,  or(F1, F2)) :- t > 0,                           entails(t, R, P, F2), possible_world(t, R, P), %*fluent_set(W),*% formula( or(F1, F2)), formula(F1), formula(F2), time(t).

not_entails(t, R1, P1, b(AG , F)) :- t > 0, not     entails(T2, R2, P2, F        ), believes(t, R1, P1, T2, R2, P2, AG), possible_world(t, R1, P1), %*fluent_set(W),*% possible_world(T2, R2, P2), %*fluent_set(W2),*% formula(b(AG , F)), formula(F), time(t), time(T2), T2 <= t, agent(AG).
entails    (t, R , P , b(AG , F)) :- t > 0, not not_entails(t , R , P , b(AG , F)),                                      possible_world(t, R , P ), %*fluent_set(W),*% formula(b(AG , F)), time(t), agent(AG).

not_entails(t, R1, P1, c(AGS, F)) :- t > 0, not     entails(T2, R2, P2, F        ), reaches(t, R1, P1, T2, R2, P2, AGS), possible_world(t, R1, P1), %*fluent_set(W),*% possible_world(T2, R2, P2), %*fluent_set(W2),*% formula(c(AGS, F)), formula(F), time(t), time(T2), T2 <= t, agent_set(AGS).
entails    (t, R , P , c(AGS, F)) :- t > 0, not not_entails(t , R , P , c(AGS, F)),                                      possible_world(t, R , P ), %*fluent_set(W),*% formula(c(AGS, F)), time(t), agent_set(AGS).


#program base.

% ********** INITIAL STATE **********

%%%CORREZIONE AGOSTINO unknown_initially(F)            :- not initially(c(all_agents, F)), not initially(c(all_agents, -F)), fluent(F).
known_initially(F) :-
       fluent(F), initially(c(all_agents, F)).
known_initially(F) :-
       fluent(F), initially(c(all_agents, -F)).
unknown_initially(F) :-
       fluent(F), not known_initially(F).

initial_state_dimension(2 ** K) :- K = #count{ F : fluent(F), unknown_initially(F) }.
initial_fluent_set(1..K)        :- initial_state_dimension(K).


%%%%%CORREZIONE ALE
not_initial_pointed(W) :- good_initial_fluent_set(W), initially( F), not holds(0, 0, W, F), fluent(F).
not_initial_pointed(W) :- good_initial_fluent_set(W), initially(-F),     holds(0, 0, W, F), fluent(F).

pointed(0, 0, W) :- good_initial_fluent_set(W), not not_initial_pointed(W).


    holds(0, 0, W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F), possible_world(0, 0, W).
not holds(0, 0, W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F), possible_world(0, 0, W).

K/2 {     holds(0, 0, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
%%%CORREZIONE AGO -SUPERFLUO K/2 { not holds(0, 0, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).


%%%CORREZIONE AGO
%:- K1 = { holds(0, 0, W1, F) : not holds(0, 0, W2, F) }, K2 = { holds(0, 0, W2, F) : not holds(0, 0, W1, F), fluent(F) }, % :- K1 = { holds(W1, F) : not holds(W2, F) }, F) : not holds(W1, F), fluent(F) },
%   initial_fluent_set(W1), initial_fluent_set(W2), W1 != W2, K1 + K2 = 0.
:- not diffzero(W1, W2),
   initial_fluent_set(W1), initial_fluent_set(W2), W1 < W2.
%%% , K1 + K2 = 0.

diffzero(W1,W2) :- holds(0, 0, W1, F), not holds(0, 0, W2, F), fluent(F),
    initial_fluent_set(W1), initial_fluent_set(W2), W1 < W2, possible_world(0, 0, W1), possible_world(0, 0, W2).
diffzero(W1,W2) :- holds(0, 0, W2, F) , not holds(0, 0, W1, F), fluent(F),
    initial_fluent_set(W1), initial_fluent_set(W2), W1 < W2, possible_world(0, 0, W1), possible_world(0, 0, W2).

%%%CORREZIONE AGO - SUPERFLUO
%:- MF1 = #max { F : fluent(F), unknown_initially(F),     holds(0, 0, W1, F), not holds(0, 0, W2, F) },
%   MF2 = #max { F : fluent(F), unknown_initially(F), not holds(0, 0, W1, F),     holds(0, 0, W2, F) },
%   good_initial_fluent_set(W1), good_initial_fluent_set(W2), W1 < W2, MF1 > MF2.

good_initial_fluent_set(W) :- initial_fluent_set(W), not bad_ini_w(W).%, 0 { initially(c(all_agents, F)) : not entails_init(W, F), initially(c(all_agents, F)), init_sf(F), not literal(F) } 0.
bad_ini_w(W):- not entails_init(W, F), initially(c(all_agents, F)), init_sf(F), not literal(F), initial_fluent_set(W).

possible_world(0, 0, W) :- good_initial_fluent_set(W).
%fluent_set(W) :- possible_world(0, 0, W).

not_believes_initially(P1, P2, AG) :- not entails_init(P1, F),     entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F), agent(AG).
not_believes_initially(P1, P2, AG) :-     entails_init(P1, F), not entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F), agent(AG).

believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1), possible_world(0, 0, P2), %*fluent_set(P1), fluent_set(P2),*% agent(AG).


% ********** TRANSITION FUNCTION **********

#program step(t).

% is_executable/2: the action ACT is executable at time t
is_executable(t, ACT) :- t > 0, plan(t, ACT), action(ACT), pointed(t-1, R, P), executable(ACT, EXEC), entails(t-1, R, P, EXEC), formula(EXEC), time(t).
is_executable(t, ACT) :- t > 0, plan(t, ACT), action(ACT), pointed(t-1, R, P), executable(ACT, true), time(t).

% is_executable_effect/6: the effect CAUSED/SENSED/ANN of the action ACT is executable at time t
is_executable_effect(t, ACT, t-1, R , P , CAUSED) :- t > 0,  plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, true ), pointed(t-1, R, P), formula(CAUSED), time(t).
is_executable_effect(t, ACT, t-1, R , P , CAUSED) :- t > 0,  plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), entails(t-1, R, P, EEXEC), pointed(t-1, R, P), formula(CAUSED), formula(EEXEC), time(t).

is_executable_effect(t, ACT, T2, R2, P2, CAUSED) :- t > 0,  plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, true ), possible_world(T2, R2, P2), T2 < t,
                                                    pointed(t-1, R, PP), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS), formula(CAUSED), agent_set(AGS), time(t), time(T2).
is_executable_effect(t, ACT, T2, R2, P2, CAUSED) :- t > 0, plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2), T2 < t, formula(EEXEC),
                                                    pointed(t-1, R, PP), entails(T2, R2, P2, EEXEC), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS), formula(CAUSED), agent_set(AGS), time(t), time(T2).


%%%%STILL TO DEBUG%%%%%%%%%%
is_executable_effect(t, ACT, t-1, R , P , SENSED) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), sensing(ACT), determines(ACT, SENSED, true ), pointed(t-1, R, P).
is_executable_effect(t, ACT, t-1, R , P , SENSED) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), sensing(ACT), determines(ACT, SENSED, EEXEC), entails(t-1, R, P, EEXEC), formula(EEXEC), pointed(t-1, R, P).

is_executable_effect(t, ACT, t-1, R , P , ANN   ) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), announcement(ACT), announces(ACT, ANN, true ), pointed(t-1, R, P).
is_executable_effect(t, ACT, t-1, R , P , ANN   ) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), announcement(ACT), announces(ACT, ANN, EEXEC), entails(t-1, R, P, EEXEC), formula(EEXEC), pointed(t-1, R, P).

% For sensing/announcement actions: let P2 be a possibility that is reachable from the pointed one with a path labeled only with fully or partially observant agents.
% Then the effect SENSED/ANN is executable in P2 if P2 entails its executability condition EEXEC
is_executable_effect(t, ACT, T2, R2, P2, SENSED) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), sensing(ACT), determines(ACT, SENSED, true), possible_world(T2, R2, P2), T2 < t, fluent(SENSED),
                                                    pointed(t-1, R, PP),                             not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).

is_executable_effect(t, ACT, T2, R2, P2, SENSED) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), sensing(ACT), determines(ACT, SENSED, EEXEC), possible_world(T2, R2, P2), T2 < t, formula(EEXEC), fluent(SENSED),
                                                    pointed(t-1, R, PP), entails(T2, R2, P2, EEXEC), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).

is_executable_effect(t, ACT, T2, R2, P2, ANN   ) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), announcement(ACT), announces(ACT, ANN, true), possible_world(T2, R2, P2), T2 < t,
                                                    pointed(t-1, R, PP),                             not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).

is_executable_effect(t, ACT, T2, R2, P2, ANN   ) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), announcement(ACT), announces(ACT, ANN, EEXEC), possible_world(T2, R2, P2), T2 < t, formula(EEXEC),
                                                    pointed(t-1, R, PP), entails(T2, R2, P2, EEXEC), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).
%%%%%%%%%%%%%%



is_fully_obs  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT), observes (ACT, AG, true), agent(AG).
is_partial_obs(t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT), aware_of (ACT, AG, true), agent(AG).
is_oblivious  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT), oblivious(ACT, AG),       agent(AG).

is_fully_obs  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), observes (ACT, AG, OBS ), agent(AG),     entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).
is_partial_obs(t, AG) :- t > 0, plan(t, ACT), time(t), soa(ACT),      aware_of (ACT, AG, OBS ), agent(AG),     entails(t-1, R, PP, OBS), formula(OBS), pointed(t-1, R, PP).

is_oblivious  (t, AG) :- t > 0, plan(t, ACT), time(t), soa(ACT),    aware_of (ACT, AG, OBS ), agent(AG), not entails(t-1, R, PP, OBS), formula(OBS), pointed(t-1, R, PP).
is_oblivious  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT),       observes (ACT, AG, OBS ), agent(AG), not entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).

oblivious_ags(t, AGS)         :- t > 0, plan(t, ACT), action(ACT), time(t), agent(AG), agent_set(AGS), contains_ag(AGS, AG), is_oblivious(t, AG).
not_oblivious_ags(t, AGS)     :- t > 0, plan(t, ACT), action(ACT), time(t), not oblivious_ags(t, AGS), agent_set(AGS), time(t). % 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0

not_all_fully_obs_ags(t, AGS) :- t > 0, plan(t, ACT), action(ACT), time(t), agent(AG), agent_set(AGS), contains_ag(AGS, AG), not is_fully_obs(t, AG).
all_fully_obs_ags(t, AGS)     :- t > 0, plan(t, ACT), action(ACT), time(t), not not_all_fully_obs_ags(t, AGS), agent_set(AGS), time(t).

%#program base.
% The maximum repetition at time t is the repetition of the pointed possibility
%CORREZIONE ALE - Rimossa max_rep(0, 0).

%#program step(t).
%max_rep(t, R) :- t > 0, pointed(t, R, P), time(t), plan(t, ACT).


% ONTIC ACTIONS
pointed(t, 2*MR+1, phi(P)) :- t > 0,  plan(t, ACT), ontic(ACT), pointed(t-1, R, P), possible_world(t, 2*MR+1, phi(P)), %*fluent_set(W),*% time(t)%*, bound(2*MR+1)*%.

possible_world(t, R+MR+1, phi(P)) :- t > 0,  plan(t, ACT), ontic(ACT), pointed(t-1, MR, PP), possible_world(t-1, R , P), %*fluent_set(W),*% time(t)%*, bound(R+MR+1)*%. %,

possible_world(t, R2+MR+1, phi(P)) :- t > 0, plan(t, ACT), time(t), time(T2), ontic(ACT), pointed(t-1, MR, PP), possible_world(T2, R2, P), %*fluent_set(W),*% T2 < t,
                                              reaches(t-1, MR, PP, T2, R2, P, AGS), not_oblivious_ags(t, AGS), agent_set(AGS)%*, bound(R2+MR+1)*%.
%pred_test(t, AGS) :- agent_set(AGS), agent(AG), contains_ag(AGS, AG), is_oblivious(t, AG), time(t).

%fluent_set(W) :- t > 0, possible_world(t, R, P), time(t).

    holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), pointed(t-1, MR, PP),
                                            is_executable_effect(t, ACT, T2, R2, P,  F), fluent(F), %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).
not holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), pointed(t-1, MR, PP),
                                            is_executable_effect(t, ACT, T2, R2, P, -F), fluent(F),  %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).
    holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), pointed(t-1, MR, PP),
                                            not is_executable_effect(t, ACT, T2, R2, P, F), not is_executable_effect(t, ACT, T2, R2, P, -F),
                                            holds(T2, R2, P, F), fluent(F),  %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).
not holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), pointed(t-1, MR, PP),
                                            not is_executable_effect(t, ACT, T2, R2, P, F), not is_executable_effect(t, ACT, T2, R2, P, -F),
                                            not holds(T2, R2, P, F), fluent(F),  %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).

believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0, plan(t, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), agent(AG), T1 < t, T2 < t, T2 <= T1,
                                                           is_fully_obs(t, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)), pointed(t-1, MR, PP), time(t), time(T1), time(T2).

believes(t, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- t > 0, plan(t, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), agent(AG), T1 < t, T2 < t, T2 <= T1,
                                                           is_oblivious(t, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), pointed(t-1, MR, PP), time(t), time(T1), time(T2).

% SENSING/ANNOUNCEMENT ACTIONS

% hold_soa_effects/4: the possibility P2 agrees with the pointed possibility on the SENSED fluents of the sensing action ACT performed at time t
% hold_soa_effects/4: the possibility P2 agrees with the pointed possibility on the ANN formulas of the announcement action ACT performed at time t
not_hold_soa_effects(T2 , R2, P2, t) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), sensing(ACT), possible_world(t-1, R, PP), possible_world(T2, R2, P2), T2 < t,
                                        is_executable_effect(t, ACT, T2, R2, P2, SENSED), determines(ACT, SENSED, _),     holds(T2, R2, P2, SENSED), not holds(t-1, R, PP, SENSED),
                                        pointed(t-1, R, PP), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).

not_hold_soa_effects(T2 , R2, P2, t) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), sensing(ACT), possible_world(t-1, R, PP), possible_world(T2, R2, P2), T2 < t,
                                        is_executable_effect(t, ACT, T2, R2, P2, SENSED), determines(ACT, SENSED, _), not holds(T2, R2, P2, SENSED),     holds(t-1, R, PP, SENSED),
                                        pointed(t-1, R, PP), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).

not_hold_soa_effects(T2 , R2, P2, t) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), announcement(ACT), possible_world(t-1, R, PP), possible_world(T2, R2, P2), T2 < t,
                                        is_executable_effect(t, ACT, T2, R2, P2, ANN), announces(ACT, ANN, _),     entails(T2, R2, P2, ANN), not entails(t-1, R, PP, ANN),
                                        pointed(t-1, R, PP), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).

not_hold_soa_effects(T2 , R2, P2, t) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), announcement(ACT), possible_world(t-1, R, PP), possible_world(T2, R2, P2), T2 < t,
                                        is_executable_effect(t, ACT, T2, R2, P2, ANN), announces(ACT, ANN, _), not entails(T2, R2, P2, ANN),     entails(t-1, R, PP, ANN),
                                        pointed(t-1, R, PP), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).

hold_soa_effects(t-1, R , P , t) :- t > 0, plan(t, ACT), time(t), time(t-1), soa(ACT), pointed(t-1, R, P).

hold_soa_effects(T2 , R2, P2, t) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), soa(ACT), possible_world(t-1, R, PP), possible_world(T2, R2, P2), T2 < t,
                                    not_hold_soa_effects(T2, R2, P2, t), pointed(t-1, R, PP), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS).


pointed(t, 2*MR+1, phi(P)) :- t > 0, plan(t, ACT), time(t), time(t-1), soa(ACT), pointed(t-1, MR, P), possible_world(t, 2*MR+1, phi(P)).

% We explicitly generate an updated pointed possibility since it could be the case that no agent believes it at time t
possible_world(t, R +MR+1, phi(P)) :- t > 0, plan(t, ACT), time(t), time(t-1), soa(ACT), pointed(t-1, MR, PP), possible_world(t-1, R, P).

% good :- is_executable(2, peek_a), sensing(peek_a), pointed(1, 1, phi(2)).
% #show good/0.

% If a fully observant agent AG believes P2 and P2 agrees with the pointed possibility on the SENSED fluents, then we create the new possibility phi(P2)
% possible_world(t+1, R2+MR+1, phi(P)) :- is_executable(t, ACT), sensing(ACT), pointed(t, R, PP), possible_world(T2, R2, P), max_rep(t, MR),
%                                             believes(t, R, PP, T2, R2, P, AG), hold_sensed_effects(T2, R2, P, t), is_fully_obs(t+1, AG).

% As for ontic actions, we do the same for all the possibilities P2 that are reachable by fully-observant-labeled paths
possible_world(t, R2+MR+1, phi(P)) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), soa(ACT), pointed(t-1, MR, PP), possible_world(T2, R2, P), T2 < t,
                                          reaches(t-1, MR, PP, T2, R2, P, AGS), all_fully_obs_ags(t, AGS),
                                          hold_soa_effects(T2, R2, P, t).

% If a partially observant agent AG believes P2, then we create the new possibility phi(P2)
possible_world(t, R2+MR+1, phi(P)) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), soa(ACT), pointed(t-1, MR, PP), possible_world(T2, R2, P), T2 < t,
                                          believes(t-1, MR, PP, T2, R2, P, AG), is_partial_obs(t, AG).

% Let P be a possibility that is reachable from the pointed one with a path labeled only with fully/partially observant agents.
% Then we create the new possibility phi(P).

% !!!
% PER PROF: la regola qui sotto può generare mondi superflui (ovvero non raggiungibili dal mondo puntato). Si provi a stampare il grafico dell'istanza
% Coin_in_the_Box__pl_6.lp con piano: plan(0, signal_a_b). plan(1, distract_b_a). plan(2, signal_b_c). plan(3, open_a). plan(4, peek_b). plan(5, peek_c).
% Le ultime due azioni generano due nuove possibility ridondanti: "5_31_ppppp2" e "6_60_pppp1".
% !!!
possible_world(t, R2+MR+1, phi(P)) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T1), time(T2), soa(ACT), pointed(t-1, MR, PP), possible_world(T1, R1, P1), possible_world(T2, R2, P), T1 < t, T2 <= T1,
                                          believes(t-1, MR, PP, T1, R1, P1, AG), is_partial_obs(t, AG), reaches(T1, R1, P1, T2, R2, P, AGS), not_oblivious_ags(t, AGS).

% Inertia: since holds depends both on a fluent set W AND a possibility P we must explicitly "copy" in phi(P) the fluents that hold in W
holds(t, R2+MR+1, phi(P), F) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T2), soa(ACT), possible_world(t, R2+MR+1, phi(P)), possible_world(T2, R2, P), T2 < t,
                                   holds(T2, R2, P, F), fluent(F), %*fluent_set(W),*% pointed(t-1, MR, PP).


% Fully observant agents (sigma, sigma): if P1 -- AG --> P2 at time t and AG is fully observant and BOTH P1 and P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T1), time(T2), soa(ACT), T1 < t, T2 < t, T2 <= T1, is_fully_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)),
                                                           hold_soa_effects(T1, R1, P1, t), hold_soa_effects(T2, R2, P2, t), pointed(t-1, MR, PP).

% Fully observant agents (tau, tau): if P1 -- AG --> P2 at time t and AG is fully observant and NEITHER P1 nor P2 agree with the pointed possibility, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T1), time(T2), soa(ACT), T1 < t, T2 < t, T2 <= T1, is_fully_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)),
                                                           not hold_soa_effects(T1, R1, P1, t), not hold_soa_effects(T2, R2, P2, t), pointed(t-1, MR, PP).

% Partially observant agents: if P1 -- AG --> P2 at time t and AG is partially observant, then phi(P1) -- AG --> phi(P2) at time t+1
believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T1), time(T2), soa(ACT), T1 < t, T2 < t, T2 <= T1, is_partial_obs(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), possible_world(t, R2+MR+1, phi(P2)), pointed(t-1, MR, PP).

% Oblivious agents: if P1 -- AG --> P2 at time t and AG is oblivious, then phi(P1) -- AG --> P2 at time t+1
believes(t, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- t > 0, plan(t, ACT), time(t), time(t-1), time(T1), time(T2), soa(ACT), T1 < t, T2 < t, T2 <= T1, is_oblivious(t, AG),
                                                           believes(T1, R1, P1, T2, R2, P2, AG), possible_world(T1, R1, P1), possible_world(T2, R2, P2),
                                                           possible_world(t, R1+MR+1, phi(P1)), pointed(t-1, MR, PP).

% Generate
#program step(t).
1 { plan(t, ACT) : action(ACT) } 1 :- t > 0, time(t).
%0 { is_executable(t, ACT) : action(ACT) } 1.



#program base.
plan(0, null).
plan(1, signal_a_b).
plan(2, open_a).
plan(3, peek_b).
reached_goal(3).

not_reached_goal(0) :- not entails(0, 0, P, F), pointed(0, 0, P), goal(F).
reached_goal(0)     :- not not_reached_goal(0).

#program step(t).
:- plan(t, null), t > 0.
%:- 0{ plan(t-1, _) } 0.

not_reached_goal(t) :- t > 0, not entails(t, R, P, F), pointed(t, R, P), goal(F), time(t), formula(F).
reached_goal(t)     :- t > 0, plan(t, ACT), not not_reached_goal(t), time(t).
:- t > 0, plan(t, ACT), not is_executable(t, ACT), time(t).
% Test
% :- t > 0, plan(t, ACT), is
% :- is_executable(t, ACT), 0 { pointed(t-1, R, P) } 0, time(t).
% :- t > 0, plan(t, ACT), not is_executable(t, ACT).
% :- t > 0, not plan(t, ACT), is_executable(t, ACT).
% :- t > 0, plan(t, ACT), pointed(t-1, R, P), not entails(t-1, R, P, EXEC), executable(ACT, EXEC), formula(EXEC).

% :- t > 0, plan(t, ACT), pointed(t-1, R, P), 0 { is_executable_effect(t, ACT, t-1, R, P, CAUSED) : causes(ACT, CAUSED, _) } 0, ontic(ACT).
% :- t > 0, possible_world(t, R, P), possible_world(t, R, P), W1 != W2.

% :- t > 0, reached_goal(t), plan(T2, ACT),    T2 > t.
% :- t > 0, reached_goal(t), reached_goal(T2), T2 > t.

% :- time(t), not plan(t, ACT), action(ACT), not reached_goal(t-1).
%:- pointed(t, R, P), pointed(T1, RR, PP), T1 < t, t > 0, time(t), time(T1),
%  0 { formula(F): entails(t, R, P, F), not entails(T1, RR, PP, F)} 0,
%  0 { formula(F): not entails(t, R, P, F), entails(T1, RR, PP, F)} 0.



#program check(t).

:- not reached_goal(t), query(t).


% Display
% !!! Uncomment while printing the results !!!
#show pointed/3.
#show possible_world/3.
#show believes/7.
#show holds/4.
#show fluent/1.

%#show is_executable/2.
%#show entails/4.

%#show is_fully_obs/2.
%#show fluent_set/1.
#show plan/2.
#show reached_goal/1.
%#show not_reached_goal/1.

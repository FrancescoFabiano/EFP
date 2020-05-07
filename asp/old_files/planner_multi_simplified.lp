#include <incmode>.

#program base.
time(0).

reaches(0, 0, P1, 0, 0, P2, AGS) :-
    believes(0, 0, P1, 0, 0, P2, AG), contains_ag(AGS, AG),
    possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), agent(AG), agent_set(AGS).

reaches(0, 0, P1, 0, 0, P2, AGS) :-
    believes(0, 0, P1, 0, 0, P3, AG), contains_ag(AGS, AG), reaches(0, 0, P3, 0, 0, P2, AGS),
    possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), possible_world(0, 0, P3, P3), agent(AG), agent_set(AGS).

#program step(t).

time(t).
% reaches/7: P1 reaches P2 with a path that contains only agents inside AGS
reaches(t, R1, P1, T2, R2, P2, AGS) :-
    t > 0, time(t), time(T2), T2 <= t, believes(t, R1, P1, T2, R2, P2, AG),
    contains_ag(AGS, AG), agent(AG), agent_set(AGS), possible_world(t, R1, P1, _), possible_world(T2, R2, P2, _).

reaches(t, R1, P1, T2, R2, P2, AGS) :-
    t > 0, time(t), time(T2), time(T3), T3 <= t, T2 <= T3, believes(t, R1, P1, T3, R3, P3, AG),
    contains_ag(AGS, AG), reaches(T3, R3, P3, T2, R2, P2, AGS), agent(AG), agent_set(AGS),
    possible_world(t, R1, P1, _), possible_world(T2, R2, P2, _), possible_world(T3, R3, P3, _).

% ********** ENTAILMENT **********

#program base.

%#const b_test = 150.
%bound(0..b_test).

literal( F) :- fluent(F).
literal(-F) :- fluent(F).

entails_init(W,  F    )          :-    holds(0, 0, W, W, F), possible_world(0, 0, W, W), initial_fluent_set(W), fluent(F), init_sf(F).
entails_init(W, -F    )          :-not holds(0, 0, W, W, F), possible_world(0, 0, W, W), initial_fluent_set(W), fluent(F), init_sf(F).

entails_init(W, neg(F))          :-not entails_init(W, F ),                      initial_fluent_set(W), init_sf(neg(F)),      init_sf(F).
entails_init(W, and(F1, F2))     :-    entails_init(W, F1), entails_init(W, F2), initial_fluent_set(W), init_sf(and(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2))     :-    entails_init(W, F1),                      initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails_init(W,  or(F1, F2))     :-                         entails_init(W, F2), initial_fluent_set(W), init_sf( or(F1, F2)), init_sf(F1), init_sf(F2).
entails(0, 0, W,  F    )         :-    holds(0, 0, W, W, F), possible_world(0, 0, W, W), initial_fluent_set(W), fluent(F).
entails(0, 0, W, -F    )         :-not holds(0, 0, W, W, F), possible_world(0, 0, W, W), initial_fluent_set(W), fluent(F).


entails(0, 0, W, neg(F))         :-not entails(0, 0, W, F ),                       initial_fluent_set(W), formula(neg(F)),      formula(F)              , possible_world(0, 0, W, W).
entails(0, 0, W, and(F1, F2))    :-    entails(0, 0, W, F1), entails(0, 0, W, F2), initial_fluent_set(W), formula(and(F1, F2)), formula(F1), formula(F2), possible_world(0, 0, W, W).
entails(0, 0, W,  or(F1, F2))    :-    entails(0, 0, W, F1),                       initial_fluent_set(W), formula( or(F1, F2)), formula(F1), formula(F2), possible_world(0, 0, W, W).
entails(0, 0, W,  or(F1, F2))    :-                          entails(0, 0, W, F2), initial_fluent_set(W), formula( or(F1, F2)), formula(F1), formula(F2), possible_world(0, 0, W, W).

not_entails(0, 0, P1, b(AG , F)) :-not     entails(0, 0, P2, F        ), believes(0, 0, P1, 0, 0, P2, AG), possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), initial_fluent_set(P1), initial_fluent_set(P2), formula(b(AG , F)), formula(F), agent(AG).
entails    (0, 0, P , b(AG , F)) :-not not_entails(0, 0, P , b(AG , F)),                                   possible_world(0, 0, P , P ), formula(b(AG , F)), possible_world(0, 0, P, P), initial_fluent_set(P), agent(AG).

not_entails(0, 0, P1, c(AGS, F)) :-not     entails(0, 0, P2, F        ), reaches(0, 0, P1, 0, 0, P2, AGS), possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), initial_fluent_set(P1), initial_fluent_set(P2), formula(c(AGS, F)), formula(F), agent_set(AGS).
entails    (0, 0, P , c(AGS, F)) :-not not_entails(0, 0, P , c(AGS, F)),                                   possible_world(0, 0, P , P ), formula(c(AGS, F)), possible_world(0, 0, P, P), initial_fluent_set(P), agent_set(AGS).

#program step(t).

% entails/4: P entails the formula F
entails(t, R, P,  F    )      :- t > 0,     holds(t, R, P, W, F),                       possible_world(t, R, P, W), %*fluent_set(W),*% time(t), fluent(F).
entails(t, R, P, -F    )      :- t > 0, not holds(t, R, P, W, F),                       possible_world(t, R, P, W), %*fluent_set(W),*% time(t), fluent(F).

entails(t, R, P, neg(F))      :- t > 0, not entails(t, R, P, F ),                       possible_world(t, R, P, W), %*fluent_set(W),*% formula(neg(F)),      formula(F), time(t).
entails(t, R, P, and(F1, F2)) :- t > 0,     entails(t, R, P, F1), entails(t, R, P, F2), possible_world(t, R, P, W), %*fluent_set(W),*% formula(and(F1, F2)), formula(F1), formula(F2), time(t).
entails(t, R, P,  or(F1, F2)) :- t > 0,     entails(t, R, P, F1),                       possible_world(t, R, P, W), %*fluent_set(W),*% formula( or(F1, F2)), formula(F1), formula(F2), time(t).
entails(t, R, P,  or(F1, F2)) :- t > 0,                           entails(t, R, P, F2), possible_world(t, R, P, W), %*fluent_set(W),*% formula( or(F1, F2)), formula(F1), formula(F2), time(t).

not_entails(t, R1, P1, b(AG , F)) :- t > 0, not     entails(T2, R2, P2, F        ), believes(t, R1, P1, T2, R2, P2, AG), possible_world(t, R1, P1, W), %*fluent_set(W),*% possible_world(T2, R2, P2, W2), %*fluent_set(W2),*% formula(b(AG , F)), formula(F), time(t), time(T2), T2 <= t, agent(AG).
entails    (t, R , P , b(AG , F)) :- t > 0, not not_entails(t , R , P , b(AG , F)),                                      possible_world(t, R , P , W), %*fluent_set(W),*% formula(b(AG , F)), time(t), agent(AG).

not_entails(t, R1, P1, c(AGS, F)) :- t > 0, not     entails(T2, R2, P2, F        ), reaches(t, R1, P1, T2, R2, P2, AGS), possible_world(t, R1, P1, W), %*fluent_set(W),*% possible_world(T2, R2, P2, W2), %*fluent_set(W2),*% formula(c(AGS, F)), formula(F), time(t), time(T2), T2 <= t, agent_set(AGS).
entails    (t, R , P , c(AGS, F)) :- t > 0, not not_entails(t , R , P , c(AGS, F)),                                      possible_world(t, R , P , W), %*fluent_set(W),*% formula(c(AGS, F)), time(t), agent_set(AGS).


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
not_initial_pointed(W) :- good_initial_fluent_set(W), initially( F), not holds(0, 0, W, W, F), fluent(F).
not_initial_pointed(W) :- good_initial_fluent_set(W), initially(-F),     holds(0, 0, W, W, F), fluent(F).

pointed(0, 0, W) :- good_initial_fluent_set(W), not not_initial_pointed(W).


    holds(0, 0, W, W, F) :- initially(c(all_agents,  F)), initial_fluent_set(W), fluent(F), possible_world(0, 0, W, W).
not holds(0, 0, W, W, F) :- initially(c(all_agents, -F)), initial_fluent_set(W), fluent(F), possible_world(0, 0, W, W).

K/2 {     holds(0, 0, W, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).
%%%CORREZIONE AGO -SUPERFLUO K/2 { not holds(0, 0, W, W, F) : initial_fluent_set(W) } K/2 :- unknown_initially(F), initial_state_dimension(K).


%%%CORREZIONE AGO
%:- K1 = { holds(0, 0, W1, W1, F) : not holds(0, 0, W2, W2, F) }, K2 = { holds(0, 0, W2, W2, F) : not holds(0, 0, W1, W1, F), fluent(F) }, % :- K1 = { holds(W1, F) : not holds(W2, F) }, K2 = { holds(W2, F) : not holds(W1, F), fluent(F) },
%   initial_fluent_set(W1), initial_fluent_set(W2), W1 != W2, K1 + K2 = 0.
:- not diffzero(W1, W2),
   initial_fluent_set(W1), initial_fluent_set(W2), W1 < W2.
%%% , K1 + K2 = 0.

diffzero(W1,W2) :- holds(0, 0, W1, W1, F), not holds(0, 0, W2, W2, F), fluent(F),
    initial_fluent_set(W1), initial_fluent_set(W2), W1 < W2, possible_world(0, 0, W1, W1), possible_world(0, 0, W2, W2).
diffzero(W1,W2) :- holds(0, 0, W2, W2, F) , not holds(0, 0, W1, W1, F), fluent(F),
    initial_fluent_set(W1), initial_fluent_set(W2), W1 < W2, possible_world(0, 0, W1, W1), possible_world(0, 0, W2, W2).

%%%CORREZIONE AGO - SUPERFLUO
%:- MF1 = #max { F : fluent(F), unknown_initially(F),     holds(0, 0, W1, W1, F), not holds(0, 0, W2, W2, F) },
%   MF2 = #max { F : fluent(F), unknown_initially(F), not holds(0, 0, W1, W1, F),     holds(0, 0, W2, W2, F) },
%   good_initial_fluent_set(W1), good_initial_fluent_set(W2), W1 < W2, MF1 > MF2.

good_initial_fluent_set(W) :- initial_fluent_set(W), not bad_ini_w(W).%, 0 { initially(c(all_agents, F)) : not entails_init(W, F), initially(c(all_agents, F)), init_sf(F), not literal(F) } 0.
bad_ini_w(W):- not entails_init(W, F), initially(c(all_agents, F)), init_sf(F), not literal(F), initial_fluent_set(W).

possible_world(0, 0, W, W) :- good_initial_fluent_set(W).
%fluent_set(W) :- possible_world(0, 0, W, W).

not_believes_initially(P1, P2, AG) :- not entails_init(P1, F),     entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F), agent(AG).
not_believes_initially(P1, P2, AG) :-     entails_init(P1, F), not entails_init(P2, F), good_initial_fluent_set(P1), good_initial_fluent_set(P2), P1 != P2, initially(c(all_agents, or(b(AG, F), b(AG, -F)))), formula(F), agent(AG).

believes(0, 0, P1, 0, 0, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_world(0, 0, P1, P1), possible_world(0, 0, P2, P2), %*fluent_set(P1), fluent_set(P2),*% agent(AG).


% ********** TRANSITION FUNCTION **********

#program step(t).

% is_executable/2: the action ACT is executable at time t
is_executable(t, ACT) :- t > 0, plan(t, ACT), action(ACT), pointed(t-1, R, P), executable(ACT, EXEC), entails(t-1, R, P, EXEC), formula(EXEC), time(t).
is_executable(t, ACT) :- t > 0, plan(t, ACT), action(ACT), pointed(t-1, R, P), executable(ACT, true), time(t).

% is_executable_effect/6: the effect CAUSED/SENSED/ANN of the action ACT is executable at time t
is_executable_effect(t, ACT, t-1, R , P , CAUSED) :- t > 0,  plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, true ), pointed(t-1, R, P), formula(CAUSED), time(t).
is_executable_effect(t, ACT, t-1, R , P , CAUSED) :- t > 0,  plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), entails(t-1, R, P, EEXEC), pointed(t-1, R, P), formula(CAUSED), formula(EEXEC), time(t).

is_executable_effect(t, ACT, T2, R2, P2, CAUSED) :- t > 0,  plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, true ), possible_world(T2, R2, P2, W2), T2 < t,
                                                    pointed(t-1, R, PP), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS), formula(CAUSED), agent_set(AGS), time(t), time(T2).
is_executable_effect(t, ACT, T2, R2, P2, CAUSED) :- t > 0, plan(t, ACT), ontic(ACT), causes(ACT, CAUSED, EEXEC), possible_world(T2, R2, P2, W2), T2 < t, formula(EEXEC),
                                                    pointed(t-1, R, PP), entails(T2, R2, P2, EEXEC), not_oblivious_ags(t, AGS), reaches(t-1, R, PP, T2, R2, P2, AGS), formula(CAUSED), agent_set(AGS), time(t), time(T2).


is_fully_obs  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT),       observes (ACT, AG, true), agent(AG).
is_oblivious  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT),       oblivious(ACT, AG),       agent(AG).

is_fully_obs  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT),       observes (ACT, AG, OBS ), agent(AG),     entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).

is_oblivious  (t, AG) :- t > 0, plan(t, ACT), action(ACT), time(t), action(ACT),       observes (ACT, AG, OBS ), agent(AG), not entails(t-1, R, PS, OBS), formula(OBS), pointed(t-1, R, PS).

oblivious_ags(t, AGS)         :- t > 0, plan(t, ACT), action(ACT), time(t), agent(AG), agent_set(AGS), contains_ag(AGS, AG), is_oblivious(t, AG).
not_oblivious_ags(t, AGS)     :- t > 0, plan(t, ACT), action(ACT), time(t), not oblivious_ags(t, AGS), agent_set(AGS), time(t). % 0 { agent(AG) : contains_ag(AGS, AG), is_oblivious(t, AG) } 0

not_all_fully_obs_ags(t, AGS) :- t > 0, plan(t, ACT), action(ACT), time(t), agent(AG), agent_set(AGS), contains_ag(AGS, AG), not is_fully_obs(t, AG).
all_fully_obs_ags(t, AGS)     :- t > 0, plan(t, ACT), action(ACT), time(t), not not_all_fully_obs_ags(t, AGS), agent_set(AGS), time(t).

%#program base.
% The maximum repetition at time t is the repetition of the pointed possibility
%CORREZIONE ALE - Rimossa max_rep(0, 0).

#program step(t).
%max_rep(t, R) :- t > 0, pointed(t, R, P), time(t), plan(t, ACT).


% ONTIC ACTIONS
pointed(t, 2*MR+1, phi(P)) :- t > 0,  plan(t, ACT), ontic(ACT), pointed(t-1, R, P), possible_world(t, 2*MR+1, phi(P), W), %*fluent_set(W),*% time(t)%*, bound(2*MR+1)*%.

possible_world(t, R+MR+1, phi(P), phi(W)) :- t > 0,  plan(t, ACT), ontic(ACT), pointed(t-1, MR, PP), possible_world(t-1, R , P, W), %*fluent_set(W),*% time(t)%*, bound(R+MR+1)*%. %,

possible_world(t, R2+MR+1, phi(P), phi(W)) :- t > 0, plan(t, ACT), time(t), time(T2), ontic(ACT), pointed(t-1, MR, PP), possible_world(T2, R2, P, W), %*fluent_set(W),*% T2 < t,
                                              reaches(t-1, MR, PP, T2, R2, P, AGS), not_oblivious_ags(t, AGS), agent_set(AGS)%*, bound(R2+MR+1)*%.
%pred_test(t, AGS) :- agent_set(AGS), agent(AG), contains_ag(AGS, AG), is_oblivious(t, AG), time(t).

%fluent_set(W) :- t > 0, possible_world(t, R, P, W), time(t).

    holds(t, R2+MR+1, phi(P), phi(W), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), pointed(t-1, MR, PP),
                                            is_executable_effect(t, ACT, T2, R2, P,  F), fluent(F), %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).
not holds(t, R2+MR+1, phi(P), phi(W), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), pointed(t-1, MR, PP),
                                            is_executable_effect(t, ACT, T2, R2, P, -F), fluent(F),  %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).
    holds(t, R2+MR+1, phi(P), phi(W), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), pointed(t-1, MR, PP),
                                            not is_executable_effect(t, ACT, T2, R2, P, F), not is_executable_effect(t, ACT, T2, R2, P, -F),
                                            holds(T2, R2, P, W, F), fluent(F),  %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).
not holds(t, R2+MR+1, phi(P), phi(W), F) :- t > 0, plan(t, ACT), ontic(ACT), possible_world(t, R2+MR+1, phi(P), phi(W)), possible_world(T2, R2, P, W), pointed(t-1, MR, PP),
                                            not is_executable_effect(t, ACT, T2, R2, P, F), not is_executable_effect(t, ACT, T2, R2, P, -F),
                                            not holds(T2, R2, P, W, F), fluent(F),  %*fluent_set(W), fluent_set(phi(W)),*% T2 < t, time(t), time(T2).

believes(t, R1+MR+1, phi(P1), t , R2+MR+1, phi(P2), AG) :- t > 0, plan(t, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), agent(AG), T1 < t, T2 < t, T2 <= T1,
                                                           is_fully_obs(t, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                           possible_world(t, R1+MR+1, phi(P1), _), possible_world(t, R2+MR+1, phi(P2), _), pointed(t-1, MR, PP), time(t), time(T1), time(T2).

believes(t, R1+MR+1, phi(P1), T2, R2     ,     P2 , AG) :- t > 0, plan(t, ACT), ontic(ACT), believes(T1, R1, P1, T2, R2, P2, AG), agent(AG), T1 < t, T2 < t, T2 <= T1,
                                                           is_oblivious(t, AG), possible_world(T1, R1, P1, _), possible_world(T2, R2, P2, _),
                                                           possible_world(t, R1+MR+1, phi(P1), _), pointed(t-1, MR, PP), time(t), time(T1), time(T2).

% Generate
#program step(t).
1 { plan(t, ACT) : action(ACT) } 1 :- t > 0, time(t).
%0 { is_executable(t, ACT) : action(ACT) } 1.



#program base.
plan(0, null).

not_reached_goal(0) :- not entails(0, 0, P, F), pointed(0, 0, P), goal(F).
reached_goal(0)     :- not not_reached_goal(0).

#program step(t).
:- plan(t, null), t > 0.
%:- 0{ plan(t-1, _) } 0.

not_reached_goal(t) :- t > 0, not entails(t, R, P, F), pointed(t, R, P), goal(F), time(t), formula(F).
reached_goal(t)     :- t > 0, plan(t, ACT), not not_reached_goal(t), time(t).
:- t > 0, plan(t, ACT), not is_executable(t, ACT), time(t).

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
%#show fluent_set/1.
#show plan/2.
#show reached_goal/1.
%#show not_reached_goal/1.

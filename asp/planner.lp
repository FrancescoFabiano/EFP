%
% EMP encoding in ASP
%

% ********** REACHABILITY **********

reaches(S1, W1, S2, W2, AGS) :- edge(S1, W1, S2, W2, AG), contains_ag(AGS, AG).
reaches(S1, W1, S2, W2, AGS) :- edge(S1, W1, S3, W3, AG), contains_ag(AGS, AG), reaches(S3, W3, S2, W2, AGS).



% ********** ENTAILMENT **********

formula(F) :- fluent(F).        % A fluent is a formula
% formula(F) :- initially(F).     % An initial condition is a formula
% formula(F) :- goal(F).          % A goal condition is a formula

holds(W, true) :- world(W).
entails(S, F) :- entails(S, W, F), pointed(S, W), formula(F).

entails(S, W, true  )      :-                                           state(S), world(W).
entails(S, W, F     )      :-     holds(W, F),                          state(S), world(W), fluent(F).
% entails(S, W, neg(F))      :- not holds(W, F),                          state(S), world(W), fluent(F).

entails(S, W, neg(F))      :- not entails(S, W, F ),                    state(S), world(W), formula(F).
entails(S, W, and(F1, F2)) :-     entails(S, W, F1), entails(S, W, F2), state(S), world(W), formula(F1), formula(F2).
entails(S, W,  or(F1, F2)) :-     entails(S, W, F1),                    state(S), world(W), formula(F1), formula(F2).
entails(S, W,  or(F1, F2)) :-                        entails(S, W, F2), state(S), world(W), formula(F1), formula(F2).

% not_entails(S, W, or(F1, F2)) :- not entails(S, W, F1), not entails(S, W, F2), state(S), world(W), formula(F1), formula(F2).
% entails(S, W, or(F1, F2)) :- not not_entails(S, W, or(F1, F2)), state(S), world(W), formula(F1), formula(F2).

% b operator
not_entails(S1, W1, b(AG , F)) :- not     entails(S2, W2, F        ), edge(S1, W1, S2, W2, AG), state(S2), world(W2), formula(F), agent(AG).
entails    (S , W , b(AG , F)) :- not not_entails(S , W , b(AG , F)),                           state(S ), world(W ), formula(b(AG , F)).

% e operator
% not_entails(S , W , e(AGS, F)) :- not     entails(S , W , b(AG , F)), contains(S, W), state(S), world(W), contains_ag(AGS, AG), formula(b(AG , F)).
% entails    (S , W , e(AGS, F)) :- not not_entails(S , W , e(AGS, F)), contains(S, W), state(S), world(W),                       formula(e(AGS, F)).

% c operator
not_entails(S1, W1, c(AGS, F)) :- not     entails(S2, W2, F        ), reaches(S1, W1, S2, W2, AGS), state(S2), world(W2), formula(F).
entails    (S , W , c(AGS, F)) :- not not_entails(S , W , c(AGS, F)),                               state(S ), world(W ), formula(c(AGS, F)).



% ********** INITIAL STATE **********

% Since we assume to deal with FINITARY S5-states, we only allow the initial conditions initially(F), where F has one of the following forms:
%  1. Phi (propositional formula)
%  2. C_all( B_i(Phi))
%  3. C_all( B_i(Phi) \/  B_i(-Phi))
%  4. C_all(-B_i(Phi) /\ -B_i(-Phi)) (this case doesn't add knowledge)

% The initial conditions of form 1. determine the pointed world of the initial state
% A world is pointed if it entails exactly those fluents that hold initially
not_pointed(0, W) :- not holds(W, F), world(W), initially( F), fluent(F).
not_pointed(0, W) :-     holds(W, F), world(W), initially(-F), fluent(F).
pointed(0, W) :- not not_pointed(0, W), world(W).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A world is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
no_good_initial_world(W) :- not holds(W, F), world(W), initially(c(AGS,     F )), agent_set(AGS), fluent(F).
no_good_initial_world(W) :-     holds(W, F), world(W), initially(c(AGS, neg(F))), agent_set(AGS), fluent(F).

contains(0, W) :- pointed(0, W).
contains(0, W) :- not no_good_initial_world(W), world(W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
no_good_edge(0, W1, 0, W2, AG) :- not entails(0, W1, F),     entails(0, W2, F), world(W1), world(W2), initially(c(AGS, or(b(AG, F), b(AG, neg(F))))), agent_set(AGS), contains_ag(AGS, AG).
no_good_edge(0, W1, 0, W2, AG) :-     entails(0, W1, F), not entails(0, W2, F), world(W1), world(W2), initially(c(AGS, or(b(AG, F), b(AG, neg(F))))), agent_set(AGS), contains_ag(AGS, AG).

edge(0, W1, 0, W2, AG) :- not no_good_edge(0, W1, 0, W2, AG), contains(0, W1), contains(0, W2), agent(AG).



% ********** TRANSITION FUNCTION **********

% Generate
0 {plan(S, ACT) : action(ACT)} 1 :- state(S), S < maxS.

% plan(0, open_a).
% plan(1, close_a).

not_reached_goal(S) :- not entails(S, F), state(S), goal(F).
reached_goal(S) :- not not_reached_goal(S), state(S).

:- 0 { reached_goal(S) : state(S) } 0.
:- reached_goal(S1), plan(S2, ACT), S2 >= S1.

% ONTIC ACTIONS

not_holds_except(W1, W2, F1) :- not holds(W1, F2),     holds(W2, F2), world(W1), world(W2), fluent(F1), fluent(F2), F1 != F2.
not_holds_except(W1, W2, F1) :-     holds(W1, F2), not holds(W2, F2), world(W1), world(W2), fluent(F1), fluent(F2), F1 != F2.
holds_except(W1, W2, F) :- not not_holds_except(W1, W2, F), world(W1), world(W2), fluent(F).

pointed(S+1, PWS1) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT, CAUSED), holds_except(PWS1, PWS, CAUSED), holds(PWS1, CAUSED), pointed(S, PWS).
pointed(S+1, PWS1) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT, -CAUSED), holds_except(PWS1, PWS, CAUSED), not holds(PWS1, CAUSED), pointed(S, PWS).

edge(S+1, W1, S+1, W2, AG) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT,  CAUSED),
                              holds_except(W1, W3, CAUSED), holds_except(W2, W4, CAUSED), edge(S, W3, S, W4, AG), W1 != W3, W2 != W4,
                              fully_obs(ACT, AG, OBS), holds(PWS, OBS), pointed(S, PWS).
% edge(S+1, W1, S+1, W2, AG) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT, -CAUSED),
%                               holds_except(W1, W3, CAUSED), holds_except(W2, W4, CAUSED), edge(S, W3, S, W4, AG), W1 != W3, W2 != W4,
%                               fully_obs(ACT, AG, OBS), holds(PWS, OBS), pointed(S, PWS).

edge(S+1, W1, S  , W2, AG) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT,  CAUSED),
                              holds_except(W1, W3, CAUSED),                               edge(S, W3, S, W2, AG), W1 != W3,
                              fully_obs(ACT, AG, OBS), not holds(PWS, OBS), pointed(S, PWS).
% edge(S+1, W1, S  , W2, AG) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT, -CAUSED),
%                               holds_except(W1, W3, CAUSED),                               edge(S, W3, S, W2, AG), W1 != W3,
%                               fully_obs(ACT, AG, OBS), not holds(PWS, OBS), pointed(S, PWS).

edge(S+1, W1, S  , W2, AG) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT,  CAUSED),
                              holds_except(W1, W3, CAUSED),                               edge(S, W3, S, W2, AG), W1 != W3,
                              oblivious(ACT, AG).
% edge(S+1, W1, S  , W2, AG) :- plan(S, ACT), entails(S, EXEC), executable(ACT, EXEC), action_type(ACT, ont), causes(ACT, -CAUSED),
%                               holds_except(W1, W3, CAUSED),                               edge(S, W3, S, W2, AG), W1 != W3,
%                               oblivious(ACT, AG).

% SENSING ACTIONS
% ANNOUNCEMENT ACTIONS

% ********** TESTS... **********

% no_good(F) :- not entails(1, F), initially(F).
% :- no_good(F).
% no_good_test :- not entails(1, c(1, or(b(1, tails), b(1, neg(tails))))).
% good_test(W3, W4) :- plan(0, open_a), entails(0, EXEC), executable(open_a, EXEC), causes(open_a, CAUSED), edge(0, W3, 0, W4, 1),
%                      holds_except(2, W3, CAUSED), holds_except(2, W4, CAUSED), fully_obs(open_a, 1, OBS), holds(PWS, OBS), pointed(0, PWS).

% #show good_test/2.
% #show no_good/1.
% #show pointed/2.
% #show edge/5.
#show plan/2.
#show reached_goal/1.

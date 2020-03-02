%
% EMP encoding in ASP
%

% Reachability

reaches(S1, W1, S2, W2, AGS) :- edge(S1, W1, S2, W2, AG), contains_ag(AGS, AG).
reaches(S1, W1, S2, W2, AGS) :- edge(S1, W1, S3, W3, AG), contains_ag(AGS, AG), reaches(S3, W3, S2, W2, AGS).

% Entailment

formula(F) :- fluent(F).        % A fluent is a formula
formula(F) :- initially(F).     % An initial condition is a formula
formula(F) :- goal(F).          % A goal condition is a formula

entails(S, F) :- entails(S, W, F), pointed(S, W), formula(F).

entails(S, W, F     )      :-     holds(W, F),                          contains(S, W), state(S), world(W), fluent(F).
entails(S, W, neg(F))      :- not holds(W, F),                          contains(S, W), state(S), world(W), fluent(F).

entails(S, W, neg(F))      :- not entails(S, W, F ),                    contains(S, W), state(S), world(W), formula(F).
entails(S, W, and(F1, F2)) :-     entails(S, W, F1), entails(S, W, F2), contains(S, W), state(S), world(W), formula(F1), formula(F2).
entails(S, W,  or(F1, F2)) :-     entails(S, W, F1),                    contains(S, W), state(S), world(W), formula(F1), formula(F2).
entails(S, W,  or(F1, F2)) :-                        entails(S, W, F2), contains(S, W), state(S), world(W), formula(F1), formula(F2).

% b operator
not_entails(S1, W1, b(AG , F)) :- not     entails(S2, W2, F        ), edge(S1, W1, S2, W2, AG), contains(S2, W2), state(S2), world(W2), formula(F), agent(AG).
entails    (S , W , b(AG , F)) :- not not_entails(S , W , b(AG , F)),                           contains(S , W) , state(S ), world(W ), formula(b(AG , F)).

% e operator
not_entails(S , W , e(AGS, F)) :- not     entails(S , W , b(AG , F)), contains(S, W), state(S), world(W), contains_ag(AGS, AG), formula(b(AG , F)).
entails    (S , W , e(AGS, F)) :- not not_entails(S , W , e(AGS, F)), contains(S, W), state(S), world(W),                       formula(e(AGS, F)).

% c operator
not_entails(S1, W1, c(AGS, F)) :- not     entails(S2, W2, e(AGS, F)), reaches(S1, W1, S2, W2, AGS), contains(S2, W2), state(S2), world(W2), formula(e(AGS, F)).
entails    (S , W , c(AGS, F)) :- not not_entails(S , W , c(AGS, F)),                               contains(S , W ), state(S ), world(W ), formula(c(AGS, F)).

% Imposing initial conditions
no_good(F) :- not entails(1, F), initially(F).
:- no_good(F).

#show no_good/1.

% Imposing final conditions
% not_entails_goals(S) :- not entails(S, F), goal(F).
% final_state(S) :- not not_entails_goals(S).
% :- 0 {final_state(S) : state(S)} 0.

% ***** Initial state *****


% ***** Transition function *****

% Ontic actions
% Sensing actions
% Announcement actions

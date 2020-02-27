%
% EMP encoding in ASP
%

% Constants and domain predicates
#const ONT = 0      % Ontic action
#const SEN = 1      % Sensing action
#const ANN = 2      % Announcement action

#const FOB = 3      % Fully observant agent
#const POB = 4      % Partially observant agent
#const OBL = 5      % Oblivious agent

fluent(1..f).
agent(1..ag).
time(1..t).

world_id(1..w).
state_id(1..s).
action_id(1..act).

action_type(ONT;SEN;ANN).
observability_type(FOB;POB;OBL).

% ***** Input format *****

action(ACT, TYPE).              % action_id(ACT), action_type(TYPE)
obs_relation(ACT, AG, TYPE).    % action_id(ACT), agent(AG), observability_type(TYPE)

world(W, T).                 % state_id(S), world_id(W), time(T)
edge(W1, AG, W2, T).         % stete_id(S), world_id(W1), world_id(W2), time(T)
pi(W, F, T).                    % world_id(W), fluent(F), time(T)

% Recursive definition of belief formulas
% literal(F)           :- fluent(F).
% literal(not(F))      :- fluent(F).

% formula(F)           :- literal(F).
% formula(not(F))      :- formula(F).
% formula(and(F1, F2)) :- formula(F1), formula(F2).
% formula( or(F1, F2)) :- formula(F1), formula(F2).
% formula(  b(AG, F )) :- agent(AG), formula(F).

formula(not(q)).
formula(and(p, q)).
formula(c(AGS, and(head, not(opened))).

% Pointed world has id equal to 0
pointed(S, W, T) :- W = 0, state_id(S), world_id(W), time(T).

entails(S, F     , T) :-     pi(W, F, T), pointed(S, W, T), state_id(S), world_id(W), literal(F)     , time(T).
entails(S, not(F), T) :- not pi(W, F, T), pointed(S, W, T), state_id(S), world_id(W), literal(not(F)), time(T).

entails(S, not(F)     , T) :- not entails(S, F , T)                   , pointed(S, W, T), state_id(S), world_id(W), formula(not(F))     , time(T).
entails(S, and(F1, F2), T) :-     entails(S, F1, T), entails(S, F2, T), pointed(S, W, T), state_id(S), world_id(W), formula(and(F1, F2)), time(T).
entails(S,  or(F1, F2), T) :-     entails(S, F1, T)                   , pointed(S, W, T), state_id(S), world_id(W), formula( or(F1, F2)), time(T).
entails(S,  or(F1, F2), T) :-                        entails(S, F2, T), pointed(S, W, T), state_id(S), world_id(W), formula( or(F1, F2)), time(T).

not_entails(S, b(AG, F), T) :- not     entails(S, F       , T), edge(S, W1, AG, W2, T), pointed(S, W1, T), state_id(S), world_id(W1), world_id(W2), formula(b(AG, F)), agent(AG), time(T).
entails    (S, b(AG, F), T) :- not not_entails(S, b(AG, F), T), state_id(S), formula(b(AG, F)), time(T).



world(W, T+1) :- .


% ***** Initial state *****


% ***** Transition function *****

% Ontic actions



% Sensing actions
% Announcement actions

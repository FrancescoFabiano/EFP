% Constants and domain predicates
#const ONT = 0      % Ontic action
#const SEN = 1      % Sensing action
#const ANN = 2      % Announcement action

#const FOB = 3      % Fully observant agent
#const POB = 4      % Partially observant agent
#const OBL = 5      % Oblivious agent

% Coin in the box instance values
#const f   = 8.
#const ag  = 3.     % a = 1, b = 2, c = 3
#const act = 21.
#const t   = 5.     % for now fixed value
#const w   = 256.   % 2^f
#const s   = 64.    % for now fixed value (should be act^t)

fluent(1..f).
agent(1..ag).
time(1..t).

world(1..w).
state(1..s).
% action_id(1..act).

% action_type(ONT;SEN;ANN).
% observability_type(FOB;POB;OBL).

% ***** Input format *****

% action(ACT, TYPE).              % action_id(ACT), action_type(TYPE)
% obs_relation(ACT, AG, TYPE).    % action_id(ACT), agent(AG), observability_type(TYPE)

% *** FLUENTS ***
% fluent(F).

fluent(tail).
fluent(opened).
fluent(looking_a).
fluent(looking_b).
fluent(looking_c).
fluent(has_key_a).
fluent(has_key_b).
fluent(has_key_c).

% *** AGENTS ***
% agent_set(AGS).
% contains_ag(AGS, AG).

agent_set(1).   % {a, b, c}

contains_ag(1, 1).
contains_ag(1, 2).
contains_ag(1, 3).

% *** FORMULAE ***

% From executability conditions
% formula(F).
% not(F).
% and(F1, F2).
% or(F1, F2).
% b(AG, F).
% c(AGS, F).
% initially(F).
% goal(F).

formula(b(1, has_key_a)).
formula(b(2, has_key_b)).
formula(b(3, has_key_c)).

formula(and(b(1, has_key_a), has_key_a)).   % open_a
formula(and(b(2, has_key_b), has_key_b)).   % open_b
formula(and(b(3, has_key_c), has_key_c)).   % open_c

formula(b(1, opened)).
formula(b(2, opened)).
formula(b(3, opened)).

formula(b(1, looking_a)).
formula(b(2, looking_b)).
formula(b(3, looking_c)).

formula(and(b(1, opened), and(b(1, looking_a), and(looking_a, opened)))).   % peek_a
formula(and(b(2, opened), and(b(2, looking_b), and(looking_b, opened)))).   % peek_b
formula(and(b(3, opened), and(b(3, looking_c), and(looking_c, opened)))).   % peek_c

formula(not(looking_a)).
formula(not(looking_b)).
formula(not(looking_c)).

formula(and(b(1, looking_a), and(b(1, not(looking_b)), and(looking_a, not(looking_b))))).   % signal_a_b
formula(and(b(1, looking_a), and(b(1, not(looking_c)), and(looking_a, not(looking_c))))).   % signal_a_c
formula(and(b(2, looking_b), and(b(2, not(looking_a)), and(looking_b, not(looking_a))))).   % signal_b_a
formula(and(b(2, looking_b), and(b(2, not(looking_c)), and(looking_b, not(looking_c))))).   % signal_b_c
formula(and(b(3, looking_c), and(b(3, not(looking_a)), and(looking_c, not(looking_a))))).   % signal_c_a
formula(and(b(3, looking_c), and(b(3, not(looking_b)), and(looking_c, not(looking_b))))).   % signal_c_b

formula(and(b(1, looking_a), and(b(1,     looking_b ), and(looking_a,     looking_b )))).   % distract_a_b
formula(and(b(1, looking_a), and(b(1,     looking_c ), and(looking_a,     looking_c )))).   % distract_a_c
formula(and(b(2, looking_b), and(b(2,     looking_a ), and(looking_b,     looking_a )))).   % distract_b_a
formula(and(b(2, looking_b), and(b(2,     looking_c ), and(looking_b,     looking_c )))).   % distract_b_c
formula(and(b(3, looking_c), and(b(3,     looking_a ), and(looking_c,     looking_a )))).   % distract_c_a
formula(and(b(3, looking_c), and(b(3,     looking_b ), and(looking_c,     looking_b )))).   % distract_c_b

formula(b(1, tail)).
formula(b(2, tail)).
formula(b(3, tail)).

formula(and(b(1, tail), tail)).    % shout_tail_a
formula(and(b(2, tail), tail)).    % shout_tail_b
formula(and(b(3, tail), tail)).    % shout_tail_c

% From initially
initially(c(1,     has_key_a) ).
initially(c(1, not(has_key_b))).
initially(c(1, not(has_key_c))).
initially(c(1, not(opened   ))).
initially(c(1,     looking_a) ).
initially(c(1, not(looking_b))).
initially(c(1, not(looking_c))).

% From goal
goal(b(1, tail)).

% *** STATES ***
% state(S).

state(1).   % initial state
state(2).   % open_a

% planned(1, 1).    % state 1 is planned at time 1
% planned(2, 2).    % state 2 is planned at time 2

% *** WORLDS ***
% world(W).
% holds(W, F).
% contains(S, W).
% pointed(S, W).

world(1).    %  tails, -opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(2).    % -tails, -opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(3).    %  tails,  opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(4).    % -tails,  opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c

holds(1, 1).   % in world 1 it holds tails
holds(1, 3).   % in world 1 it holds looking_a
holds(1, 6).   % in world 1 it holds has_key_a

holds(2, 3).   % in world 2 it holds looking_a
holds(2, 6).   % in world 2 it holds has_key_a

holds(3, 1).   % in world 3 it holds tails
holds(3, 2).   % in world 3 it holds opened
holds(3, 3).   % in world 3 it holds looking_a
holds(3, 6).   % in world 3 it holds has_key_a

holds(4, 2).   % in world 4 it holds opened
holds(4, 3).   % in world 4 it holds looking_a
holds(4, 6).   % in world 4 it holds has_key_a

% Initial state
contains(1, 1).  % world 1 is present in state 1
contains(1, 2).  % world 2 is present in state 1
pointed (1, 1).  % world 1 is pointed in state 1

% open_a
contains(2, 1).  % world 0 is present in state 2
contains(2, 2).  % world 1 is present in state 2
contains(2, 3).  % world 2 is present in state 2
contains(2, 4).  % world 3 is present in state 2
pointed(2, 3).   % world 2 is pointed in state 2

% *** EDGES ***
% edge(S1, W1, S2, W2, AG).

% Initial state
edge(1, 1, 1, 1, 1).   % edge from 1 (state 1) to 1 (state 1) labeled with a (1)
edge(1, 1, 1, 1, 2).   % edge from 1 (state 1) to 1 (state 1) labeled with b (2)
edge(1, 1, 1, 1, 3).   % edge from 1 (state 1) to 1 (state 1) labeled with c (3)
edge(1, 2, 1, 2, 1).   % edge from 2 (state 1) to 2 (state 1) labeled with a (1)
edge(1, 2, 1, 2, 2).   % edge from 2 (state 1) to 2 (state 1) labeled with b (2)
edge(1, 2, 1, 2, 3).   % edge from 2 (state 1) to 2 (state 1) labeled with c (3)

edge(1, 1, 1, 2, 1).   % edge from 1 (state 1) to 2 (state 1) labeled with a (1)
edge(1, 1, 1, 2, 2).   % edge from 1 (state 1) to 2 (state 1) labeled with b (2)
edge(1, 1, 1, 2, 3).   % edge from 1 (state 1) to 2 (state 1) labeled with c (3)
edge(1, 2, 1, 1, 1).   % edge from 2 (state 1) to 1 (state 1) labeled with a (1)
edge(1, 2, 1, 1, 2).   % edge from 2 (state 1) to 1 (state 1) labeled with b (2)
edge(1, 2, 1, 1, 3).   % edge from 2 (state 1) to 1 (state 1) labeled with c (3)

% open_a
edge(2, 3, 2, 3, 1).   % edge from 3 (state 2) to 3 (state 2) labeled with a (1)
edge(2, 3, 2, 3, 2).   % edge from 3 (state 2) to 3 (state 2) labeled with b (2)
edge(2, 4, 2, 4, 1).   % edge from 4 (state 2) to 4 (state 2) labeled with a (1)
edge(2, 4, 2, 4, 2).   % edge from 4 (state 2) to 4 (state 2) labeled with b (2)

edge(2, 3, 2, 4, 1).   % edge from 3 (state 2) to 4 (state 2) labeled with a (1)
edge(2, 3, 2, 4, 2).   % edge from 3 (state 2) to 4 (state 2) labeled with b (2)
edge(2, 4, 2, 3, 1).   % edge from 4 (state 2) to 3 (state 2) labeled with a (1)
edge(2, 4, 2, 3, 2).   % edge from 4 (state 2) to 3 (state 2) labeled with b (2)
%
edge(2, 3, 1, 1, 3).   % edge from 3 (state 2) to 1 (state 1) labeled with c (3)
edge(2, 3, 1, 2, 3).   % edge from 3 (state 2) to 2 (state 1) labeled with c (3)
edge(2, 4, 1, 1, 3).   % edge from 4 (state 2) to 1 (state 1) labeled with c (3)
edge(2, 4, 1, 2, 3).   % edge from 4 (state 2) to 2 (state 1) labeled with c (3)

% Constants and domain predicates
#const ont = 0.      % Ontic action
#const sen = 1.      % Sensing action
#const ann = 2.      % Announcement action

#const fob = 3.      % Fully observant agent
#const pob = 4.      % Partially observant agent
#const obl = 5.      % Oblivious agent

% Coin in the box instance values
#const f    = 8.
#const ag   = 3.     % a = 1, b = 2, c = 3
#const act  = 21.
#const w    = 4.     % for now fixed value (should be 2^f)
#const maxS = 2.     % for now fixed value (should be act^t)

% fluent(1..f).
agent(1..ag).
% time(1..t).

world(1..w).
state(0..maxS).
% action_id(1..act).

% action_type(ONT;SEN;ANN).
% observability_type(FOB;POB;OBL).

% ***** Input format *****

% action(ACT, TYPE).              % action_id(ACT), action_type(TYPE)
% obs_relation(ACT, AG, TYPE).    % action_id(ACT), agent(AG), observability_type(TYPE)

% *** FLUENTS ***
% fluent(F).

fluent(tails).
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

agent_set(all_agents).   % {a, b, c}

contains_ag(all_agents, 1).
contains_ag(all_agents, 2).
contains_ag(all_agents, 3).

% *** FORMULAE ***

% From executability conditions
% formula(F).
% neg(F).
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

formula(neg(looking_a)).
formula(neg(looking_b)).
formula(neg(looking_c)).

% formula(b(1, neg(looking_b)).
% todo: finish

formula(and(b(1, looking_a), and(b(1, neg(looking_b)), and(looking_a, neg(looking_b))))).   % signal_a_b
formula(and(b(1, looking_a), and(b(1, neg(looking_c)), and(looking_a, neg(looking_c))))).   % signal_a_c
formula(and(b(2, looking_b), and(b(2, neg(looking_a)), and(looking_b, neg(looking_a))))).   % signal_b_a
formula(and(b(2, looking_b), and(b(2, neg(looking_c)), and(looking_b, neg(looking_c))))).   % signal_b_c
formula(and(b(3, looking_c), and(b(3, neg(looking_a)), and(looking_c, neg(looking_a))))).   % signal_c_a
formula(and(b(3, looking_c), and(b(3, neg(looking_b)), and(looking_c, neg(looking_b))))).   % signal_c_b

formula(and(b(1, looking_a), and(b(1,     looking_b ), and(looking_a,     looking_b )))).   % distract_a_b
formula(and(b(1, looking_a), and(b(1,     looking_c ), and(looking_a,     looking_c )))).   % distract_a_c
formula(and(b(2, looking_b), and(b(2,     looking_a ), and(looking_b,     looking_a )))).   % distract_b_a
formula(and(b(2, looking_b), and(b(2,     looking_c ), and(looking_b,     looking_c )))).   % distract_b_c
formula(and(b(3, looking_c), and(b(3,     looking_a ), and(looking_c,     looking_a )))).   % distract_c_a
formula(and(b(3, looking_c), and(b(3,     looking_b ), and(looking_c,     looking_b )))).   % distract_c_b

formula(b(1, tails)).
formula(b(2, tails)).
formula(b(3, tails)).

formula(and(b(1, tails), tails)).    % shout_tail_a
formula(and(b(2, tails), tails)).    % shout_tail_b
formula(and(b(3, tails), tails)).    % shout_tail_c

% Entailment test formulae
% formula(and(has_key_a, looking_a)).
% formula(and(tails, and(has_key_a, looking_a))).

% formula(or(opened, tails)).

% formula(neg(opened)).
% formula(b(3, neg(opened))).
% formula(b(1, b(3, neg(opened)))).
% formula(b(3, b(1, b(3, neg(opened))))).

% formula(e(1, has_key_a)).

% initially(and(tails, and(has_key_a, looking_a))).
% initially(or(opened, tails)).
% initially(b(1, has_key_a)).
% initially(e(1, has_key_a)).

% formula(e(1, has_key_a)).
% formula(c(1, has_key_a)).
% initially(c(1, has_key_a)).

% formula(neg(tails)).
% formula(neg(neg(tails))).
% initially(neg(neg(tails))).

% From initially
% formula(e(1,     has_key_a) ).
% formula(e(1, neg(has_key_b))).
% formula(e(1, neg(has_key_c))).
% formula(e(1, neg(opened   ))).
% formula(e(1,     looking_a) ).
% formula(e(1, neg(looking_b))).
% formula(e(1, neg(looking_c))).

formula(c(all_agents,     has_key_a) ).
formula(c(all_agents, neg(has_key_b))).
formula(c(all_agents, neg(has_key_c))).
formula(c(all_agents, neg(opened   ))).
formula(c(all_agents,     looking_a) ).
formula(c(all_agents, neg(looking_b))).
formula(c(all_agents, neg(looking_c))).

initially(tails).
initially(-opened).
initially(has_key_a).
initially(-has_key_b).
initially(-has_key_c).
initially(looking_a).
initially(-looking_b).
initially(-looking_c).

formula(neg(tails)).
formula(b(1, tails)).
formula(b(1, neg(tails))).
% formula(neg(b(1, tails))).
formula(or(b(1, tails), b(1, neg(tails)))).
% formula(or(b(1, tails), neg(b(1, tails)))).
formula(c(all_agents, or(b(1, tails), b(1, neg(tails))))).
% formula(c(1, or(b(1, tails), neg(b(1, tails))))).

% initially(b(1, tails)).
% initially(c(all_agents, or(b(1, tails), b(1, neg(tails))))).
% initially(c(1, b(1, tails))).
% initially(c(1, or(b(1, tails), neg(b(1, tails))))).
% initially(or(b(1, tails), b(1, neg(tails)))).
% initially(b(1, tails)).

initially(c(all_agents,     has_key_a )).
initially(c(all_agents, neg(has_key_b))).
initially(c(all_agents, neg(has_key_c))).
initially(c(all_agents, neg(opened   ))).
initially(c(all_agents,     looking_a )).
initially(c(all_agents, neg(looking_b))).
initially(c(all_agents, neg(looking_c))).

% From goal
% goal(b(1, tails)).
formula(b(2, opened)).
formula(neg(opened)).
goal(b(2, opened)).
goal(neg(opened)).

% *** ACTIONS ***
action(open_a).
action_type(open_a, ont).
executable(open_a, and(b(1, has_key_a), has_key_a)).
causes(open_a, opened).

fully_obs(open_a, 1, true).
fully_obs(open_a, 2, true).
fully_obs(open_a, 3, looking_c).

formula(b(1, opened)).
formula(and(b(1, opened), opened)).

action(close_a).
action_type(close_a, ont).
executable(close_a, and(b(1, opened), opened)).
causes(close_a, -opened).

fully_obs(close_a, 1, true).
fully_obs(close_a, 2, looking_b).
fully_obs(close_a, 3, looking_c).

action(open_b).
action_type(open_b, ont).
executable(open_b, and(b(2, has_key_b), has_key_b)).
causes(open_b, opened).

fully_obs(open_b, 1, looking_a).
fully_obs(open_b, 2, true).
fully_obs(open_b, 3, looking_c).

action(open_c).
action_type(open_c, ont).
executable(open_c, and(b(3, has_key_c), has_key_c)).
causes(open_c, opened).

fully_obs(open_c, 1, looking_a).
fully_obs(open_c, 2, looking_b).
fully_obs(open_c, 3, true).


% *** STATES ***
% state(S).

state(1).   % initial state
state(2).   % open_a

% planned(1, 1).    % state 1 is planned at time 1
% planned(2, 2).    % state 2 is planned at time 2

% *** WORLDS ***
% world(W).
% holds(W, F).
% pointed(S, W).

world(1).    %  tails, -opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(2).    % -tails, -opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(3).    %  tails,  opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(4).    % -tails,  opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c

holds(1, tails    ).   % in world 1 it holds tails
holds(1, looking_a).   % in world 1 it holds looking_a
holds(1, has_key_a).   % in world 1 it holds has_key_a

holds(2, looking_a).   % in world 2 it holds looking_a
holds(2, has_key_a).   % in world 2 it holds has_key_a

holds(3, tails    ).   % in world 3 it holds tails
holds(3, opened   ).   % in world 3 it holds opened
holds(3, looking_a).   % in world 3 it holds looking_a
holds(3, has_key_a).   % in world 3 it holds has_key_a

holds(4, opened   ).   % in world 4 it holds opened
holds(4, looking_a).   % in world 4 it holds looking_a
holds(4, has_key_a).   % in world 4 it holds has_key_a

% Initial state
% contains(1, 1).  % world 1 is present in state 1
% contains(1, 2).  % world 2 is present in state 1
% pointed (1, 1).  % world 1 is pointed in state 1

% open_a
% contains(2, 1).  % world 0 is present in state 2
% contains(2, 2).  % world 1 is present in state 2
% contains(2, 3).  % world 2 is present in state 2
% contains(2, 4).  % world 3 is present in state 2
% pointed(2, 3).   % world 2 is pointed in state 2

% *** EDGES ***
% edge(S1, W1, S2, W2, AG).

% TODO: controllare se gestiamo correttamente repetition

% Initial state
% edge(1, 1, 1, 1, 1).   % edge from 1 (state 1) to 1 (state 1) labeled with a (1)
% edge(1, 1, 1, 1, 2).   % edge from 1 (state 1) to 1 (state 1) labeled with b (2)
% edge(1, 1, 1, 1, 3).   % edge from 1 (state 1) to 1 (state 1) labeled with c (3)
% edge(1, 2, 1, 2, 1).   % edge from 2 (state 1) to 2 (state 1) labeled with a (1)
% edge(1, 2, 1, 2, 2).   % edge from 2 (state 1) to 2 (state 1) labeled with b (2)
% edge(1, 2, 1, 2, 3).   % edge from 2 (state 1) to 2 (state 1) labeled with c (3)

% edge(1, 1, 1, 2, 1).   % edge from 1 (state 1) to 2 (state 1) labeled with a (1)
% edge(1, 1, 1, 2, 2).   % edge from 1 (state 1) to 2 (state 1) labeled with b (2)
% edge(1, 1, 1, 2, 3).   % edge from 1 (state 1) to 2 (state 1) labeled with c (3)
% edge(1, 2, 1, 1, 1).   % edge from 2 (state 1) to 1 (state 1) labeled with a (1)
% edge(1, 2, 1, 1, 2).   % edge from 2 (state 1) to 1 (state 1) labeled with b (2)
% edge(1, 2, 1, 1, 3).   % edge from 2 (state 1) to 1 (state 1) labeled with c (3)

% open_a
% edge(2, 3, 2, 3, 1).   % edge from 3 (state 2) to 3 (state 2) labeled with a (1)
% edge(2, 3, 2, 3, 2).   % edge from 3 (state 2) to 3 (state 2) labeled with b (2)
% edge(2, 4, 2, 4, 1).   % edge from 4 (state 2) to 4 (state 2) labeled with a (1)
% edge(2, 4, 2, 4, 2).   % edge from 4 (state 2) to 4 (state 2) labeled with b (2)

% edge(2, 3, 2, 4, 1).   % edge from 3 (state 2) to 4 (state 2) labeled with a (1)
% edge(2, 3, 2, 4, 2).   % edge from 3 (state 2) to 4 (state 2) labeled with b (2)
% edge(2, 4, 2, 3, 1).   % edge from 4 (state 2) to 3 (state 2) labeled with a (1)
% edge(2, 4, 2, 3, 2).   % edge from 4 (state 2) to 3 (state 2) labeled with b (2)

% edge(2, 3, 1, 1, 3).   % edge from 3 (state 2) to 1 (state 1) labeled with c (3)
% edge(2, 3, 1, 2, 3).   % edge from 3 (state 2) to 2 (state 1) labeled with c (3)
% edge(2, 4, 1, 1, 3).   % edge from 4 (state 2) to 1 (state 1) labeled with c (3)
% edge(2, 4, 1, 2, 3).   % edge from 4 (state 2) to 2 (state 1) labeled with c (3)

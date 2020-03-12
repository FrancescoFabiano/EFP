% Constants and domain predicates
#const ont = 0.      % Ontic action
#const sen = 1.      % Sensing action
#const ann = 2.      % Announcement action

#const fob = 3.      % Fully observant agent
#const pob = 4.      % Partially observant agent
#const obl = 5.      % Oblivious agent

% Coin in the box instance values
#const f    = 8.
#const p    = 11.     % for now fixed value (should be 2^f)
% #const maxS = 5.     % for now fixed value (should be act^t)
#const maxT = 5.

#const ags  = 7.     % 2^|{a, b, c}| - 1 (we do not count the empty set)
#const all_agents = 7.

% fluent(1..f).
agent(a;b;c).
time(0..maxT).

agent_set(1..ags).

% world(1..w).
possibility(1..p).
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

contains_ag(1, a).

contains_ag(2, b).

contains_ag(3, c).

contains_ag(4, a).
contains_ag(4, b).

contains_ag(5, a).
contains_ag(5, c).

contains_ag(6, b).
contains_ag(6, c).

agent_set(all_agents).   % {a, b, c}

contains_ag(all_agents, a).
contains_ag(all_agents, b).
contains_ag(all_agents, c).

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

formula(b(a, has_key_a)).
formula(b(b, has_key_b)).
formula(b(c, has_key_c)).

formula(and(b(a, has_key_a), has_key_a)).   % open_a
formula(and(b(b, has_key_b), has_key_b)).   % open_b
formula(and(b(c, has_key_c), has_key_c)).   % open_c

formula(b(a, opened)).
formula(b(b, opened)).
formula(b(c, opened)).

formula(b(a, looking_a)).
formula(b(b, looking_b)).
formula(b(c, looking_c)).

formula(and(b(a, opened), and(b(a, looking_a), and(looking_a, opened)))).   % peek_a
formula(and(b(b, opened), and(b(b, looking_b), and(looking_b, opened)))).   % peek_b
formula(and(b(c, opened), and(b(c, looking_c), and(looking_c, opened)))).   % peek_c

formula(neg(looking_a)).
formula(neg(looking_b)).
formula(neg(looking_c)).

% formula(b(a, neg(looking_b)).
% todo: finish

formula(and(b(a, looking_a), and(b(a, neg(looking_b)), and(looking_a, neg(looking_b))))).   % signal_a_b
formula(and(b(a, looking_a), and(b(a, neg(looking_c)), and(looking_a, neg(looking_c))))).   % signal_a_c
formula(and(b(b, looking_b), and(b(b, neg(looking_a)), and(looking_b, neg(looking_a))))).   % signal_b_a
formula(and(b(b, looking_b), and(b(b, neg(looking_c)), and(looking_b, neg(looking_c))))).   % signal_b_c
formula(and(b(c, looking_c), and(b(c, neg(looking_a)), and(looking_c, neg(looking_a))))).   % signal_c_a
formula(and(b(c, looking_c), and(b(c, neg(looking_b)), and(looking_c, neg(looking_b))))).   % signal_c_b

formula(and(b(a, looking_a), and(b(a,     looking_b ), and(looking_a,     looking_b )))).   % distract_a_b
formula(and(b(a, looking_a), and(b(a,     looking_c ), and(looking_a,     looking_c )))).   % distract_a_c
formula(and(b(b, looking_b), and(b(b,     looking_a ), and(looking_b,     looking_a )))).   % distract_b_a
formula(and(b(b, looking_b), and(b(b,     looking_c ), and(looking_b,     looking_c )))).   % distract_b_c
formula(and(b(c, looking_c), and(b(c,     looking_a ), and(looking_c,     looking_a )))).   % distract_c_a
formula(and(b(c, looking_c), and(b(c,     looking_b ), and(looking_c,     looking_b )))).   % distract_c_b

formula(b(a, tails)).
formula(b(b, tails)).
formula(b(c, tails)).

formula(and(b(a, tails), tails)).    % shout_tail_a
formula(and(b(b, tails), tails)).    % shout_tail_b
formula(and(b(c, tails), tails)).    % shout_tail_c

% From initially

formula(c(all_agents,  has_key_a)).
formula(c(all_agents, -has_key_b)).
formula(c(all_agents, -has_key_c)).
formula(c(all_agents, -opened   )).
formula(c(all_agents,  looking_a)).
formula(c(all_agents, -looking_b)).
formula(c(all_agents, -looking_c)).

initially(tails).
initially(-opened).
initially(has_key_a).
initially(-has_key_b).
initially(-has_key_c).
initially(looking_a).
initially(-looking_b).
initially(-looking_c).

formula(neg(tails)).
formula(b(a, tails)).
formula(b(a, neg(tails))).
% formula(neg(b(a, tails))).
formula(or(b(a, tails), b(a, neg(tails)))).
% formula(or(b(a, tails), neg(b(a, tails)))).
formula(c(all_agents, or(b(a, tails), b(a, neg(tails))))).
% formula(c(a, or(b(a, tails), neg(b(a, tails))))).


initially(c(all_agents,  has_key_a)).
initially(c(all_agents, -has_key_b)).
initially(c(all_agents, -has_key_c)).
initially(c(all_agents, -opened   )).
initially(c(all_agents,  looking_a)).
initially(c(all_agents, -looking_b)).
initially(c(all_agents, -looking_c)).

% From goal
% goal(b(a, tails)).
formula(b(a, -opened)).
formula(b(b, opened)).
formula(b(c, opened)).
formula(opened).

#const goals = 4.

goal(b(a, -opened)).
goal(b(b, opened)).
goal(b(c, opened)).
goal(opened).

% *** ACTIONS ***

% formula(true).
% open_a
% formula(and(and(b(a, has_key_a), has_key_a), -opened)).
formula(-opened).
formula(b(a, -opened)).
formula(and(b(a, -opened), -opened)).
formula(and(and(b(a, -opened), -opened), has_key_a)).

ontic_action(open_a).
% action_type(open_a, ont).
executable(open_a, and(and(b(a, -opened), -opened), has_key_a)).
causes(open_a, opened).

fully_obs(open_a, a, true).
fully_obs(open_a, b, true). % looking_b
oblivious(open_a, c).
% fully_obs(open_a, c, true). % looking_c

% close_a
formula(b(a, opened)).
formula(and(b(a, opened), opened)).

ontic_action(close_a).
% action_type(close_a, ont).
executable(close_a, and(b(a, opened), opened)).
causes(close_a, -opened).

fully_obs(close_a, a, true).
% fully_obs(close_a, b, true).
oblivious(close_a, b).
oblivious(close_a, c).

% open_b
% ontic_action(open_b).
% % action_type(open_b, ont).
% executable(open_b, and(b(b, has_key_b), has_key_b)).
% causes(open_b, opened).

% fully_obs(open_b, a, looking_a).
% fully_obs(open_b, b, true).
% fully_obs(open_b, c, looking_c).

% open_c
formula(b(c, -opened)).
formula(and(b(c, -opened), -opened)).

ontic_action(open_c).
% action_type(open_c, ont).
executable(open_c, and(b(c, -opened), -opened)). % and(b(c, has_key_c), has_key_c)
causes(open_c, opened).

oblivious(open_c, a).
% oblivious(open_c, b).
fully_obs(open_c, b, true).
fully_obs(open_c, c, true).

% fully_obs(open_c, a, looking_a).
% fully_obs(open_c, 2, looking_b).


% *** STATES ***
% possibility(S).

% possibility(1).   % initial possibility
% possibility(2).   % open_a

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

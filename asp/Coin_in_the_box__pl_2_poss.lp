% Constants and domain predicates
#const f    = 8.
#const p    = 20.     % for now fixed value (should be 2^f)
#const maxT = 4.

#const ags  = 7.     % 2^|{a, b, c}| - 1 (we do not count the empty set)
#const all_agents = 7.

possibility(1..p).
time(0..maxT).
agent(a;b;c).
agent_set(1..ags).

% *** FLUENTS ***
% fluent(F).

fluent(true).
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
formula(b(a, tails)).
formula(b(b, tails)).
formula(b(c, tails)).
formula(tails).

#const goals = 4.

goal(b(a, tails)).
goal(b(b, tails)).
goal(b(c, tails)).
goal(b(a, opened)).
goal(b(b, opened)).
goal(b(c, opened)).
goal(tails).

% *** ACTIONS ***

formula(true).
% open_a
% formula(and(and(b(a, has_key_a), has_key_a), -opened)).
formula(-opened).
formula(b(a, -opened)).
formula(and(b(a, -opened), -opened)).
formula(and(and(b(a, -opened), -opened), has_key_a)).

ontic(open_a).
executable(open_a, and(and(b(a, -opened), -opened), has_key_a)).
causes(open_a, opened, true).

observes(open_a, a, true).
observes(open_a, b, true). % looking_b
oblivious(open_a, c).
% observes(open_a, c, true). % looking_c

% close_a
formula(b(a, opened)).
formula(and(b(a, opened), opened)).

ontic(close_a).
executable(close_a, and(b(a, opened), opened)).
causes(close_a, -opened, true).

observes(close_a, a, true).
% observes(close_a, b, true).
oblivious(close_a, b).
oblivious(close_a, c).

% open_b
% ontic(open_b).
% executable(open_b, and(b(b, has_key_b), has_key_b)).
% causes(open_b, opened).

% observes(open_b, a, looking_a).
% observes(open_b, b, true).
% observes(open_b, c, looking_c).

% open_c
formula(b(c, -opened)).
formula(and(b(c, -opened), -opened)).

ontic(open_c).
executable(open_c, b(c, -opened)). % and(b(c, has_key_c), has_key_c)
causes(open_c, opened, true).

oblivious(open_c, a).
% oblivious(open_c, b).
observes(open_c, b, true).
observes(open_c, c, true).

% observes(open_c, a, looking_a).
% observes(open_c, 2, looking_b).

% peek_a
formula(and(b(a,opened), opened)).

sensing(peek_a).
executable(peek_a, and(b(a,opened), opened)).
determines(peek_a, tails, true).

observes(peek_a, a, true).
aware_of(peek_a, b, true).
oblivious(peek_a, c).

% peek_c
formula(and(b(c,opened), opened)).

sensing(peek_c).
executable(peek_c, and(b(c,opened), opened)).
determines(peek_c, tails, true).

observes(peek_c, c, true).
aware_of(peek_c, b, true).
oblivious(peek_c, a).

% shout_tail_a
formula(b(a,tails)).
formula(and(b(a,tails), tails)).

announcement(shout_tail_a).

executable(shout_tail_a, and(b(a,tails), tails)).
announces(shout_tail_a, tails, true).

observes(shout_tail_a, a, true).
observes(shout_tail_a, b, true).
observes(shout_tail_a, c, true).

% *** WORLDS ***
% world(W).
% holds(W, F).
% pointed(S, W).

world(1).    %  tails, -opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(2).    % -tails, -opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(3).    %  tails,  opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c
world(4).    % -tails,  opened, looking_a, -looking_b, -looking_c, has_key_a, -has_key_b, -has_key_c

holds(1,  tails    ).
% holds(1, -opened   ).
holds(1,  looking_a).
% holds(1, -looking_b).
% holds(1, -looking_c).
holds(1,  has_key_a).
% holds(1, -has_key_b).
% holds(1, -has_key_c).

% holds(2, -tails    ).
% holds(2, -opened   ).
holds(2,  looking_a).
% holds(2, -looking_b).
% holds(2, -looking_c).
holds(2,  has_key_a).
% holds(2, -has_key_b).
% holds(2, -has_key_c).

holds(3,  tails    ).
holds(3,  opened   ).
holds(3,  looking_a).
% holds(3, -looking_b).
% holds(3, -looking_c).
holds(3,  has_key_a).
% holds(3, -has_key_b).
% holds(3, -has_key_c).

% holds(4, -tails    ).
holds(4,  opened   ).
holds(4,  looking_a).
% holds(4, -looking_b).
% holds(4, -looking_c).
holds(4,  has_key_a).
% holds(4, -has_key_b).
% holds(4, -has_key_c).

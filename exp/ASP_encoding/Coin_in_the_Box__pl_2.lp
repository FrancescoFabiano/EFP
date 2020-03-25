% *** FLUENTS ***
fluent(has_key_a).
fluent(has_key_b).
fluent(has_key_c).
fluent(looking_a).
fluent(looking_b).
fluent(looking_c).
fluent(opened).
fluent(tail).


% *** AGENTS ***
agent(a).
agent(b).
agent(c).


% *** ACTIONS ***
action(distract_a_b).
ontic_action(distract_a_b).
formula(looking_a).
formula(looking_b).
formula(and(looking_a,looking_b)).
formula(b(a,looking_b)).
formula(b(a,looking_a)).
formula(and(b(a,looking_a),b(a,looking_b))).
formula(and(and(b(a,looking_a),b(a,looking_b)),and(looking_a,looking_b))).
executable(distract_a_b, and(and(b(a,looking_a),b(a,looking_b)),and(looking_a,looking_b))).
fully_obs(distract_a_b, a, true).
fully_obs(distract_a_b, b, true).


action(distract_a_c).
ontic_action(distract_a_c).
formula(looking_c).
formula(and(looking_a,looking_c)).
formula(b(a,looking_c)).
formula(and(b(a,looking_a),b(a,looking_c))).
formula(and(and(b(a,looking_a),b(a,looking_c)),and(looking_a,looking_c))).
executable(distract_a_c, and(and(b(a,looking_a),b(a,looking_c)),and(looking_a,looking_c))).
fully_obs(distract_a_c, a, true).
fully_obs(distract_a_c, c, true).


action(distract_b_a).
ontic_action(distract_b_a).
formula(b(b,looking_a)).
formula(b(b,looking_b)).
formula(and(b(b,looking_b),b(b,looking_a))).
formula(and(and(b(b,looking_b),b(b,looking_a)),and(looking_a,looking_b))).
executable(distract_b_a, and(and(b(b,looking_b),b(b,looking_a)),and(looking_a,looking_b))).
fully_obs(distract_b_a, a, true).
fully_obs(distract_b_a, b, true).


action(distract_b_c).
ontic_action(distract_b_c).
formula(and(looking_b,looking_c)).
formula(b(b,looking_c)).
formula(and(b(b,looking_b),b(b,looking_c))).
formula(and(and(b(b,looking_b),b(b,looking_c)),and(looking_b,looking_c))).
executable(distract_b_c, and(and(b(b,looking_b),b(b,looking_c)),and(looking_b,looking_c))).
fully_obs(distract_b_c, b, true).
fully_obs(distract_b_c, c, true).


action(distract_c_a).
ontic_action(distract_c_a).
formula(b(c,looking_a)).
formula(b(c,looking_c)).
formula(and(b(c,looking_c),b(c,looking_a))).
formula(and(and(b(c,looking_c),b(c,looking_a)),and(looking_a,looking_c))).
executable(distract_c_a, and(and(b(c,looking_c),b(c,looking_a)),and(looking_a,looking_c))).
fully_obs(distract_c_a, a, true).
fully_obs(distract_c_a, c, true).


action(distract_c_b).
ontic_action(distract_c_b).
formula(b(c,looking_b)).
formula(and(b(c,looking_c),b(c,looking_b))).
formula(and(and(b(c,looking_c),b(c,looking_b)),and(looking_b,looking_c))).
executable(distract_c_b, and(and(b(c,looking_c),b(c,looking_b)),and(looking_b,looking_c))).
fully_obs(distract_c_b, b, true).
fully_obs(distract_c_b, c, true).


action(open_a).
ontic_action(open_a).
formula(has_key_a).
formula(b(a,has_key_a)).
formula(and(b(a,has_key_a),has_key_a)).
executable(open_a, and(b(a,has_key_a),has_key_a)).
fully_obs(open_a, a, true).
fully_obs(open_a, b, looking_b).
fully_obs(open_a, c, looking_c).


action(open_b).
ontic_action(open_b).
formula(has_key_b).
formula(b(b,has_key_b)).
formula(and(b(b,has_key_b),has_key_b)).
executable(open_b, and(b(b,has_key_b),has_key_b)).
fully_obs(open_b, a, looking_a).
fully_obs(open_b, b, true).
fully_obs(open_b, c, looking_c).


action(open_c).
ontic_action(open_c).
formula(has_key_c).
formula(b(c,has_key_c)).
formula(and(b(c,has_key_c),has_key_c)).
executable(open_c, and(b(c,has_key_c),has_key_c)).
fully_obs(open_c, a, looking_a).
fully_obs(open_c, b, looking_b).
fully_obs(open_c, c, true).


action(peek_a).
sensing_action(peek_a).
formula(opened).
formula(and(looking_a,opened)).
formula(b(a,opened)).
formula(and(b(a,opened),b(a,looking_a))).
formula(and(and(b(a,opened),b(a,looking_a)),and(looking_a,opened))).
executable(peek_a, and(and(b(a,opened),b(a,looking_a)),and(looking_a,opened))).
fully_obs(peek_a, a, true).
partial_obs(peek_a, b, looking_b).
partial_obs(peek_a, c, looking_c).


action(peek_b).
sensing_action(peek_b).
formula(and(looking_b,opened)).
formula(b(b,opened)).
formula(and(b(b,opened),b(b,looking_b))).
formula(and(and(b(b,opened),b(b,looking_b)),and(looking_b,opened))).
executable(peek_b, and(and(b(b,opened),b(b,looking_b)),and(looking_b,opened))).
fully_obs(peek_b, b, true).
partial_obs(peek_b, a, looking_a).
partial_obs(peek_b, c, looking_c).


action(peek_c).
sensing_action(peek_c).
formula(and(looking_c,opened)).
formula(b(c,opened)).
formula(and(b(c,opened),b(c,looking_c))).
formula(and(and(b(c,opened),b(c,looking_c)),and(looking_c,opened))).
executable(peek_c, and(and(b(c,opened),b(c,looking_c)),and(looking_c,opened))).
fully_obs(peek_c, c, true).
partial_obs(peek_c, a, looking_a).
partial_obs(peek_c, b, looking_b).


action(shout_tail_a).
announcement_action(shout_tail_a).
formula(tail).
formula(b(a,tail)).
formula(and(b(a,tail),tail)).
executable(shout_tail_a, and(b(a,tail),tail)).
fully_obs(shout_tail_a, a, true).
fully_obs(shout_tail_a, b, looking_b).
fully_obs(shout_tail_a, c, looking_c).


action(shout_tail_b).
announcement_action(shout_tail_b).
formula(b(b,tail)).
formula(and(b(b,tail),tail)).
executable(shout_tail_b, and(b(b,tail),tail)).
fully_obs(shout_tail_b, a, looking_a).
fully_obs(shout_tail_b, b, true).
fully_obs(shout_tail_b, c, looking_c).


action(shout_tail_c).
announcement_action(shout_tail_c).
formula(b(c,tail)).
formula(and(b(c,tail),tail)).
executable(shout_tail_c, and(b(c,tail),tail)).
fully_obs(shout_tail_c, a, looking_a).
fully_obs(shout_tail_c, b, looking_b).
fully_obs(shout_tail_c, c, true).


action(signal_a_b).
ontic_action(signal_a_b).
formula(-looking_b).
formula(and(looking_a,-looking_b)).
formula(b(a,-looking_b)).
formula(and(b(a,looking_a),b(a,-looking_b))).
formula(and(and(b(a,looking_a),b(a,-looking_b)),and(looking_a,-looking_b))).
executable(signal_a_b, and(and(b(a,looking_a),b(a,-looking_b)),and(looking_a,-looking_b))).
fully_obs(signal_a_b, a, true).
fully_obs(signal_a_b, b, true).


action(signal_a_c).
ontic_action(signal_a_c).
formula(-looking_c).
formula(and(looking_a,-looking_c)).
formula(b(a,-looking_c)).
formula(and(b(a,looking_a),b(a,-looking_c))).
formula(and(and(b(a,looking_a),b(a,-looking_c)),and(looking_a,-looking_c))).
executable(signal_a_c, and(and(b(a,looking_a),b(a,-looking_c)),and(looking_a,-looking_c))).
fully_obs(signal_a_c, a, true).
fully_obs(signal_a_c, c, true).


action(signal_b_a).
ontic_action(signal_b_a).
formula(-looking_a).
formula(and(-looking_a,looking_b)).
formula(b(b,-looking_a)).
formula(and(b(b,looking_b),b(b,-looking_a))).
formula(and(and(b(b,looking_b),b(b,-looking_a)),and(-looking_a,looking_b))).
executable(signal_b_a, and(and(b(b,looking_b),b(b,-looking_a)),and(-looking_a,looking_b))).
fully_obs(signal_b_a, a, true).
fully_obs(signal_b_a, b, true).


action(signal_b_c).
ontic_action(signal_b_c).
formula(and(looking_b,-looking_c)).
formula(b(b,-looking_c)).
formula(and(b(b,looking_b),b(b,-looking_c))).
formula(and(and(b(b,looking_b),b(b,-looking_c)),and(looking_b,-looking_c))).
executable(signal_b_c, and(and(b(b,looking_b),b(b,-looking_c)),and(looking_b,-looking_c))).
fully_obs(signal_b_c, b, true).
fully_obs(signal_b_c, c, true).


action(signal_c_a).
ontic_action(signal_c_a).
formula(and(-looking_a,looking_c)).
formula(b(c,-looking_a)).
formula(and(b(c,looking_c),b(c,-looking_a))).
formula(and(and(b(c,looking_c),b(c,-looking_a)),and(-looking_a,looking_c))).
executable(signal_c_a, and(and(b(c,looking_c),b(c,-looking_a)),and(-looking_a,looking_c))).
fully_obs(signal_c_a, a, true).
fully_obs(signal_c_a, c, true).


action(signal_c_b).
ontic_action(signal_c_b).
formula(and(-looking_b,looking_c)).
formula(b(c,-looking_b)).
formula(and(b(c,looking_c),b(c,-looking_b))).
formula(and(and(b(c,looking_c),b(c,-looking_b)),and(-looking_b,looking_c))).
executable(signal_c_b, and(and(b(c,looking_c),b(c,-looking_b)),and(-looking_b,looking_c))).
fully_obs(signal_c_b, b, true).
fully_obs(signal_c_b, c, true).




% *** INITIALLY ***

% ** initial pointed **
initially(has_key_a).
formula(-has_key_b).
initially(-has_key_b).
formula(-has_key_c).
initially(-has_key_c).
initially(looking_a).
initially(-looking_b).
initially(-looking_c).
formula(-opened).
initially(-opened).
initially(tail).

% ** initial beliefs **
agent_set(abc).
contains(abc,a).
contains(abc,b).
contains(abc,c).
formula(c(abc,has_key_a)).
initially(c(abc,has_key_a)).
formula(c(abc,-has_key_b)).
initially(c(abc,-has_key_b)).
formula(c(abc,-has_key_c)).
initially(c(abc,-has_key_c)).
formula(c(abc,-opened)).
initially(c(abc,-opened)).
formula(c(abc,looking_a)).
initially(c(abc,looking_a)).
formula(c(abc,-looking_b)).
initially(c(abc,-looking_b)).
formula(c(abc,-looking_c)).
initially(c(abc,-looking_c)).


% *** GOALS ***
goal(b(a,tail)).


% *** WORLDS ***
fluent_set(1).
holds(1, has_key_a).
holds(1, has_key_b).
holds(1, has_key_c).
holds(1, looking_a).
holds(1, looking_b).
holds(1, looking_c).
holds(1, opened).
holds(1, tail).

fluent_set(2).
holds(2, has_key_a).
holds(2, has_key_b).
holds(2, has_key_c).
holds(2, looking_a).
holds(2, looking_b).
holds(2, looking_c).
holds(2, opened).

fluent_set(3).
holds(3, has_key_a).
holds(3, has_key_b).
holds(3, has_key_c).
holds(3, looking_a).
holds(3, looking_b).
holds(3, looking_c).
holds(3, tail).

fluent_set(4).
holds(4, has_key_a).
holds(4, has_key_b).
holds(4, has_key_c).
holds(4, looking_a).
holds(4, looking_b).
holds(4, looking_c).

fluent_set(5).
holds(5, has_key_a).
holds(5, has_key_b).
holds(5, has_key_c).
holds(5, looking_a).
holds(5, looking_b).
holds(5, opened).
holds(5, tail).

fluent_set(6).
holds(6, has_key_a).
holds(6, has_key_b).
holds(6, has_key_c).
holds(6, looking_a).
holds(6, looking_b).
holds(6, opened).

fluent_set(7).
holds(7, has_key_a).
holds(7, has_key_b).
holds(7, has_key_c).
holds(7, looking_a).
holds(7, looking_b).
holds(7, tail).

fluent_set(8).
holds(8, has_key_a).
holds(8, has_key_b).
holds(8, has_key_c).
holds(8, looking_a).
holds(8, looking_b).

fluent_set(9).
holds(9, has_key_a).
holds(9, has_key_b).
holds(9, has_key_c).
holds(9, looking_a).
holds(9, looking_c).
holds(9, opened).
holds(9, tail).

fluent_set(10).
holds(10, has_key_a).
holds(10, has_key_b).
holds(10, has_key_c).
holds(10, looking_a).
holds(10, looking_c).
holds(10, opened).

fluent_set(11).
holds(11, has_key_a).
holds(11, has_key_b).
holds(11, has_key_c).
holds(11, looking_a).
holds(11, looking_c).
holds(11, tail).

fluent_set(12).
holds(12, has_key_a).
holds(12, has_key_b).
holds(12, has_key_c).
holds(12, looking_a).
holds(12, looking_c).

fluent_set(13).
holds(13, has_key_a).
holds(13, has_key_b).
holds(13, has_key_c).
holds(13, looking_a).
holds(13, opened).
holds(13, tail).

fluent_set(14).
holds(14, has_key_a).
holds(14, has_key_b).
holds(14, has_key_c).
holds(14, looking_a).
holds(14, opened).

fluent_set(15).
holds(15, has_key_a).
holds(15, has_key_b).
holds(15, has_key_c).
holds(15, looking_a).
holds(15, tail).

fluent_set(16).
holds(16, has_key_a).
holds(16, has_key_b).
holds(16, has_key_c).
holds(16, looking_a).

fluent_set(17).
holds(17, has_key_a).
holds(17, has_key_b).
holds(17, has_key_c).
holds(17, looking_b).
holds(17, looking_c).
holds(17, opened).
holds(17, tail).

fluent_set(18).
holds(18, has_key_a).
holds(18, has_key_b).
holds(18, has_key_c).
holds(18, looking_b).
holds(18, looking_c).
holds(18, opened).

fluent_set(19).
holds(19, has_key_a).
holds(19, has_key_b).
holds(19, has_key_c).
holds(19, looking_b).
holds(19, looking_c).
holds(19, tail).

fluent_set(20).
holds(20, has_key_a).
holds(20, has_key_b).
holds(20, has_key_c).
holds(20, looking_b).
holds(20, looking_c).

fluent_set(21).
holds(21, has_key_a).
holds(21, has_key_b).
holds(21, has_key_c).
holds(21, looking_b).
holds(21, opened).
holds(21, tail).

fluent_set(22).
holds(22, has_key_a).
holds(22, has_key_b).
holds(22, has_key_c).
holds(22, looking_b).
holds(22, opened).

fluent_set(23).
holds(23, has_key_a).
holds(23, has_key_b).
holds(23, has_key_c).
holds(23, looking_b).
holds(23, tail).

fluent_set(24).
holds(24, has_key_a).
holds(24, has_key_b).
holds(24, has_key_c).
holds(24, looking_b).

fluent_set(25).
holds(25, has_key_a).
holds(25, has_key_b).
holds(25, has_key_c).
holds(25, looking_c).
holds(25, opened).
holds(25, tail).

fluent_set(26).
holds(26, has_key_a).
holds(26, has_key_b).
holds(26, has_key_c).
holds(26, looking_c).
holds(26, opened).

fluent_set(27).
holds(27, has_key_a).
holds(27, has_key_b).
holds(27, has_key_c).
holds(27, looking_c).
holds(27, tail).

fluent_set(28).
holds(28, has_key_a).
holds(28, has_key_b).
holds(28, has_key_c).
holds(28, looking_c).

fluent_set(29).
holds(29, has_key_a).
holds(29, has_key_b).
holds(29, has_key_c).
holds(29, opened).
holds(29, tail).

fluent_set(30).
holds(30, has_key_a).
holds(30, has_key_b).
holds(30, has_key_c).
holds(30, opened).

fluent_set(31).
holds(31, has_key_a).
holds(31, has_key_b).
holds(31, has_key_c).
holds(31, tail).

fluent_set(32).
holds(32, has_key_a).
holds(32, has_key_b).
holds(32, has_key_c).

fluent_set(33).
holds(33, has_key_a).
holds(33, has_key_b).
holds(33, looking_a).
holds(33, looking_b).
holds(33, looking_c).
holds(33, opened).
holds(33, tail).

fluent_set(34).
holds(34, has_key_a).
holds(34, has_key_b).
holds(34, looking_a).
holds(34, looking_b).
holds(34, looking_c).
holds(34, opened).

fluent_set(35).
holds(35, has_key_a).
holds(35, has_key_b).
holds(35, looking_a).
holds(35, looking_b).
holds(35, looking_c).
holds(35, tail).

fluent_set(36).
holds(36, has_key_a).
holds(36, has_key_b).
holds(36, looking_a).
holds(36, looking_b).
holds(36, looking_c).

fluent_set(37).
holds(37, has_key_a).
holds(37, has_key_b).
holds(37, looking_a).
holds(37, looking_b).
holds(37, opened).
holds(37, tail).

fluent_set(38).
holds(38, has_key_a).
holds(38, has_key_b).
holds(38, looking_a).
holds(38, looking_b).
holds(38, opened).

fluent_set(39).
holds(39, has_key_a).
holds(39, has_key_b).
holds(39, looking_a).
holds(39, looking_b).
holds(39, tail).

fluent_set(40).
holds(40, has_key_a).
holds(40, has_key_b).
holds(40, looking_a).
holds(40, looking_b).

fluent_set(41).
holds(41, has_key_a).
holds(41, has_key_b).
holds(41, looking_a).
holds(41, looking_c).
holds(41, opened).
holds(41, tail).

fluent_set(42).
holds(42, has_key_a).
holds(42, has_key_b).
holds(42, looking_a).
holds(42, looking_c).
holds(42, opened).

fluent_set(43).
holds(43, has_key_a).
holds(43, has_key_b).
holds(43, looking_a).
holds(43, looking_c).
holds(43, tail).

fluent_set(44).
holds(44, has_key_a).
holds(44, has_key_b).
holds(44, looking_a).
holds(44, looking_c).

fluent_set(45).
holds(45, has_key_a).
holds(45, has_key_b).
holds(45, looking_a).
holds(45, opened).
holds(45, tail).

fluent_set(46).
holds(46, has_key_a).
holds(46, has_key_b).
holds(46, looking_a).
holds(46, opened).

fluent_set(47).
holds(47, has_key_a).
holds(47, has_key_b).
holds(47, looking_a).
holds(47, tail).

fluent_set(48).
holds(48, has_key_a).
holds(48, has_key_b).
holds(48, looking_a).

fluent_set(49).
holds(49, has_key_a).
holds(49, has_key_b).
holds(49, looking_b).
holds(49, looking_c).
holds(49, opened).
holds(49, tail).

fluent_set(50).
holds(50, has_key_a).
holds(50, has_key_b).
holds(50, looking_b).
holds(50, looking_c).
holds(50, opened).

fluent_set(51).
holds(51, has_key_a).
holds(51, has_key_b).
holds(51, looking_b).
holds(51, looking_c).
holds(51, tail).

fluent_set(52).
holds(52, has_key_a).
holds(52, has_key_b).
holds(52, looking_b).
holds(52, looking_c).

fluent_set(53).
holds(53, has_key_a).
holds(53, has_key_b).
holds(53, looking_b).
holds(53, opened).
holds(53, tail).

fluent_set(54).
holds(54, has_key_a).
holds(54, has_key_b).
holds(54, looking_b).
holds(54, opened).

fluent_set(55).
holds(55, has_key_a).
holds(55, has_key_b).
holds(55, looking_b).
holds(55, tail).

fluent_set(56).
holds(56, has_key_a).
holds(56, has_key_b).
holds(56, looking_b).

fluent_set(57).
holds(57, has_key_a).
holds(57, has_key_b).
holds(57, looking_c).
holds(57, opened).
holds(57, tail).

fluent_set(58).
holds(58, has_key_a).
holds(58, has_key_b).
holds(58, looking_c).
holds(58, opened).

fluent_set(59).
holds(59, has_key_a).
holds(59, has_key_b).
holds(59, looking_c).
holds(59, tail).

fluent_set(60).
holds(60, has_key_a).
holds(60, has_key_b).
holds(60, looking_c).

fluent_set(61).
holds(61, has_key_a).
holds(61, has_key_b).
holds(61, opened).
holds(61, tail).

fluent_set(62).
holds(62, has_key_a).
holds(62, has_key_b).
holds(62, opened).

fluent_set(63).
holds(63, has_key_a).
holds(63, has_key_b).
holds(63, tail).

fluent_set(64).
holds(64, has_key_a).
holds(64, has_key_b).

fluent_set(65).
holds(65, has_key_a).
holds(65, has_key_c).
holds(65, looking_a).
holds(65, looking_b).
holds(65, looking_c).
holds(65, opened).
holds(65, tail).

fluent_set(66).
holds(66, has_key_a).
holds(66, has_key_c).
holds(66, looking_a).
holds(66, looking_b).
holds(66, looking_c).
holds(66, opened).

fluent_set(67).
holds(67, has_key_a).
holds(67, has_key_c).
holds(67, looking_a).
holds(67, looking_b).
holds(67, looking_c).
holds(67, tail).

fluent_set(68).
holds(68, has_key_a).
holds(68, has_key_c).
holds(68, looking_a).
holds(68, looking_b).
holds(68, looking_c).

fluent_set(69).
holds(69, has_key_a).
holds(69, has_key_c).
holds(69, looking_a).
holds(69, looking_b).
holds(69, opened).
holds(69, tail).

fluent_set(70).
holds(70, has_key_a).
holds(70, has_key_c).
holds(70, looking_a).
holds(70, looking_b).
holds(70, opened).

fluent_set(71).
holds(71, has_key_a).
holds(71, has_key_c).
holds(71, looking_a).
holds(71, looking_b).
holds(71, tail).

fluent_set(72).
holds(72, has_key_a).
holds(72, has_key_c).
holds(72, looking_a).
holds(72, looking_b).

fluent_set(73).
holds(73, has_key_a).
holds(73, has_key_c).
holds(73, looking_a).
holds(73, looking_c).
holds(73, opened).
holds(73, tail).

fluent_set(74).
holds(74, has_key_a).
holds(74, has_key_c).
holds(74, looking_a).
holds(74, looking_c).
holds(74, opened).

fluent_set(75).
holds(75, has_key_a).
holds(75, has_key_c).
holds(75, looking_a).
holds(75, looking_c).
holds(75, tail).

fluent_set(76).
holds(76, has_key_a).
holds(76, has_key_c).
holds(76, looking_a).
holds(76, looking_c).

fluent_set(77).
holds(77, has_key_a).
holds(77, has_key_c).
holds(77, looking_a).
holds(77, opened).
holds(77, tail).

fluent_set(78).
holds(78, has_key_a).
holds(78, has_key_c).
holds(78, looking_a).
holds(78, opened).

fluent_set(79).
holds(79, has_key_a).
holds(79, has_key_c).
holds(79, looking_a).
holds(79, tail).

fluent_set(80).
holds(80, has_key_a).
holds(80, has_key_c).
holds(80, looking_a).

fluent_set(81).
holds(81, has_key_a).
holds(81, has_key_c).
holds(81, looking_b).
holds(81, looking_c).
holds(81, opened).
holds(81, tail).

fluent_set(82).
holds(82, has_key_a).
holds(82, has_key_c).
holds(82, looking_b).
holds(82, looking_c).
holds(82, opened).

fluent_set(83).
holds(83, has_key_a).
holds(83, has_key_c).
holds(83, looking_b).
holds(83, looking_c).
holds(83, tail).

fluent_set(84).
holds(84, has_key_a).
holds(84, has_key_c).
holds(84, looking_b).
holds(84, looking_c).

fluent_set(85).
holds(85, has_key_a).
holds(85, has_key_c).
holds(85, looking_b).
holds(85, opened).
holds(85, tail).

fluent_set(86).
holds(86, has_key_a).
holds(86, has_key_c).
holds(86, looking_b).
holds(86, opened).

fluent_set(87).
holds(87, has_key_a).
holds(87, has_key_c).
holds(87, looking_b).
holds(87, tail).

fluent_set(88).
holds(88, has_key_a).
holds(88, has_key_c).
holds(88, looking_b).

fluent_set(89).
holds(89, has_key_a).
holds(89, has_key_c).
holds(89, looking_c).
holds(89, opened).
holds(89, tail).

fluent_set(90).
holds(90, has_key_a).
holds(90, has_key_c).
holds(90, looking_c).
holds(90, opened).

fluent_set(91).
holds(91, has_key_a).
holds(91, has_key_c).
holds(91, looking_c).
holds(91, tail).

fluent_set(92).
holds(92, has_key_a).
holds(92, has_key_c).
holds(92, looking_c).

fluent_set(93).
holds(93, has_key_a).
holds(93, has_key_c).
holds(93, opened).
holds(93, tail).

fluent_set(94).
holds(94, has_key_a).
holds(94, has_key_c).
holds(94, opened).

fluent_set(95).
holds(95, has_key_a).
holds(95, has_key_c).
holds(95, tail).

fluent_set(96).
holds(96, has_key_a).
holds(96, has_key_c).

fluent_set(97).
holds(97, has_key_a).
holds(97, looking_a).
holds(97, looking_b).
holds(97, looking_c).
holds(97, opened).
holds(97, tail).

fluent_set(98).
holds(98, has_key_a).
holds(98, looking_a).
holds(98, looking_b).
holds(98, looking_c).
holds(98, opened).

fluent_set(99).
holds(99, has_key_a).
holds(99, looking_a).
holds(99, looking_b).
holds(99, looking_c).
holds(99, tail).

fluent_set(100).
holds(100, has_key_a).
holds(100, looking_a).
holds(100, looking_b).
holds(100, looking_c).

fluent_set(101).
holds(101, has_key_a).
holds(101, looking_a).
holds(101, looking_b).
holds(101, opened).
holds(101, tail).

fluent_set(102).
holds(102, has_key_a).
holds(102, looking_a).
holds(102, looking_b).
holds(102, opened).

fluent_set(103).
holds(103, has_key_a).
holds(103, looking_a).
holds(103, looking_b).
holds(103, tail).

fluent_set(104).
holds(104, has_key_a).
holds(104, looking_a).
holds(104, looking_b).

fluent_set(105).
holds(105, has_key_a).
holds(105, looking_a).
holds(105, looking_c).
holds(105, opened).
holds(105, tail).

fluent_set(106).
holds(106, has_key_a).
holds(106, looking_a).
holds(106, looking_c).
holds(106, opened).

fluent_set(107).
holds(107, has_key_a).
holds(107, looking_a).
holds(107, looking_c).
holds(107, tail).

fluent_set(108).
holds(108, has_key_a).
holds(108, looking_a).
holds(108, looking_c).

fluent_set(109).
holds(109, has_key_a).
holds(109, looking_a).
holds(109, opened).
holds(109, tail).

fluent_set(110).
holds(110, has_key_a).
holds(110, looking_a).
holds(110, opened).

fluent_set(111).
holds(111, has_key_a).
holds(111, looking_a).
holds(111, tail).

fluent_set(112).
holds(112, has_key_a).
holds(112, looking_a).

fluent_set(113).
holds(113, has_key_a).
holds(113, looking_b).
holds(113, looking_c).
holds(113, opened).
holds(113, tail).

fluent_set(114).
holds(114, has_key_a).
holds(114, looking_b).
holds(114, looking_c).
holds(114, opened).

fluent_set(115).
holds(115, has_key_a).
holds(115, looking_b).
holds(115, looking_c).
holds(115, tail).

fluent_set(116).
holds(116, has_key_a).
holds(116, looking_b).
holds(116, looking_c).

fluent_set(117).
holds(117, has_key_a).
holds(117, looking_b).
holds(117, opened).
holds(117, tail).

fluent_set(118).
holds(118, has_key_a).
holds(118, looking_b).
holds(118, opened).

fluent_set(119).
holds(119, has_key_a).
holds(119, looking_b).
holds(119, tail).

fluent_set(120).
holds(120, has_key_a).
holds(120, looking_b).

fluent_set(121).
holds(121, has_key_a).
holds(121, looking_c).
holds(121, opened).
holds(121, tail).

fluent_set(122).
holds(122, has_key_a).
holds(122, looking_c).
holds(122, opened).

fluent_set(123).
holds(123, has_key_a).
holds(123, looking_c).
holds(123, tail).

fluent_set(124).
holds(124, has_key_a).
holds(124, looking_c).

fluent_set(125).
holds(125, has_key_a).
holds(125, opened).
holds(125, tail).

fluent_set(126).
holds(126, has_key_a).
holds(126, opened).

fluent_set(127).
holds(127, has_key_a).
holds(127, tail).

fluent_set(128).
holds(128, has_key_a).

fluent_set(129).
holds(129, has_key_b).
holds(129, has_key_c).
holds(129, looking_a).
holds(129, looking_b).
holds(129, looking_c).
holds(129, opened).
holds(129, tail).

fluent_set(130).
holds(130, has_key_b).
holds(130, has_key_c).
holds(130, looking_a).
holds(130, looking_b).
holds(130, looking_c).
holds(130, opened).

fluent_set(131).
holds(131, has_key_b).
holds(131, has_key_c).
holds(131, looking_a).
holds(131, looking_b).
holds(131, looking_c).
holds(131, tail).

fluent_set(132).
holds(132, has_key_b).
holds(132, has_key_c).
holds(132, looking_a).
holds(132, looking_b).
holds(132, looking_c).

fluent_set(133).
holds(133, has_key_b).
holds(133, has_key_c).
holds(133, looking_a).
holds(133, looking_b).
holds(133, opened).
holds(133, tail).

fluent_set(134).
holds(134, has_key_b).
holds(134, has_key_c).
holds(134, looking_a).
holds(134, looking_b).
holds(134, opened).

fluent_set(135).
holds(135, has_key_b).
holds(135, has_key_c).
holds(135, looking_a).
holds(135, looking_b).
holds(135, tail).

fluent_set(136).
holds(136, has_key_b).
holds(136, has_key_c).
holds(136, looking_a).
holds(136, looking_b).

fluent_set(137).
holds(137, has_key_b).
holds(137, has_key_c).
holds(137, looking_a).
holds(137, looking_c).
holds(137, opened).
holds(137, tail).

fluent_set(138).
holds(138, has_key_b).
holds(138, has_key_c).
holds(138, looking_a).
holds(138, looking_c).
holds(138, opened).

fluent_set(139).
holds(139, has_key_b).
holds(139, has_key_c).
holds(139, looking_a).
holds(139, looking_c).
holds(139, tail).

fluent_set(140).
holds(140, has_key_b).
holds(140, has_key_c).
holds(140, looking_a).
holds(140, looking_c).

fluent_set(141).
holds(141, has_key_b).
holds(141, has_key_c).
holds(141, looking_a).
holds(141, opened).
holds(141, tail).

fluent_set(142).
holds(142, has_key_b).
holds(142, has_key_c).
holds(142, looking_a).
holds(142, opened).

fluent_set(143).
holds(143, has_key_b).
holds(143, has_key_c).
holds(143, looking_a).
holds(143, tail).

fluent_set(144).
holds(144, has_key_b).
holds(144, has_key_c).
holds(144, looking_a).

fluent_set(145).
holds(145, has_key_b).
holds(145, has_key_c).
holds(145, looking_b).
holds(145, looking_c).
holds(145, opened).
holds(145, tail).

fluent_set(146).
holds(146, has_key_b).
holds(146, has_key_c).
holds(146, looking_b).
holds(146, looking_c).
holds(146, opened).

fluent_set(147).
holds(147, has_key_b).
holds(147, has_key_c).
holds(147, looking_b).
holds(147, looking_c).
holds(147, tail).

fluent_set(148).
holds(148, has_key_b).
holds(148, has_key_c).
holds(148, looking_b).
holds(148, looking_c).

fluent_set(149).
holds(149, has_key_b).
holds(149, has_key_c).
holds(149, looking_b).
holds(149, opened).
holds(149, tail).

fluent_set(150).
holds(150, has_key_b).
holds(150, has_key_c).
holds(150, looking_b).
holds(150, opened).

fluent_set(151).
holds(151, has_key_b).
holds(151, has_key_c).
holds(151, looking_b).
holds(151, tail).

fluent_set(152).
holds(152, has_key_b).
holds(152, has_key_c).
holds(152, looking_b).

fluent_set(153).
holds(153, has_key_b).
holds(153, has_key_c).
holds(153, looking_c).
holds(153, opened).
holds(153, tail).

fluent_set(154).
holds(154, has_key_b).
holds(154, has_key_c).
holds(154, looking_c).
holds(154, opened).

fluent_set(155).
holds(155, has_key_b).
holds(155, has_key_c).
holds(155, looking_c).
holds(155, tail).

fluent_set(156).
holds(156, has_key_b).
holds(156, has_key_c).
holds(156, looking_c).

fluent_set(157).
holds(157, has_key_b).
holds(157, has_key_c).
holds(157, opened).
holds(157, tail).

fluent_set(158).
holds(158, has_key_b).
holds(158, has_key_c).
holds(158, opened).

fluent_set(159).
holds(159, has_key_b).
holds(159, has_key_c).
holds(159, tail).

fluent_set(160).
holds(160, has_key_b).
holds(160, has_key_c).

fluent_set(161).
holds(161, has_key_b).
holds(161, looking_a).
holds(161, looking_b).
holds(161, looking_c).
holds(161, opened).
holds(161, tail).

fluent_set(162).
holds(162, has_key_b).
holds(162, looking_a).
holds(162, looking_b).
holds(162, looking_c).
holds(162, opened).

fluent_set(163).
holds(163, has_key_b).
holds(163, looking_a).
holds(163, looking_b).
holds(163, looking_c).
holds(163, tail).

fluent_set(164).
holds(164, has_key_b).
holds(164, looking_a).
holds(164, looking_b).
holds(164, looking_c).

fluent_set(165).
holds(165, has_key_b).
holds(165, looking_a).
holds(165, looking_b).
holds(165, opened).
holds(165, tail).

fluent_set(166).
holds(166, has_key_b).
holds(166, looking_a).
holds(166, looking_b).
holds(166, opened).

fluent_set(167).
holds(167, has_key_b).
holds(167, looking_a).
holds(167, looking_b).
holds(167, tail).

fluent_set(168).
holds(168, has_key_b).
holds(168, looking_a).
holds(168, looking_b).

fluent_set(169).
holds(169, has_key_b).
holds(169, looking_a).
holds(169, looking_c).
holds(169, opened).
holds(169, tail).

fluent_set(170).
holds(170, has_key_b).
holds(170, looking_a).
holds(170, looking_c).
holds(170, opened).

fluent_set(171).
holds(171, has_key_b).
holds(171, looking_a).
holds(171, looking_c).
holds(171, tail).

fluent_set(172).
holds(172, has_key_b).
holds(172, looking_a).
holds(172, looking_c).

fluent_set(173).
holds(173, has_key_b).
holds(173, looking_a).
holds(173, opened).
holds(173, tail).

fluent_set(174).
holds(174, has_key_b).
holds(174, looking_a).
holds(174, opened).

fluent_set(175).
holds(175, has_key_b).
holds(175, looking_a).
holds(175, tail).

fluent_set(176).
holds(176, has_key_b).
holds(176, looking_a).

fluent_set(177).
holds(177, has_key_b).
holds(177, looking_b).
holds(177, looking_c).
holds(177, opened).
holds(177, tail).

fluent_set(178).
holds(178, has_key_b).
holds(178, looking_b).
holds(178, looking_c).
holds(178, opened).

fluent_set(179).
holds(179, has_key_b).
holds(179, looking_b).
holds(179, looking_c).
holds(179, tail).

fluent_set(180).
holds(180, has_key_b).
holds(180, looking_b).
holds(180, looking_c).

fluent_set(181).
holds(181, has_key_b).
holds(181, looking_b).
holds(181, opened).
holds(181, tail).

fluent_set(182).
holds(182, has_key_b).
holds(182, looking_b).
holds(182, opened).

fluent_set(183).
holds(183, has_key_b).
holds(183, looking_b).
holds(183, tail).

fluent_set(184).
holds(184, has_key_b).
holds(184, looking_b).

fluent_set(185).
holds(185, has_key_b).
holds(185, looking_c).
holds(185, opened).
holds(185, tail).

fluent_set(186).
holds(186, has_key_b).
holds(186, looking_c).
holds(186, opened).

fluent_set(187).
holds(187, has_key_b).
holds(187, looking_c).
holds(187, tail).

fluent_set(188).
holds(188, has_key_b).
holds(188, looking_c).

fluent_set(189).
holds(189, has_key_b).
holds(189, opened).
holds(189, tail).

fluent_set(190).
holds(190, has_key_b).
holds(190, opened).

fluent_set(191).
holds(191, has_key_b).
holds(191, tail).

fluent_set(192).
holds(192, has_key_b).

fluent_set(193).
holds(193, has_key_c).
holds(193, looking_a).
holds(193, looking_b).
holds(193, looking_c).
holds(193, opened).
holds(193, tail).

fluent_set(194).
holds(194, has_key_c).
holds(194, looking_a).
holds(194, looking_b).
holds(194, looking_c).
holds(194, opened).

fluent_set(195).
holds(195, has_key_c).
holds(195, looking_a).
holds(195, looking_b).
holds(195, looking_c).
holds(195, tail).

fluent_set(196).
holds(196, has_key_c).
holds(196, looking_a).
holds(196, looking_b).
holds(196, looking_c).

fluent_set(197).
holds(197, has_key_c).
holds(197, looking_a).
holds(197, looking_b).
holds(197, opened).
holds(197, tail).

fluent_set(198).
holds(198, has_key_c).
holds(198, looking_a).
holds(198, looking_b).
holds(198, opened).

fluent_set(199).
holds(199, has_key_c).
holds(199, looking_a).
holds(199, looking_b).
holds(199, tail).

fluent_set(200).
holds(200, has_key_c).
holds(200, looking_a).
holds(200, looking_b).

fluent_set(201).
holds(201, has_key_c).
holds(201, looking_a).
holds(201, looking_c).
holds(201, opened).
holds(201, tail).

fluent_set(202).
holds(202, has_key_c).
holds(202, looking_a).
holds(202, looking_c).
holds(202, opened).

fluent_set(203).
holds(203, has_key_c).
holds(203, looking_a).
holds(203, looking_c).
holds(203, tail).

fluent_set(204).
holds(204, has_key_c).
holds(204, looking_a).
holds(204, looking_c).

fluent_set(205).
holds(205, has_key_c).
holds(205, looking_a).
holds(205, opened).
holds(205, tail).

fluent_set(206).
holds(206, has_key_c).
holds(206, looking_a).
holds(206, opened).

fluent_set(207).
holds(207, has_key_c).
holds(207, looking_a).
holds(207, tail).

fluent_set(208).
holds(208, has_key_c).
holds(208, looking_a).

fluent_set(209).
holds(209, has_key_c).
holds(209, looking_b).
holds(209, looking_c).
holds(209, opened).
holds(209, tail).

fluent_set(210).
holds(210, has_key_c).
holds(210, looking_b).
holds(210, looking_c).
holds(210, opened).

fluent_set(211).
holds(211, has_key_c).
holds(211, looking_b).
holds(211, looking_c).
holds(211, tail).

fluent_set(212).
holds(212, has_key_c).
holds(212, looking_b).
holds(212, looking_c).

fluent_set(213).
holds(213, has_key_c).
holds(213, looking_b).
holds(213, opened).
holds(213, tail).

fluent_set(214).
holds(214, has_key_c).
holds(214, looking_b).
holds(214, opened).

fluent_set(215).
holds(215, has_key_c).
holds(215, looking_b).
holds(215, tail).

fluent_set(216).
holds(216, has_key_c).
holds(216, looking_b).

fluent_set(217).
holds(217, has_key_c).
holds(217, looking_c).
holds(217, opened).
holds(217, tail).

fluent_set(218).
holds(218, has_key_c).
holds(218, looking_c).
holds(218, opened).

fluent_set(219).
holds(219, has_key_c).
holds(219, looking_c).
holds(219, tail).

fluent_set(220).
holds(220, has_key_c).
holds(220, looking_c).

fluent_set(221).
holds(221, has_key_c).
holds(221, opened).
holds(221, tail).

fluent_set(222).
holds(222, has_key_c).
holds(222, opened).

fluent_set(223).
holds(223, has_key_c).
holds(223, tail).

fluent_set(224).
holds(224, has_key_c).

fluent_set(225).
holds(225, looking_a).
holds(225, looking_b).
holds(225, looking_c).
holds(225, opened).
holds(225, tail).

fluent_set(226).
holds(226, looking_a).
holds(226, looking_b).
holds(226, looking_c).
holds(226, opened).

fluent_set(227).
holds(227, looking_a).
holds(227, looking_b).
holds(227, looking_c).
holds(227, tail).

fluent_set(228).
holds(228, looking_a).
holds(228, looking_b).
holds(228, looking_c).

fluent_set(229).
holds(229, looking_a).
holds(229, looking_b).
holds(229, opened).
holds(229, tail).

fluent_set(230).
holds(230, looking_a).
holds(230, looking_b).
holds(230, opened).

fluent_set(231).
holds(231, looking_a).
holds(231, looking_b).
holds(231, tail).

fluent_set(232).
holds(232, looking_a).
holds(232, looking_b).

fluent_set(233).
holds(233, looking_a).
holds(233, looking_c).
holds(233, opened).
holds(233, tail).

fluent_set(234).
holds(234, looking_a).
holds(234, looking_c).
holds(234, opened).

fluent_set(235).
holds(235, looking_a).
holds(235, looking_c).
holds(235, tail).

fluent_set(236).
holds(236, looking_a).
holds(236, looking_c).

fluent_set(237).
holds(237, looking_a).
holds(237, opened).
holds(237, tail).

fluent_set(238).
holds(238, looking_a).
holds(238, opened).

fluent_set(239).
holds(239, looking_a).
holds(239, tail).

fluent_set(240).
holds(240, looking_a).

fluent_set(241).
holds(241, looking_b).
holds(241, looking_c).
holds(241, opened).
holds(241, tail).

fluent_set(242).
holds(242, looking_b).
holds(242, looking_c).
holds(242, opened).

fluent_set(243).
holds(243, looking_b).
holds(243, looking_c).
holds(243, tail).

fluent_set(244).
holds(244, looking_b).
holds(244, looking_c).

fluent_set(245).
holds(245, looking_b).
holds(245, opened).
holds(245, tail).

fluent_set(246).
holds(246, looking_b).
holds(246, opened).

fluent_set(247).
holds(247, looking_b).
holds(247, tail).

fluent_set(248).
holds(248, looking_b).

fluent_set(249).
holds(249, looking_c).
holds(249, opened).
holds(249, tail).

fluent_set(250).
holds(250, looking_c).
holds(250, opened).

fluent_set(251).
holds(251, looking_c).
holds(251, tail).

fluent_set(252).
holds(252, looking_c).

fluent_set(253).
holds(253, opened).
holds(253, tail).

fluent_set(254).
holds(254, opened).

fluent_set(255).
holds(255, tail).

fluent_set(256).


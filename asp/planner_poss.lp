%
% EMP encoding in ASP
%

% ********** REACHABILITY **********

reaches(P1, P2, AGS) :- believes(P1, P2, AG), contains_ag(AGS, AG).
reaches(P1, P2, AGS) :- believes(P1, P3, AG), contains_ag(AGS, AG), reaches(P3, P2, AGS).



% ********** ENTAILMENT **********

formula(F) :- fluent(F).        % A fluent is a formula
% formula(F) :- initially(F).     % An initial condition is a formula
% formula(F) :- goal(F).          % A goal condition is a formula

holds(W, true) :- world(W).
% holds(P, F   ) :- possible_world(P, W), holds(W, F), world(W), fluent(F).

entails(T, F) :- entails(P, F), pointed(T, P), formula(F).

entails(P, true  )      :-                                           possibility(P).
entails(P,  F    )      :-     holds(W, F), possible_world(P, W), world(W), fluent(F).
entails(P, -F    )      :- not holds(W, F), possible_world(P, W), world(W), fluent(F).

entails(P, neg(F))      :- not entails(P, F ),                 possibility(P), formula(F).
entails(P, and(F1, F2)) :-     entails(P, F1), entails(P, F2), possibility(P), formula(F1), formula(F2).
entails(P,  or(F1, F2)) :-     entails(P, F1),                 possibility(P), formula(F1), formula(F2).
entails(P,  or(F1, F2)) :-                     entails(P, F2), possibility(P), formula(F1), formula(F2).

% b operator
entails(P1, b(AG , F))  :- 0 { not entails(P2, F) : believes(P1, P2, AG) } 0, possibility(P1), formula(b(AG, F)), formula(F).

% not_entails(P1, b(AG , F)) :- not     entails(P2, F        ), believes(P1, P2, AG), possibility(P2), formula(F), agent(AG).
% entails    (P , b(AG , F)) :- not not_entails(P , b(AG , F)),                       possibility(P ), formula(b(AG , F)).

% c operator
entails(P1, c(AGS, F))  :- 0 { not entails(P2, F) : reaches(P1, P2, AGS) } 0, possibility(P1), formula(c(AGS, F)), formula(F).

% not_entails(P1, c(AGS, F)) :- not     entails(P2, F        ), reaches(P1, P2, AGS), possibility(P2), formula(F).
% entails    (P , c(AGS, F)) :- not not_entails(P , c(AGS, F)),                       possibility(P ), formula(c(AGS, F)).



% ********** INITIAL STATE **********

% Since we assume to deal with FINITARY S5-states, we only allow the initial conditions initially(F), where F has one of the following forms:
%  1. Phi (propositional formula)
%  2. C_all( B_i(Phi))
%  3. C_all( B_i(Phi) \/  B_i(-Phi))
%  4. C_all(-B_i(Phi) /\ -B_i(-Phi)) (this case doesn't add knowledge)

% The initial conditions of form 1. determine the pointed possibility of the initial state
% A possibility is pointed if it entails exactly those fluents that hold initially

no_good_initial_pointed(W) :- not holds(W, F), world(W), initially( F), fluent(F).
no_good_initial_pointed(W) :-     holds(W, F), world(W), initially(-F), fluent(F).

pointed(0, P) :- possible_initial_world(P, W), not no_good_initial_pointed(W), world(W).

% The initial conditions of form 2. determine the possible worlds of the initial state
% A possibility is good if it entails all the propositional formulas which are COMMONLY KNOWN initially (2.)
no_good_initial_world(W) :- not holds(W, F), world(W), initially(c(AGS,  F)), agent_set(AGS), fluent(F).
no_good_initial_world(W) :-     holds(W, F), world(W), initially(c(AGS, -F)), agent_set(AGS), fluent(F).

good_initial_world(W) :- not no_good_initial_world(W), world(W).

% We force the initial possibilities to be numbered from 1 to K, where K is the number of the initial possible worlds
1 { possible_initial_world(P, W) : possibility(P), P <= K } 1 :- good_initial_world(W), K = { good_initial_world(W1) : world(W1) }.
possible_world(P1, W) :- possible_initial_world(P1, W).

% The initial conditions of form 3. filter out some of the agents' beliefs inside the initial state
not_believes_initially(P1, P2, AG) :- not entails(P1, F),     entails(P2, F), possibility(P1), possibility(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).
not_believes_initially(P1, P2, AG) :-     entails(P1, F), not entails(P2, F), possibility(P1), possibility(P2), initially(c(all_agents, or(b(AG, F), b(AG, -F)))).

believes(P1, P2, AG) :- not not_believes_initially(P1, P2, AG), possible_initial_world(P1, W1), possible_initial_world(P2, W2),
                        not no_good_initial_world(W1), not no_good_initial_world(W2), agent(AG).

% ********** TRANSITION FUNCTION **********

is_executable(T, ACT) :- plan(T, ACT), entails(T, EXEC), executable(ACT, EXEC).

%  is_fully_obs(0, AG) :- agent(AG).
% -is_oblivious(0, AG) :- agent(AG).

is_fully_obs(T ,AG) :- is_executable(T, ACT), ontic_action(ACT), fully_obs(ACT, AG, OBS), agent(AG),     holds(WS, OBS), pointed(T, PS), possible_world(PS, WS).
is_oblivious(T, AG) :- is_executable(T, ACT), ontic_action(ACT), fully_obs(ACT, AG, OBS), agent(AG), not holds(WS, OBS), pointed(T, PS), possible_world(PS, WS).
is_oblivious(T, AG) :- is_executable(T, ACT), ontic_action(ACT), oblivious(ACT, AG).

fully_obs_agents(T+1, AGS) :- is_executable(T, ACT), ontic_action(ACT), agent_set(AGS),
                              0 { is_oblivious(T, AG) :     contains_ag(AGS, AG)            } 0,
                              0 { is_fully_obs(T, AG) : not contains_ag(AGS, AG), agent(AG) } 0.

holds_except(W1, W2,  F) :- world(W1), world(W2),     fluent(F), W1 != W2,
                            0 { fluent(F2) : F2 != F,     holds(W1, F2), not holds(W2, F2) } 0,
                            0 { fluent(F2) : F2 != F, not holds(W1, F2),     holds(W2, F2) } 0.

% holds_except(W1, W2, -F) :- world(W1), world(W2), not holds(W1, F), fluent(F), W1 != W2,
%                             0 { fluent(F2) : F2 != F,     holds(W1, F2), not holds(W2, F2) } 0,
%                             0 { fluent(F2) : F2 != F, not holds(W1, F2),     holds(W2, F2) } 0.

% The predicate holds if P1 = phi_D(P2, ACT) s.t. plan(T, ACT)
% 0 { phi(P1, P2, T, ACT) : possibility(P1) } 1 :- plan(T, ACT), possibility(P2).

% :- plan(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED), W1 != W2,
%   holds_except(W1, W2, CAUSED). %, possible_world(P1, W1), possible_world(P2, W2).
% 0 { phi(P1, P2, T, ACT) : possibility(P1), possibility(P2) } 1 :- plan(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED), W1 != W2,
                                                                %   holds_except(W1, W2, CAUSED). %, possible_world(P1, W1), possible_world(P2, W2).




same_path(FROM1, TO1, FROM2, TO2) :- believes(FROM1, TO1,   AG), believes(FROM2, TO2,   AG), FROM1 != TO1,   FROM2 != TO2.
same_path(FROM1, TO1, FROM2, TO2) :- believes(FROM1, FROM3, AG), believes(FROM2, FROM4, AG), FROM1 != FROM3, FROM2 != FROM4,
                                     same_path(FROM3, TO1, FROM4, TO2).

% phi(P1, P2, T, ACT) :- is_executable(T, ACT), ontic_action(ACT), pointed(T+1, P1), pointed(T, P2).

1 { phi(P1, P2, T, ACT) : possibility(P1), P1 > P2 } 1 :- is_executable(T, ACT), ontic_action(ACT), pointed(T, PP), believes(PP, P2, AG), %* P2 >= PP, *% is_fully_obs(T, AG).

% phi(P1, P2, T, ACT) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
%                        pointed(T, PP), believes(PP, P2, AG), is_fully_obs(T+1, AG),
%                        holds_except(W1, W2, CAUSED), possible_world(P1, W1), possible_world(P2, W2).

% phi(P1, P2, T, ACT) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
%                        pointed(T+1, PP1), pointed(T, PP2), same_path(PP1, P3, PP2, P4), phi(P3, P4, T, ACT),
%                        believes(P4, P2, AG), is_fully_obs(T, AG), holds_except(W1, W2, CAUSED), possible_world(P1, W1), possible_world(P2, W2).

% phi(P1, P2, T, ACT) :- plan(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
%                        pointed(T, PP), reaches(PP, P2, AGS), fully_obs_agents(T, AGS),
%                        holds_except(W1, W2, -CAUSED), possible_world(P1, W1), possible_world(P2, W2).

% possible_world(P1, W1) :- plan(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
%                           holds_except(W1, W2,  CAUSED), phi(P1, P2, T, ACT), possible_world(P2, W2).

% possible_world(P2, W2) :- plan(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
%                           holds_except(W1, W2,  CAUSED), phi(P1, P2, T, ACT), possible_world(P1, W1).

% possible_world(P1, W1) :- plan(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED),
%                           holds_except(W1, W2, -CAUSED), phi(P1, P2, T, ACT), possible_world(P2, W2).

% possible_world(P2, W2) :- plan(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED),
%                           holds_except(W1, W2, -CAUSED), phi(P1, P2, T, ACT), possible_world(P1, W1).

% ONTIC ACTIONS

% 1 { possible_world(P, W) : world(W) } 1 :- possibility(P).

possible_world(P1, W1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
                          holds_except(W1, W2, CAUSED),     holds(W1, CAUSED), phi(P1, P2, T, ACT), possible_world(P2, W2).

possible_world(P1, W1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED),
                          holds_except(W1, W2, CAUSED), not holds(W1, CAUSED), phi(P1, P2, T, ACT), possible_world(P2, W2).


% possible_world(P1, W1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED),
%                           pointed(T, PP), believes(PP, P2, AG), is_fully_obs(T+1, AG),
%                           holds_except(W1, W2, CAUSED), possible_world(P1, W1), possible_world(P2, W2),     holds(W1, CAUSED).

% possible_world(P1, W1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED),
%                           pointed(T, PP), believes(PP, P2, AG), is_fully_obs(T+1, AG),
%                           holds_except(W1, W2, CAUSED), possible_world(P1, W1), possible_world(P2, W2), not holds(W1, CAUSED).

% #minimize{ P : possible_world(P, W) }.

pointed(T+1, P1) :- pointed(T, P2), phi(P1, P2, T, ACT).

% pointed(T+1, P1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED), pointed(T, P2),
%                     holds_except(W1, W2, CAUSED),     holds(W1, CAUSED), possible_world(P1, W1), possible_world(P2, W2).

% pointed(T+1, P1) :- is_executable(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED), pointed(T, P2),
%                     holds_except(W1, W2, CAUSED), not holds(W1, CAUSED), possible_world(P1, W1), possible_world(P2, W2).

% We set the beliefs of the fully observant agents in the new possibilities created by the transition function phi

% TODO: fix the following. Usare questa versione crea believes errati rispetto all'agente b (Secondo output su outputs.lp)
% believes(P1, P2, AG) :- plan(T, ACT), ontic_action(ACT), phi(P1, P3, T, ACT), phi(P2, P4, T, ACT),
                        % pointed(T, P3), reaches(P3, P4, AGS), fully_obs_agents(T, ACT, AGS), is_fully_obs(T, ACT, AG).
                        % pointed(T, P3), believes(P3, P4, AG), is_fully_obs(T, ACT, AG). % TODO: forse è sufficiente believes al posto di reaches??

% TODO: fix the following. Usare questa versione crea believes in più (Terzo output su outputs.lp)
% believes(P1, P2, AG) :- plan(T, ACT), ontic_action(ACT), phi(P1, P3, T, ACT), phi(P2, P4, T, ACT), believes(P3, P4, AG),
%                         pointed(T, PP), reaches(PP, P3, AGS), fully_obs_agents(T, AGS), is_fully_obs(T, AG).

% We maintain the beliefs of the oblivious agents by adding edges from the new portion of the state to the old one.
% believes(P1, P2, AG) :- plan(T, ACT), ontic_action(ACT), phi(P1, P3, T, ACT), believes(P3, P2, AG),
%                         pointed(T, PP), reaches(PP, P3, AGS), fully_obs_agents(T, AGS), is_oblivious(T, AG). % TODO: CONTROLLA CORRETTEZZA!!! FORSE CI SIAMO



% ONTIC 2.0

% Caso base: dal nuovo pointed P1 ho un arco verso le possibilities P2 = phi_D(P3, ACT) credute da un fully obs nello stato precedente
believes(P1, P2, AG) :- is_executable(T, ACT), ontic_action(ACT), pointed(T+1, P1), pointed(T, PP),
                        believes(PP, P3, AG), phi(P2, P3, T, ACT), is_fully_obs(T, AG).

% Caso base per oblivious
believes(P1, P2, AG) :- is_executable(T, ACT), ontic_action(ACT), pointed(T+1, P1), pointed(T, PP),
                        believes(PP, P2, AG), is_oblivious(T, AG).

% Passo induttivo
believes(P1, P2, AG) :- is_executable(T, ACT), ontic_action(ACT), pointed(T+1, PP1), pointed(T, PP2), same_path(PP1, P1, PP2, P3),
                        believes(P3, P4, AG), phi(P1, P3, T, ACT), phi(P2, P4, T, ACT), is_fully_obs(T, AG).

believes(P1, P2, AG) :- is_executable(T, ACT), ontic_action(ACT), pointed(T+1, PP1), pointed(T, PP2), same_path(PP1, P1, PP2, P3),
                        believes(P3, P2, AG), is_oblivious(T, AG). % phi(P1, P3, T, ACT), 

% TODO: CONTROLLA CORRETTEZZA

% SENSING ACTIONS
% ANNOUNCEMENT ACTIONS


% Generate
% 0 {plan(T, ACT) : action(ACT)} 1 :- time(T), T < maxT.

% 1 { possible_world(P, W) : possibility(P) } 1 :- world(W).

% Define
action(ACT) :- ontic_action(ACT).
% action(ACT) :- sensing_action(ACT).
% action(ACT) :- announcement_action(ACT).

plan(0, open_a).
plan(1, close_a).
plan(2, open_c).

% not_reached_goal(T) :- not entails(T, F), time(T), goal(F).
% reached_goal(T) :- not not_reached_goal(T), time(T).
% reached_goal(T) :- time(T), goals { entails(T, F) : goal(F) } goals.


% Test
% :- plan(T, ACT), not entails(T, EXEC), executable(ACT, EXEC), time(T), formula(EXEC), action(ACT).

:- possible_world(P, W1), possible_world(P, W2), W1 != W2.

% :- plan(T, ACT), ontic_action(ACT), causes(ACT,  CAUSED), fluent(CAUSED), pointed(T, PP), reaches(PP, P1, AGS), fully_obs_agents(T, ACT, AGS), 
%    possible_world(P1, W1), holds_except(W1, W2, CAUSED), 0 { possible_world(P, W2) : possibility(P) } 0.

% :- plan(T, ACT), ontic_action(ACT), causes(ACT, -CAUSED), fluent(CAUSED), pointed(T, PP), reaches(PP, P1, AGS), fully_obs_agents(T, ACT, AGS), 
%    possible_world(P1, W1), holds_except(W1, W2, CAUSED), 0 { possible_world(P, W2) : possibility(P) } 0.

% :- 0 { reached_goal(T) : time(T) } 0.
% :- reached_goal(T1), plan(T2, ACT), T2 >= T1.
% :- reached_goal(T1), pointed(T2, P), T2 >= T1.


% Display

% #show is_fully_obs/2.
% #show is_oblivious/2.
% #show is_executable/2.

#show pointed/2.
% #show pointed_world/2.
#show possible_world/2.
#show believes/3.
#show phi/4.
% #show holds_except/3.

#show plan/2.
% #show reached_goal/1.

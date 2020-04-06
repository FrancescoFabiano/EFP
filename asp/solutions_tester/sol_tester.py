#!/usr/bin/python

#execution example: " python3 sol_tester.py Coin_in_the_Box__pl_2 ../../exp/ICAPS20/Coin_In_The_Box/" --- first argument: name of the file (without extension---considered .txt), second argument: relative path to the argument (with the final /)
#This script also assumes the presence of a file called 'argument_name_out_asp.txt' --- example: 'Coin_in_the_Box__pl_2_out_asp.txt'

#the output is a file called 'argument_name_sol_correctness.txt' --- example: 'Coin_in_the_Box__pl_2_sol_correctness.txt'

import sys
import re
import os
from collections import defaultdict
from sortedcontainers import SortedSet, SortedDict
import subprocess
from subprocess import Popen
#sys.argv[1] contains the name of the domain (c++ format)

def clean_plan(plan):
	replaced = re.sub('Answer:.+\s', '', plan)
	replaced = re.sub(' reached_goal', '', replaced)
	return replaced
	
def action_number(action):
	act_num = re.search('\(\d,', action).group(0)
	act_num = re.sub('\(', '', act_num)
	act_num = re.sub(',', '', act_num)
	return act_num
	
def action_name(action):
	act_name = re.search(',\w+\)', action).group(0)
	act_name = re.sub('\)', '', act_name)
	act_name = re.sub(',', '', act_name)
	return act_name

def initialize_plan(plan, action_map):
	cleaned_plan = clean_plan(plan)
	actions = cleaned_plan.split(' ')
	for act in actions:
		action_map[str(action_number(act))] = SortedSet()

def generate_plan(plan, action_map):	
	cleaned_plan = clean_plan(plan)
	actions = cleaned_plan.split(' ')
	for act in actions:
		action_map[str(action_number(act))] = str(action_name(act))
	

n_answers = re.compile('Answer:.+\s.+reached_goal')
outputfile = open(sys.argv[1]+'_sol_correctness.txt', 'w') 
well_done = re.compile('Well Done, Goal found')
sol_counter = 1;
no_errors = True;

#n_answers = re.compile('plan\(\w+,\w+\)\s)+')
with open(sys.argv[1]+'_out_asp.txt', 'r') as n:
	n = n.read()
	
	plans = re.findall(n_answers, n)
	for plan in plans:
		action_map = SortedDict()
		initialize_plan(plan, action_map)
		generate_plan(plan, action_map)
		actions_list = list()
		for key,values in action_map.items():
			actions_list.append(values)
			#print(values, end =" ")
		#print('\n')
		process = subprocess.Popen(['./sol_test.sh',sys.argv[2]+sys.argv[1]+'.txt'] + actions_list)
		process.wait()

		with open('tmp_sol_test.tmp', 'r') as n:
			n = n.read()
				
			goals = re.findall(well_done, n)
			if len(goals) < 1:
				print('Answer ' + str(sol_counter) + ' is wrong.', end ="\n", file = outputfile)
				print('Wrong plan:', end =" ", file = outputfile)
				print(*actions_list, sep = ", ", file = outputfile)
				print('\n', file = outputfile)

				no_errors = False;
			sol_counter+=1
				
	if no_errors:
		print('Well done, all the answers are correct :)', end ="", file = outputfile)

if os.path.exists("tmp_sol_test.tmp"):
	os.remove("tmp_sol_test.tmp")
#!/usr/bin/python

#execution example: "python3 out_reader.py output" --- rember not tu put the txt extension, is considered like that as default
#after that run "dot -Tpng output.dot >> output.png" or "dot -Tpdf output.dot >> output.pdf"

import sys
import re
from collections import defaultdict
from sortedcontainers import SortedSet, SortedDict


def clean_row(row, string):
	replaced = re.sub(string, '', row)
	replaced = re.sub(' ', '', replaced)
	replaced = re.sub(';', '', replaced)
	return replaced

def generate_single(agent_row, string):
	cleaned_row = clean_row(agent_row,string + ' ')
	splitted = cleaned_row.split(',')
	for agent in splitted:
		print(string + '(' + agent + ').', file = outputfile) 


outputfile = open(sys.argv[1]+'.lp', 'w') 

#n_fluent_rows = re.compile('fluent(\s*\w*,*\s*)*;')
n_fluent_rows = re.compile('fluent\s.+;')
n_action_rows = re.compile('action\s.+;')
n_agent_rows = re.compile('agent\s.+;')



with open(sys.argv[1]+'.txt', 'r') as n:
	n = n.read()
	
	#FLUENTS
	print('% *** FLUENTS ***',end="\n", file = outputfile) 
	fluent_rows = re.findall(n_fluent_rows, n)
	for fluent_row in fluent_rows:
		generate_single(fluent_row, 'fluent')
		
	#ACTIONS
	print('\n\n% *** ACTIONS ***',end="\n", file = outputfile) 
	action_rows = re.findall(n_action_rows, n)
	for action_row in action_rows:
		generate_single(action_row, 'action')
		
	#AGENTS
	print('\n\n% *** AGENTS ***',end="\n", file = outputfile) 
	agent_rows = re.findall(n_agent_rows, n)
	for agent_row in agent_rows:
		generate_single(agent_row, 'agent')

outputfile.close() 

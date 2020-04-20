#!/usr/bin/python

#execution example: "python3 out_reader.py output" --- rember not to put the txt extension, is considered like that as default
#after that run "dot -Tpng output.dot >> output.png" or "dot -Tpdf output.dot >> output.pdf"

import sys
import re
from collections import defaultdict
from sortedcontainers import SortedSet, SortedDict


def remove_phi(string):
	replaced = re.sub('phi\(', 'p', string)
	replaced = re.sub('\)', '', replaced)
	return replaced

def generate_pointed(new_pointed):
	new_pointed = re.sub('^pointed\(', '', new_pointed)
	new_pointed = re.sub('\)$', '', new_pointed)
	new_pointed = remove_phi(new_pointed)
	return new_pointed
	
def last_pointed(new_pointed,pointed):
	new_pointed = generate_pointed(new_pointed)
	np_splitted = new_pointed.split(',')
	p_splitted = pointed.split(',')
	if np_splitted[0] >= p_splitted[0]:
		return new_pointed
	else:
		return pointed
	
def generate_world_key(world):
	replaced = re.sub('^possible_world\(', '', world)
	replaced = re.sub('\)$', '', replaced)
	no_phi = remove_phi(replaced)
	return no_phi
	
def generate_world(world, pointed):
	no_phi = generate_world_key(world)
	splitted = no_phi.split(',')
	if (splitted[0] + ',' + splitted[1] + ',' + splitted[2]) != pointed:
		print('\tnode [shape = circle] "' + splitted[0] + '_' + splitted[1] + '_' + splitted[2] + '";', file = outputfile)
	else:
		print('\tnode [shape = doublecircle] "' + splitted[0] + '_' + splitted[1] + '_' + splitted[2] + '";', file = outputfile) 
	return splitted
	
def initialize_rank(world, rank_map):
	no_phi = generate_world_key(world)
	splitted = no_phi.split(',')
	rank_map[splitted[0]+splitted[1]] = SortedSet()
	
def generate_rank(world, rank_map):
	no_phi = generate_world_key(world)
	splitted = no_phi.split(',')
	rank_map[splitted[0]+splitted[1]].add('"' + splitted[0] + '_' + splitted[1] + '_' + splitted[2] + '"')
	
def generate_hold_key(hold):
	replaced = re.sub('^holds\(', '', hold)
	replaced = re.sub('\)$', '', replaced)
	no_phi = remove_phi(replaced)
	return no_phi
		
def initialize_fluent_table(hold, fluent_table, key_table):
	hold_key = generate_hold_key(hold)
	splitted = hold_key.split(',')
	fluent_table[splitted[0]+splitted[1]+splitted[2]+splitted[3]] = SortedSet()
	key_table[splitted[0]+splitted[1]+splitted[2]+splitted[3]] = splitted[0]+'_'+splitted[1]+'_'+splitted[2];
	
def generate_fluent_table(hold, fluent_table):
	hold_key = generate_hold_key(hold)
	splitted = hold_key.split(',')
	fluent_table[splitted[0]+splitted[1]+splitted[2]+splitted[3]].add(splitted[4])

	
def initialize_cluster(world, cluster_map):
	no_phi = generate_world_key(world)
	splitted = no_phi.split(',')
	cluster_map[splitted[0]] = SortedSet()
	
def generate_cluster(world, cluster_map):
	no_phi = generate_world_key(world)
	splitted = no_phi.split(',')
	cluster_map[splitted[0]].add('"' + splitted[0] + '_' + splitted[1] + '_' + splitted[2] + '"')
	
def generate_edge_key(edge):
	replaced = re.sub('^believes\(', '', edge)
	replaced1 = re.sub('\)$', '', replaced)
	no_phi = remove_phi(replaced1)
	return no_phi

def initialize_edges(edge, edges_map):
	key = generate_edge_key(edge)
	splitted = key.split(',')
	edges_map['"' + splitted[0] + '_' + splitted[1] + '_' + splitted[2]+ '"'+' -> '+ '"' + splitted[3] + '_' + splitted[4] + '_' + splitted[5]+ '"'] = SortedSet()

def generate_edges(edge,edges_map):
	key = generate_edge_key(edge)
	splitted = key.split(',')
	edges_map['"' + splitted[0] + '_' + splitted[1] + '_' + splitted[2]+ '"'+' -> '+ '"' + splitted[3] + '_' + splitted[4] + '_' + splitted[5]+ '"'].add(splitted[6])

def compare_keys(key1,key2,edges_map,edges_map_both):
	key1_mod = re.sub('\_|->|"', '', key1)
	substrings = re.split('\s+',key1_mod)
	key1_inverted = substrings[1] + substrings[0]
	key2_mod = re.sub('\_|->|"|\s+', '', key2)
	if key1_inverted == key2_mod:
		edges_map_both[key1] = SortedSet();
		edges_map_both[key1] = edges_map[key1].intersection(edges_map[key2])
		edges_map[key1] = edges_map[key1].difference(edges_map_both[key1])
		edges_map[key2] = edges_map[key2].difference(edges_map_both[key1])

def check_backforth_edge(edges_map,edges_map_both):
	for key, values in edges_map.items():
		for key_nested, values_nested in edges_map.items():
			if key_nested != key:
				compare_keys(key,key_nested,edges_map,edges_map_both)



outputfile = open(sys.argv[1]+'.dot', 'w') 
#outputfile_table = open(sys.argv[1]+'_table.dot', 'w')
#print('digraph K_structure{',end="\n", file = outputfile_table) 
#print('\trankdir=BT;',end="\n", file = outputfile_table)
#print('\tranksep=0.75',end="\n", file = outputfile_table)
#print('\tnewrank=true;',end="\n", file = outputfile_table)
#print('\tsize="8,5;"',end="\n", file = outputfile_table) 


pointed = '0,0,1'
n_pointed = re.compile('pointed\(\S*\)')
n_world = re.compile('possible_world\(\S*\)')
n_edge = re.compile('believes\(\S*\)')
n_holds = re.compile('holds\(\S*\)')
n_fluent = re.compile('fluent\(\S*\)')


print('digraph K_structure{',end="\n", file = outputfile) 
print('\trankdir=BT;',end="\n", file = outputfile)
print('\tranksep=0.75',end="\n", file = outputfile)
print('\tnewrank=true;',end="\n", file = outputfile)
print('\tsize="8,5;"',end="\n", file = outputfile) 
print('\n//WORLDS List:',end="\n", file = outputfile) 

with open(sys.argv[1]+'.txt', 'r') as n:
	n = n.read()
	
	#POINTED
	pointeds = re.findall(n_pointed, n)
	for new_pointed in pointeds:
		pointed = last_pointed(new_pointed,pointed)

	
	#WORLDS
	worlds = re.findall(n_world, n)
	for world in worlds:
		generate_world(world,pointed) 
	
	#RANK
	cluster_map = SortedDict()
	for world in worlds:
		initialize_cluster(world,cluster_map)
		
	for world in worlds:
		generate_cluster(world,cluster_map)
	
	counter_cluster = 0
	print("\n//SUBGRAPHS List:", end ="\n", file = outputfile) 
	for key,values in cluster_map.items():
		print("\t", end ="", file = outputfile) 
		#print(key, end ="")
		#print('{rank = ' + str(counter_rank) + '; ', end ="", file = outputfile) 
		print('subgraph cluster_'+str(counter_cluster)+ '{', end ="", file = outputfile) 
		for val in values:
			print(val, end ="", file = outputfile)
			if values.index(val) != len(values)-1:
				print('; ', end ="", file = outputfile) 
		print('};', file = outputfile)
		counter_cluster+=1
	
	#RANK
	rank_map = SortedDict()
	for world in worlds:
		initialize_rank(world,rank_map)
		
	for world in worlds:
		generate_rank(world,rank_map)
		
	#counter_rank = 0
	print("\n//RANKS List:", end ="\n", file = outputfile) 
	for key,values in rank_map.items():
		#counter_rank+=1
		print("\t", end ="", file = outputfile) 
		#print(key, end ="")
		#print('{rank = ' + str(counter_rank) + '; ', end ="", file = outputfile) 
		print('{rank = same;', end ="", file = outputfile) 
		for val in values:
			print(val, end ="", file = outputfile) 
			if values.index(val) != len(values)-1:
				print('; ', end ="", file = outputfile) 
		print('};', file = outputfile)
	
	#EDGES
	edges_map = dict()
	edges_map_both = dict()
	edges = re.findall(n_edge, n)
	for edge in edges:
		initialize_edges(edge,edges_map)
		
	for edge in edges:
		generate_edges(edge,edges_map)
		
	check_backforth_edge(edges_map,edges_map_both)
	
	print("\n//EDGES List:", end ="\n", file = outputfile) 
	for key,values in edges_map.items():
		if len(values) > 0:
			print("\t", end ="", file = outputfile) 
			print(key, end ="", file = outputfile) 
			print(' [label="', end ="", file = outputfile) 
			for val in values:
				print(val, end ="", file = outputfile) 
				if values.index(val) != len(values)-1:
					print(',', end ="", file = outputfile) 
			print('"];', file = outputfile)
	for key,values in edges_map_both.items():
		if len(values) > 0:
			print("\t", end ="", file = outputfile) 
			print(key, end ="", file = outputfile) 
			print(' [dir=both label="', end ="", file = outputfile) 
			for val in values:
				print(val, end ="", file = outputfile) 
				if values.index(val) != len(values)-1:
					print(',', end ="", file = outputfile) 
			print('"];', file = outputfile) 

#FLUENTS-TABLE

#reading all the fluents for complete table
	fluents = SortedSet()
	fluent_predicates = re.findall(n_fluent, n)
	for fluent_predicate in fluent_predicates:
		fluent_predicate = re.sub('^fluent\(', '', fluent_predicate)
		fluent_predicate = re.sub('\)$', '', fluent_predicate)
		fluents.add(fluent_predicate)
		
#table print
	holds = re.findall(n_holds, n)
	fluent_table = SortedDict()
	ft_keys = SortedDict()

	for hold in holds:
		initialize_fluent_table(hold, fluent_table,ft_keys)
		
	for hold in holds:
		generate_fluent_table(hold, fluent_table)
		
	print('\n//WORLDS description Table:\n\tnode [shape = plain]description[label=<\n\t<table border = "0" cellborder = "1" cellspacing = "0" >', end ="\n", file = outputfile)
	for key,values in fluent_table.items():
		#counter_rank+=1
		print("\t\t", end ="", file = outputfile) 
		#print(key, end ="")
		#print('{rank = ' + str(counter_rank) + '; ', end ="", file = outputfile) 
		print('<tr><td>'+ ft_keys[key] + '</td>\t<td>', end ="", file = outputfile)
		for fluent in fluents:
			if fluent in fluent_table[key]:
				print('<font color="#0000ff"> '+fluent + '</font>', end ="", file = outputfile)
			else:
				print('<font color="#ff1020">-'+fluent+"</font>", end ="", file = outputfile)
			if fluents.index(fluent) != len(fluents)-1:
				print(', ', end ="", file = outputfile) 
		print('</td></tr>', end ="\n", file = outputfile)
	print('\t</table>>]',end ="\n", file = outputfile)

print('}', file = outputfile) 

outputfile.close() 

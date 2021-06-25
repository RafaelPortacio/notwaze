import json
import os
from tqdm import trange
import random as rnd
import networkx as nx


DEGREES = 3

os.makedirs(f"test-graphs-{DEGREES}")

for rep in trange(100):
    rnd.seed(rep)
    G = nx.generators.random_graphs.connected_watts_strogatz_graph(500, DEGREES, 0.3, seed=rep)
    G = G.to_directed()
    
    for iter_edge in G.edges:
        G[iter_edge[0]][iter_edge[1]]['weight'] = rnd.uniform(0,1)
    
    #create node dictionary

    nodes = {}
    for i in G.nodes:
        nodes[i] = {'latitude':1, 'longitude':1}
        
    #create edge list
    edges = []
    for i in G.edges:
        dict_aux = G.edges[i]
        origin_node = i[0]
        destination_node = i[1]
        edges.append({'origin_node': str(origin_node),
                      'destination_node': str(destination_node),
                      'eta': G.edges[origin_node, destination_node]['weight'],
                      'length': G.edges[origin_node, destination_node]['weight'] * 0.1})

    #create dictionary for json
    json_dictio = {}
    json_dictio['nodes'] = nodes
    json_dictio['edges'] = edges

    #save in .json
    with open(f'test-graphs-{DEGREES}/graph'+str(rep)+'.json', 'w') as outfile:
        json.dump(json_dictio, outfile)

    #save in .txt
    sPath_lst = []
    for iterat in range(30):
        shortest_path_str = ''
        random1 = rnd.randint(0,499)
        random2 = rnd.randint(0,499)
        shortest_path_lst = nx.shortest_path(G, random1, random2, weight='weight')
        for i in shortest_path_lst:
            shortest_path_str = shortest_path_str + str(i) + ' '
        shortest_path_str = shortest_path_str[0:len(shortest_path_str)-1]
        sPath_lst.append(shortest_path_str)
        

    with open(f'test-graphs-{DEGREES}/graph'+str(rep)+'.txt', 'w') as outfile:
        outfile.write('\n'.join(sPath_lst))

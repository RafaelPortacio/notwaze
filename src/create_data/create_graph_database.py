import json
import osmnx as ox


print("Acquiring graph...")
G = ox.graph_from_place("Rio de Janeiro, RJ, Brasil", network_type='drive', simplify=False)
print("Got the graph!")

# impute speed on all edges missing data
G = ox.add_edge_speeds(G)

# calculate travel time (seconds) for all edges
G = ox.add_edge_travel_times(G)
print("added time info!")

#create node dictionary
nodes = {}
for i in G.nodes:
    dict_aux = G.nodes[i]
    y = dict_aux['y']
    x = dict_aux['x']
    nodes[i] = {'latitude':y, 'longitude':x}
    
#create edge list
edges = []
for i in G.edges:
    dict_aux = G.edges[i]
    origin_node = str(i[0])
    destination_node = str(i[1])
    eta = dict_aux['travel_time']
    leng = dict_aux['length']
    edges.append({'origin_node': origin_node,
                  'destination_node': destination_node,
                  'eta': eta,
                  'length': leng})

#create dictionary for json
json_dictio = {}
json_dictio['nodes'] = nodes
json_dictio['edges'] = edges

#save in .txt
with open('rj_graph_database.json', 'w') as outfile:
    json.dump(json_dictio, outfile)

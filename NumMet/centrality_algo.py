# -*- coding: utf-8 -*-



import numpy as np
import time
import networkx as nx

start = time.time()
dataset_array = np.genfromtxt("cit-HepTh.txt",skip_header=4,dtype=int)

G = nx.DiGraph()

for data in dataset_array:
    G.add_edge(data[0], data[1])

centrality = nx.eigenvector_centrality_numpy(G)
#centrality = nx.katz_centrality_numpy(G)
#centrality = nx.in_degree_centrality(G)
#centrality = nx.pagerank(G)

for i in range(10):
    max_value = max(centrality.values())
    for key, value in centrality.items():
        if value == max_value:
            max_key = key
    print("{0}. {1} - {2} centrality".format(i + 1, max_key, max_value))
    centrality.pop(max_key)
    
end = time.time()
print(end - start)
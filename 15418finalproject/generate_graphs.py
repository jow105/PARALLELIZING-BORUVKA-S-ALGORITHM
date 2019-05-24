import sys
from random import randint


#python generate_graphs.py 5 sparse sparse_graph


n = int(sys.argv[1])
typeof = sys.argv[2]
name = sys.argv[3]

edges = []
weights = []
usedEdges = []
if(typeof == "sparse"):
    numEdges = int(10*n)
elif(typeof == "dense"):
    numEdges = int((n*(n-1))/4)
weight = numEdges
print numEdges

if weight < numEdges:
    print "not enough edges"
    sys.exit()


randWeight = []
for i in range(numEdges):
    randWeight.append(i+1)



randEdge = []
for i in range(n):
    for j in range(i,n):
        if i != j:
            randEdge.append((i,j))

print "done starting"
#psuedocode but
# for i in range(numEdges):
#     randEdge.append(i)

# def generate_edge(i):
#     first = i/n
#     second = i%n
#     if first > second
#         return (first,second)
#     else
#         return (second,first)

print "done starting"

# for i in range(numEdges):

#     to = randint(0,n-1)
#     fro = randint(0,n-1)
#     while Uedges[(to,fro)] == 1 or Uedges[(fro,to)] == 1:
#         to = randint(0,n-1)
#         fro = randint(0,n-1)
#     Uedges[(to,fro)] = 1 
#     # while (to,fro) in usedEdges or to == fro or (fro,to) in usedEdges:
#     #     to = randint(0,n-1)
#     #     fro = randint(0,n-1)
#     # usedEdges.append((to,fro))
#     w = randint(1,weight)
#     # while w in weights:
#     #     w = randint(1,weight)
#     while UWeights[w] == 1:
#         w = randint(1,weight)
#     UWeights[w] = 1
#     # weights.append(w)
#     edges.append(((to,fro),w))

file_obj = open("graphs/" +name +".txt", "w")
file_obj.write(typeof+ " " +  str(n) + " " + str(numEdges) +"\n")

for i in range(numEdges):
    randW = randint(0,len(randWeight) -1)
    randE = randint(0,len(randEdge) -1)
    edge = randEdge[randE]
    file_obj.write(str(edge[0]) + " " + str(edge[1]) + " " + str(randWeight[randW]) + "\n")
    del randWeight[randW]
    del randEdge[randE]

# for i in edges:
#     file_obj.write(str(i[0][0]) + " " + str(i[0][1]) + " " + str(i[1]) + "\n")

file_obj.close()



#format of adjlist will be key = first vertex, value = (2nd vertex, weight of edge)


# adjlist = {}

# for i in edges: #adj list
#     to = i[0][0]
#     fro = i[0][1]
#     w = i[1]
#     if adjlist.get(to) == None:
#         adj[to] = [(fro,w)]
#     else:
#         adj[to].append((fro,w))



# print adjlist   
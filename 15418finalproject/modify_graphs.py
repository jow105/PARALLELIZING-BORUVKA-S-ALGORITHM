from random import randint
import sys
import re

#python modify_graphs.py graphs/facebook_combined.txt


filename = sys.argv[1]
# weight = int(sys.argv[2])
# typeof = sys.argv[3]
# name = sys.argv[4]


num_lines = sum(1 for line in open(filename))

randWeight = []
for i in range(num_lines):
    randWeight.append(i+1)

# edges= {}
# with open(filename, "r") as orig:
#     lines = orig.readlines()
#     for line in lines:
#         k = line.split()
#         if edges.get((k[0],k[1])) == None and edges.get((k[1],k[0])) == None:
#             edges[(k[0],k[1])] = 1


# print len(edges)
# print num_lines


vertices = set()
with open(filename, "r+") as orig:
    lines = orig.readlines()
    for line in lines:
        k = line.split()
        vertices.add(int(k[0]))
        vertices.add(int(k[1]))
    orig.seek(0)
    orig.truncate()
    orig.write(filename+ " " +  str(len(vertices)) + " " + str(num_lines) +"\n")
    for line in lines:
        rand = randint(0,len(randWeight)-1)
        orig.write(line.rstrip("\n\r") + " " + str(randWeight[rand]) + "\n")
        del randWeight[rand]
        
print len(vertices)

# file_obj = open("graphs/" +filename[7:-4] +"_weights.txt", "w")
# file_obj.write(filename+ " " +  str(len(vertices)) + " " + str(num_lines) +"\n")

# with open(filename, "w") as orig:
#     lines = orig.readlines()
#     orig.seek(0)
#     orig.truncate()
#     orig.write(filename+ " " +  str(len(vertices)) + " " + str(num_lines) +"\n")
#     for line in lines:
#         rand = randint(0,len(randWeight)-1)
#         orig.write(line.rstrip("\n\r") + " " + str(randWeight[rand]) + "\n")
#         del randWeight[rand]


# file_obj.close()

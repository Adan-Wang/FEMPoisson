import matplotlib.pyplot as plt
import numpy as np
import csv


grid=[[],[]]
with open('grid.csv') as gridFile:
    csv_reader = csv.reader(gridFile, delimiter = ',')
    for row in csv_reader:
        grid[0].append(row[0])
        grid[1].append(row[1])

triangles = []
with open('triangles.csv') as triangleFile:
    csv_reader = csv.reader(triangleFile, delimiter = ',')
    for row in csv_reader:
        triangles.append([row[0],row[1],row[2]])

x=np.asarray(grid[0][:])
y=np.asarray(grid[1][:])
T=np.asarray(triangles)

fig1, ax1 = plt.subplots()
ax1.triplot(x,y,T,'bo-',lw=1.0)
ax1.set_title('Triangulation')
ax1.set_aspect('equal')

plt.show()

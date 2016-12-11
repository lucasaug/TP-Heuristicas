#!/bin/python
# -*- coding: utf-8 -*-

from os import system
from glob import glob

import matplotlib.pyplot as plt

FOLDER_LOCATION = "./instances/EUC_2D/"
FILE_EXTENSION  = ".tsp"

files = glob(FOLDER_LOCATION + "*" + FILE_EXTENSION)
files.sort()
i = 10
X = []
X2 = []
YtMST = []
YrMST = []
Xlabels = []
for test in files:
    system("./bin/st " + test + " > temp.txt")
    data = open("temp.txt")
    lines = data.readlines()
    lines = [l.strip("\n") for l in lines]
    X.append(i)
    X2.append(i + 40)
    Xlabels.append(lines[0])
    YtMST.append(float(lines[1]))
    YrMST.append(float(lines[2]))
    data.close()
    i += 150

ax = plt.subplot(111)
t = ax.bar(X, YtMST, 40, facecolor='red', edgecolor='black')
r = ax.bar(X2, YrMST, 40, facecolor='blue', edgecolor='black')
ax.legend((t[0], r[0]), ('Arvore Minima dos terminais', 'Arvore Minima com remocao de folhas') )
ax.set_xticks(X2)
ax.set_xticklabels(Xlabels, size = "small")
plt.xlabel('Instancia')
plt.ylabel('Tempo gasto')
plt.show()

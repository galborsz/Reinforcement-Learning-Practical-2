import matplotlib.pyplot as plt
import sys

iterations = 10000

#Get input
algorithm = input("Choose algorithm: qlearning, sarsa, expected_sarsa, double_qlearning\n")
exploration = input("Choose decay rate for the exploration method:\n")

print("algorithm: ", algorithm)
print("exploration: ", exploration)

file_name = "avg_total_score_" + algorithm + "_" + exploration + ".txt"
title = "Learning progress - " + algorithm + " (Decaying epsilon-greedy e=" + exploration + ")"

file1 = open(file_name,"r")
string1 = file1.read().splitlines()
y = [int(line) for line in string1]
print("y: ", len(y))
x = [*range(0, iterations, 1)] 
print("x: ", len(x))
plt.scatter(x,y, s=0.5)
plt.xlabel("Iteration")
plt.ylabel("Score")
plt.title(title)
plt.show()
import matplotlib.pyplot as plt

file1 = open("total_score_sarsa.txt","r")
string1 = file1.read().splitlines()
y = [int(line) for line in string1]
print("y: ", len(y))
x = [*range(0, 52000, 1)] 
print("x: ", len(x))
plt.scatter(x,y, s=0.5)
plt.xlabel("Iteration")
plt.ylabel("Score")
plt.title("Learning progress - Sarsa (greedy)")
plt.show()
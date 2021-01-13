import matplotlib.pyplot as plt

iterations = 52000

file1 = open("total_score_qlearning_greedy.txt","r")
string1 = file1.read().splitlines()
y = [int(line) for line in string1]
print("y: ", len(y))
x = [*range(0, iterations, 1)] 
print("x: ", len(x))
plt.scatter(x,y, s=0.5)
plt.xlabel("Iteration")
plt.ylabel("Score")
plt.title("Learning progress - Q-learning (greedy)")
plt.show()
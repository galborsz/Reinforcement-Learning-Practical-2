import matplotlib.pyplot as plt

iterations = 100000

file1 = open("total_score_sarsa_epsilon_greedy_0.01.txt","r")
string1 = file1.read().splitlines()
y = [int(line) for line in string1]
print("y: ", len(y))
x = [*range(0, iterations, 1)] 
print("x: ", len(x))
plt.scatter(x,y, s=0.5)
plt.xlabel("Iteration")
plt.ylabel("Score")
plt.title("Learning progress - Sarsa (epsilon-greedy, epsilon=0.01)")
plt.show()
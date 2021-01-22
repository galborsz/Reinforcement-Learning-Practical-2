import matplotlib.pyplot as plt

iterations = 15000

file1 = open("total_score_expected_sarsa_epsilon_greedy_0.1.txt","r")
string1 = file1.read().splitlines()
y = [int(line) for line in string1]
print("y: ", len(y))
x = [*range(0, iterations, 1)] 
print("x: ", len(x))
plt.scatter(x,y, s=0.5)
plt.xlabel("Iteration")
plt.ylabel("Score")
plt.title("Learning progress - Expected Sarsa (epsilon-greedy, epsilon=0.1)")
plt.show()
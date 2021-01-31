import matplotlib.pyplot as plt
#Sarsa
file1 = open("avg_total_score_egreedy_sarsa.txt","r")
string1 = file1.read().splitlines()
y = [int(line) for line in string1]

#Expected sarsa
file2 = open("avg_total_score_egreedy_expected_sarsa.txt","r")
string2 = file2.read().splitlines()
y2 = [int(line) for line in string2]

#Q-Learning
file3 = open("avg_total_score_egreedy_qlearning.txt","r")
string3 = file3.read().splitlines()
y3 = [int(line) for line in string3]

x = [*range(0, 10500, 1)] 
plt.scatter(x,y, s=0.5, label="Sarsa")
plt.scatter(x,y2, s=0.5, label="Expected Sarsa")
plt.scatter(x,y3, s=0.5, label="Q-Learning")
plt.xlabel("Iteration")
plt.ylabel("Score")
plt.title("Average score for RL algorithms using Epsilon-greedy")
#plt.legend(scatterpoints=1, fontsize=10)
lgnd = plt.legend(loc="top left", numpoints=1, fontsize=10)

#change the marker size manually for both lines
for handle in lgnd.legendHandles:
    handle.set_sizes([12.0])
plt.show()
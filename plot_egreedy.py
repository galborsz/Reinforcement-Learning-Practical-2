import matplotlib.pyplot as plt
#Sarsa
file1 = open("avg_total_score_egreedy_sarsa123.txt","r")
string1 = file1.read().splitlines()
y = [int(line) for line in string1]

#Expected sarsa
file2 = open("avg_total_score_egreedy_esarsa123.txt","r")
string2 = file2.read().splitlines()
y2 = [int(line) for line in string2]

#Q-Learning
file3 = open("avg_total_score_egreedy_qlearning123.txt","r")
string3 = file3.read().splitlines()
y3 = [int(line) for line in string3]

<<<<<<< HEAD
x = [*range(0, 8000, 1)] 
=======
x = [*range(0, 5000, 1)] 
>>>>>>> 7c253b97af4194d37ad6550ff9d16fe26aadfcd9
plt.scatter(x,y, s=0.5, label="Sarsa")
plt.scatter(x,y2, s=0.5, label="Expected Sarsa")
plt.scatter(x,y3, s=0.5, label="Q-Learning")
plt.xlabel("Iteration")
plt.ylabel("Score")
plt.title("Average score for RL algorithms using Epsilon-greedy")
lgnd = plt.legend(loc="top left", numpoints=1, fontsize=10)

#change the marker size manually for both lines
for handle in lgnd.legendHandles:
    handle.set_sizes([12.0])
plt.show()
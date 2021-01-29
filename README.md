# Playing Flappy Bird with Reinforcement Learning
![screenshot](https://denizbasgoren.github.io/sfml-flappy-bird/screenshots/s2.png)
## How to compile and start the program
In order to compile and start the program, follow these simple steps:
1) Make sure that you have SMFL 2+ installed, which you can find in https://www.sfml-dev.org/download/sfml/2.5.1/
- Windows: https://www.youtube.com/watch?v=GE-4hJDlmeA
- OSX: https://www.youtube.com/watch?v=mtEiyDbYMxQ
2) Navigate to the folder with all the code
3) Excute the following commands:
- `g++ -std=c++11 *.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio`
- `./a.out`
## How the results can be seen
## Parameters
In the main.cpp file, we can find the experiment parameters to be chosen by the person running the code. <br>
-```agent_type```: reinforcement learning algorithms being used, i.e. 1 = qlearning, 2 = sarsa, 3 = expected sarsa, 4 = double qlearning<br>
-```exploration_strategy```: exploration startegy being used, i.e. "greedy", "egreedy", "ucb"<br>
-```iteration_limit```: number of iterations the code will run for<br>
-```number_of_experiments```: number of times the experiment will be repeated. The score results will be averaged and saved as a .txt file<br>
-```rate_of_decay```: rate of decay for the Decaying epsilon-greedy algorithm<br>
Morever, the learning parameters for each of the algorithms and exploration methods can be found in their respective file, for example sarsa_agent.cpp. These learning parameters can also be tuned by the person running the code, although the values with which we obtained optimal results are set as default


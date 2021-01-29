#include <iostream>
#include <stdlib.h>
#include "sarsa_agent.hpp"

sarsa_agent::sarsa_agent(string exploration_strategy): agent(exploration_strategy) {
    next_state = "starting_state";
    next_action = 1;
    srand(time(NULL));

    //learning parameters
    GAMMA = 1;
    ALPHA = 0.7;
    //UCB parameter
    c = 1.5;
}

int sarsa_agent::act() {
    if (next_state == last_state) return 0;
    last_action = next_action;
    last_state = next_state;
    if (p_exploration_strategy == "ucb") {
        ACTION_COUNTS[last_state][last_action]++;  
        t++;
    }
    return last_action;
}

void sarsa_agent::update(int xdif, int ydif, int velocity, double reward) {
    //Choose A' from S' using policy derived from Q (e.g. e-greedy)
    next_state = create_state(xdif, ydif, velocity);
    if (next_state == last_state) return;
    
    double QS0 = Q_TABLE[next_state][0];
    double QS1 = Q_TABLE[next_state][1];

    int greedy_act;
    if (QS0 >= QS1) { //doesnt flap in case of tie
        greedy_act = 0;
    } else {
        greedy_act = 1;
    }

    if (p_exploration_strategy == "greedy") {
        next_action = greedy_act;
    } else if (p_exploration_strategy == "egreedy") { //epsilon greedy
        double random = (double)rand() / RAND_MAX;
	    if (random < EPSILON){
            int random_action = rand() % N_ACTIONS;
		    next_action = random_action;
	    } else {
		    next_action = greedy_act;
	    }
    } else if (p_exploration_strategy == "ucb") { //upper confidence bound
        bool first_ecounter = false;
        for (int i = 0; i < N_ACTIONS; i++) {
            if (ACTION_COUNTS[next_state][i] == 0) {
                next_action = i;
                first_ecounter = true;
                break;
            }
        }
        if (!first_ecounter) {
            double QS0_bonus = c*(sqrt(log(t)/ACTION_COUNTS[next_state][0]));
            double QS1_bonus = c*(sqrt(log(t)/ACTION_COUNTS[next_state][1]));
            if ((QS0 + QS0_bonus) >= (QS1 + QS1_bonus)) { //doesnt flap in case of tie
                next_action = 0;
            } else {
                next_action = 1;
            }
        }
    } else {
        cout << "Error: invalid exploration strategy" << endl;
    }

    double update = ALPHA * (reward + GAMMA * Q_TABLE[next_state][next_action] - Q_TABLE[last_state][last_action]);

	//Update Q(S,A)
	Q_TABLE[last_state][last_action] += update;
}



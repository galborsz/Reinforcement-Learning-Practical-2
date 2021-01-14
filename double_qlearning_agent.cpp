#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "double_qlearning_agent.hpp"

double_qlearning_agent::double_qlearning_agent(): agent() {
    srand(time(NULL));
    next_state = "starting_state";
    //next_action = e_greedy_policy(next_state);
    next_action = 1;
}

int double_qlearning_agent::act() {
    if (next_state == last_state) return 0;
    last_state = next_state;
    last_action = next_action;
    return next_action;
}


void double_qlearning_agent::update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy) {
    
    next_state = create_state(xdif, ydif, velocity);
    if (next_state == last_state) return;
    
    double Q1S0 = Q_TABLE1[next_state][0];
    double Q1S1 = Q_TABLE1[next_state][1];
    double Q2S0 = Q_TABLE2[next_state][0];
    double Q2S1 = Q_TABLE2[next_state][1];

    int greedy_act;
    if ((Q1S0 + Q2S0) >= (Q1S1 + Q2S1)) { //doesnt flap in case of tie
        greedy_act = 0;
    } else {
        greedy_act = 1;
    }

    if (greedy) {
        next_action = greedy_act;
    } else {
        double random = (double)rand() / RAND_MAX;
	    if (random < EPSILON){
            int random_action = rand() % N_ACTIONS;
		    next_action = random_action;
	    } else {
		    next_action = greedy_act;
	    }
    }

    double random = (double)rand() / RAND_MAX;
    if (random < 0.5) {
        int greedy_act_qtable1 = (Q1S0 >= Q1S1) ? 0 : 1;
        Q_TABLE1[last_state][last_action] += ALPHA * (reward + GAMMA * Q_TABLE2[next_state][greedy_act_qtable1] - Q_TABLE1[last_state][last_action]);
	} else {
        int greedy_act_qtable2 = (Q2S0 >= Q2S1) ? 0 : 1;
		Q_TABLE2[last_state][last_action] += ALPHA * (reward + GAMMA * Q_TABLE1[next_state][greedy_act_qtable2] - Q_TABLE2[last_state][last_action]);
	}
}


//returns greedy action given the state
/*
int double_qlearning_agent::greedy_action(string state) {
    if ((Q_TABLE1[state][0] + Q_TABLE2[state][0]) >= (Q_TABLE1[state][1] + Q_TABLE2[state][1])) { //doesnt flap in case of tie
        return 0;
    } else {
        return 1;
    }
} */

//returns greedy action given the state
/*
int double_qlearning_agent::greedy_action_per_qtable(unordered_map<string, unordered_map<int, double> > Q_TABLE, string state) {
    if (Q_TABLE[state][0] >= Q_TABLE[state][1]) { //doesnt flap in case of tie
        return 0;
    } else {
        return 1;
    }
} */



/* Performs greedy policy. With prob epsilon pick action
belonging to maximum action-value. With prob 1-epsilon
pick a random action. */ /*
int double_qlearning_agent::e_greedy_policy(string state) {
	double random = (double)rand() / RAND_MAX;
	if (random < EPSILON){
        int random_action = rand() % N_ACTIONS;
		return random_action;
	} else {
		return greedy_action(state);
	}
} */


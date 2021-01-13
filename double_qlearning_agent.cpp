#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "double_qlearning_agent.hpp"

double_qlearning_agent::double_qlearning_agent(): agent() {
    srand(time(NULL));
    next_state = "starting_state";
    next_action = e_greedy_policy(next_state);
}

int double_qlearning_agent::act() {
    if (next_state == last_state) return 0;
    last_state = next_state;
    last_action = next_action;
    return next_action;
}


void double_qlearning_agent::update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy) {
    //could be faster
    
    next_state = create_state(xdif, ydif, velocity);
    if (next_state == last_state) return;

	int greedy_act = greedy_action(next_state); //A*
    next_action = e_greedy_policy(next_state); //A'

    if (greedy) {
        next_action = greedy_act;
    }

    double random = (double)rand() / RAND_MAX;
    if (random < 0.5) {
        int greedy_act_qtable1 = greedy_action_per_qtable(Q_TABLE1, next_state);
        Q_TABLE1[last_state][last_action] += ALPHA * (reward + GAMMA * (Q_TABLE2[next_state][greedy_act_qtable1] - Q_TABLE1[last_state][last_action]));
	} else {
        int greedy_act_qtable2 = greedy_action_per_qtable(Q_TABLE2, next_state);
		Q_TABLE2[last_state][last_action] += ALPHA * (reward + GAMMA * (Q_TABLE2[next_state][greedy_act_qtable2] - Q_TABLE2[last_state][last_action]));
	}

}


void double_qlearning_agent::initializeNewStates(string state, int action) {
    if (Q_TABLE1.find(state) == Q_TABLE1.end() && Q_TABLE1[state].find(0) == Q_TABLE1[state].end()) { //looks for new state and creates it if it doesnt exist
        Q_TABLE1[state][action] = 0;
        state_count++;
    }
    if (Q_TABLE2.find(state) == Q_TABLE2.end() && Q_TABLE2[state].find(0) == Q_TABLE2[state].end()) { //looks for new state and creates it if it doesnt exist
        Q_TABLE2[state][action] = 0;
        state_count++;
    }
}


//returns greedy action given the state
int double_qlearning_agent::greedy_action(string state) {

    initializeNewStates(state, 0);
    initializeNewStates(state, 1);

    if (Q_TABLE1[state][0] + Q_TABLE2[state][0] >= Q_TABLE1[state][1] + Q_TABLE2[state][1]) { //doesnt flap in case of tie
        return 0;
    } else {
        return 1;
    }
}

//returns greedy action given the state
int double_qlearning_agent::greedy_action_per_qtable(unordered_map<string, unordered_map<int, double> > Q_TABLE, string state) {
    initializeNewStates(state, 0);
    initializeNewStates(state, 1);

    if (Q_TABLE[state][0] >= Q_TABLE[state][1]) { //doesnt flap in case of tie
        return 0;
    } else {
        return 1;
    }
}



/* Performs greedy policy. With prob epsilon pick action
belonging to maximum action-value. With prob 1-epsilon
pick a random action. */
int double_qlearning_agent::e_greedy_policy(string state) {
	double random = (double)rand() / RAND_MAX;

	if (random < EPSILON){
        int random_action = rand() % N_ACTIONS;
        initializeNewStates(state, random_action);
		return random_action;
	} else {
		return greedy_action(state);
	}
}


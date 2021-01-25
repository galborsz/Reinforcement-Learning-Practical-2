#include <iostream>
#include <stdlib.h>
#include "sarsa_agent.hpp"

sarsa_agent::sarsa_agent(): agent() {
    next_state = "starting_state";
    next_action = e_greedy_policy(next_state);
    srand(time(NULL));
}

int sarsa_agent::act() {
    if (next_state == last_state) return 0;
    last_action = next_action;
    last_state = next_state;
    return last_action;
}

void sarsa_agent::update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy) {
    //Choose A' from S' using policy derived from Q (e.g. e-greedy)
    next_state = create_state(xdif, ydif, velocity);
    if (next_state == last_state) return;
    if (greedy) next_action = greedy_action(next_state);
    else next_action = e_greedy_policy(next_state);
    double update = ALPHA * (reward + GAMMA * Q_TABLE[next_state][next_action] - Q_TABLE[last_state][last_action]);

	//Update Q(S,A)
	Q_TABLE[last_state][last_action] += update;
}

//returns greedy action given the state
int sarsa_agent::greedy_action(string state) {
    if (Q_TABLE[state][0] >= Q_TABLE[state][1]) { //doesnt flap in case of tie
        return 0;
    } else {
        return 1;
    }
}


/* Performs greedy policy. With prob epsilon pick action
belonging to maximum action-value. With prob 1-epsilon
pick a random action. */
int sarsa_agent::e_greedy_policy(string state) {
	double random = (double)rand() / RAND_MAX;
	if (random < EPSILON){
        int random_action = rand() % N_ACTIONS;
		return random_action;
	} else {
		return greedy_action(state);
	}
}

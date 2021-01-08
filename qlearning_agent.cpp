#include <iostream>
#include <stdlib.h>
#include "qlearning_agent.hpp"

qlearning_agent::qlearning_agent() {
    pap = 0;
    srand(time(NULL));
}

string qlearning_agent::create_state(int xdif, int ydif, int velocity) {
    //NEEDS TO BE DISCRETISED
    xdif = xdif / 2;
    ydif = ydif / 2;
    velocity = velocity/2;

    string state = to_string(xdif) + "_" + to_string(ydif) + "_" + to_string(velocity);
    //std::cout << state << '\n';
    return state;
}

int qlearning_agent::act(int xdif, int ydif, int velocity) {
    //two next lines can be moved to the end of update
    string state = create_state(xdif, ydif, velocity);
    last_state = state;
    last_action = e_greedy_policy(state);
    return last_action;
}

int qlearning_agent::actgreedy(int xdif, int ydif, int velocity) {
    string state = create_state(xdif, ydif, velocity);
    return greedy_action(state);
}

void qlearning_agent::update_qtable(int xdif, int ydif, int velocity, int reward) {
    string new_state = create_state(xdif, ydif, velocity);
	int new_action = greedy_action(new_state);

	//if state-action pair does not exist yet make new entry
	if (Q_TABLE.find(last_state) == Q_TABLE.end() && Q_TABLE[last_state].find(last_action) == Q_TABLE[last_state].end()) { //not found
		Q_TABLE[last_state][last_action] = 0;
        pap++;
	} else { // found ie exists
		// Update Q(S,A)
		Q_TABLE[last_state][last_action] += ALPHA * (reward + GAMMA * Q_TABLE[new_state][new_action] - Q_TABLE[last_state][last_action]);
        //Q_TABLE[last_state][last_action] += 1;
        //std::cout << "ls " << last_state << " la " << last_action << " Q " << Q_TABLE[last_state][last_action] <<  '\n';
	}
}


//returns greedy action given the state
int qlearning_agent::greedy_action(string state) {
	int imax = 0;
	double max = 0;
	for(int a=0; a<N_ACTIONS; a++) {
		if (Q_TABLE[state][a] > max){
			max = Q_TABLE[state][a];
			imax = a;
		}
	}
	return imax;
}

/* Performs greedy policy. With prob epsilon pick action
belonging to maximum action-value. With prob 1-epsilon
pick a random action. */
int qlearning_agent::e_greedy_policy(string state) {
	double random = (double)rand() / RAND_MAX;

	if (random < EPSILON){
		return rand() % N_ACTIONS;
	} else {
		return greedy_action(state);
	}
}

void qlearning_agent::print_count(){std::cout << pap << '\n';}

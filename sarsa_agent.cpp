#include <iostream>
#include <stdlib.h>
#include "sarsa_agent.hpp"

sarsa_agent::sarsa_agent() {
    state_count = 0;
    srand(time(NULL));
}

string sarsa_agent::create_state(int xdif, int ydif, int velocity) {
    if (xdif <= 500) {
        xdif = xdif - (xdif % 10);
        ydif = ydif - (ydif % 10);
        //velocity = velocity - (velocity); //edit
    } else {
        //xdif = xdif - (xdif % 100);
        xdif = 1234;
        ydif = ydif - (ydif % 100);
        velocity = velocity - (velocity % 5);
    }



    string state = to_string(xdif) + "_" + to_string(ydif) + "_" + to_string(velocity);
    //std::cout << state << '\n';
    return state;
}

void sarsa_agent::choose_action_greedy(int xdif, int ydif, int velocity){
    string state = create_state(xdif, ydif, velocity);
    last_state = state;
    last_action = greedy_action(state);
}

void sarsa_agent::choose_action(int xdif, int ydif, int velocity){
    string state = create_state(xdif, ydif, velocity);
    last_state = state;
    last_action = e_greedy_policy(state);
}

int sarsa_agent::act() {
    return last_action;
}

void sarsa_agent::update_qtable_sarsa(int xdif, int ydif, int velocity, double reward, bool greedy) {
    //Choose A' from S' using policy derived from Q (e.g. e-greedy)
    string new_state = create_state(xdif, ydif, velocity);
    if (new_state == last_state) return;
	int new_action;
    if (greedy) new_action = greedy_action(new_state);
    else new_action = e_greedy_policy(new_state);

    double update = ALPHA * (reward + GAMMA * (Q_TABLE[new_state][new_action] - Q_TABLE[last_state][last_action]));

	//Update Q(S,A)
	Q_TABLE[last_state][last_action] += update;

    last_action = new_action;
    last_state = new_state;

}

//returns greedy action given the state
int sarsa_agent::greedy_action(string state) {

    if (Q_TABLE.find(state) == Q_TABLE.end() && Q_TABLE[state].find(0) == Q_TABLE[state].end()) { //looks for new state and creates it if it doesnt exist
        Q_TABLE[state][0] = 0;
            //Q_TABLE_COUNT[new_state][new_action] = 0;
        state_count++;
    }

    if (Q_TABLE.find(state) == Q_TABLE.end() && Q_TABLE[state].find(1) == Q_TABLE[state].end()) { //looks for new state and creates it if it doesnt exist
        Q_TABLE[state][1] = 0;
            //Q_TABLE_COUNT[new_state][new_action] = 0;
        state_count++;
    }

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
        if (Q_TABLE.find(state) == Q_TABLE.end() && Q_TABLE[state].find(random_action) == Q_TABLE[state].end()) { //looks for new state and creates it if it doesnt exist
            Q_TABLE[state][random_action] = 0;
                //Q_TABLE_COUNT[new_state][new_action] = 0;
            state_count++;
        }
		return random_action;
	} else {
		return greedy_action(state);
	}
}

void sarsa_agent::print_count(){std::cout << "state_count: "<< state_count << '\n';}

void sarsa_agent::clearQtable(){Q_TABLE.clear();}
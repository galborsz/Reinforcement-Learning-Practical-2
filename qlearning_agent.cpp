#include <iostream>
#include <stdlib.h>
#include "qlearning_agent.hpp"

qlearning_agent::qlearning_agent() {
    state_count = 0;
    srand(time(NULL));
}

string qlearning_agent::create_state(int xdif, int ydif, int velocity) {
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

int qlearning_agent::act(int xdif, int ydif, int velocity) {
    //two next lines can be moved to the end of update
    string state = create_state(xdif, ydif, velocity);
    if (state == last_state) return 0;
    last_state = state;
    last_action = e_greedy_policy(state);
    return last_action;
}


int qlearning_agent::actgreedy(int xdif, int ydif, int velocity) {
    string state = create_state(xdif, ydif, velocity);
    if (state == last_state) return 0;
    int action = greedy_action(state);
    return action;
}


void qlearning_agent::update_qtable(int xdif, int ydif, int velocity, double reward) {
    string new_state = create_state(xdif, ydif, velocity);
    if (new_state == last_state) return;
	int new_action = greedy_action(new_state);

    //std::cout << "ls " << last_state << " maxa " << last_action << " Q " << Q_TABLE[last_state][last_action] <<  '\n';
    //std::cout << "reward " << reward << '\n';

    double update = ALPHA * (reward + GAMMA * (Q_TABLE[new_state][new_action] - Q_TABLE[last_state][last_action]));
    //std::cout << "ALPHA " << ALPHA << '\n';
    //std::cout << "GAMMA " << GAMMA << '\n';
    //std::cout << "Q_TABLE[new_state][new_action] " << Q_TABLE[new_state][new_action] << '\n';
    //std::cout << "Q_TABLE[last_state][last_action]" << Q_TABLE[last_state][last_action] << '\n';
    //std::cout << "GAMMA * Q_TABLE[new_state][new_action] - Q_TABLE[last_state][last_action] " << GAMMA * (Q_TABLE[new_state][new_action] - Q_TABLE[last_state][last_action]) << '\n';
    //std::cout << "new_state " << new_state << " new_action " << new_action << " Q " << Q_TABLE[new_state][new_action] <<  '\n';
    //std::cout << "update " << update << '\n';

	// Update Q(S,A)
	Q_TABLE[last_state][last_action] += update;
        //Q_TABLE_COUNT[last_state][last_action] += 1;
        //Q_TABLE[last_state][last_action] += 1;

    //std::cout << "ls " << last_state << " maxa " << last_action << " Q " << Q_TABLE[last_state][last_action] <<  '\n';

    //std::cout << "\n" << '\n';

    /*
    std::cout << "ls " << last_state << " la " << 0 << " Q " << Q_TABLE[last_state][0] <<  '\n';
    std::cout << "ls " << last_state << " la " << 1 << " Q " << Q_TABLE[last_state][1] <<  '\n';
    std::cout << "ls " << last_state << " maxa " << last_action << " Q " << Q_TABLE[last_state][last_action] <<  '\n';
    */

}


//returns greedy action given the state
int qlearning_agent::greedy_action(string state) {

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
int qlearning_agent::e_greedy_policy(string state) {
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

void qlearning_agent::print_count(){std::cout << "state_count: "<< state_count << '\n';}

void qlearning_agent::clearQtable(){Q_TABLE.clear();}

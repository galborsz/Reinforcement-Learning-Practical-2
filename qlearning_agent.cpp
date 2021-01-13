#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "qlearning_agent.hpp"

qlearning_agent::qlearning_agent(bool eligibility_traces): agent() {
    srand(time(NULL));
    next_state = "starting_state";
    next_action = e_greedy_policy(next_state);
    traces = eligibility_traces;

}

string qlearning_agent::create_state_action_pair(string state, int action) {
    return state + ":" + to_string(action);
}

int qlearning_agent::act() {
    if (next_state == last_state) return 0;
    last_state = next_state;
    last_action = next_action;
    return next_action;
}


void qlearning_agent::update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy) {
    next_state = create_state(xdif, ydif, velocity);
    if (next_state == last_state) return;

    string last_state_action_pair = create_state_action_pair(last_state, last_action);
    iteration_history.push_back(last_state_action_pair);

	int greedy_act = greedy_action(next_state); //A*
    next_action = e_greedy_policy(next_state); //A'

    if (greedy) {
        next_action = greedy_act;
    }

    double update = ALPHA * (reward + GAMMA * (Q_TABLE[next_state][greedy_act] - Q_TABLE[last_state][last_action]));

	// Update Q(S,A)

    if (traces) {
        TRACES[last_state][last_action] += 1; //accumulating traces

        string delimiter = ":";
        for (auto & past_state_action_pairs : iteration_history) {
            size_t pos = 0;
            pos = past_state_action_pairs.find(delimiter);
            string past_state = past_state_action_pairs.substr(0, pos);
            past_state_action_pairs.erase(0, pos + delimiter.length());
            int past_action = atoi(past_state_action_pairs.c_str());

            //update
            Q_TABLE[past_state][past_action] += update * TRACES[past_state][past_action];
            if (next_action == greedy_act) TRACES[past_state][past_action] = GAMMA * LAMBDA * TRACES[past_state][past_action];
            else TRACES[past_state][past_action] = 0;
        }

        if (dead) iteration_history.clear();
    } else Q_TABLE[last_state][last_action] += update;

}


//returns greedy action given the state
int qlearning_agent::greedy_action(string state) {

    if (Q_TABLE.find(state) == Q_TABLE.end() && Q_TABLE[state].find(0) == Q_TABLE[state].end()) { //looks for new state and creates it if it doesnt exist
        Q_TABLE[state][0] = 0;
        if (traces) TRACES[state][0] = 0;
            //Q_TABLE_COUNT[new_state][new_action] = 0;
        state_count++;
    }

    if (Q_TABLE.find(state) == Q_TABLE.end() && Q_TABLE[state].find(1) == Q_TABLE[state].end()) { //looks for new state and creates it if it doesnt exist
        Q_TABLE[state][1] = 0;
        if (traces)TRACES[state][1] = 0;
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
            if (traces) TRACES[state][random_action] = 0;
                //Q_TABLE_COUNT[new_state][new_action] = 0;
            state_count++;
        }
		return random_action;
	} else {
		return greedy_action(state);
	}
}

void qlearning_agent::save_qvalues_to_file() {
 	//csv file configuration
    string filename = "qvalues";
    ofstream myfile(filename + ".csv");

    myfile << "GAMMA" << GAMMA << "ALPHA" << ALPHA << "EPSILON" << EPSILON << endl;

	for (auto const& x : Q_TABLE) {
        for (auto const& y : x.second) {
            myfile << x.first << ':' << y.first << ':' << y.second << endl;
        }
	}

	myfile.close();

}

void qlearning_agent::load_qtables_from_file(string filename) {
    string line;
    string delimiter = ":";
    ifstream myfile (filename + ".csv");
    if (myfile.is_open()) {
        while ( getline (myfile, line) ) {
            size_t pos = 0;
            pos = line.find(delimiter);
            string state = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());
            pos = line.find(delimiter);
            int action = atoi((line.substr(0, pos)).c_str());
            line.erase(0, pos + delimiter.length());
            double qvalue = atof(line.c_str());
            Q_TABLE[state][action] = qvalue;
        }
        myfile.close();
    } else cout << "Unable to open file";
}


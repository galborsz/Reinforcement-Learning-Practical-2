#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "expected_sarsa_agent.hpp"

expected_sarsa_agent::expected_sarsa_agent(): agent() {
    srand(time(NULL));
    next_state = "starting_state";
    //next_action = e_greedy_policy(next_state);
    next_action = 1;
}

string expected_sarsa_agent::create_state_action_pair(string state, int action) {
    return state + ":" + to_string(action);
}

int expected_sarsa_agent::act() {
    if (next_state == last_state) return 0;
    last_state = next_state;
    last_action = next_action;
    return next_action;
}


void expected_sarsa_agent::update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy) {
    next_state = create_state(xdif, ydif, velocity);
    if (next_state == last_state) return;

    /* int greedy_act = greedy_action(next_state); //A*
    next_action = e_greedy_policy(next_state); //A'

    if (greedy) {
        next_action = greedy_act;
    } */

    double QS0 = Q_TABLE[next_state][0];
    double QS1 = Q_TABLE[next_state][1];

    int greedy_act;
    if (QS0 >= QS1) { //doesnt flap in case of tie
        greedy_act = 0;
    } else {
        greedy_act = 1;
    }

    if (greedy) {
        next_action = greedy_act;
    } else { //epsilon greedy
        double random = (double)rand() / RAND_MAX;
	    if (random < EPSILON){
            int random_action = rand() % N_ACTIONS;
		    next_action = random_action;
	    } else {
		    next_action = greedy_act;
	    }
    }

    // mean Q(S',a) for all a
    double meanQ = 0;
    for (int a=0; a<N_ACTIONS; a++){
        if (a == greedy_act) {
            if (greedy){
                meanQ += 1 * Q_TABLE[next_state][next_action];
            } else {
                meanQ += (EPSILON/N_ACTIONS + 1 - EPSILON) * Q_TABLE[next_state][next_action];
            }
        } else {
            if (greedy){
                meanQ += 0;
            } else {
                meanQ += (EPSILON/N_ACTIONS) * Q_TABLE[next_state][next_action];
            } 
        }
    }

    //cout<<"meanQ: "<<meanQ<<endl;

    /*# mean Q(S',a) for all a
        meanQ = 0; aMax = np.argmax(self.Q_FUN[state, :])
        for a in range(self.N_ACTIONS):
          if a == aMax:
            meanQ += (self.EPSILON/self.N_ACTIONS + 1 - self.EPSILON) * self.Q_FUN[next_state, a]
          else:
            meanQ += (self.EPSILON/self.N_ACTIONS) * self.Q_FUN[next_state, a]*/

    double update = ALPHA * (reward + GAMMA * meanQ - Q_TABLE[last_state][last_action]);

	// Update Q(S,A)
    Q_TABLE[last_state][last_action] += update;

}


//returns greedy action given the state 
/*
int qlearning_agent::greedy_action(string state) {
    if (Q_TABLE[state][0] >= Q_TABLE[state][1]) { //doesnt flap in case of tie
        return 0;
    } else {
        return 1;
    }
} */

/* Performs greedy policy. With prob epsilon pick action
belonging to maximum action-value. With prob 1-epsilon
pick a random action. */ /*
int qlearning_agent::e_greedy_policy(string state) {
	double random = (double)rand() / RAND_MAX;
	if (random < EPSILON){
        int random_action = rand() % N_ACTIONS;
		return random_action;
	} else {
		return greedy_action(state);
	}
} */


void expected_sarsa_agent::save_qvalues_to_file() {
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

void expected_sarsa_agent::load_qtables_from_file(string filename) {
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
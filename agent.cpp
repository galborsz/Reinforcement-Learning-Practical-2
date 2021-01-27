#include <iostream>
#include <iomanip>
#include <fstream>
#include "agent.hpp"

using namespace std;

agent::agent() {
    state_count = 0;
    EPSILON = 1;
}

int agent::act() {
    cout << "failed action, implement for agent" << endl;
    return 0;
}

void agent::update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy) {
    cout << "failed update, implement for agent" << endl;
}

void agent::set_epsilon(int iteration, double rate_of_decay) {
    //exponnential decay
	EPSILON = exp(-1*(double)iteration*rate_of_decay);
}

string agent::create_state(int xdif, int ydif, int velocity) {
    if (xdif <= 500) {
        xdif = xdif - (xdif % 15);
        ydif = ydif - (ydif % 15);
    } else {
        xdif = 1234;
        ydif = ydif - (ydif % 150);
        velocity = velocity - (velocity % 2);
    }

    string state = to_string(xdif) + "_" + to_string(ydif) + "_" + to_string(velocity);
    return state;
}

void agent::print_state_count(){std::cout << "state_count: "<< state_count << '\n';}

void agent::save_qvalues_to_file() {cout << "qvalues save failed, not implemented for this agent" << endl;}

void agent::load_qtables_from_file(string filename) {cout << "load failed, not implemented for this agent" << endl;}





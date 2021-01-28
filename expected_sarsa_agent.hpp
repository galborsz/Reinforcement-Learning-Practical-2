#ifndef __MY_EXPECTED_SARSA_AGENT_H__
#define __MY_EXPECTED_SARSA_AGENT_H__

#include <iostream>
#include "agent.hpp"


using namespace std;

class expected_sarsa_agent: public agent {
    public:
        expected_sarsa_agent(string exploration_strategy);
        int act();
        void update(int xdif, int ydif, int velocity, double reward);
        void save_qvalues_to_file();
        void load_qtables_from_file(string filename);
};

#endif /* EXPECTED_SARSA_AGENT */

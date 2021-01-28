#ifndef __MY_SARSA_AGENT_H__
#define __MY_SARSA_AGENT_H__

#include <iostream>
#include "agent.hpp"


using namespace std;

class sarsa_agent: public agent {
    public:
        sarsa_agent(string exploration_strategy);
        void set_epsilon(int iteration);
        int act();
        void update(int xdif, int ydif, int velocity, double reward);
};

#endif /* SARSA_AGENT */

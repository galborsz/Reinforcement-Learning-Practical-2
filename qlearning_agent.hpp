#ifndef __MY_QLEARNING_AGENT_H__
#define __MY_QLEARNING_AGENT_H__

#include <iostream>
#include "agent.hpp"


using namespace std;

class qlearning_agent: public agent {
    public:
        qlearning_agent(string exploration_strategy);
        int act();
        void set_epsilon(int iteration);
        void update(int xdif, int ydif, int velocity, double reward);
        void save_qvalues_to_file();
        void load_qtables_from_file(string filename);
};

#endif /* QLEARNING_AGENT */

#ifndef __MY_SARSA_AGENT_H__
#define __MY_SARSA_AGENT_H__

#include <iostream>
#include <unordered_map>
#include "agent.hpp"


using namespace std;

class sarsa_agent: public agent {
    private:
        static constexpr double GAMMA = 1;
        static constexpr double ALPHA = 0.7;
        static constexpr double EPSILON = 0.01;
        string last_state;
        int last_action;
        string next_state;
        int next_action;
        unordered_map<string, unordered_map<int, double> > Q_TABLE;
        int greedy_action(string state);
        int e_greedy_policy(string state);

    public:
        sarsa_agent();
        int act();
        void update(int xdif, int ydif, int velocity, double reward,  bool dead, bool greedy);
};

#endif /* SARSA_AGENT */

#ifndef __MY_EXPECTED_SARSA_AGENT_H__
#define __MY_EXPECTED_SARSA_AGENT_H__

#include <iostream>
#include <unordered_map>
#include <vector>
#include "agent.hpp"


using namespace std;

class expected_sarsa_agent: public agent {
    private:
        static constexpr double GAMMA = 1;
        static constexpr double ALPHA = 0.7;
        static constexpr double LAMBDA = 0.5;
        int state_count;
        string last_state;
        int last_action;
        int next_action;
        string next_state;
        unordered_map<string, unordered_map<int, double> > Q_TABLE;
        vector<string> iteration_history;
        //int greedy_action(string state);
        //int e_greedy_policy(string state);
        string create_state_action_pair(string state, int action);

    public:
        expected_sarsa_agent();
        int act();
        void update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy);
        void save_qvalues_to_file();
        void load_qtables_from_file(string filename);
};

#endif /* EXPECTED_SARSA_AGENT */

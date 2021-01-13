#ifndef __MY_QLEARNING_AGENT_H__
#define __MY_QLEARNING_AGENT_H__

#include <iostream>
#include <unordered_map>
#include <vector>


using namespace std;

class qlearning_agent {
    private:
        static constexpr double GAMMA = 1;
        static constexpr double ALPHA = 0.7;
        static constexpr double EPSILON = 0.005;
        static constexpr int N_ACTIONS = 2;
        static constexpr double LAMBDA = 0.5;
        bool traces;
        int state_count;
        string last_state;
        int last_action;
        int next_action;
        string next_state;
        unordered_map<string, unordered_map<int, double> > Q_TABLE;
        unordered_map<string, unordered_map<int, double> > TRACES;
        vector<string> iteration_history;

    public:
        qlearning_agent(bool eligibility_traces);
        int act();
        void update_qtable(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy);
        string create_state_action_pair(string state, int action);
        int greedy_action(string state);
        int e_greedy_policy(string state);
        string create_state(int xdif, int ydif, int velocity);
        void print_count();
        void save_qvalues_to_file();
        void load_qtables_from_file(string filename);
};

#endif /* QLEARNING_AGENT */

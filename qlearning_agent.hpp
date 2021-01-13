#ifndef __MY_QLEARNING_AGENT_H__
#define __MY_QLEARNING_AGENT_H__

#include <iostream>
#include <unordered_map>


using namespace std;

class qlearning_agent {
    private:
        static constexpr double GAMMA = 1;
        static constexpr double ALPHA = 0.7;
        static constexpr double EPSILON = 0.01;
        static constexpr int N_ACTIONS = 2;
        int pap;
        string last_state;
        int last_action;
        int state_count;
        unordered_map<string, unordered_map<int, double> > Q_TABLE;
        unordered_map<string, unordered_map<int, double> > Q_TABLE_COUNT;
        void initialize_qtable();

    public:
        qlearning_agent();
        int act(int xdif, int ydif, int velocity);
        int actgreedy(int xdif, int ydif, int velocity);
        void update_qtable_qlearning(int xdif, int ydif, int velocity, double reward);
        int greedy_action(string state);
        int e_greedy_policy(string state);
        string create_state(int xdif, int ydif, int velocity);
        void print_count();
        void clearQtable();
};

#endif /* QLEARNING_AGENT */

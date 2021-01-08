#ifndef __MY_QLEARNING_AGENT_H__
#define __MY_QLEARNING_AGENT_H__

#include <iostream>
#include <unordered_map>


using namespace std;

class qlearning_agent {
    private:
        static const double GAMMA = 1;
        static const double ALPHA = 0.5;
        static const double EPSILON = 0.1;
        static const int N_ACTIONS = 2;
        int pap;
        string last_state;
        int last_action;
        unordered_map<string, unordered_map<int, float> > Q_TABLE;

    public:
        qlearning_agent();
        int act(int xdif, int ydif, int velocity);
        int actgreedy(int xdif, int ydif, int velocity);
        void update_qtable(int xdif, int ydif, int velocity, int reward);
        int greedy_action(string state);
        int e_greedy_policy(string state);
        string create_state(int xdif, int ydif, int velocity);
        void print_count();
};

#endif /* QLEARNING_AGENT */

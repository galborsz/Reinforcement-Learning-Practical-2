#ifndef __MY_AGENT_H__
#define __MY_AGENT_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

class agent { 
    protected:
        static constexpr int N_ACTIONS = 2;
        double GAMMA;
        double ALPHA;
        double EPSILON;
        double c;
        int t;
        string p_exploration_strategy;
        string last_state;
        int last_action;
        string next_state;
        int next_action;
        unordered_map<string, unordered_map<int, double> > Q_TABLE;
        unordered_map<string, unordered_map<int, int> > ACTION_COUNTS;

    public:
        agent();            
        agent(string exploration_strategy);
        virtual int act();
        virtual void update(int xdif, int ydif, int velocity, double reward);
        void set_epsilon(int iteration, double rate_of_decay);
        string create_state(int xdif, int ydif, int velocity);
        virtual void save_qvalues_to_file();
        virtual void load_qtables_from_file(string filename);
};

#endif /* MY_AGENT_H */
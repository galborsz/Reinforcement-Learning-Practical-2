#ifndef __MY_AGENT_H__
#define __MY_AGENT_H__

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class agent { 
    protected:
        static constexpr int N_ACTIONS = 2;
        int state_count;
        double EPSILON;

    public:            
        agent();
        virtual int act();
        virtual void update(int xdif, int ydif, int velocity, double reward, bool dead, bool greedy);
        void set_epsilon(int iteration, double rate_of_decay);
        string create_state(int xdif, int ydif, int velocity);
        void print_state_count();
        virtual void save_qvalues_to_file();
        virtual void load_qtables_from_file(string filename);
};

#endif /* MY_AGENT_H */
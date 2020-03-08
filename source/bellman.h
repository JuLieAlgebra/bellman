/*
Class for solving the finite-space, deterministic-reward,
stochastic Bellman equation via fixed-point iteration.
*/

////////////////////////////////////////////////// DEPENDENCIES

#include <vector>
#include <cmath>
#include <limits>
#include <fstream>

////////////////////////////////////////////////// ALIASES

namespace bellman {

using Index = uint;
using Real = double;

template <class T>
using Vector = std::vector<T>;

Real constexpr INF = std::numeric_limits<Real>::infinity();

////////////////////////////////////////////////// MAIN

class Bellman {
    uint const nS; // cardinality of the state space
    uint const nA; // cardinality of the action space
    Real const discount; // factor to discount future reward, on [0,1]
    Vector<Real> value; // current optimal value function estimate
    Vector<Index> policy; // current optimal policy estimate

public:
    Bellman(uint nS, uint nA, Real discount) :
        nS(nS),
        nA(nA),
        discount(discount),
        value(nS),
        policy(nS) {
    }

    // Returns the probability of transitioning to state s1 given state s and action a
    virtual Real dynamic(Index s, Index a, Index s1) const =0;
    // Returns the reward for selecting action a in state s
    virtual Real reward(Index s, Index a) const =0;

    // Access methods
    Real get_value_at(Index s) const {return value.at(s);}
    Index get_action_at(Index s) const {return policy.at(s);}
    Vector<Real> get_value() const {return value;}
    Vector<Index> get_policy() const {return policy;}

    // Write optimal actions and values to the given file
    void record(std::string const& file) const {
        std::ofstream stream;
        stream.open(file);
        stream << "s, a, v" << std::endl;
        for(Index s=0; s<nS; ++s) {
            stream << s << ", " << policy[s] << ", " << value[s] << std::endl;
        }
        stream.close();
    }

    // Improves the current value function and policy estimate by the given number of fixed-point iterations
    void improve(uint iterations) {
        for(uint i=0; i<iterations; ++i) {
            // Iterate over starting states
            for(Index s=0; s<nS; ++s) {
                // Prepare to maximize over actions
                Real best_value = -INF;
                Index best_action = 0;
                // Iterate over action choices
                for(Index a=0; a<nA; ++a) {
                    // Prepare to compute expected next value
                    Real expectation = 0.0;
                    // Iterate over ending states
                    for(Index s1=0; s1<nS; ++s1) {
                        // Accrue expectation integral
                        expectation += dynamic(s, a, s1) * value[s1];
                    }
                    // Compare candidate to best so far
                    Real candidate = reward(s, a) + discount*expectation;
                    if(candidate > best_value) {
                        best_value = candidate;
                        best_action = a;
                    }
                }
                // Fixed-point iterate on value for this starting state
                value[s] = best_value;
                policy[s] = best_action;
            }
        }
    }
};

//////////////////////////////////////////////////

} // namespace bellman

/*
Class for solving the finite stochastic Bellman equation via fixed-point iteration.
*/

////////////////////////////////////////////////// DEPENDENCIES

#include <vector>
#include <cmath>
#include <limits>

////////////////////////////////////////////////// ALIASES

using Index = uint;
using Real = double;

template <class T>
using Vector = std::vector<T>;

Real constexpr INF = std::numeric_limits<Real>::infinity();

////////////////////////////////////////////////// MAIN

class Bellman {
    uint const nS;
    uint const nA;
    Real const discount;
    Vector<Real> value;
    Vector<Index> policy;

public:
    Bellman(uint nS, uint nA, Real discount) :
        nS(nS),
        nA(nA),
        discount(discount),
        value(nS),
        policy(nS) {
    }

    virtual Real dynamic(Index s, Index a, Index s1) =0;
    virtual Real reward(Index s, Index a) =0;

    void solve(uint iterations) {
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

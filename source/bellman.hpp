/*
Abstract-base-class for solving the finite-space, deterministic-reward,
time-invariant, stochastic Bellman equation via fixed-point iteration.
*/

////////////////////////////////////////////////// DEPENDENCIES

// Standard math
#include <vector>
#include <cmath>
#include <limits>

// Standard interfacing
#include <iostream>
#include <fstream>

////////////////////////////////////////////////// ALIASES

namespace bellman {

using Index = uint; // discrete representation for the state and action spaces
using Real = double; // ordinary real numbers

template <class T>
using Vector = std::vector<T>; // just an abbreviation

Real constexpr INF = std::numeric_limits<Real>::infinity(); // floating-point infinity

////////////////////////////////////////////////// CORE

// Abstract-base-class that various Markov decision processes can inherit from to
// get solved by the value-iteration algorithm. Derived classes need to implement
// the dynamic and reward methods as shown.
class Bellman {
protected:
    uint const nS; // cardinality of the state space
    uint const nA; // cardinality of the action space
    Real const discount; // factor to discount future reward, between 0.0 and 1.0
    Vector<Real> value; // current optimal value function estimate
    Vector<Index> policy; // current optimal policy estimate
    Vector<Vector<Vector<std::pair<Index, Real>>>> transitions; // optional sparse transition matrix SxAxS'

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
    // Returns the (deterministic) reward for selecting action a in state s
    virtual Real reward(Index s, Index a) const =0;

    // Access methods
    Real get_value_at(Index s) const {return value.at(s);}
    Index get_action_at(Index s) const {return policy.at(s);}
    Vector<Real> get_value() const {return value;}
    Vector<Index> get_policy() const {return policy;}

    // Write the current solution to the given file or terminal
    void record_solution(std::string const& file) const {
        // Open and clear file
        std::ofstream stream;
        stream.open(file);
        // Write header string as first line
        stream << "s, a, v" << std::endl;
        for(Index s=0; s<nS; ++s) {
            // Write comma-delimited state-action-value tuples
            stream << s << ", " << policy[s] << ", " << value[s] << std::endl;
        }
        // Close file
        stream.close();
    }
    void print_solution() const {
        // Print header
        std::cout << "================" << std::endl;
        std::cout << "Bellman Solution" << std::endl;
        std::cout << "s | a | v" << std::endl;
        std::cout << "----------------" << std::endl;
        for(Index s=0; s<nS; ++s) {
            // Print pipe-delimited state-action-value tuples
            std::cout << s << " | " << policy[s] << " | " << value[s] << std::endl;
        }
        std::cout << "================" << std::endl;
    }

    // Improves the current value function and policy estimate by the given number of
    // fixed-point iterations or until the given convergence tolerance is met
    void improve(uint iterations, Real tolerance) {
        bool converged;
        std::cout << "================" << std::endl;
        std::cout << "Bellman improvement beginning..." << std::endl;
        for(uint i=1; i<=iterations; ++i) {
            // Alert user of progress
            if(fmod(100.0*i/iterations, 20.0) == 0.0) {
                std::cout << "(" << i << " / " << iterations << ")" << std::endl;
            }
            // Assume converged unless any values prove to still be changing
            converged = true;
            // Iterate over starting states
            for(Index s=0; s<nS; ++s) {
                // Prepare to maximize over actions
                Real best_value = -INF;
                Index best_action = 0;
                // Iterate over action choices
                for(Index a=0; a<nA; ++a) {
                    // Prepare to compute expected next value
                    Real expectation = 0.0;
                    // Iterate over ending states to accrue expectation integral
                    if(transitions.size()) {
                        // Leverage sparsity to sum only possible transitions
                        for(std::pair<Index, Real> const& s1_p : transitions[s][a]) {
                            expectation += s1_p.second * value[s1_p.first];
                        }
                    } else {
                        // Sum over all ending states
                        for(Index s1=0; s1<nS; ++s1) {
                            expectation += dynamic(s, a, s1) * value[s1];
                        }
                    }
                    // Compare candidate to best so far
                    Real candidate = reward(s, a) + discount*expectation;
                    if(candidate > best_value) {
                        best_value = candidate;
                        best_action = a;
                    }
                }
                // Check convergence of this state's value
                converged = converged and (fabs(value[s] - best_value) < tolerance);
                // Fixed-point iterate on value for this starting state
                value[s] = best_value;
                policy[s] = best_action;
            }
            // If value converged for all states, finish early
            if(converged) {
                std::cout << "... Converged at iteration " << i << " of " << iterations << "." << std::endl;
                break;
            }
        }
        if(not converged) {
            std::cout << "... Finished at max iteration " << iterations << "." << std::endl;
        }
        std::cout << "================" << std::endl;
    }

    // Helper for converting multidimensional coordinates to a linear vector index
    Index index_from_coords(Vector<uint> const& coords, Vector<uint> const& dims) const {
        Index index = 0;
        uint const n = std::min(coords.size(), dims.size());
        for(uint i=0; i<n; ++i) {
            index = coords[i] + dims[i]*index;
        }
        return index;
    }

    // Helper for converting a linear vector index into multidimensional coordinates
    Vector<uint> coords_from_index(Index index, Vector<uint> const& dims) const {
        Vector<uint> coords(dims.size());
        for(int i=dims.size()-1; i>=0; --i) {
            coords[i] = index % dims[i];
            index /= dims[i];
        }
        return coords;
    }
};

//////////////////////////////////////////////////

} // namespace bellman

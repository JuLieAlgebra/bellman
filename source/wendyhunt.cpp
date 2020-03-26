/*
Using the Bellman class to solve the Wendy-Hunt Markov decision process.
*/

////////////////////////////////////////////////// DEPENDENCIES

#include "bellman.hpp"
using namespace bellman;

////////////////////////////////////////////////// CORE

class WendyHunt : public Bellman {
    Vector<Vector<Vector<Real>>> const T; // transition matrix
    Vector<Vector<Real>> const R; // reward matrix

public:
    WendyHunt() :
        //      nS nA   g
        Bellman(3, 2, 0.99),
        // Assign transition matrix values
        T({{{  1,   0,   0},
            {  1,   0,   0},
            {  0, 0.3, 0.7}},
           {{0.4,   0, 0.6},
            {0.1, 0.6, 0.3},
            {  0, 0.1, 0.9}}}),
        // Assign reward matrix values
        R({{1, 1, 3},
           {0, 0, 2}}) {
        // Sanity checks
        verify_dynamic();
    }

    Real dynamic(Index s, Index a, Index s1) const {
        return T[a][s][s1];
    }

    Real reward(Index s, Index a) const {
        return R[a][s];
    }
};

////////////////////////////////////////////////// MAIN

// Solves the Wendy Hunt problem and displays the results
int main(/*int argc, char** argv*/) {
    WendyHunt mdp;
    mdp.improve(2000, 1e-4);
    mdp.print_solution();
    return 0;
}

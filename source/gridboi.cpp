/*
Using the Bellman class to solve the Grid-Boi Markov decision process.
*/

////////////////////////////////////////////////// DEPENDENCIES

#include "bellman.hpp"
using namespace bellman;

////////////////////////////////////////////////// CORE

class GridBoi : public Bellman {
public:
    // Grid dimensions
    uint const nX;
    uint const nY;

    // State description
    struct State {
        struct Coord {
            uint x;
            uint y;
        };
        Coord boi;
        Coord gob;
        Coord goo;
    };

    // Action description
    enum Action {WAIT, UP, DOWN, LEFT, RIGHT};

    // Setup the Grid-Boi problem
    GridBoi() :
        //            nS    nA   g
        Bellman(pow(5*5, 3), 5, 0.9),
        nX(5),
        nY(5) {
    }

    // Computes the state space linear index corresponding to the given structured state description
    Index index_from_state(State const& state) const {
        return state.boi.x + nX*(
                   state.boi.y + nY*(
                       state.gob.x + nX*(
                           state.gob.y + nY*(
                               state.goo.x + nX*(
                                   state.goo.y)))));
    }

    // Computes the structured state description corresponding to the given state space linear index
    State state_from_index(Index index) const {
        State s; Index p = index;
        s.boi.x = p % nX; p /= nX;
        s.boi.y = p % nY; p /= nY;
        s.gob.x = p % nX; p /= nX;
        s.gob.y = p % nY; p /= nY;
        s.goo.x = p % nX; p /= nX;
        s.goo.y = p % nY; p /= nY;
        return s;
    }

    Real dynamic(Index s, Index a, Index s1) const {
        return 0; // ???
    }

    Real reward(Index s, Index a) const {
        return 0; /// ???
    }
};

////////////////////////////////////////////////// MAIN

// Solves the Grid-Boi problem and displays the results
int main(/*int argc, char** argv*/) {
    GridBoi mdp;

    // mdp.improve(5000, 1e-4);
    // mdp.print_solution();
    return 0;
}

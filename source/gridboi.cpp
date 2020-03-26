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
            bool operator==(Coord const& other) const {
                return (x == other.x) and (y == other.y);
            }
            bool operator!=(Coord const& other) const {
                return (x != other.x) or (y != other.y);
            }
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

    // Returns the probability of transitioning to state s1 given state s and action a
    Real dynamic(Index s_index, Index a, Index s1_index) const {
        State const s = state_from_index(s_index);
        State const s1 = state_from_index(s1_index);
        // Evaluate validity of boi move
        if(a == Action::WAIT) {
            // Stand still
            if(s1.boi != s.boi) return 0.0;
        } else if(a == Action::UP) {
            // Stand still if top of grid
            if((s.boi.y == nY-1) and (s1.boi != s.boi)) return 0.0;
            // Move y+1
            else if((s1.boi.x != s.boi.x) or (s1.boi.y != s.boi.y+1)) return 0.0;
        } else if(a == Action::DOWN) {
            // Stand still if bottom of grid
            if((s.boi.y == 0) and (s1.boi != s.boi)) return 0.0;
            // Move y-1
            else if((s1.boi.x != s.boi.x) or (s1.boi.y != s.boi.y-1)) return 0.0;
        } else if(a == Action::LEFT) {
            // Stand still if leftmost of grid
            if((s.boi.x == 0) and (s1.boi != s.boi)) return 0.0;
            // Move x-1
            else if((s1.boi.x != s.boi.x-1) or (s1.boi.y != s.boi.y)) return 0.0;
        } else if(a == Action::RIGHT) {
            // Stand still if rightmost of grid
            if((s.boi.x == nX-1) and (s1.boi != s.boi)) return 0.0;
            // Move x+1
            else if((s1.boi.x != s.boi.x+1) or (s1.boi.y != s.boi.y)) return 0.0;
        }
        // TODO ???
        Real p = 1.0;
        return p;
    }

    // Returns the (deterministic) reward for selecting action a in state s
    Real reward(Index s_index, Index a) const {
        State const s = state_from_index(s_index);
        // Get the goo!
        if(s.boi == s.goo) return 1;
        // Avoid the gob!
        if(s.boi == s.gob) return -1;
        return 0;
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

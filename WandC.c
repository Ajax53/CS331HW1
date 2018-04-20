#include <vector> //For the priority queue generation
#include <queue>  //For the priority queue generation
#include <cstdio> //For fprintf and FILE* streams. 

class Node {
     public:
	  int lChickens;
	  int lWolves;
	  int lBoats;
	  int rChickens;
	  int rWolves;
	  int rBoats;
	  Node* parent;
	  int priority;
};

//Four evaluation functions.
//     Returning a string that represents the path to the optimal solution.
//Four successor functions.
//     Returns the nodes that can be reached from a given node. 
//A single is goal node
//     bool matches(Node n, Node g)

int main(int argc, char** argv) {
     //Get the intial nodes
     //Get the goal node.
     //Switch on the modde
     //     BFS
     //          call BFSEval
     //     DFS
     //          call DFSEval
     //     IDDFS
     //          call IDDFSEval
     //     A*
     //          call A*Eval
     //Print the solution to stdout and to a file. 
     return 0;
}

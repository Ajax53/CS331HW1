#include <vector> //For the priority queue generation
#include <queue>  //For the priority queue generation
#include <cstdio> //For fprintf and FILE* streams. 
#include <string> //For the toString function. And generally passing things around. 

class Node {
     public:
	  Node();
	  Node(int lc, int lw, int lb, int rc, int rw, int rb, Node *pp, int p);
	  Node(char tempArray[100]);

	  int lChickens;
	  int lWolves;
	  int lBoats;
	  int rChickens;
	  int rWolves;
	  int rBoats;
	  Node* parent;
	  int priority;
	  std::string toString();
};

//Four evaluation functions.
//     Returning a string that represents the path to the optimal solution.
//Four successor functions.
//     Returns the nodes that can be reached from a given node. 
//A single is goal node
//     bool matches(Node n, Node g)

int main(int argc, char** argv) {
     FILE * filePointer;
     char tempArray[100];
     
     //Get the initial node
     filePointer = fopen(argv[1], "r");
     if (filePointer == NULL) {
          fprintf(stderr, "Error opening the initialization file\n");
	  fprintf(stderr, "%s\n", argv[1]);
     }
     fgets(tempArray, 100, filePointer);
     Node initNode(tempArray);

     //Get the goal node.
     filePointer = fopen(argv[2], "r");
     if (filePointer == NULL) {
          fprintf(stderr, "Error opening the goal file\n");
	  fprintf(stderr, "%s\n", argv[2]);
     }
     fgets(tempArray, 100, filePointer);
     Node goalNode(tempArray);

     fprintf(stdout, "%s\n%s\n", initNode.toString(), goalNode.toString());


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

Node::Node() {
     lChickens = 0;
     lWolves = 0;
     lBoats = 0;
     rChickens = 0;
     rWolves = 0;
     rBoats = 0;
     parent = NULL;
     priority = 0;
}

Node::Node(int lc, int lw, int lb, int rc, int rw, int rb, Node *pp, int p) {
     lChickens = ls;
     lWolves = lw;
     lBoats = lb;
     rChickens = rc;
     rWolves = rw;
     rBoats = rb;
     parent = pp;
     priority = p;
}

Node::Node(char tempArray[100]) { 
     //Parse the character array
     lChickens = 0;
     lWolves = 0;
     lBoats = 0;
     rChickens = 0;
     rWolves = 0;
     rBoats = 0;
     parent = NULL;
     priority = 0;
}

string Node::toString() {
     std::string s(lChickens, ",", lWolves, ",", lBoats, "\n", rChickens, ",", rWolves, ",", rBoats);
     return s;
}


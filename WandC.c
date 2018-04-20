#include <vector>  //For the priority queue generation
#include <queue>   //For the priority queue generation
#include <cstdio>  //For fprintf and FILE* streams.
//#include <stdio.h> //For strtok this is cstdio
//#include <string.h> //For strtok this is cstring
#include <cstdio>  //For fprintf and FILE* streams.
#include <cstring> //For the strtok. In c++ you want to include these instead of .h files
#include <string>  //For the toString function. And generally passing things around.
#include <map>     //For the already expanded nodes container.

class Node {
     public:
	  Node();
	  Node(int lc, int lw, int lb, int rc, int rw, int rb, Node *pp, int p);
	  Node(char leftChars[50], char rightChars[50]);

	  bool exists;
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

struct Successor {
     Node c;
     Node cc;
     Node w;
     Node wc;
     Node ww;
};

//Four evaluation functions.
//     Returning a string that represents the path to the optimal solution.
//     The returned string should be a composition of all of the toStrings
//     from the found goal node all the way back up to the root node.
//     Expanded node list maps std::string to Node[5]

//Four successor functions.
//     Returns the nodes that can be reached from a given node.
//     This will return a Successor type

//A single is goal node
//     bool matches(Node n, Node g)
bool isGoal(Node checkNode, Node goalNode);

int astarHeuristic(Node checkNode, Node goalNode);

int main(int argc, char** argv) {
     FILE * filePointer;
     char leftChars[50];
     char rightChars[50];

     //Get the initial node
     filePointer = fopen(argv[1], "r");
     if (filePointer == NULL) {
          fprintf(stderr, "Error opening the initialization file\n");
	  fprintf(stderr, "%s\n", argv[1]);
     }
     fgets(leftChars, 50, filePointer);
     fgets(rightChars, 50, filePointer);
     Node initNode(leftChars, rightChars);
     fclose(filePointer);

     //Get the goal node.
     filePointer = fopen(argv[2], "r");
     if (filePointer == NULL) {
          fprintf(stderr, "Error opening the goal file\n");
	  fprintf(stderr, "%s\n", argv[2]);
     }
     fgets(leftChars, 50, filePointer);
     fgets(rightChars, 50, filePointer);
     Node goalNode(leftChars, rightChars);
     fclose(filePointer);

     fprintf(stdout, "%s\n%s\n", initNode.toString().c_str(), goalNode.toString().c_str());

    
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
     exists = false;
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
     exists = true;
     lChickens = lc;
     lWolves = lw;
     lBoats = lb;
     rChickens = rc;
     rWolves = rw;
     rBoats = rb;
     parent = pp;
     priority = p;
}

Node::Node(char leftChars[50], char rightChars[50]) {
     //Parse the character array
     char * pch;
     pch = strtok(leftChars, ", ");
     lChickens = atoi(pch);

     pch = strtok(NULL, " ,");
     lWolves = atoi(pch);

     pch = strtok(NULL, " ,");
     lBoats = atoi(pch);


     pch = strtok(rightChars, ", ");
     rChickens = atoi(pch);

     pch = strtok(NULL, " ,");
     rWolves = atoi(pch);

     pch = strtok(NULL, " ,");
     rBoats = atoi(pch);

     parent = NULL;
     priority = 0;
}

std::string Node::toString() {
     std::string s;
     s += std::to_string(lChickens);
     s += ",";
     s += std::to_string(lWolves);
     s += ",";
     s += std::to_string(lBoats);
     s += "\n";
     s += std::to_string(rChickens);
     s += ",";
     s += std::to_string(rWolves);
     s += ",";
     s += std::to_string(rBoats);
     s += "\n";
     return s;
}

int astarHeuristic(Node checkNode, Node goalNode){
    int diff = 0;

    // Add all differences betweeen the two node states
    diff += abs(goalNode.lChickens - checkNode.lChickens);
    diff += abs(goalNode.lWolves - checkNode.lWolves);
    diff += abs(goalNode.lBoats - checkNode.lBoats);
    diff += abs(goalNode.rChickens - checkNode.rChickens);
    diff += abs(goalNode.rWolves - checkNode.rWolves);
    diff += abs(goalNode.rBoats - checkNode.rBoats);

return diff;
}

bool isGoal(Node checkNode, Node goalNode){
    //Check differences between states
    if (astarHeuristic(checkNode, goalNode) == 0)
        return true;
    else
        return false;
}

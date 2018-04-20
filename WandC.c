#include <vector>  //For the priority queue generation
#include <queue>   //For the priority queue generation
#include <cstdio>  //For fprintf and FILE* streams.
#include <cstdio>  //For fprintf and FILE* streams.
#include <cstring> //For the strtok. 
#include <string>  //For the toString function. And generally passing things around.
#include <map>     //For the already expanded nodes container.
#include <functional> //For the comparator functions for the priority queue

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
	  int depth;

	  std::string toString();
};

struct Successor {
     Node c;
     Node cc;
     Node w;
     Node wc;
     Node ww;
};

//Comparison functions
bool bfsCmp(Node lhs, Node rhs) { return lhs.depth < rhs.depth; }
bool dfsCmp(Node lhs, Node rhs) { return lhs.depth > rhs.depth; }

//Four evaluation functions.
//     Returning a string that represents the path to the optimal solution.
//     The returned string should be a composition of all of the toStrings
//     from the found goal node all the way back up to the root node.
//     Expanded node list maps std::string to Node[5] (struct Successor)
std::string bfsEval(Node initialNode, Node goalNode);
std::string dfsEval(Node initialNode, Node goalNode);
std::string iddfsEval(Node initialNode, Node goalNode);
std::string astarEval(Node initialNode, Node goalNode);

//Four successor functions.
//     Returns the nodes that can be reached from a given node.
//     This will return a Successor struct

struct Successor bfsSuccessor(Node parent);
struct Successor dfsSuccessor(Node parent);
struct Successor iddfsSuccessor(Node parent);
struct Successor astarSuccessor(Node parent);

//A single is goal node
//     bool matches(Node n, Node g)
bool isGoal(Node checkNode, Node goalNode);

int astarHeuristic(Node checkNode, Node goalNode);

int main(int argc, char** argv) {
     if (argc != 5) {
     	fprintf(stdout, "Please enter the WandC command in the following format:\n");
	fprintf(stdout, "\tWandC <initial state file> <goal state file> <mode> <output file>\n");
	return 1;
     }
     
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

     //Switch on the mode
     switch (argv[3][0]) {
	  case 'b':
	       fprintf(stdout, "BFS!\n");
	       break;
	  case 'd':
	       fprintf(stdout, "DFS!\n");
	       break;
	  case 'i':
               fprintf(stdout, "IDDFS!\n");
	       break;
	  case 'a':
	       fprintf(stdout, "ASTAR!\n");
	       break;
	  default:
	       fprintf(stdout, "Please enter a valid mode. (bfs, dfs, iddfs, astar)\n");
     }
    
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
     depth = 0;
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
     depth = pp->depth + 1;
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
     depth = 0;
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

std::string bfsEval(Node initialNode, Node goalNode) {
     std::string returnString("");
     int nodesExpanded = 0, solutionLength;
     
     if (isGoal(initialNode, goalNode)) {
          solutionLength = 1;
          returnString += initialNode.toString();
     } else {
          //Make the expanded nodes map
	  std::map<std::string, struct Successor> expendedNodes;

	  //Make the priority queue.
	  std::priority_queue<Node, std::vector<Node>, std::function<bool(Node, Node)>> fringe(bfsCmp);
	  //loop over the priority queue until we find a solution on the queue. 
     }

     returnString += "\nnodes expanded: ";
     returnString += std::to_string(nodesExpanded);
     returnString += "\nnodes in solution: ";
     returnString += std::to_string(solutionLength);
     returnString += "\n";

     return returnString;
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

//  fprintf(stdout, "%s\n%s\n", initNode.toString().c_str(), goalNode.toString().c_str());

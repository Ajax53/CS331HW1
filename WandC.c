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
	  std::string getPathString();
};

struct Successor {
     Node c;
     Node cc;
     Node w;
     Node wc;
     Node ww;
};

struct iterativeDeepening {
     std::string value;
     int nodesExpanded;
     bool finished;
};

//Comparison functions
bool bfsCmp(Node lhs, Node rhs) { return lhs.priority < rhs.priority; }
bool dfsCmp(Node lhs, Node rhs) { return lhs.priority > rhs.priority; }

//Four evaluation functions.
//     Returning a string that represents the path to the optimal solution.
//     The returned string should be a composition of all of the toStrings
//     from the found goal node all the way back up to the root node.
//     Expanded node list maps std::string to Node[5] (struct Successor)
std::string bfsEval(Node initialNode, Node goalNode);
std::string dfsEval(Node initialNode, Node goalNode);
struct iterativeDeepening iddfsEval(Node initialNode, Node goalNode, int depthLimit, struct iterativeDeepening oldValues);
std::string astarEval(Node initialNode, Node goalNode);

//two successor functions.
//     Returns the nodes that can be reached from a given node.
//     This will return a Successor struct
//     We found that the only difference is the priority assigned to
//     the nodes which is just depth in the first three cases.

struct Successor depthSuccessor(Node *parent);
struct Successor astarSuccessor(Node *parent, Node goalNode);

//A single is goal node
//     bool matches(Node n, Node g)
bool isGoal(Node checkNode, Node goalNode);

//Heuristic function for a-star search
int astarHeuristic(Node checkNode, Node goalNode);

//isValid function determines whether a node is a valid next move
bool isValid(Node parent, int lc, int lw, int lb, int rc, int rw, int rb);

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
	  return 1;
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
	  return 1;
     }
     fgets(leftChars, 50, filePointer);
     fgets(rightChars, 50, filePointer);
     Node goalNode(leftChars, rightChars);
     fclose(filePointer);

     std::string evaluationResult;
     struct iterativeDeepening resultStruct;
     resultStruct.nodesExpanded = 0;
     int depthLimit;

     //Opening the output file here so we don't go through
     //the whole problem only to not be able to output at the end.
     filePointer = fopen(argv[4], "w");
     if (filePointer == NULL) {
          fprintf(stderr, "Error opening the output file\n");
	      fprintf(stderr, "%s\n", argv[4]);
          return 1;
     }

     //Switch on the mode
     switch (argv[3][0]) {
	  case 'b':
	       evaluationResult = bfsEval(initNode, goalNode);
	       break;
	  case 'd':
	       evaluationResult = dfsEval(initNode, goalNode);
	       break;
	  case 'i':
           depthLimit = 0;
           resultStruct = iddfsEval(initNode, goalNode, depthLimit, resultStruct);
           while (strcmp(evaluationResult.c_str(), "") == 0) {
               depthLimit++;
               resultStruct = iddfsEval(initNode, goalNode, depthLimit, resultStruct);
               if (depthLimit > 20000) {
                   fprintf(stderr, "This is getting out of hand. Cancelling.\n");
                   return 1;
               }
               evaluationResult = resultStruct.value;
           }
	       break;
	  case 'a':
	       evaluationResult = astarEval(initNode, goalNode);
	       break;
	  default:
	       fprintf(stdout, "Please enter a valid mode. (bfs, dfs, iddfs, astar)\n");
     }

     //Print the solution to stdout and to a file.
     fprintf(stdout, "%s\n", evaluationResult.c_str());
     fprintf(filePointer, "%s\n", evaluationResult.c_str());

     fclose(filePointer);

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
     exists = true;

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

std::string Node::getPathString() {
     std::string returnString("");
     if (parent == NULL) {
        returnString += this->toString();
     } else {
        returnString += this->toString();
        returnString += "\n";
        returnString += this->parent->getPathString();
     }
     return returnString;
}

std::string bfsEval(Node initialNode, Node goalNode) {
     std::string returnString("");
     int nodesExpanded = 0, solutionLength;

     //Make the expanded nodes map
     std::map<std::string, Node*> expandedNodes;

     //Make the priority queue.
     std::priority_queue<Node, std::vector<Node>, std::function<bool(Node, Node)>> fringe(bfsCmp);

     //loop over the priority queue until we find a solution on the queue.
     fringe.push(initialNode);

     while (!isGoal(fringe.top(), goalNode)) {
          Node *tempNode = new Node(fringe.top());
          fringe.pop();

          if (expandedNodes.find(tempNode->toString()) == expandedNodes.end()) {
               nodesExpanded++;
	           struct Successor s = depthSuccessor(tempNode);
	           expandedNodes[tempNode->toString()] = tempNode;

	           if (s.c.exists) fringe.push(s.c);
	           if (s.cc.exists) fringe.push(s.cc);
	           if (s.w.exists) fringe.push(s.w);
	           if (s.wc.exists) fringe.push(s.wc);
	           if (s.ww.exists) fringe.push(s.ww);
	       }

           if (fringe.empty()) {
                returnString += "No solution found\nNodes expanded: ";
                returnString += std::to_string(nodesExpanded);
                return returnString;
           }
     }

     Node tempNode = fringe.top();

     solutionLength = tempNode.depth + 1;
     returnString += tempNode.getPathString();

     returnString += "\nnodes expanded: ";
     returnString += std::to_string(nodesExpanded);
     returnString += "\nnodes in solution: ";
     returnString += std::to_string(solutionLength);
     returnString += "\n";

     return returnString;
}

std::string dfsEval(Node initialNode, Node goalNode) {
     std::string returnString("");
     int nodesExpanded = 0, solutionLength;

     //Make the expanded nodes map
     std::map<std::string, Node*> expandedNodes;

     //Make the priority queue.
     std::priority_queue<Node, std::vector<Node>, std::function<bool(Node, Node)>> fringe(dfsCmp);

     //loop over the priority queue until we find a solution on the queue.
     fringe.push(initialNode);

     while (!isGoal(fringe.top(), goalNode)) {
          Node *tempNode = new Node(fringe.top());
          fringe.pop();

          if (expandedNodes.find(tempNode->toString()) == expandedNodes.end()) {
               nodesExpanded++;
	           struct Successor s = depthSuccessor(tempNode);
	           expandedNodes[tempNode->toString()] = tempNode;

	           if (s.c.exists) fringe.push(s.c);
	           if (s.cc.exists) fringe.push(s.cc);
	           if (s.w.exists) fringe.push(s.w);
	           if (s.wc.exists) fringe.push(s.wc);
	           if (s.ww.exists) fringe.push(s.ww);
	       }

           if (fringe.empty()) {
                returnString += "No solution found\nNodes expanded: ";
                returnString += std::to_string(nodesExpanded);
                return returnString;
           }
     }

     Node tempNode = fringe.top();

     solutionLength = tempNode.depth + 1;
     returnString += tempNode.getPathString();

     returnString += "\nnodes expanded: ";
     returnString += std::to_string(nodesExpanded);
     returnString += "\nnodes in solution: ";
     returnString += std::to_string(solutionLength);
     returnString += "\n";

     return returnString;
}

struct iterativeDeepening iddfsEval(Node initialNode, Node goalNode, int depthLimit, struct iterativeDeepening oldValues) {
     std::string returnString("");
     int nodesExpanded = 0, solutionLength;
     struct iterativeDeepening returnStruct;

     //Make the expanded nodes map
     std::map<std::string, Node*> expandedNodes;

     //Make the priority queue.
     std::priority_queue<Node, std::vector<Node>, std::function<bool(Node, Node)>> fringe(dfsCmp);

     //loop over the priority queue until we find a solution on the queue.
     fringe.push(initialNode);

     while (!isGoal(fringe.top(), goalNode)) {
          Node *tempNode = new Node(fringe.top());
          fringe.pop();

          if (expandedNodes.find(tempNode->toString()) == expandedNodes.end()) {
               nodesExpanded++;
	           struct Successor s = depthSuccessor(tempNode);
	           expandedNodes[tempNode->toString()] = tempNode;

	           if (s.c.exists && s.c.depth <= depthLimit) fringe.push(s.c);
	           if (s.cc.exists && s.cc.depth<= depthLimit) fringe.push(s.cc);
	           if (s.w.exists && s.w.depth <= depthLimit) fringe.push(s.w);
	           if (s.wc.exists && s.wc.depth <= depthLimit) fringe.push(s.wc);
	           if (s.ww.exists && s.ww.depth <= depthLimit) fringe.push(s.ww);
	       }

           if (fringe.empty()) {
                returnStruct.value = "";
                returnStruct.nodesExpanded = oldValues.nodesExpanded + nodesExpanded;
                returnStruct.finished = false;
                return returnStruct;
           }
     }

     Node tempNode = fringe.top();

     solutionLength = tempNode.depth + 1;
     returnString += tempNode.getPathString();

     returnString += "\nnodes expanded: ";
     returnString += std::to_string(nodesExpanded + oldValues.nodesExpanded);
     returnString += "\nnodes in solution: ";
     returnString += std::to_string(solutionLength);
     returnString += "\n";

     returnStruct.value = returnString;
     returnStruct.nodesExpanded = oldValues.nodesExpanded + nodesExpanded;
     returnStruct.finished = true;

     return returnStruct;
}

std::string astarEval(Node initialNode, Node goalNode) {
     std::string returnString("");
     int nodesExpanded = 0, solutionLength;

     //Make the expanded nodes map
     std::map<std::string, Node*> expandedNodes;

     //Make the priority queue.
     std::priority_queue<Node, std::vector<Node>, std::function<bool(Node, Node)>> fringe(dfsCmp);

     //loop over the priority queue until we find a solution on the queue.
     fringe.push(initialNode);

     while (!isGoal(fringe.top(), goalNode)) {
          Node *tempNode = new Node(fringe.top());
          fringe.pop();

          if (expandedNodes.find(tempNode->toString()) == expandedNodes.end()) {
               nodesExpanded++;
	           struct Successor s = astarSuccessor(tempNode, goalNode);
	           expandedNodes[tempNode->toString()] = tempNode;

	           if (s.c.exists) fringe.push(s.c);
	           if (s.cc.exists) fringe.push(s.cc);
	           if (s.w.exists) fringe.push(s.w);
	           if (s.wc.exists) fringe.push(s.wc);
	           if (s.ww.exists) fringe.push(s.ww);
	       }

           if (fringe.empty()) {
                returnString += "No solution found\nNodes expanded: ";
                returnString += std::to_string(nodesExpanded);
                return returnString;
           }
     }

     Node tempNode = fringe.top();

     solutionLength = tempNode.depth + 1;
     returnString += tempNode.getPathString();

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

bool isValid(Node parent, int lc, int lw, int lb, int rc, int rw, int rb){
        //Check for negative values
        if (lc < 0 || lw < 0|| lb < 0 || rc < 0 || rw < 0 || rb <0)
            return false;
        //Check for more wolves than chickens
        if (lc < lw && lc > 0)
            return false;
        if (rc < rw && rc > 0)
            return false;

    return true;
}

struct Successor depthSuccessor(Node* parent){
    struct Successor succ; //return successors
    int lc, lw, lb, rc, rw, rb; //Temp values

    //Move one chicken
    lc = parent->lChickens - (parent->lBoats) + (parent->rBoats);
    lw = parent->lWolves;
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens + (parent->lBoats) - (parent->rBoats);
    rw = parent->rWolves;
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.c = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
    }
    else{
        succ.c = Node();
    }

    //Move two chickens
    lc = parent->lChickens - 2*(parent->lBoats) + 2*(parent->rBoats);
    lw = parent->lWolves;
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens + 2*(parent->lBoats) - 2*(parent->rBoats);
    rw = parent->rWolves;
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.cc = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
    }
    else{
        succ.cc = Node();
    }

    //Move  one wolf
    lc = parent->lChickens;
    lw = parent->lWolves - (parent->lBoats) + (parent->rBoats);
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens;
    rw = parent->rWolves + (parent->lBoats) - (parent->rBoats);
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.w = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
    }
    else{
        succ.w = Node();
    }

    //Move one chicken, one wolf
    lc = parent->lChickens - (parent->lBoats) + (parent->rBoats);
    lw = parent->lWolves - (parent->lBoats) + (parent->rBoats);
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens + (parent->lBoats) - (parent->rBoats);
    rw = parent->rWolves + (parent->lBoats) - (parent->rBoats);
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.wc =  Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
    }
    else{
        succ.wc =  Node();
    }

    //Move  two wolf
    lc = parent->lChickens;
    lw = parent->lWolves - 2*(parent->lBoats) + 2*(parent->rBoats);
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens;
    rw = parent->rWolves + 2*(parent->lBoats) - 2*(parent->rBoats);
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.ww = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
    }
    else{
        succ.ww = Node();
    }

    return succ;
}

struct Successor astarSuccessor(Node* parent, Node goalNode){
    struct Successor succ; //return successors
    int lc, lw, lb, rc, rw, rb; //Temp values

    //Move one chicken
    lc = parent->lChickens - (parent->lBoats) + (parent->rBoats);
    lw = parent->lWolves;
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens + (parent->lBoats) - (parent->rBoats);
    rw = parent->rWolves;
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.c = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
        succ.c.priority += astarHeuristic(succ.c, goalNode);
    }
    else{
        succ.c = Node();
    }

    //Move two chickens
    lc = parent->lChickens - 2*(parent->lBoats) + 2*(parent->rBoats);
    lw = parent->lWolves;
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens + 2*(parent->lBoats) - 2*(parent->rBoats);
    rw = parent->rWolves;
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.cc = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
        succ.cc.priority += astarHeuristic(succ.cc, goalNode);
    }
    else{
        succ.cc = Node();
    }

    //Move  one wolf
    lc = parent->lChickens;
    lw = parent->lWolves - (parent->lBoats) + (parent->rBoats);
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens;
    rw = parent->rWolves + (parent->lBoats) - (parent->rBoats);
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.w = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
        succ.w.priority += astarHeuristic(succ.w, goalNode);
    }
    else{
        succ.w = Node();
    }

    //Move one chicken, one wolf
    lc = parent->lChickens - (parent->lBoats) + (parent->rBoats);
    lw = parent->lWolves - (parent->lBoats) + (parent->rBoats);
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens + (parent->lBoats) - (parent->rBoats);
    rw = parent->rWolves + (parent->lBoats) - (parent->rBoats);
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.wc =  Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
        succ.wc.priority += astarHeuristic(succ.wc, goalNode);
    }
    else{
        succ.wc =  Node();
    }

    //Move  two wolf
    lc = parent->lChickens;
    lw = parent->lWolves - 2*(parent->lBoats) + 2*(parent->rBoats);
    lb = parent->lBoats - (parent->lBoats) + (parent->rBoats);
    rc = parent->rChickens;
    rw = parent->rWolves + 2*(parent->lBoats) - 2*(parent->rBoats);
    rb = parent->rBoats + (parent->lBoats) - (parent->rBoats);
    //If math adds up, create a node
    if (isValid(*parent, lc, lw, lb, rc, rw, rb) == true){
        succ.ww = Node(lc, lw, lb, rc, rw, rb, parent, parent->depth +1);
        succ.ww.priority += astarHeuristic(succ.ww, goalNode);
    }
    else{
        succ.ww = Node();
    }

    return succ;
}

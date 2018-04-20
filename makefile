default:
	g++ WandC.c -o WandC -std=c++11

runbfs:
	./WandC initialState.txt goalState.txt bfs output.txt

rundfs:
	./WandC initialState.txt goalState.txt dfs output.txt

runiddfs:
	./WandC initialState.txt goalState.txt iddfs output.txt

runastar:
	./WandC initialState.txt goalState.txt astar output.txt

clean:
	rm -f WandC

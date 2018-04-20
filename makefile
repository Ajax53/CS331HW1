default:
	g++ WandC.c -o WandC -std=c++11

runbfs:
	./WandC initialState.txt goalState.txt bfs output.txt

clean:
	rm -f WandC

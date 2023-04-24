#include <iostream>
#include <vector>
#include <windows.h>

enum Status {UNVISITED, VISITED};

class gridCell {
public:
	char cellType;
	enum Status status;
	int x;
	int y;
	int distance;
	gridCell* prev = nullptr;

	void print() {
		std::cout << x << ' ' << y;
	}
};

class grid {
public:
	int gridLength;
	int gridHeight;
	std::vector< std::vector<gridCell> > matrix;

public:
	grid(int l, int h) {
		gridLength = l;
		gridHeight = h;

		matrix.resize(gridLength);
		for (int i = 0; i < gridLength; i++)
			matrix[i].resize(gridHeight);

		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridHeight; j++) {
				matrix[i][j].x = i;
				matrix[i][j].y = j;
			}
		}
	}

	void print() {
		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridHeight; j++) {
				std::cout << matrix[j][i].cellType << ' ';
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	void initializeDistance() {
		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridHeight; j++) {
				matrix[i][j].distance = INT_MAX;
			}
		}
	}

	void setEmpty() {
		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridHeight; j++) {
				matrix[i][j].cellType = ' ';
				matrix[i][j].status = UNVISITED;
			}
		}
	}

	void setBorders() {
		for (int i = 0; i < gridLength; i++) {
			matrix[0][i].cellType = '#';
			matrix[gridHeight - 1][i].cellType = '#';
			matrix[i][0].cellType = '#';
			matrix[i][gridHeight - 1].cellType = '#';

			matrix[0][i].status = VISITED;
			matrix[gridHeight - 1][i].status = VISITED;
			matrix[i][0].status = VISITED;
			matrix[i][gridHeight - 1].status = VISITED;

			
		}
	}

	bool unvisitedsExist() {

		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridHeight; j++) {
				if (matrix[i][j].status == UNVISITED) {
					return true;
				}
			}
		}
		return false;

	}

	void printcoords() {
		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridHeight; j++) {
				matrix[i][j].print();
				std::cout << "  ";
			}
			std::cout << "\n";
		}
	}

	void makeObstacle(int x1, int y1, int x2, int y2) {

		for (int i = 0; i < x2 - x1; i++) {
			matrix[x1 + i][y1].cellType = '#';
			matrix[x1 + i][y2 - 1].cellType = '#';

			matrix[x1 + i][y1].status = VISITED;
			matrix[x1 + i][y2 - 1].status = VISITED;
		}

		for (int i = 0; i < y2 - y1; i++) {
			matrix[x1][y1 + i].cellType = '#';
			matrix[x2 - 1][y1 + i].cellType = '#';

			matrix[x1][y1 + i].status = VISITED;
			matrix[x2 - 1][y1 + i].status = VISITED;
		}


	}

	void dijkstra(gridCell start, gridCell end) {
		
		gridCell* startingCell = &matrix[start.x][start.y];
		gridCell* endingCell = &matrix[end.x][end.y];

		*startingCell = start;
		*endingCell = end;

		gridCell* current = startingCell;

		// while there are still unvisited nodes AND end has not been reached
		while (unvisitedsExist() && current != endingCell) {

			// find the unvisited node with smallest distance from the source. this is the new current
			
			// find first unvisited cell
			bool found = false;
			for (int i = 0; i < gridLength; i++) {
				for (int j = 0; j < gridHeight; j++) {
					if (matrix[i][j].status == UNVISITED) {
						current = &matrix[i][j];
						found = true;
						break;
					}
				}
				if (found) break;
			}

			// after that, go through and find unvisited cell with least distance from the source
			for (int i = 0; i < gridLength; i++) {
				for (int j = 0; j < gridHeight; j++) {
					if (matrix[i][j].distance < current->distance && matrix[i][j].status == UNVISITED)
						current = &matrix[i][j];
				}
			}

			//std::cout << current->x << ' ' << current->y << '\n';

			// update current distance from source for its unvisited adjacent nodes

			// check left
			if (matrix[current->x - 1][current->y].status == UNVISITED && current->distance + 1 < matrix[current->x - 1][current->y].distance) {
				matrix[current->x - 1][current->y].distance = current->distance + 1;
				matrix[current->x - 1][current->y].prev = current;
			}

			// check right
			if (matrix[current->x + 1][current->y].status == UNVISITED && current->distance + 1 < matrix[current->x + 1][current->y].distance) {
				matrix[current->x + 1][current->y].distance = current->distance + 1;
				matrix[current->x + 1][current->y].prev = current;
			}

			// check up
			if (matrix[current->x][current->y - 1].status == UNVISITED && current->distance + 1 < matrix[current->x][current->y - 1].distance) {
				matrix[current->x][current->y - 1].distance = current->distance + 1;
				matrix[current->x][current->y - 1].prev = current;
			}

			// check down
			if (matrix[current->x][current->y + 1].status == UNVISITED && current->distance + 1 < matrix[current->x][current->y + 1].distance) {
				matrix[current->x][current->y + 1].distance = current->distance + 1;
				matrix[current->x][current->y + 1].prev = current;
			}

			// mark current node as visited
			current->status = VISITED;

			
			// change cellType to reflect that it is visited
		
			current->cellType = '.';
			startingCell->cellType = 'O';
			
			
			// DELETE CURRENT OUTPUT
			system("cls");
			
			
			// RE RENDER
			print();

			// sleep
			Sleep(200);
		}

		//print();

		gridCell* backtrack = current;
		while (backtrack->prev != nullptr) {
			//std::cout << backtrack->x << ' ' << backtrack->y << '\n';
			backtrack->cellType = '*';
			backtrack = backtrack->prev;
		}
		endingCell->cellType = 'X';


		for (int i = 0; i < gridLength; i++) {
			for (int j = 0; j < gridHeight; j++) {
				if (matrix[i][j].status == UNVISITED) {
					//matrix[i][j].cellType = '`';
				}
			}
		}

		print();

	}

};

int main() {

	gridCell start;
	start.cellType = 'O';
	start.x = 10;
	start.y = 3;
	start.status = UNVISITED;
	start.distance = 0;

	gridCell end;
	end.cellType = 'X';
	end.x = 10;
	end.y = 17;
	end.status = UNVISITED;
	end.distance = INT_MAX;

	grid g(20, 20);
	g.setEmpty();
	g.setBorders();
	g.makeObstacle(5,5,15,15);
	g.initializeDistance();


	g.dijkstra(start, end);

	return 0;
}
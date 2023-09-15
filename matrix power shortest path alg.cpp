#include <iostream>
#include <vector>

class matrix {
private:
	int** adjmat;
	int n;

public:
	matrix() {
		int n = 0;
		int** adjmat = nullptr;
	}

	matrix(int n) {
		this->n = n;
		adjmat = new int* [n];

		for (int i = 0; i < n; ++i) {
			adjmat[i] = new int[n];
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				adjmat[i][j] = 0;
			}
		}
	}

	matrix(const matrix& other) {
		n = other.n;
		adjmat = new int* [n];
		for (int i = 0; i < n; ++i) {
			adjmat[i] = new int[n];
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				adjmat[i][j] = other.adjmat[i][j];
			}
		}
	}

	~matrix() {
		for (int i = 0; i < n; ++i) {
			delete[] adjmat[i];
		}
		delete[] adjmat;
	}

	// fix memory leak here?
	matrix& operator=(const matrix& other) {
		for (int i = 0; i < n; ++i) {
			delete[] adjmat[i];
		}
		delete[] adjmat;

		n = other.n;
		adjmat = new int* [n];
		for (int i = 0; i < n; ++i) {
			adjmat[i] = new int[n];
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				adjmat[i][j] = other.adjmat[i][j];
			}
		}
		return *this;
	}

	void operator*=(const matrix& othermatrix) {
		matrix temp(n);
		temp = *this;

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				adjmat[i][j] = 0;
				for (int k = 0; k < n; k++)
				{
					adjmat[i][j] += temp.adjmat[i][k] * othermatrix.adjmat[k][j];
				}
			}
		}
	}

	void print() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cout << adjmat[i][j] << ' ';
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}

	void setexamplematrix() {
		int row[5][5] = {
			{ 0, 1, 1, 0, 0 },
			{ 1, 0, 1, 1, 0 },
			{ 1, 1, 0, 1, 0 },
			{ 0, 1, 1, 0, 1 },
			{ 0, 0, 0, 1, 0 }
		};

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				adjmat[i][j] = row[i][j];
		}
	}

	std::vector<matrix> powers(int start, int end) {
		
		std::vector<matrix> resultvec;
		//dummy in power[0]
		matrix dummy(n);
		resultvec.push_back(dummy);

		resultvec.push_back(*this);

		while (!adjmat[start][end]) {
			*this *= resultvec[1];
			resultvec.push_back(*this);
		}

		for (int i = 0; i < resultvec.size(); i++) 
			resultvec[i].print();

		return resultvec;
	}

	friend void findshortestpath(std::vector<matrix> powers, int start, int end);
};

void findshortestpath(std::vector<matrix> powers, int start, int end) {

	int head = end;
	int shortest_path_length = powers.size() - 1; // bcos dummy
	int current_power = shortest_path_length - 1;
	
	// check nodes with a path to start to see if it is adjacent to end
	while (current_power > 0) {

		for (int i = 0; i < powers[0].n; i++) {
			if (powers[current_power].adjmat[start][i] && powers[1].adjmat[head][i]) {
				std::cout << "push (" << i << "," << head << ")\n";
				head = i;
				current_power--;
				break;
			}
		}
	}

	std::cout << "push (" << start << "," << head << ")\n";

}

	
int main() {

	matrix mat(5);
	mat.setexamplematrix();
	
	std::vector<matrix> powers = mat.powers(4, 1);

	findshortestpath(powers, 4, 1);

	return 0;
}
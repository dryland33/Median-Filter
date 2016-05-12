#include <cstdlib>
#include <iostream> 
#include <iomanip>  
#include <fstream> 
#include <sstream> 
#include "BinarySearchTree.h"
using namespace std;

int findMedian(BinarySearchTree&, int size);

int main(int argc, char *argv[]) {
	int row(0), column(0), numb_rows(0), numb_cols(0), side_len(atoi(argv[1]));
	ifstream infile(argv[2]);
	ofstream outfile(argv[3]);
	stringstream cuyahoga;
	string in_line("");
	BinarySearchTree bonsai;
	bonsai.setTotal(0);
	bonsai.setSize(0);

	//read and write "P2"
	getline(infile, in_line);
	outfile << in_line << endl;

	//read and write filename comment
	getline(infile, in_line);
	outfile << in_line << endl;

	//read, store, and write dimensions
	getline(infile, in_line);
	istringstream buffer(in_line);
	buffer >> numb_cols >> numb_rows;
	outfile << numb_cols << ' ' << numb_rows << endl;

	//read and write max grey value
	getline(infile, in_line);
	outfile << in_line << endl;

	//put everything else in this string stream
	cuyahoga << infile.rdbuf();

	//declare a 2D array to store the source values 
	int **array = new int*[numb_rows];
	for (int i = 0; i < numb_rows; ++i)
		array[i] = new int[numb_cols];

	//declare a 2D array to store the output values 
	int **new_array = new int*[numb_rows];
	for (int i = 0; i < numb_rows; ++i)
		new_array[i] = new int[numb_cols];

	//move the data from the stream to the source array
	for (row = 0; row < numb_rows; ++row)
		for (column = 0; column < numb_cols; ++column)
			cuyahoga >> array[row][column];


	const int radius = side_len / 2;
	//populate the new_array with median values
	for (row = 0; row < numb_rows; ++row){
		for (column = 0; column < numb_cols; ++column){
			//add pixels to the tree
			for (int i = (row - radius); i <= (row + radius); ++i){
				for (int j = (column - radius); j <= (column + radius); ++j){
					if (i >= 0 && j >= 0 && i < numb_rows && j < numb_cols)
						bonsai.insert(new Node(array[i][j]));
				}
			}
			//bonsai.display(bonsai.root, 5);
			new_array[row][column] = findMedian(bonsai, bonsai.getSize());
			bonsai.empty();
		}
	}

	//write everything to the new file
	for (row = 0; row < numb_rows; ++row) {
		for (column = 0; column < numb_cols; ++column) {
			outfile << setw(3) << new_array[row][column] << " ";
		}
		outfile << endl;
	}

	//call housekeeping
	infile.close();
	outfile.close();

	for (int i = 0; i < numb_rows; ++i) {
		delete[] array[i];
	}
	delete[] array;

	for (int i = 0; i < numb_rows; ++i) {
		delete[] new_array[i];
	}
	delete[] new_array;
}

/*for an odd number of nodes getMedian() calls delMax() and delMin() until one value is left and returns that value
for an even number of nodes it calls delMax() and delMin() until two values remain and returns their average*/
int findMedian(BinarySearchTree& bonsai, int size){

	if (size % 2 == 0){                    //even number of nodes
		while (bonsai.getSize() > 2){
			bonsai.delMax(bonsai.root);    //delete max and min until two nodes remain   
			bonsai.delMin(bonsai.root);
		}
		return bonsai.getTotal() / 2;
	}
	else
		while (bonsai.getSize() > 1){           //odd number of nodes

		bonsai.delMax(bonsai.root);
		bonsai.delMin(bonsai.root);        //delete max and min until one node remains
		}
	return bonsai.getTotal();
}


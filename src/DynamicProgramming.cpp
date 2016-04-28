#include <iostream>
#include <vector>

using namespace std;

void alignSequences(string seq1, string seq2, vector< vector<int> > &matrix)
{
	int seq1Length = seq1.length();
	int seq2Length = seq2.length();

	string seq1New = "";
	string seq2New = "";

	int match = 0;

	int currentCol = seq1Length;
	int currentRow = seq2Length;
	int currentCell = 0;

	int left, top, diagonal = 0;
	int num = 0;

	string seq1Rev = "";
	string seq2Rev = "";

	while(true)
	{

		if (currentCol == 0 and currentRow == 0)
		{
			break; //breaks when traceback gets to top left cell
		}

		left = matrix[currentRow][currentCol - 1];
		top = matrix[currentRow - 1][currentCol];

		diagonal = matrix[currentRow - 1][currentCol - 1];

		currentCell = matrix[currentRow][currentCol];

		num = min(min(left, top), diagonal);

		if (seq1[currentCol - 1] == seq2[currentRow - 1])
		{
			match = 1;
		}

		if ((num == left and num != diagonal and match != 1) or (currentRow == 0))
		{
			// left

			seq2New += '-'; //add a gap
			seq1New += seq1[currentCol - 1]; //take character
			currentCol -= 1;
		}

		else if ((num == top and num != diagonal and match != 1) or (currentCol == 0))
		{
			// top
			
			seq1New += '-'; //add a gap
			seq2New += seq2[currentRow - 1]; //take character
			currentRow -= 1;
		}
		else
		{
			// diagonal

			seq1New += seq1[currentCol - 1]; //take character
			seq2New += seq2[currentRow - 1]; //take character

			currentCol -= 1;
			currentRow -= 1;
		}
		match = 0;
	}

	for (int i = seq1Length; i > 0; i--)
	{
		seq1Rev += seq1New[i];
	}
	for (int i = seq2Length; i > 0; i--)
	{
		seq2Rev += seq2New[i];
	}

	cout << seq1Rev << endl;
	cout << seq2Rev << endl;

}

void buildMatrix(string seq1, string seq2)
{

	int numCols = seq1.length() + 1;
	int numRows = seq2.length() + 1;
	int substitution = 0;

	int left, top, diagonal, num = 0;

	string s1New = "";
	string s2New = "";

	vector < vector<int> > matrix;
	matrix.resize(numRows, vector<int>(numCols, 0));

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			if (row == 0 and col == 0)
			{
				matrix[row][col] = 0;
			}
			else if (row == 0) {
				matrix[row][col] = matrix[row][col - 1] + 1;
			}
			else if (col == 0) {
				matrix[row][col] = matrix[row - 1][col] + 1;
			}
			else {
				if (seq1[col - 1] == seq2[row - 1])
				{
					substitution = 0; //match score = 0
				}
				else
				{
					substitution = 1; //mismatch score = 1
				}

				left = matrix[row][col - 1] + 1; //gap penalty: +1
				top = matrix[row - 1][col] + 1; //gap penalty: +1
				diagonal = matrix[row - 1][col - 1] + substitution; //match/mismatch

				num = min(min(left, top), diagonal);
				matrix[row][col] = num;
			}
		}
	}

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}

	alignSequences(seq1, seq2, matrix);
}

int main(int argc, char const *argv[])
{
	string seq1 = "ACGTA";
	string seq2 = "AGTA";
	buildMatrix(seq1, seq2);
	return 0;
}



/***************************************
File: matrix.cpp
Author: Grisha Goldberg
Description: Managing a Matrix
***************************************/

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Matrix {
public:
    // Constructor. Starts at size 0.
    Matrix(int rows, int cols)
        : m_rows(rows), m_cols(cols), m_data(rows, vector<double>(cols, 0.0)) {}

    int getRows() const { 
        return m_rows; 
    }
    int getCols() const { 
        return m_cols; 
    }

    // Interactive element modification
    void setElementInteractive() {
        bool done = false;
        while (!done) {
            cout << "Enter row index (0-" << m_rows - 1 << ") or -1 to finish: ";
            int i;
            cin >> i;
            if (i == -1) {
                // This would be to exit.
                done = true;
            } else {
                cout << "Enter column index (0-" << m_cols - 1 << ") or -1 to finish: ";
                int j;
                cin >> j;
                if (j == -1) {
                    // Exit
                    done = true;
                } else {
                    // Some validation.
                    if (i < 0 || i >= m_rows || j < 0 || j >= m_cols) {
                        cout << "Index out of range. Enter value: \n";
                    } else {
                        // What is the element?
                        cout << "Enter value: ";
                        double value;
                        cin >> value;
                        m_data[i][j] = value;
                    }
                }
            }
        }
    }

    // Direct element modification by index
    bool setElement(int row, int col, double value) {
        if (row < 0 || row >= m_rows || col < 0 || col >= m_cols) {
            cout << "Index out of range.\n";
            return false;
        }
        m_data[row][col] = value;
        return true;
    }

    const vector<vector<double>>& data() const { 
        return m_data; 
    }
    vector<vector<double>>& data() { 
        return m_data; 
    }

    // Print matrix
    void print() const {
        // To beauty up the matrix.
        for (int i = 0; i < m_rows; ++i) {
            cout << "[";
            for (int j = 0; j < m_cols; ++j) {
                cout << m_data[i][j];
                if (j == m_cols - 1) {
                    cout << "]" << endl;
                }
                else {
                    cout << " ";
                }
            }
            
        }
    }
    private:
    int m_rows, m_cols;
    vector<vector<double>> m_data;
};

// Add two matrices
Matrix add(const Matrix& A, const Matrix& B) {
    if (A.getRows() != B.getRows() || A.getCols() != B.getCols()) {
        exit(1);
    }
    Matrix result(A.getRows(), A.getCols());
    for (int i = 0; i < A.getRows(); ++i) {
        for (int j = 0; j < A.getCols(); ++j) {
            result.data()[i][j] = A.data()[i][j] + B.data()[i][j];
        }
    }
    return result;
}

// Multiply matrix by scalar
Matrix scalarMultiply(const Matrix& A, double scalar) {
    Matrix result(A.getRows(), A.getCols());
    for (int i = 0; i < A.getRows(); ++i) {
        for (int j = 0; j < A.getCols(); ++j) {
            result.data()[i][j] = A.data()[i][j] * scalar;
        }
    }
    return result;
}

// Multiply two matrices
Matrix multiply(const Matrix& A, const Matrix& B) {
    if (A.getCols() != B.getRows()) {
        // Inside dimensions need to match.
        exit(1);
    }
    Matrix result(A.getRows(), B.getCols());
    for (int i = 0; i < A.getRows(); ++i) {
        for (int j = 0; j < B.getCols(); ++j) {
            double sum = 0.0;
            for (int k = 0; k < A.getCols(); ++k) {
                sum += A.data()[i][k] * B.data()[k][j];
            }
            result.data()[i][j] = sum;
        }
    }
    return result;
}

// Transpose a matrix
Matrix transpose(const Matrix& A) {
    Matrix result(A.getCols(), A.getRows());
    for (int i = 0; i < A.getRows(); ++i) {
        for (int j = 0; j < A.getCols(); ++j) {
            // Does the swapping.
            result.data()[j][i] = A.data()[i][j];
        }
    }
    return result;
}

int main() {
    // I hard coded in the specific example that was asked to be solved.
    // However, because Professor Kidd said it should be versatile for
    // any matrix, I made another interactive function to set elements of
    // a matrix.
    Matrix A(2, 2), B(2, 3), C(2, 3);

    /* A
    // [6  4]
    // [8  3]
    */
    A.setElement(0, 0, 6);
    A.setElement(0, 1, 4);
    A.setElement(1, 0, 8);
    A.setElement(1, 1, 3);

    /* B
    // [1  2  3]
    // [4  5  6]
    */
    B.setElement(0, 0, 1);
    B.setElement(0, 1, 2);
    B.setElement(0, 2, 3);
    B.setElement(1, 0, 4);
    B.setElement(1, 1, 5);
    B.setElement(1, 2, 6);
    /* C
    // [2  4  6]
    // [1  3  5]
    */
    C.setElement(0, 0, 2);
    C.setElement(0, 1, 4);
    C.setElement(0, 2, 6);
    C.setElement(1, 0, 1);
    C.setElement(1, 1, 3);
    C.setElement(1, 2, 5);

    // Compute D = A + (3 * B) × C^T
    Matrix threeB   = scalarMultiply(B, 3.0);
    Matrix C_t      = transpose(C);
    Matrix product  = multiply(threeB, C_t);
    Matrix D        = add(A, product);

    cout << "Result: " << endl;
    D.print();

    return 0;
}

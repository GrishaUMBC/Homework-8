/***************************************
File: matrix.cpp
Author: Grisha Goldberg
Description: Managing a Matrix
***************************************/

#include <stdio.h>
#include <stdlib.h>

// Matrix data
typedef struct {
    int rows;
    int cols;
    double *data;
} Matrix;

// Allocate memory for matrix
Matrix *createMatrix(int rows, int cols) {
    Matrix *m = malloc(sizeof *m);
    if (!m) {
        exit(1);
    }
    m->rows = rows;
    m->cols = cols;

    unsigned long n = (unsigned long)rows * cols;
    m->data = malloc(n * sizeof *m->data);
    if (!m->data) {
        exit(1);
    }
    for (unsigned long i = 0; i < n; ++i) {
        m->data[i] = 0.0;
    }
    return m;
}

// Free a matrix
void freeMatrix(Matrix *m) {
    if (!m) return;
    free(m->data);
    free(m);
}

// Return the index
int getIndex(const Matrix *m, int r, int c) {
    return r * m->cols + c;
}

// Direct element modification
int setElement(Matrix *m, int row, int col, double value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        printf("Index out of range.\n");
        return 0;
    }
    m->data[getIndex(m, row, col)] = value;
    return 1;
}

// Print matrix with %g so you only get as many decimals as needed 
void printMatrix(const Matrix *m) {
    for (int i = 0; i < m->rows; ++i) {
        printf("[");
        for (int j = 0; j < m->cols; ++j) {
            // I think this is the right way to format the print statement.
            printf("%g", m->data[getIndex(m, i, j)]);
            if (j < m->cols - 1) printf(" ");
        }
        printf("]\n");
    }
}

// Add two matrices
Matrix *addMatrix(const Matrix *A, const Matrix *B) {
    if (A->rows != B->rows || A->cols != B->cols) {
        printf("Dimension mismatch in add\n");
        exit(1);
    }
    Matrix *R = createMatrix(A->rows, A->cols);
    int n = A->rows * A->cols;
    for (int i = 0; i < n; ++i) {
        R->data[i] = A->data[i] + B->data[i];
    }
    return R;
}

// Multiply matrix by scalar
Matrix *scalarMultiply(const Matrix *A, double s) {
    Matrix *R = createMatrix(A->rows, A->cols);
    int n = A->rows * A->cols;
    for (int i = 0; i < n; ++i) {
        R->data[i] = A->data[i] * s;
    }
    return R;
}

// Multiply two matrices
Matrix *multiplyMatrix(const Matrix *A, const Matrix *B) {
    if (A->cols != B->rows) {
        printf("Dimension mismatch in multiply\n");
        exit(1);
    }
    Matrix *R = createMatrix(A->rows, B->cols);
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < B->cols; ++j) {
            double sum = 0.0;
            for (int k = 0; k < A->cols; ++k) {
                sum += A->data[getIndex(A, i, k)] * B->data[getIndex(B, k, j)];
            }
            R->data[getIndex(R, i, j)] = sum;
        }
    }
    return R;
}

// Transpose a matrix
Matrix *transposeMatrix(const Matrix *A) {
    Matrix *R = createMatrix(A->cols, A->rows);
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < A->cols; ++j) {
            R->data[getIndex(R, j, i)] = A->data[getIndex(A, i, j)];
        }
    }
    return R;
}

// Here are all of my tests.

int testCreateAndSet(void) {
    Matrix *M = createMatrix(2, 2);
    if (!M || M->rows != 2 || M->cols != 2) return 0;
    
    // setElement valid
    if (!setElement(M, 1, 1, 4.2) ||
        M->data[getIndex(M, 1, 1)] != 4.2) {
        freeMatrix(M);
        return 0;
    }
    // setElement out of range. 
    // Should print "Index out of range."
    if (setElement(M, 2, 2, 1.0)) {
        freeMatrix(M);
        return 0;
        
    }
    freeMatrix(M);
    // This is to prevent confusion for the grader.
    printf("Out of Range expected\n");
    return 1;
}

int testAdd(void) {
    Matrix *A = createMatrix(2, 2);
    Matrix *B = createMatrix(2, 2);
    // A = [1 2]  B = [5 6]
    //     [3 4]      [7 8]    
    for (int i = 0; i < 4; ++i) {
        A->data[i] = i + 1;
        B->data[i] = i + 5;
    }
    Matrix *S = addMatrix(A, B);
    //  S should equal [6  8]
    //                 [10 12]
    double expect[4] = {6, 8, 10, 12};
    for (int i = 0; i < 4; ++i) {
        if (S->data[i] != expect[i]) {
            freeMatrix(A);
            freeMatrix(B);
            freeMatrix(S);
            return 0;
        }
    }
    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(S);
    return 1;
}

int testScalarAndMultiply(void) {
    Matrix *M = createMatrix(2, 2);
    // M = [2 0]
    //     [0 2]
    setElement(M, 0, 0, 2.0);
    setElement(M, 1, 1, 2.0);
    // will get [6 0]
    //          [0 6] when multiplying by three
    Matrix *S = scalarMultiply(M, 3.0);
    if (S->data[getIndex(S, 0, 0)] != 6.0 ||
        S->data[getIndex(S, 1, 1)] != 6.0) {
        freeMatrix(M);
        freeMatrix(S);
        return 0;
    }
    // multiplyMatrix M×M: [2 0] * itself = [4 0]
    //                     [0 2]            [0 4]
    Matrix *P = multiplyMatrix(M, M);
    if (P->data[getIndex(P, 0, 0)] != 4.0 ||
        P->data[getIndex(P, 1, 1)] != 4.0) {
        freeMatrix(M);
        freeMatrix(S);
        freeMatrix(P);
        return 0;
    }
    freeMatrix(M);
    freeMatrix(S);
    freeMatrix(P);
    return 1;
}

int testTranspose(void) {
    Matrix *A = createMatrix(2, 3);
    // A = [1 2 3]
    //     [4 5 6] 
    for (int i = 0; i < 6; ++i) {
        A->data[i] = i + 1;
    }
    Matrix *T = transposeMatrix(A);
    // Expect T to equal [1 4]
    //                   [2 5]
    //                   [3 6]
    double expect[6] = {1, 4, 2, 5, 3, 6};
    for (int i = 0; i < 6; ++i) {
        if (T->data[i] != expect[i]) {
            freeMatrix(A);
            freeMatrix(T);
            return 0;
        }
    }
    freeMatrix(A);
    freeMatrix(T);
    return 1;
}

int main(void) {
    Matrix *A = createMatrix(2, 2);
    Matrix *B = createMatrix(2, 3);
    Matrix *C = createMatrix(2, 3);

    // A = [6 4]
    //     [8 3]

    setElement(A, 0, 0, 6.0);
    setElement(A, 0, 1, 4.0);
    setElement(A, 1, 0, 8.0);
    setElement(A, 1, 1, 3.0);

    // B = [1 2 3]
    //     [4 5 6]

    setElement(B, 0, 0, 1.0);
    setElement(B, 0, 1, 2.0);
    setElement(B, 0, 2, 3.0);
    setElement(B, 1, 0, 4.0);
    setElement(B, 1, 1, 5.0);
    setElement(B, 1, 2, 6.0);

    // C = [2 4 6]
    //     [1 3 5]

    setElement(C, 0, 0, 2.0);
    setElement(C, 0, 1, 4.0);
    setElement(C, 0, 2, 6.0);
    setElement(C, 1, 0, 1.0);
    setElement(C, 1, 1, 3.0);
    setElement(C, 1, 2, 5.0);

    // D = A + (3*B) × C^T 
    Matrix *threeB  = scalarMultiply(B, 3.0);
    Matrix *Ct      = transposeMatrix(C);
    Matrix *product = multiplyMatrix(threeB, Ct);
    Matrix *D       = addMatrix(A, product);

    printf("Result:\n");
    printMatrix(D);

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);
    freeMatrix(threeB);
    freeMatrix(Ct);
    freeMatrix(product);
    freeMatrix(D);

     /*

    // For testing, you can uncomment this code
    //********************************************************************************
        
    if (!testCreateAndSet()) {
        printf("testCreateAndSet Failed\n");
        return 1;
    }
    if (!testAdd()) {
        printf("testAdd Failed\n");
        return 1;
    }
    if (!testScalarAndMultiply()) {
        printf("testScalarAndMul Failed\n");
        return 1;
    }
    if (!testTranspose()) {
        printf("testTranspose Failed\n");
        return 1;
    }

    printf("ALL TESTS PASSED\n");
    return 0;

    */
    
    return 0;   
}

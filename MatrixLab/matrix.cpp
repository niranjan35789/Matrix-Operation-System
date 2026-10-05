#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

class Matrix {
public:
    int rows, cols;
    vector<vector<double> > a;

    Matrix(int r = 2, int c = 2) : rows(r), cols(c), a(r, vector<double>(c, 0)) {}

    void input() {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                cin >> a[i][j];
    }

    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++)
                cout << a[i][j] << " ";
            cout << '\n';
        }
    }
};

Matrix add(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.cols; j++)
            C.a[i][j] = A.a[i][j] + B.a[i][j];
    return C;
}

Matrix subtractMatrix(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.cols; j++)
            C.a[i][j] = A.a[i][j] - B.a[i][j];
    return C;
}

Matrix multiply(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows, B.cols);
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < B.cols; j++)
            for (int k = 0; k < A.cols; k++)
                C.a[i][j] += A.a[i][k] * B.a[k][j];
    return C;
}

Matrix scalarMultiply(const Matrix& A, double k) {
    Matrix C(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.cols; j++)
            C.a[i][j] = A.a[i][j] * k;
    return C;
}

Matrix transpose(const Matrix& A) {
    Matrix C(A.cols, A.rows);
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.cols; j++)
            C.a[j][i] = A.a[i][j];
    return C;
}

double trace(const Matrix& A) {
    double t = 0;
    int n = min(A.rows, A.cols);
    for (int i = 0; i < n; i++) t += A.a[i][i];
    return t;
}

double determinant(const Matrix& A) {
    if (A.rows != A.cols) return 0;

    if (A.rows == 1) return A.a[0][0];

    if (A.rows == 2)
        return A.a[0][0] * A.a[1][1] - A.a[0][1] * A.a[1][0];

    if (A.rows == 3) {
        return
            A.a[0][0] * (A.a[1][1]*A.a[2][2] - A.a[1][2]*A.a[2][1])
          - A.a[0][1] * (A.a[1][0]*A.a[2][2] - A.a[1][2]*A.a[2][0])
          + A.a[0][2] * (A.a[1][0]*A.a[2][1] - A.a[1][1]*A.a[2][0]);
    }

    return 0; // This beginner version supports determinants up to 3x3.
}

Matrix power(Matrix A, int n) {
    Matrix result(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++) result.a[i][i] = 1;

    for (int p = 0; p < n; p++)
        result = multiply(result, A);

    return result;
}

int main() {
    cout << "MatrixLab C++ Engine\n";
    cout << "This file contains the basic matrix calculations used by the project.\n";
    return 0;
}

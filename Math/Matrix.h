#include<bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;

struct Matrix {
    vector<vector<int>> v;
    int rows, cols;
    Matrix() {}
    Matrix(int r, int c, int val = 0) : rows(r), cols(c) {
        v = vector<vector<int>> (r, vector<int>(c, val));
    }
    static Matrix Identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; i++) {
            res.v[i][i] = 1;
        }
        return res;
    }
    Matrix operator+(const Matrix& other) const {
        Matrix res(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                res.v[i][j] = (v[i][j] + other.v[i][j]) % MOD;
            }
        }
        return res;
    }
    Matrix operator*(const Matrix& other) const {
        Matrix res(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (v[i][k] == 0) continue;
                for (int j = 0; j < other.cols; j++) {
                    res.v[i][j] += (v[i][k] * other.v[k][j]) % MOD;
                    res.v[i][j] %= MOD;
                }
            }
        }
        return res;
    }
};
Matrix power(Matrix m, int p) {
    Matrix res = Matrix::Identity(m.rows);
    while (p) {
        if (p & 1) res = res * m;
        m = m * m;
        p >>= 1;
    }
    return res;
}
Matrix sumPower(Matrix a, int k) {
    if (k == 0) return Matrix(a.rows, a.cols, 0); 
    if (k & 1) return a + (a * sumPower(a, k - 1));
    Matrix half = sumPower(a, k >> 1); 
    return half + (half * power(a, k >> 1));
}

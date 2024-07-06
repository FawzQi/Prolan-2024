#include <iostream>
#include <vector>

using namespace std;

// Fungsi untuk mencetak matriks
void printMatrix(const vector<vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Fungsi untuk melakukan eliminasi Gauss-Jordan
void gaussJordanElimination(vector<vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; i++) {
        // Mencari baris dengan elemen terbesar pada kolom i
        int maxRow = i;
        for (int j = i + 1; j < rows; j++) {
            if (matrix[j][i] > matrix[maxRow][i]) {
                maxRow = j;
            }
        }

        // Menukar baris dengan elemen terbesar dengan baris i
        swap(matrix[i], matrix[maxRow]);

        // Mengubah elemen diagonal menjadi 1
        double pivot = matrix[i][i];
        for (int j = i; j < cols; j++) {
            matrix[i][j] /= pivot;
        }

        // Mengeliminasi elemen di bawah dan di atas pivot
        for (int j = 0; j < rows; j++) {
            if (j != i) {
                double factor = matrix[j][i];
                for (int k = i; k < cols; k++) {
                    matrix[j][k] -= factor * matrix[i][k];
                }
            }
        }
    }
}

int main() {
    int rows, cols;
    cout << 0.6762 + 0.2266 * 9.2 - 0.0064 * 9.2 * 9.2 << endl;
    cout << 0.6762 + 0.2266 * 8 - 0.0064 * 8 * 8 << endl;
    cout << 0.6762 + 0.2266 * 9 - 0.0064 * 9 * 9 << endl;
    cout << 0.6762 + 0.2266 * 9.5 - 0.0064 * 9.5 * 9.5 << endl;
    cout << "Masukkan jumlah baris: ";
    cin >> rows;
    cout << "Masukkan jumlah kolom: ";
    cin >> cols;

    vector<vector<double>> matrix(rows, vector<double>(cols));

    cout << "Masukkan elemen matriks:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> matrix[i][j];
        }
    }

    cout << "Matriks awal:" << endl;
    printMatrix(matrix);

    gaussJordanElimination(matrix);

    cout << "Matriks setelah eliminasi Gauss-Jordan:" << endl;
    printMatrix(matrix);

    return 0;
}
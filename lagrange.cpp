#include <iostream>
using namespace std;

int main() {
    // int orde;
    // float target;
    // cout << "ORDE : ";
    // cin >> orde;
    // cout << endl
    //      << "X yang dicari : ";
    // cin >> target;
    // cout << endl;

    // float x[orde];
    // float y[orde];

    int orde = 4;
    float target = 0.5;
    float x[orde] = {0.0, 0.4, 0.8, 1.2};
    float y[orde] = {1.0, 0.921061, 0.696707, 0.362358};
    float hasil;

    // int orde = 3;
    // float target = 1.2;
    // float x[orde] = {0.0, 0.4, 0.8};
    // float y[orde] = {1.0, 0.921061, 0.696707};
    // float hasil;

    for (int i = 0; i < orde; i++) {
        float hasilkali = y[i];
        for (int j = 0; j < orde; j++) {
            if (i != j) {
                hasilkali *= (target - x[j]);
                hasilkali /= (x[i] - x[j]);
            }
            if (i == j) {
                cout << i << " " << j << endl;
                break;
            }
        }

        hasil += hasilkali;
    }
    cout << "hasil : " << hasil << endl;
}
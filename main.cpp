#include <cstdint>  // Include the <cstdint> header
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
typedef enum {
    HIGHSCORE,
    GOLD
} dataset;

enum info {
    Niga,
    hitam,
    jomuk
};

// Struktur untuk menyimpan data pengguna
struct UserData {
    info data_key;
    vector<int> data_value;
};

enum {

};

void save_data(const map<info, UserData>& users) {
    ofstream file("userdata.txt");
    for (const auto& pair : users) {
        const auto& user = pair.second;
        file << user.data_key << " ";
        for (int i = 0; i < user.data_value.size(); i++) {
            file << user.data_value[i] << " ";
        }
        file << "\n";
    }
}

map<info, UserData> load_data() {
    map<info, UserData> users;  // Initialize the users map
    ifstream file("userdata.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        info data_key;
        int32_t data_key_value;                        // Define an int32_t variable to store the enum value
        ss >> data_key_value;                          // Read the enum value as an int32_t
        data_key = static_cast<info>(data_key_value);  // Convert the int32_t value to the enum type
        vector<int> data_value;
        int value;
        while (ss >> value) {
            data_value.push_back(value);
        }
        users[data_key] = {data_key, data_value};  // Assign the data to the users map
    }
    return users;
}

int main() {
    map<info, UserData> users;
    users[Niga] = {Niga, {100}};
    users[hitam] = {hitam, {200}};
    users[jomuk] = {jomuk, {300}};

    save_data(users);

    map<info, UserData> loadedUsers = load_data();

    cout << loadedUsers[Niga].data_key << " " << loadedUsers[Niga].data_value[0] << "\n";
    cout << loadedUsers[hitam].data_key << " " << loadedUsers[hitam].data_value[0] << endl;
    cout << loadedUsers[jomuk].data_key << " " << loadedUsers[jomuk].data_value[0];
    return 0;
}
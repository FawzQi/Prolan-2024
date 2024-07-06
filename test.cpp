#include <iostream>
#include <string>

class saiyan {
   private:
    std::string name;
    int powerLevel;

   public:
    saiyan(std::string _name, int _powerLevel) : name(_name), powerLevel(_powerLevel) {}

    saiyan operator+(const saiyan& other) const {
        std::string fusedName = name + "-" + other.name;
        int fusedPowerLevel = powerLevel + other.powerLevel;
        return saiyan(fusedName, fusedPowerLevel);
    }

    friend std::ostream& operator<<(std::ostream& out, const saiyan& c) {
        out << c.name << " " << c.powerLevel;
        return out;
    }
};

int main() {
    saiyan Goku("Goku", 9000);
    saiyan Vegeta("Vegeta", 8000);
    saiyan Gogeta = Goku + Vegeta;
    std::cout << Gogeta << std::endl;

    return 0;
}

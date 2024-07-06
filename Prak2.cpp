#include <iostream>
#include <memory>
#include <string>
// Kelas dasar abstrak untuk Logistics
class Logistics {
   public:
    virtual void send() = 0;
};
// Kelas turunan untuk Motorcycle
class Motorcycle : public Logistics {
   public:
    void send() override {
        std::cout << "Mengirim paket dengan Motorcycle..." << std::endl;
    }
};
// Kelas turunan untuk Van
class Van : public Logistics {
   public:
    void send() override {
        std::cout << "Mengirim paket dengan Van..." << std::endl;
    }
};
// Kelas turunan untuk Airplane
class Airplane : public Logistics {
   public:
    void send() override {
        std::cout << "Mengirim paket dengan Airplane..." << std::endl;
    }
};
// Kelas Factory untuk membuat objek Logistics yang tepat
class LogisticFactory {
   public:
    std::shared_ptr<Logistics> createLogistics(int radius) {
        if (radius < 10) {
            return std::make_shared<Motorcycle>();
        } else if (radius < 50) {
            return std::make_shared<Van>();
        } else {
            return std::make_shared<Airplane>();
        }
    }
};
int main() {
    std::string package;
    int radius;
    std::cout << "input package: ";
    std::getline(std::cin, package);
    std::cout << "radius: ";
    std::cin >> radius;
    LogisticFactory factory;
    std::shared_ptr<Logistics> logistic = factory.createLogistics(radius);
    std::cout << "package: " << package << std::endl;
    std::cout << "Radius: " << radius << std::endl;
    logistic->send();
    return 0;
}
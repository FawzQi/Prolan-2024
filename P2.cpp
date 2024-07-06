#include <iostream>
using namespace std;

class Npc {
   public:
    virtual int getHp() = 0;
    virtual void setHp(int hp) = 0;
};

// class Prajurit;

class Monster : public Npc {
   private:
    int hitpoint;
    int damage;

   public:
    Monster(int hp, int dmg) {
        hitpoint = hp;
        damage = dmg;
    }

    int getHp() override {
        return hitpoint;
    }

    void setHp(int hp) override {
        hitpoint = hp;
    }

    void serang(Npc* a) {
        a->setHp(a->getHp() - damage);
    }

    void info() {
        cout << "Hitpoint Monster : " << hitpoint << " , Status : " << (hitpoint > 0 ? "Hidup" : "Mati") << endl;
    }
};

class Prajurit : public Npc {
   private:
    int hitpoint;
    int damage;

   public:
    Prajurit(int hp, int dmg) {
        hitpoint = hp;
        damage = dmg;
    }

    int getHp() {
        return hitpoint;
    }

    void setHp(int hp) {
        hitpoint = hp;
    }

    void serang(Npc* a) {
        a->setHp(a->getHp() - damage);
    }

    void info() {
        cout << "Hitpoint Prajurit : " << hitpoint << " , Status : " << (hitpoint > 0 ? "Hidup" : "Mati") << endl;
    }
};

int main() {
    Monster sukuna(100, 40);
    Prajurit gojo(100, -5);

    while (sukuna.getHp() > 0 && gojo.getHp() > 0) {
        sukuna.serang(&gojo);
        gojo.serang(&sukuna);
    }

    sukuna.info();
    gojo.info();
}
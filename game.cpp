#include <raylib.h>

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class Kendaraan {
   public:
    virtual void draw() = 0;
    virtual void update() = 0;
};

class Truk : public Kendaraan {
   private:
    Vector2 posisi;
    float radius;
    // Color warna;
    int kecepatan;
    Vector2 lingkaranPosisi;
    float sudut;

   public:
    Truk(Vector2 p, float r, int s) : posisi(p), radius(r), sudut(0), kecepatan(s) {}
    void draw() override {
        DrawRectangle(posisi.x, posisi.y, 200, 80, BROWN);                     // Badan
        DrawRectangle(posisi.x + 200, posisi.y + 25, 40, 55, GREEN);           // Kepala
        DrawRectangle(posisi.x + 210, posisi.y + 35, 20, 20, GRAY);            // Jendela
        DrawCircle(posisi.x + 50, posisi.y + 80, 25, BLACK);                   // Roda belakang
        DrawCircle(posisi.x + 150, posisi.y + 80, 25, BLACK);                  // Roda depan
        DrawCircle(lingkaranPosisi.x + 50, lingkaranPosisi.y + 80, 7, GRAY);   // Efek putar roda belakang
        DrawCircle(lingkaranPosisi.x + 150, lingkaranPosisi.y + 80, 7, GRAY);  // Efek putar roda depan
    }
    void update() override {
        posisi.x += kecepatan;
        if (posisi.x > 1300) {
            posisi.x = -300;
            int px[] = {150, 300, 450};
            kecepatan = GetRandomValue(2, 10);
            posisi.y = px[GetRandomValue(0, 2)];
        }
        sudut += 90;
        lingkaranPosisi.x = posisi.x + cos(sudut) * 7;
        lingkaranPosisi.y = posisi.y + sin(sudut) * 7;
    }
    Vector2 getPosisi() {
        return posisi;
    }

    void tabrakanDengan(Truk& trukLain) {
        if (CheckCollisionCircles(getPosisi(), radius, trukLain.getPosisi(), 100)) {
            if (posisi.y != 450) {
                posisi.y += 150;
            } else {
                posisi.y -= 150;
            }
        }
    }
};

class Manusia : public Kendaraan {
   private:
    Vector2 posisi;
    float radius;
    Color warna;

   public:
    Manusia(Vector2 posisi, float radius, Color warna) : posisi(posisi), radius(radius), warna(warna) {}
    void draw() override {
        DrawCircleV(posisi, radius, warna);
    }
    void update() override {
        Vector2 mousePosisi = GetMousePosition();
        posisi.y = mousePosisi.y;
    }
};

class Mobil : public Kendaraan {
   private:
    Vector2 posisi;
    float radius;
    // Color warna;
    int kecepatan;
    Vector2 lingkaranPosisi;
    float sudut;

   public:
    Mobil(Vector2 posisi, float radius, int s) : posisi(posisi), radius(radius), kecepatan(s) {}
    void draw() override {
        DrawRectangle(posisi.x, posisi.y, 80, 30, RED);                       // atas
        DrawRectangle(posisi.x - 20, posisi.y + 30, 120, 40, RED);            // bawah
        DrawRectangle(posisi.x + 10, posisi.y + 10, 25, 20, GRAY);            // jendela
        DrawRectangle(posisi.x + 45, posisi.y + 10, 25, 20, GRAY);            // jendela
        DrawCircle(posisi.x + 15, posisi.y + 70, 15, BLACK);                  // roda belakang
        DrawCircle(posisi.x + 65, posisi.y + 70, 15, BLACK);                  // roda depan
        DrawCircle(lingkaranPosisi.x + 15, lingkaranPosisi.y + 70, 5, GRAY);  // efek putar roda belakang
        DrawCircle(lingkaranPosisi.x + 65, lingkaranPosisi.y + 70, 5, GRAY);  // efek putar roda depan

        // DrawCircleV(posisi, radius, GRAY);
    }
    void update() override {
        posisi.x += 7;
        if (posisi.x > 1300) {
            posisi.x = -300;
            int px[] = {150, 300, 450};
            posisi.y = px[GetRandomValue(0, 2)];
            kecepatan = GetRandomValue(2, 6);
        }
        sudut += 90;
        lingkaranPosisi.x = posisi.x + cos(sudut) * 7;
        lingkaranPosisi.y = posisi.y + sin(sudut) * 7;
    }
    Vector2 getPosisi() {
        return posisi;
    }

    void tabrakanDengan(Mobil& mobilLain) {
        if (CheckCollisionCircles(getPosisi(), radius, mobilLain.getPosisi(), 100)) {
            if (posisi.y != 450) {
                posisi.y += 150;
            } else {
                posisi.y -= 150;
            }
        }
    }

    void tabrakanDengan(Truk& mobilLain) {
        if (CheckCollisionCircles(getPosisi(), radius, mobilLain.getPosisi(), 100)) {
            if (posisi.y != 450) {
                posisi.y += 150;
            } else {
                posisi.y -= 150;
            }
        }
    }
    void collision(Truk t, Mobil m) {
        Vector2 mobilPosisi = m.getPosisi();
        Vector2 trukPosisi = t.getPosisi();

        if (CheckCollisionCircleRec(mobilPosisi, radius, {trukPosisi.x, trukPosisi.y, 200, 80})) {
            // Handle collision between mobil and truk
            // ...
        }
    }
};

int main() {
    // Truk truk({400, 300}, 50, BROWN);
    // Mobil mobil({100, 100}, 50, RED);

    const int windowWidth = 1000;
    const int windowHeight = 600;
    Manusia manusia({windowWidth - 10, windowHeight / 2}, 10, BLUE);

    SetTargetFPS(60);
    InitWindow(windowWidth, windowHeight, "mbill");

    // while (!WindowShouldClose()) {
    //     BeginDrawing();

    //     // truk.update();
    //     ClearBackground(RAYWHITE);
    //     manusia.update();
    //     manusia.draw();
    //     mobil.draw();
    //     mobil.update();
    //     truk.draw();
    //     truk.update();
    //     EndDrawing();
    // }

    // CloseWindow();

    // Di dalam fungsi main
    std::vector<Mobil> mobilList;
    std::vector<Truk> trukList;

    int frameCounter = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        manusia.update();
        manusia.draw();

        // Tambahkan mobil dan truk setiap 60 frame (1 detik jika FPS adalah 60)
        if (frameCounter % 240 == 0 || frameCounter == 0) {
            bool isMobil = GetRandomValue(0, 1);

            if (mobilList.size() < 4 && isMobil == 1) {
                mobilList.push_back(Mobil({-3000, 150}, 50, 10));
            }
            if (trukList.size() < 4 && isMobil == 0) {
                trukList.push_back(Truk({-400, 300}, 50, 10));
            }
        }

        for (auto& mobil : mobilList) {
            mobil.draw();
            mobil.update();
        }
        for (auto& truk : trukList) {
            truk.draw();
            truk.update();
        }
        for (auto& mobil : mobilList) {
            for (auto& mobilLain : mobilList) {
                if (&mobil != &mobilLain) {
                    mobil.tabrakanDengan(mobilLain);
                }
            }
            mobil.draw();
            mobil.update();
        }
        for (auto& truk : trukList) {
            for (auto& trukLain : trukList) {
                if (&truk != &trukLain) {
                    truk.tabrakanDengan(trukLain);
                }
            }
            truk.draw();
            truk.update();
        }

        for (auto& mobil : mobilList) {
            for (auto& truk : trukList) {
                mobil.tabrakanDengan(truk);
                truk.draw();
                truk.update();
            }
            mobil.draw();
            mobil.update();
        }

        EndDrawing();
        frameCounter++;
    }

    CloseWindow();
}
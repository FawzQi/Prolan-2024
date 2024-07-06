#include <raylib.h>
#include <stdint.h>
#define DR_MP3_IMPLEMENTATION
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define screenWidth 1920
#define screenHeight 1080

bool isLeave = false;
Texture2D imgobst[3];

enum DATA {
    HIGHSCORE,
    MONEY,
    OBST1,
    OBST2,
    OBST3,
    OBST4,
    OBST5,
    OBST6,
    OBST7,
    OBST8,
    OBST9,
    OBST10,
    LASTSCORE,
    ISLEAVE,
    PLAYER,
    SCORER,
    VOLUME
};

struct UserData {
    DATA data_key;
    vector<int> data_value;
};

map<DATA, vector<int>> Users_Save;
map<DATA, UserData> Users_Load;

void Save_Data(const map<DATA, UserData>& users) {
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

map<DATA, UserData> Load_Data() {
    map<DATA, UserData> users;
    ifstream file("userdata.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        DATA data_key;
        int32_t data_key_value;
        ss >> data_key_value;
        data_key = static_cast<DATA>(data_key_value);
        vector<int> data_value;
        int value;
        while (ss >> value) {
            data_value.push_back(value);
        }
        users[data_key] = {data_key, data_value};
    }
    return users;
}

void saveGameData(const map<DATA, vector<int>>& gameData) {
    std::ofstream file("gamedata.txt");
    for (const auto& pair : gameData) {
        file << static_cast<int>(pair.first) << " ";
        for (int value : pair.second) {
            file << value << " ";
        }
        file << "\n";
    }
}

// Fungsi untuk memuat data game
std::map<DATA, std::vector<int>> loadGameData() {
    std::map<DATA, std::vector<int>> gameData;
    std::ifstream file("gamedata.txt");
    int key;
    while (file >> key) {
        std::vector<int> values;
        int value;
        while (file.peek() != '\n' && file >> value) {
            values.push_back(value);
        }
        gameData[static_cast<DATA>(key)] = values;
    }
    return gameData;
}

Music main_menu_s;
Music playing_s;
Music game_over_s;
Music jump_s;
Music hit_s;

int last_frameTime = 0;

int xpos[2] = {0, 0};
int ypos[2] = {0, 0};

int frameTime = 0;
enum STATE {
    LOADING,
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    POP_MAINMENU,
    RESUME_DELAY,
    SHOP
};
// STATE STATE = MAIN_MENU;
STATE MODE = MAIN_MENU;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                 //
//                                                                                                                                                 //
//                                                                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Objek {
   public:
    virtual void draw() = 0;
    virtual void update() = 0;
};

class Obst : public Objek {
   private:
    float radius;
    int kecepatan;
    float interval;
    Texture2D obsimg[3];

    Vector2 size;

   public:
    int type;
    Vector2 posisi;
    Obst(Vector2 p) : posisi(p), interval(600), kecepatan(10), type(GetRandomValue(0, 2)) {
    }
    void draw() override {
        if (type == 0) {
            // DrawRectangle(posisi.x, posisi.y, 80, 140, RED);
            DrawTexturePro(obsimg[0], {0, 0, (float)obsimg[0].width, (float)obsimg[0].height}, {posisi.x - 20, posisi.y - 20, (float)120, (float)160}, {0, 0}, 0, WHITE);
        }
        if (type == 1) {
            // DrawRectangle(posisi.x, posisi.y - 110, 80, 250, BLUE);
            DrawTexturePro(obsimg[1], {0, 0, (float)obsimg[1].width, (float)obsimg[1].height}, {posisi.x - 20, posisi.y - 130, (float)120, (float)270}, {0, 0}, 0, WHITE);
        }
        if (type == 2) {
            // DrawRectangle(posisi.x, posisi.y - 700, 80, 720, GREEN);
            DrawTexturePro(obsimg[2], {0, 0, (float)obsimg[2].width, (float)obsimg[2].height}, {posisi.x - 10, posisi.y - 700, (float)100, (float)720}, {0, 0}, 0, WHITE);
        }
    }
    void update() override {
    }

    void initexture() {
        obsimg[0] = imgobst[0];
        obsimg[1] = imgobst[1];
        obsimg[2] = imgobst[2];
    }

    void generate(Vector2 pos, float intrvl) {
        posisi.x -= kecepatan;
        if (pos.x == 1920 - intrvl) {
            posisi.x = 1920;
            type = GetRandomValue(0, 2);
        }
    }

    bool checkCollision(Rectangle pos) {
        Rectangle temp;
        if (type == 0) temp = {posisi.x, posisi.y, 80, 140};
        if (type == 1) temp = {posisi.x, posisi.y - 110, 80, 250};
        if (type == 2) temp = {posisi.x, posisi.y - 700, 80, 720};

        return CheckCollisionRecs(pos, temp);
        // cout << "COLLISION" << endl;
    }
    Vector2 getPosisi() {
        return posisi;
    }

    void unload() {
        for (int i = 0; i < 3; i++) {
            UnloadTexture(obsimg[i]);
        }
    }

    float getInterval() {
        return interval;
    }
};
Obst* Obstacle[10];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Obst Obst Obst  Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst    //
//                                                                                                                                                 //
// Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Player : public Objek {
   private:
    Texture2D player[4];
    Texture2D playerjump;
    Texture2D playerjump2[10];
    Texture2D playerslide;

    float grvt = 6;

   public:
    int lives = 1;
    int savedLives = lives;
    int playerFrame = 0;
    Vector2 posisi = {screenWidth / 4 - 150, screenHeight - screenHeight / 4 - 320};
    Rectangle hitBox = {posisi.x + 60, posisi.y + 160, 90, 170};
    Rectangle hitBoxSlide = {posisi.x, posisi.y + 260, 160, 70};
    Rectangle hitBoxJump1 = {posisi.x + 40, posisi.y + 160, 120, 160};
    Rectangle hitBoxJumpland = {posisi.x + 65, posisi.y + 145, 70, 200};
    bool isSlide = false;
    int isHit = 0;
    float jumpSpeed = 0;
    int jumpCount = 0;
    int opacity = 255;
    Color color = {255, 255, 255, opacity};
    void draw() override {
        if (isSlide) {
            DrawTexturePro(playerslide, {0, 0, (float)playerslide.width, (float)playerslide.height}, {posisi.x, posisi.y, (float)200, (float)440}, {0, 0}, 0, color);
        } else if (jumpCount == 0) {
            DrawTexturePro(player[playerFrame % 4], {0, 0, (float)player[playerFrame % 4].width, (float)player[playerFrame % 4].height}, {posisi.x, posisi.y, (float)200, (float)440}, {0, 0}, 0, color);
        } else if (jumpCount == 1) {
            DrawTexturePro(playerjump, {0, 0, (float)playerjump.width, (float)playerjump.height}, {posisi.x, posisi.y, (float)200, (float)440}, {0, 0}, 0, color);
        } else if (jumpCount == 2) {
            DrawTexturePro(playerjump2[playerFrame % 10], {0, 0, (float)playerjump2[playerFrame % 10].width, (float)playerjump2[playerFrame % 10].height}, {posisi.x, posisi.y, (float)200, (float)440}, {0, 0}, 0, color);
        }
        // isSlide ? DrawRectangleRec(hitBoxSlide, RED) : DrawRectangleRec(hitBox, RED);

        // DrawRectangleRec(hitBoxJump1, RED);
        // DrawTexturePro(playerjump, {0, 0, (float)playerjump.width, (float)playerjump.height}, {posisi.x, posisi.y, (float)200, (float)440}, {0, 0}, 0, WHITE);
    }
    void update() override {
        if (frameTime % 4 == 0) {
            if (jumpCount == 1) {
                playerFrame = 0;
            } else if (isSlide) {
                playerFrame = 9;
            } else {
                playerFrame++;
                if (jumpCount == 2 && playerFrame > 9) playerFrame = 9;
            }
        }

        if (isSlide) {
            grvt = 10;
        } else {
            grvt = 5.5;
        }

        if (frameTime % 2 == 0) {
            posisi.y -= jumpSpeed;
            jumpSpeed -= grvt;
            if (posisi.y >= 1080 - 1080 / 4 - 320) {
                jumpCount = 0;
                posisi.y = 1080 - 1080 / 4 - 320;
                jumpSpeed = 0;
            }
        }
        hitBox = {posisi.x + 60, posisi.y + 160, 90, 170};
        hitBoxSlide = {posisi.x + 30, posisi.y + 260, 150, 70};
        hitBoxJump1 = {posisi.x + 40, posisi.y + 160, 120, 160};
        hitBoxJumpland = {posisi.x + 65, posisi.y + 145, 70, 200};
    }
    Vector2 getPosisi() {
        return posisi;
    }

    Rectangle getHitBox() {
        if (isSlide) {
            return hitBoxSlide;
        } else if (jumpCount == 0) {
            return hitBox;
        } else if (jumpCount == 1) {
            return hitBox;

        } else {
            if (playerFrame == 0 || playerFrame == 9) {
                return hitBoxJumpland;
            } else {
                return hitBoxJump1;
            }
        }
    }

    void initTexture() {
        player[0] = LoadTexture("assets/player/Proyek 10_1.png");
        player[1] = LoadTexture("assets/player/Proyek 10_2.png");
        player[2] = LoadTexture("assets/player/Proyek 10_3.png");
        player[3] = LoadTexture("assets/player/Proyek 10_4.png");

        playerjump = LoadTexture("assets/player/Proyek 10_5.png");

        playerjump2[0] = LoadTexture("assets/player/Proyek 10_10.png");
        playerjump2[1] = LoadTexture("assets/player/Proyek 10_6.png");
        playerjump2[2] = LoadTexture("assets/player/Proyek 10_7.png");
        playerjump2[3] = LoadTexture("assets/player/Proyek 10_8.png");
        playerjump2[4] = LoadTexture("assets/player/Proyek 10_9.png");
        playerjump2[5] = playerjump2[1];
        playerjump2[6] = playerjump2[2];
        playerjump2[7] = playerjump2[3];
        playerjump2[8] = playerjump2[4];
        playerjump2[9] = LoadTexture("assets/player/Proyek 10_11.png");

        playerslide = LoadTexture("assets/player/Proyek 10_12.png");
    }
    void Unload() {
        for (int i = 0; i < 4; i++) {
            UnloadTexture(player[i]);
        }
        UnloadTexture(playerjump);
        for (int i = 0; i < 10; i++) {
            UnloadTexture(playerjump2[i]);
        }
        UnloadTexture(playerslide);
    }
};
Player player;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Obst Obst Obst  Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst    //
//                                                                                                                                                 //
// Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Coin : public Objek {
   public:
    Vector2 posisi = {1920, 790};
    int collectedCoin = 0;

    bool isTaken = false;
    void draw() override {
        if (!isTaken) DrawCircle(posisi.x, posisi.y, 25, YELLOW);
    }

    void update() override {
        posisi.x -= 10;
        if (posisi.x < -100) {
            posisi.x = GetRandomValue(1920, 2300);
            posisi.y = GetRandomValue(540, 790);
            isTaken = false;
        }
    }

    void CheckCollision(Rectangle player, Obst obst) {
        if (CheckCollisionCircleRec(posisi, 50, player)) {
            if (!isTaken) collectedCoin++;
            isTaken = true;
        }

        if (obst.type == 0) {
            if (CheckCollisionCircleRec(posisi, 50, {obst.posisi.x, obst.posisi.y, 80, 140})) {
                posisi.x += 230;
            }
        } else if (obst.type == 1) {
            if (CheckCollisionCircleRec(posisi, 50, {obst.posisi.x, obst.posisi.y - 110, 80, 250})) {
                posisi.x += 230;
            }
        } else if (obst.type == 2) {
            if (CheckCollisionCircleRec(posisi, 50, {obst.posisi.x, obst.posisi.y - 700, 80, 720})) {
                posisi.x += 230;
            }
        }
    }
};
Coin coin;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Obst Obst Obst  Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst Obst    //
//                                                                                                                                                 //
// Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player Player     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Property {
   public:
    Rectangle start = {820, 600, 290, 100};
    Rectangle quit = {820, 800, 290, 100};
    Texture2D bg_MAIN_MENU;
    Texture2D Logo;
    Texture2D bg_PLAYING[3];
    Texture2D platform[3];
    Texture2D heart;
    Vector2 posisiPlatform = {0, 840};
    int Score_multiplier = 1;
    int Score = 0;
    int HighScore = 0;
    float posxBg = 0;
    float xsound = 1410;
    float ysound = 820;
    bool soundclick = false;
    int volume = 0;

    float range(float i, float j, float k, float l, float m) {
        if (i < j) i = j;
        if (i > k) i = k;
        return ((i - j) / (k - j) * (m - l));
    }

    void MAIN_MENU() {
        ClearBackground(RAYWHITE);
        DrawTexturePro(bg_PLAYING[0], {0, 0, (float)bg_PLAYING[0].width, (float)bg_PLAYING[0].height}, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
        DrawTexturePro(Logo, {0, 0, (float)Logo.width, (float)Logo.height}, {960, 120, (float)660, (float)200}, {0, 0}, 0, WHITE);
        DrawText("Upgrades!", 120, 310, 50, BLACK);
        DrawRectangle(120, 360, 480, 480, BROWN);
        DrawRectangle(130, 370, 460, 460, DARKGREEN);  // Coin
        // Lives

        if (CheckCollisionPointRec(GetMousePosition(), {140, 540, 440, 100})) {
            DrawRectangleRec({140, 540, 440, 100}, DARKBROWN);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coin.collectedCoin >= 500) {
                coin.collectedCoin -= 500;
                player.savedLives++;
            }
        } else {
            DrawRectangleRec({140, 540, 440, 100}, BROWN);
        }

        DrawText(TextFormat("Coin :    %d", coin.collectedCoin), 160, 380, 40, BLACK);
        DrawCircle(300, 400, 15, GOLD);

        DrawText(TextFormat("Current Lives : %d", player.savedLives), 160, 480, 40, BLACK);
        DrawText("Lives", 160, 560, 60, BLACK);
        DrawText("   500", 400, 560, 60, BLACK);
        DrawCircle(440, 580, 20, GOLD);
        // Scores
        if (CheckCollisionPointRec(GetMousePosition(), {140, 720, 440, 100})) {
            DrawRectangleRec({140, 720, 440, 100}, DARKBROWN);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && coin.collectedCoin >= 500) {
                coin.collectedCoin -= 500;
                Score_multiplier++;
            }
        } else {
            DrawRectangleRec({140, 720, 440, 100}, BROWN);
        }

        DrawText(TextFormat("Current Scorer : %d", Score_multiplier), 160, 670, 40, BLACK);

        DrawText("Scores", 160, 740, 60, BLACK);
        DrawText("   500", 400, 740, 60, BLACK);
        DrawCircle(440, 770, 20, GOLD);

        if (CheckCollisionPointRec(GetMousePosition(), start)) {
            DrawRectangleRec(start, DARKBROWN);
        } else {
            DrawRectangleRec(start, BROWN);
        }
        if (CheckCollisionPointRec(GetMousePosition(), quit)) {
            DrawRectangleRec(quit, DARKBROWN);
        } else {
            DrawRectangleRec(quit, BROWN);
        }

        DrawText("START", 840, 620, 70, BLACK);
        DrawText("QUIT", 870, 820, 70, BLACK);
        DrawText("Volume", 1410, 750, 40, BLACK);
        DrawRectangle(1400, 800, 300, 40, BROWN);
        DrawRectangle(1410, 810, 280, 20, DARKBROWN);
        DrawRectangle(1410, 810, xsound - 1410, 20, WHITE);

        DrawCircle(xsound, ysound, 20, BLACK);
        if (CheckCollisionPointCircle(GetMousePosition(), {xsound, ysound}, 20)) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                soundclick = true;
            }
        }
        if (soundclick) {
            if (GetMouseX() <= 1690 && GetMouseX() >= 1410) {
                xsound = GetMouseX();
                volume = range(xsound, 1410.0f, 1690.0f, 0.0f, 100.0f);
                // DrawText(TextFormat("%f", range(xsound, 1410.0f, 1690.0f, 0.0f, 1.0f)), 1410, 900, 40, BLACK);
            }
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            soundclick = false;
        }

        // volume = range(xsound, 1410.0f, 1690.0f, 0.0f, 1.0f);
    }

    void PLAYING() {
        DrawTexturePro(bg_PLAYING[0], {0, 0, (float)bg_PLAYING[0].width, (float)bg_PLAYING[0].height}, {posxBg, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
        DrawTexturePro(bg_PLAYING[1], {0, 0, (float)bg_PLAYING[1].width, (float)bg_PLAYING[1].height}, {posxBg + 1920, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
        DrawTexturePro(bg_PLAYING[2], {0, 0, (float)bg_PLAYING[2].width, (float)bg_PLAYING[2].height}, {posxBg + 1920 * 2, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
        DrawTexturePro(platform[0], {0, 0, (float)platform[0].width, (float)platform[0].height}, {posisiPlatform.x, posisiPlatform.y, (float)1920, (float)320}, {0, 0}, 0, WHITE);
        DrawTexturePro(platform[2], {0, 0, (float)platform[2].width, (float)platform[2].height}, {posisiPlatform.x + 1920, posisiPlatform.y, (float)1920, (float)320}, {0, 0}, 0, WHITE);
        DrawTexturePro(platform[0], {0, 0, (float)platform[0].width, (float)platform[0].height}, {posisiPlatform.x + 3840, posisiPlatform.y, (float)1920, (float)320}, {0, 0}, 0, WHITE);
        DrawTexturePro(heart, {0, 0, (float)heart.width, (float)heart.height}, {840, 30, 80, 80}, {0, 0}, 0, WHITE);
        DrawText(TextFormat("%d", player.lives), 940, 40, 65, BLACK);
        // DrawRectangle(GetScreenWidth() - 100, 0, 100, 100, BROWN);  // Pause button
        DrawRectangle(GetScreenWidth() - 40, 15, 20, 70, GRAY);  // Pause button
        DrawRectangle(GetScreenWidth() - 80, 15, 20, 70, GRAY);  // Pause button
    }

    void PLAYING_UPDATE(Coin coin) {
        DrawText(TextFormat("Score : %d", Score), 10, 70, 50, BLACK);
        DrawText(TextFormat("High Score : %d", HighScore), 10, 140, 50, BLACK);
        DrawText(TextFormat("Coin : %d", coin.collectedCoin), 10, 210, 50, BLACK);
        Score += Score_multiplier;
        if (Score > HighScore) {
            HighScore = Score;
        }
        posisiPlatform.x -= 10;
        posxBg -= 1;
        if (posisiPlatform.x < -3840) {
            posisiPlatform.x = 0;
        }
        if (posxBg < -3840) {
            posxBg = 0;
        }
    }

    void PAUSED() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4f));
        DrawRectangle(660, 120, 540, 640, BROWN);
        DrawRectangle(670, 130, 520, 620, DARKGREEN);
        if (CheckCollisionPointRec(GetMousePosition(), {720, 430, 420, 90})) {
            DrawRectangle(720, 430, 420, 90, DARKBROWN);
        } else {
            DrawRectangle(720, 430, 420, 90, BROWN);
        }
        if (CheckCollisionPointRec(GetMousePosition(), {720, 580, 420, 90})) {
            DrawRectangle(720, 580, 420, 90, DARKBROWN);
        } else {
            DrawRectangle(720, 580, 420, 90, BROWN);
        }
        DrawText("PAUSED", 785, 170, 70, BLACK);
        DrawText("RESUME", 820, 450, 50, BLACK);
        DrawText("SAVE N QUIT", 755, 600, 50, BLACK);
    }

    void GAME_OVER() {
        DrawRectangle(660, 60, 600, 660, BROWN);
        DrawRectangle(670, 70, 580, 640, DARKGREEN);
        DrawText("GAME OVER", 710, 100, 80, BLACK);
        DrawText("Score  ", 710, 200, 50, BLACK);
        DrawText(TextFormat("%d", Score), 900, 200, 50, BLACK);
        DrawText("High Score  ", 710, 300, 50, BLACK);
        DrawText(TextFormat("%d", HighScore), 1000, 300, 50, BLACK);
        if (CheckCollisionPointRec(GetMousePosition(), {720, 435, 480, 90})) {
            DrawRectangle(720, 435, 480, 90, DARKBROWN);
        } else {
            DrawRectangle(720, 435, 480, 90, BROWN);
        }
        if (CheckCollisionPointRec(GetMousePosition(), {720, 565, 480, 90})) {
            DrawRectangle(720, 565, 480, 90, DARKBROWN);
        } else {
            DrawRectangle(720, 565, 480, 90, BROWN);
        }
        DrawText("RETRY", 871, 465, 50, BLACK);
        DrawText("QUIT", 898, 585, 50, BLACK);
    }

    void POP_MAINMENU() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4f));
        DrawRectangle(690, 120, 540, 640, BROWN);
        DrawRectangle(700, 130, 520, 620, DARKGREEN);
        if (CheckCollisionPointRec(GetMousePosition(), {750, 430, 420, 90})) {
            DrawRectangle(750, 430, 420, 90, DARKBROWN);
        } else {
            DrawRectangle(750, 430, 420, 90, BROWN);
        }
        if (CheckCollisionPointRec(GetMousePosition(), {750, 580, 420, 90})) {
            DrawRectangle(750, 580, 420, 90, DARKBROWN);
        } else {
            DrawRectangle(750, 580, 420, 90, BROWN);
        }
        DrawText("You left in the\nmiddle of run\nlast time! ", 800, 170, 50, BLACK);
        DrawText("Continue Run", 790, 450, 50, BLACK);
        DrawText("New Run", 855, 600, 50, BLACK);
    }

    void initTextur() {
        bg_MAIN_MENU = LoadTexture("assets/wallpaper main.png");
        bg_PLAYING[0] = LoadTexture("assets/playyybg1.png");
        bg_PLAYING[1] = LoadTexture("assets/playybg2.png");
        bg_PLAYING[2] = bg_PLAYING[0];
        platform[0] = LoadTexture("assets/IMG_20240611_195737.png");
        platform[1] = LoadTexture("assets/IMG_20240611_195441.png");
        platform[2] = platform[1];
        heart = LoadTexture("assets/1718090833088.png");
        Logo = LoadTexture("assets/Screenshot_2024-07-06_153138-removebg-preview.png");
    }

    void Unload() {
        UnloadTexture(bg_MAIN_MENU);
        UnloadTexture(bg_PLAYING[0]);
        UnloadTexture(bg_PLAYING[1]);
        UnloadTexture(bg_PLAYING[2]);
        UnloadTexture(platform[0]);
        UnloadTexture(platform[1]);
        UnloadTexture(heart);
        UnloadTexture(Logo);
    }
};

Property prop;

void Reset() {
    prop.Score = 0;
    player.jumpCount = 0;
    player.jumpSpeed = 0;
    player.isSlide = false;
    player.posisi.y = screenHeight - screenHeight / 4 - 320;
    player.playerFrame = 0;
    player.lives = player.savedLives;
    player.isHit = 0;

    for (int i = 0; i < 10; i++) {
        Obstacle[i]->posisi.x = 1920 + i * 1920;
    }
    Obstacle[0]->type = GetRandomValue(0, 2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                 //
//                                                                                                                                                 //
//                                                                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Control {
   public:
    void main_menu(Property& prop) {
        if (CheckCollisionPointRec(GetMousePosition(), prop.start)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (isLeave == 0) {
                    Reset();
                    isLeave = 1;
                    MODE = PLAYING;

                } else {
                    MODE = POP_MAINMENU;
                }
            }
        }
    }

    void playing(Player& player) {
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (player.jumpCount < 2) {
                player.jumpCount += 1;
                player.jumpSpeed = 45;
            }
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            player.isSlide = true;
        } else {
            player.isSlide = false;
        }
        if (IsKeyPressed(KEY_P)) {
            MODE = PAUSED;
        }
        if (CheckCollisionPointRec(GetMousePosition(), {float(GetScreenWidth()) - 100, 0, 100, 100})) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                MODE = PAUSED;
            }
        }
    }

    void paused() {
        if (CheckCollisionPointRec(GetMousePosition(), {720, 430, 420, 90})) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                MODE = PLAYING;
            }
        }
        if (CheckCollisionPointRec(GetMousePosition(), {720, 580, 420, 90})) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < 10; i++) {
                    Users_Save[static_cast<DATA>(OBST1 + i)] = {{static_cast<int>(OBST1 + i), static_cast<int>(Obstacle[i]->posisi.x), Obstacle[i]->type}};
                }
                Users_Save[LASTSCORE] = {{prop.Score}};
                Users_Save[ISLEAVE] = {{ISLEAVE, 1}};

                MODE = MAIN_MENU;
            }
        }
    }

    void game_over() {
        if (CheckCollisionPointRec(GetMousePosition(), {720, 435, 480, 90})) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                MODE = PLAYING;
                isLeave = 1;
                Reset();
            }
        }
        if (CheckCollisionPointRec(GetMousePosition(), {720, 565, 480, 90})) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                MODE = MAIN_MENU;
                Reset();
            }
        }
    }

    void pop_mainmenu() {
        if (CheckCollisionPointRec(GetMousePosition(), {750, 430, 420, 90})) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                frameTime = 0;
                MODE = RESUME_DELAY;
            }
        }
        if (CheckCollisionPointRec(GetMousePosition(), {750, 580, 420, 90})) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Reset();
                MODE = PLAYING;
            }
        }
    }
};
Control control;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                 //
//                                                                                                                                                 //
//                                                                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int shakesFrame = 0;

void load_music() {
    main_menu_s = LoadMusicStream("music/videoplayback (1).mp3");
    playing_s = LoadMusicStream("music/videoplayback.mp3");
    game_over_s = LoadMusicStream("music/videoplayback (2).mp3");
    jump_s = LoadMusicStream("music/cartoon-jump-6462.mp3");
    hit_s = LoadMusicStream("music/punch-2-37333.mp3");
}

void music_prep(int m) {
    if (m == 0) {
        for (int x = 0; x < 12000; x++) {
            UpdateMusicStream(main_menu_s);
        }
    } else if (m == 1) {
        for (int x = 0; x < 12000; x++) {
            UpdateMusicStream(playing_s);
        }
    } else if (m == 2) {
        for (int x = 0; x < 12000; x++) {
            UpdateMusicStream(game_over_s);
        }
    } else if (m == 3) {
        for (int x = 0; x < 12000; x++) {
            UpdateMusicStream(jump_s);
        }
    } else if (m == 4) {
        for (int x = 0; x < 12000; x++) {
            UpdateMusicStream(hit_s);
        }
    }
}

void music_control() {
    SetMusicVolume(main_menu_s, prop.range(prop.xsound, 1410.0f, 1690.0f, 0.0f, 1.0f));
    SetMusicVolume(playing_s, prop.range(prop.xsound, 1410.0f, 1690.0f, 0.0f, 1.0f));
    SetMusicVolume(game_over_s, prop.range(prop.xsound, 1410.0f, 1690.0f, 0.0f, 1.0f));

    if (MODE == MAIN_MENU || MODE == POP_MAINMENU) {
        UpdateMusicStream(main_menu_s);

        if (!IsMusicStreamPlaying(main_menu_s)) {
            StopMusicStream(main_menu_s);  // Stop to reset internal counters
            PlayMusicStream(main_menu_s);  // Play again
            music_prep(0);
        }
    } else {
        StopMusicStream(main_menu_s);
    }

    if (MODE == PLAYING || MODE == PAUSED) {
        UpdateMusicStream(playing_s);

        if (!IsMusicStreamPlaying(playing_s)) {
            StopMusicStream(playing_s);  // Stop to reset internal counters
            PlayMusicStream(playing_s);  // Play again
            music_prep(1);
        }

    } else {
        StopMusicStream(playing_s);
    }
}

void unload_music() {
    UnloadMusicStream(main_menu_s);
    UnloadMusicStream(playing_s);
    UnloadMusicStream(game_over_s);
    UnloadMusicStream(jump_s);
    UnloadMusicStream(hit_s);
}

int main() {
    Camera2D camera;
    camera.target = {0, 0};
    camera.offset = {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    bool iframe = true;

    InitAudioDevice();
    load_music();

    for (int i = 0; i < 10; i++) {
        Vector2 temp = {float(i * 1920 + 1920), 700};
        Obstacle[i] = new Obst(temp);
    }

    map<DATA, UserData> SAVEME;
    Users_Load = Load_Data();
    isLeave = Users_Load[ISLEAVE].data_value[0];
    prop.HighScore = Users_Load[HIGHSCORE].data_value[0];
    prop.Score = Users_Load[LASTSCORE].data_value[0];
    prop.Score_multiplier = Users_Load[SCORER].data_value[0];
    player.posisi.y = static_cast<float>(Users_Load[PLAYER].data_value[0]);
    player.jumpCount = Users_Load[PLAYER].data_value[1];
    player.jumpSpeed = static_cast<float>(Users_Load[PLAYER].data_value[2]);
    player.isSlide = Users_Load[PLAYER].data_value[3];
    player.playerFrame = Users_Load[PLAYER].data_value[4];
    player.lives = Users_Load[PLAYER].data_value[5];
    player.savedLives = Users_Load[PLAYER].data_value[6];
    player.isHit = Users_Load[PLAYER].data_value[7];
    coin.collectedCoin = Users_Load[MONEY].data_value[0];
    prop.xsound = prop.range(static_cast<float>(Users_Load[VOLUME].data_value[0]), 0.0f, 100.0f, 0.0f, 280.0f) + 1410.0f;
    prop.volume = Users_Load[VOLUME].data_value[0];

    for (int i = 0; i < 10; i++) {
        Obstacle[i]->posisi.x = Users_Load[static_cast<DATA>(OBST1 + i)].data_value[0];
        Obstacle[i]->type = Users_Load[static_cast<DATA>(OBST1 + i)].data_value[1];
    }

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    prop.initTextur();
    player.initTexture();
    imgobst[0] = LoadTexture("assets/obs/1718113986562.png");
    imgobst[1] = LoadTexture("assets/obs/1718114162844.png");
    imgobst[2] = LoadTexture("assets/obs/1718114143324.png");  // obsimg[1] =
                                                               // obsimg[2] =
                                                               // obsimg[3]
    for (int i = 0; i < 10; i++) {
        Obstacle[i]->initexture();
    }
    PlayMusicStream(main_menu_s);
    music_prep(0);
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        music_control();
        BeginDrawing();
        BeginMode2D(camera);
        if (MODE == MAIN_MENU) {
            prop.MAIN_MENU();
            control.main_menu(prop);

            if (CheckCollisionPointRec(GetMousePosition(), prop.quit)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    break;
                }
            }
        } else if (MODE == PLAYING) {
            ClearBackground(RAYWHITE);
            prop.PLAYING();
            prop.PLAYING_UPDATE(coin);

            player.draw();
            player.update();
            coin.draw();
            coin.update();
            control.playing(player);
            // DrawText(TextFormat("iframe value: %d", iframe), 80, 800, 30, RED);

            for (int i = 0; i < 10; i++) {
                Obstacle[i]->draw();
                int index = (i - 1) >= 0 ? i - 1 : 9;
                Obstacle[i]->generate(Obstacle[index]->getPosisi(), Obstacle[index]->getInterval());
                coin.CheckCollision(player.getHitBox(), *Obstacle[i]);
                if (Obstacle[i]->checkCollision(player.getHitBox()) && player.isHit <= 1) {
                    if (player.lives == 1) {
                        StopMusicStream(game_over_s);
                        PlayMusicStream(game_over_s);
                        last_frameTime = frameTime;
                        MODE = GAME_OVER;
                        isLeave = 0;

                    } else {
                        player.lives--;
                        player.isHit = 180;
                        shakesFrame = 10;
                    }
                }
            }

            if (player.isHit > 0) {
                if (player.isHit % 15 == 0) {
                    if (iframe)
                        iframe = false;
                    else
                        iframe = true;
                }
                if (iframe)
                    player.color = {255, 255, 255, 155};
                else
                    player.color = {255, 255, 255, 255};
                // player.color = {255, 255, 255, 155};

                player.isHit--;
                // print iframe value
            } else {
                player.color = {255, 255, 255, 255};
            }
            if (shakesFrame > 0) {
                int shakeAmount = 10;
                camera.offset.x = GetRandomValue(-shakeAmount, shakeAmount);
                camera.offset.y = GetRandomValue(-shakeAmount, shakeAmount);
                shakesFrame--;
            } else {
                camera.offset.x = 0;
                camera.offset.y = 0;
            }
        } else if (MODE == PAUSED) {
            ClearBackground(RAYWHITE);
            prop.PLAYING();
            player.draw();
            for (int i = 0; i < 10; i++) {
                Obstacle[i]->draw();
            }
            prop.PAUSED();
            //  control.paused();
            if (CheckCollisionPointRec(GetMousePosition(), {720, 430, 420, 90})) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    frameTime = 0;
                    MODE = RESUME_DELAY;
                }
            }
            if (CheckCollisionPointRec(GetMousePosition(), {720, 580, 420, 90})) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    MODE = MAIN_MENU;
                }
            }

        } else if (MODE == GAME_OVER) {
            ClearBackground(RAYWHITE);
            if (frameTime - last_frameTime <= 180) {
                UpdateMusicStream(game_over_s);
            }
            prop.PLAYING();
            player.draw();
            for (int i = 0; i < 10; i++) {
                Obstacle[i]->draw();
            }
            prop.GAME_OVER();
            control.game_over();
        } else if (MODE == POP_MAINMENU) {
            prop.MAIN_MENU();
            prop.POP_MAINMENU();
            control.pop_mainmenu();
        } else if (MODE == RESUME_DELAY) {
            ClearBackground(RAYWHITE);
            prop.PLAYING();
            player.draw();
            coin.draw();
            for (int i = 0; i < 10; i++) {
                Obstacle[i]->draw();
            }
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4f));
            if (frameTime < 60) DrawText("3", 910, 490, 100, RED);
            if (frameTime >= 60 && frameTime < 120) DrawText("2", 910, 490, 100, RED);
            if (frameTime >= 120 && frameTime < 180) DrawText("1", 910, 490, 100, RED);

            if (frameTime > 180) MODE = PLAYING;
        }

        // Draw horizontal lines
        // for (int y = 0; y < GetScreenHeight(); y += 120) {
        //     DrawLine(0, y, GetScreenWidth(), y, BLACK);
        // }

        // // Draw vertical lines
        // for (int x = 0; x < GetScreenWidth(); x += 120) {
        //     DrawLine(x, 0, x, GetScreenHeight(), BLACK);
        // }

        frameTime++;
        // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        //     xpos[1] = xpos[0];
        //     ypos[1] = ypos[0];

        //     xpos[0] = GetMouseX();
        //     ypos[0] = GetMouseY();
        // }

        // DrawText(TextFormat("Mouse Position: [%i, %i]", xpos[0], ypos[0]), 500, 20, 40, BLACK);
        // DrawText(TextFormat("LENGTH prev Pos : [%i, %i]", abs(xpos[0] - xpos[1]), abs(ypos[0] - ypos[1])), 500, 100, 40, BLACK);
        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    prop.Unload();
    player.Unload();
    UnloadTexture(imgobst[0]);
    UnloadTexture(imgobst[1]);
    UnloadTexture(imgobst[2]);

    UnloadMusicStream(main_menu_s);
    UnloadMusicStream(playing_s);
    CloseAudioDevice();

    for (int i = 0; i < 10; i++) {
        Obstacle[i]->unload();
        delete Obstacle[i];
    }

    for (int i = 0; i < 10; i++) {
        SAVEME[static_cast<DATA>(OBST1 + i)] = {static_cast<DATA>(OBST1 + i), {static_cast<int>(Obstacle[i]->posisi.x), Obstacle[i]->type}};
    }
    SAVEME[VOLUME] = {VOLUME, {prop.volume}};
    SAVEME[MONEY] = {MONEY, {coin.collectedCoin}};
    SAVEME[LASTSCORE] = {LASTSCORE, {prop.Score}};
    SAVEME[SCORER] = {SCORER, {prop.Score_multiplier}};
    SAVEME[ISLEAVE] = {ISLEAVE, {isLeave}};
    SAVEME[HIGHSCORE] = {HIGHSCORE, {prop.HighScore}};
    SAVEME[PLAYER] = {PLAYER, {static_cast<int>(player.posisi.y), static_cast<int>(player.jumpCount), static_cast<int>(player.jumpSpeed), static_cast<int>(player.isSlide), static_cast<int>(player.playerFrame), player.lives, player.savedLives, player.isHit}};
    Save_Data(SAVEME);
    // saveGameData(Users_Save);

    return 0;
}
#include <fstream>
#include <iostream>
#include <vector>
#include "vector2.hh"
namespace Raylib {
    #include <raylib.h>
};

typedef unsigned char u8;
const int W = 800, H = 600;
const float
        FOV    = 90.0f * M_PI / 180.0f,
        MaxDst = 1000;
struct Line { Vector2 a, b; };

void Export(u8* Data, std::string FileName) {
    std::ofstream ofs(FileName + ".ppm");
    ofs << "P5\n" << W << " " << H << "\n" << "255\n";
    ofs.write((char*)Data, W * H);
    ofs.close();
}

float LineCol(Vector2 Origin, const Vector2& ray, Vector2 lineA, Vector2 lineB, float maxDst = 100) {
    float x1 = Origin.x, y1 = Origin.y;
    float x2 = ray.x * maxDst, y2 = ray.y * maxDst;
    float x3 = lineA.x, y3 = lineA.y;
    float x4 = lineB.x, y4 = lineB.y;

    float uA = ((x4-x3)*(y1-y3)-(y4-y3)*(x1-x3))/((y4-y3)*(x2-x1)-(x4-x3)*(y2-y1));
    float uB = ((x2-x1)*(y1-y3)-(y2-y1)*(x1-x3))/((y4-y3)*(x2-x1)-(x4-x3)*(y2-y1));
    if(uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
        return (Vector2){uA * (x2 - x1), uA * (y2 - y1)}.mag();
    return maxDst + 1;
}

void RenderScreen(const std::vector<Line>& Scene, Vector2 Origin) {
    for(int x =0; x < W; x++) {
        float Angle = FOV / W * (int)(x - W / 2);
        Vector2 Ray = Vector2::normal({ sinf(Angle), cosf(Angle) });

        float MinDst = MaxDst + 1;
        for(Line line : Scene) {
            float dst = LineCol(Origin, Ray, line.a, line.b, MaxDst);
            if(dst < MinDst) MinDst = dst;
        }
        
        if(MinDst > MaxDst) continue;
        ushort
            ActualDistance = H / (MinDst + 1),
            Middle = (H - ActualDistance) / 2;
        Raylib::DrawLine(x, Middle, x, Middle + ActualDistance, Raylib::WHITE);
    }
}

int main() {
    std::vector<Line> Scene = {
        {{-1, 5}, {1, 3}},
        {{-1, 5}, {-2, 3}},
    };
    float SpeedMultiplier = 2.56f;
    Vector2 Origin = {0, 0};
    Raylib::SetTraceLogLevel(Raylib::LOG_NONE);
    Raylib::InitWindow(W, H, "RayCaster");
    Raylib::SetTargetFPS(60);
    while(!Raylib::WindowShouldClose()) {
        Raylib::BeginDrawing();
        Raylib::ClearBackground(Raylib::BLACK);
        RenderScreen(Scene, Origin);
        Raylib::EndDrawing();
        if(Raylib::IsKeyDown(Raylib::KEY_W)) Origin.y += Raylib::GetFrameTime() * SpeedMultiplier;
        if(Raylib::IsKeyDown(Raylib::KEY_S)) Origin.y -= Raylib::GetFrameTime() * SpeedMultiplier;
        if(Raylib::IsKeyDown(Raylib::KEY_D)) Origin.x += Raylib::GetFrameTime() * SpeedMultiplier;
        if(Raylib::IsKeyDown(Raylib::KEY_A)) Origin.x -= Raylib::GetFrameTime() * SpeedMultiplier;
    }
}

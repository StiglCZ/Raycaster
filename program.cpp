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

std::vector<Vector2> points;
void DebugWindow(const std::vector<Line>& Scene, Vector2 origin, float RotationX) {
    for(Line line : Scene) {
        Raylib::DrawLine(line.a.x * 20 + 100,
                         line.a.y * 20 + 100,
                         line.b.x * 20 + 100,
                         line.b.y * 20 + 100, Raylib::WHITE);
    }

    for(Vector2 point : points) {
        float rad = 3;
        Vector2 middle = {point.x * 20 + 100, point.y * 20 + 100};
        Raylib::DrawLine(middle.x - rad, middle.y - rad, middle.x + rad, middle.y + rad, Raylib::RED);
        Raylib::DrawLine(middle.x - rad, middle.y + rad, middle.x + rad, middle.y - rad, Raylib::RED);
    }
    origin = origin * 20 + 100;
    Raylib::DrawCircle(origin.x, origin.y, 5, Raylib::BLUE);
    Vector2 Ray1 = Vector2::normal({ cosf(RotationX - FOV / 2), sinf(RotationX - FOV / 2) }) * 300;
    Vector2 Ray2 = Vector2::normal({ cosf(RotationX + FOV / 2), sinf(RotationX + FOV / 2) }) * 300;
    Raylib::DrawLine(origin.x, origin.y, origin.x + Ray1.x, origin.y - Ray1.y, Raylib::BLUE);
    Raylib::DrawLine(origin.x, origin.y, origin.x - Ray2.x, origin.y + Ray2.y, Raylib::BLUE);
    
    points.clear();
}

float LineCol(Vector2 Origin, Vector2 ray, Line line, float maxDst) {
    float x1 = Origin.x, y1 = Origin.y;
    float x2 = ray.x * maxDst, y2 = ray.y * maxDst;
    float x3 = line.a.x, y3 = line.a.y;
    float x4 = line.b.x, y4 = line.b.y;

    float uA = ((x4-x3)*(y1-y3)-(y4-y3)*(x1-x3))/((y4-y3)*(x2-x1)-(x4-x3)*(y2-y1));
    float uB = ((x2-x1)*(y1-y3)-(y2-y1)*(x1-x3))/((y4-y3)*(x2-x1)-(x4-x3)*(y2-y1));
    if(uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        points.push_back((Vector2){x1 + uA * (x2 - x1), y1 + uA * (y2 - y1)});
        return (Vector2){uA * (x2 - x1), uA * (y2 - y1)}.mag();
    }
    return maxDst + 1;
}

void RenderScreen(const std::vector<Line>& Scene, Vector2 Origin, float RotationX) {
    for(int x =0; x < W; x++) {
        float Angle = FOV / W * (int)(x - W / 2);
        Vector2 Ray = Vector2::normal({ sinf(Angle + RotationX), cosf(Angle + RotationX) });

        float MinDst = MaxDst + 1;
        for(Line line : Scene) {
            float dst = LineCol(Origin, Ray, line, MaxDst);
            dst = dst * cosf(Angle);
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
        {{-1, 3}, {1, 3}},
        {{-1, -3}, {1, -3}},
        {{-1, 5}, {-2, 3}},
    };
    float SpeedMultiplier = 2.56f;
    Vector2 Origin = {0, 0};
    float RotationX = 0.00f;
    Raylib::SetTraceLogLevel(Raylib::LOG_NONE);
    Raylib::InitWindow(W, H, "RayCaster");
    Raylib::SetTargetFPS(60);
    while(!Raylib::WindowShouldClose()) {
        Raylib::BeginDrawing();
        Raylib::ClearBackground(Raylib::BLACK);
        DebugWindow(Scene, Origin, RotationX);
        RenderScreen(Scene, Origin, RotationX);
        Raylib::EndDrawing();
        if(Raylib::IsKeyDown(Raylib::KEY_W))
            Origin += (Vector2){sinf(RotationX), cosf(RotationX)} * (Raylib::GetFrameTime() * SpeedMultiplier);
        if(Raylib::IsKeyDown(Raylib::KEY_S))
            Origin += (Vector2){-sinf(RotationX), -cosf(RotationX)} * (Raylib::GetFrameTime() * SpeedMultiplier);
        if(Raylib::IsKeyDown(Raylib::KEY_D))
            Origin += (Vector2){cosf(RotationX), -sinf(RotationX)} * (Raylib::GetFrameTime() * SpeedMultiplier);
        if(Raylib::IsKeyDown(Raylib::KEY_A))
            Origin += (Vector2){-cosf(RotationX), sinf(RotationX)} * (Raylib::GetFrameTime() * SpeedMultiplier);
        if(Raylib::IsKeyDown(Raylib::KEY_SPACE)) RotationX -= Raylib::GetFrameTime() * SpeedMultiplier;
    }
}

#include <fstream>
#include <iostream>
#include <vector>
#include "vector2.hh"

typedef unsigned char u8;
const int W = 1440, H = 900;
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

float LineCol(const Vector2& ray, Vector2 lineA, Vector2 lineB, float maxDst = 100) {
    float x1 = 0, y1 = 0;
    float x2 = ray.x * maxDst, y2 = ray.y * maxDst;
    float x3 = lineA.x, y3 = lineA.y;
    float x4 = lineB.x, y4 = lineB.y;

    float uA = ((x4-x3)*(y1-y3)-(y4-y3)*(x1-x3))/((y4-y3)*(x2-x1)-(x4-x3)*(y2-y1));
    float uB = ((x2-x1)*(y1-y3)-(y2-y1)*(x1-x3))/((y4-y3)*(x2-x1)-(x4-x3)*(y2-y1));
    if(uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        float mag = (Vector2){uA * (x2 - x1), uA * (y2 - y1)}.mag();
        return mag;
    }
    return maxDst + 1;
}

int main() {
    u8* Output = new u8[W * H];
    std::vector<Line> Scene = {
        {{-1, 5}, {1, 3}},
        {{-1, 5}, {-2, 3}},
    };
    
    for(int i =0; i < W; i++) {
        float Angle = FOV / W * (int)(i - W / 2);
        Vector2 Ray = Vector2::normal({ sinf(Angle), cosf(Angle) });

        float MinDst = MaxDst + 1;
        for(Line line : Scene) {
            float dst = LineCol(Ray, line.a, line.b, MaxDst);
            if(dst < MinDst) MinDst = dst;
        }
        
        if(MinDst > MaxDst) continue;
        ushort
            ActualDistance = H / (MinDst + 1),
            Middle = (H - ActualDistance) / 2;
        for(int y = Middle; y < ActualDistance + Middle; y++)
            Output[W * y + i] = 255;
    }
    Export(Output, "image");
    delete[] Output;
}

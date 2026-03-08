#ifndef WORLD_H
#define WORLD_H

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <vector>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float SAFE_ZONE_RADIUS = 15.0f; 
const float MOUNTAIN_RADIUS = 65.0f; // Made mountains slightly wider
const float MAP_LIMIT = 480.0f; 

struct Tree {
    Vector3 position;
    bool isWisteria;
};

inline std::vector<Tree> forest;
inline std::vector<Vector3> mountains;

void InitWorld() {
    forest.clear();
    mountains.clear();

    // 1. Create fewer, larger mountains first
    for (int i = 0; i < 8; i++) {
        mountains.push_back((Vector3){
            (float)GetRandomValue(-420, 420), 
            -2.0f, 
            (float)GetRandomValue(-420, 420)
        });
    }
    
    // 2. Create a dense forest (250 trees)
    for (int i = 0; i < 250; i++) {
        Vector3 pos = { (float)GetRandomValue(-470, 470), 0, (float)GetRandomValue(-470, 470) };
        
        // Check if tree is inside a mountain; if so, skip or reposition
        bool insideMountain = false;
        for (auto& m : mountains) {
            if (Vector2Distance({pos.x, pos.z}, {m.x, m.z}) < MOUNTAIN_RADIUS + 5.0f) {
                insideMountain = true;
                break;
            }
        }

        if (!insideMountain) {
            forest.push_back({pos, (GetRandomValue(0, 10) > 8)}); // 20% are Wisteria
        }
    }
}

void DrawFencing() {
    Color fenceColor = (Color){ 35, 25, 15, 255 };
    for (float i = -MAP_LIMIT; i <= MAP_LIMIT; i += 12.0f) {
        DrawCube((Vector3){ i, 1.5f, MAP_LIMIT }, 0.5f, 3.0f, 0.5f, fenceColor);
        DrawCube((Vector3){ i, 1.5f, -MAP_LIMIT }, 0.5f, 3.0f, 0.5f, fenceColor);
        DrawCube((Vector3){ MAP_LIMIT, 1.5f, i }, 0.5f, 3.0f, 0.5f, fenceColor);
        DrawCube((Vector3){ -MAP_LIMIT, 1.5f, i }, 0.5f, 3.0f, 0.5f, fenceColor);
    }
}

void DrawEnvironment(float time) {
    DrawSphere((Vector3){ -100, 80, -180 }, 15.0f, (Color){ 255, 255, 230, 255 });
    DrawFencing();
    
    // Draw Mountains (Darker and taller)
    for (auto& m : mountains) 
        DrawCylinderEx(m, Vector3Add(m, (Vector3){0, 250, 0}), MOUNTAIN_RADIUS, MOUNTAIN_RADIUS * 0.3f, 12, (Color){8, 8, 15, 255});

    // Draw Dense Forest
    for (auto &t : forest) {
        DrawCylinderEx(t.position, Vector3Add(t.position, (Vector3){0, 10, 0}), 0.5f, 0.4f, 6, DARKBROWN);
        if (t.isWisteria) {
            float pulse = sinf(time * 2.0f) * 0.15f + 1.0f;
            DrawSphereEx(Vector3Add(t.position, (Vector3){0, 12, 0}), 6.0f * pulse, 8, 8, (Color){ 160, 80, 255, 160 });
            DrawCircle3D(t.position, SAFE_ZONE_RADIUS, (Vector3){1, 0, 0}, 90.0f, Fade(PURPLE, 0.2f));
            DrawSphereWires(t.position, SAFE_ZONE_RADIUS, 10, 10, Fade(VIOLET, 0.1f * pulse));
        } else {
            DrawCylinderEx(Vector3Add(t.position, (Vector3){0, 6, 0}), Vector3Add(t.position, (Vector3){0, 18, 0}), 5.0f, 0.0f, 5, (Color){10, 30, 10, 255});
        }
    }
}
#endif
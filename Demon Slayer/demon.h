#ifndef DEMON_H
#define DEMON_H

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

struct Demon {
    Vector3 position;
    float health;
    bool active;
    float seed;
    float attackTimer; 
    bool isAttacking;  
};

void DrawScaryDemon(Demon d, Vector3 playerPos, float time) {
    float angle = atan2f(playerPos.x - d.position.x, playerPos.z - d.position.z) * RAD2DEG;
    float twitch = sinf(time * 35.0f + d.seed) * 0.03f;
    float legSwing = sinf(time * 10.0f + d.seed) * 35.0f;
    
    float swingArc = 0.0f;
    if (d.isAttacking) {
        float progress = fmaxf(0.0f, (0.5f - d.attackTimer) / 0.5f);
        swingArc = (progress * 180.0f) - 90.0f; 
    }

    rlPushMatrix();
        rlTranslatef(d.position.x + twitch, d.position.y, d.position.z + twitch);
        rlScalef(1.4f, 1.4f, 1.4f);
        rlRotatef(angle, 0, 1, 0);

        DrawCube((Vector3){ 0, 1.2f, -0.2f }, 0.6f, 1.4f, 0.5f, (Color){ 30, 30, 35, 255 });
        DrawCube((Vector3){ 0, 2.2f, 0.1f }, 0.5f, 0.5f, 0.5f, (Color){ 20, 20, 25, 255 });
        DrawSphere((Vector3){ 0.15f, 2.3f, 0.35f }, 0.08f, RED);
        DrawSphere((Vector3){ -0.15f, 2.3f, 0.35f }, 0.08f, RED);

        rlPushMatrix(); rlTranslatef(-0.25f, 0.6f, -0.1f); rlRotatef(legSwing, 1, 0, 0); DrawCube((Vector3){ 0, -0.4f, 0 }, 0.25f, 0.9f, 0.25f, (Color){ 15, 15, 20, 255 }); rlPopMatrix();
        rlPushMatrix(); rlTranslatef(0.25f, 0.6f, -0.1f); rlRotatef(-legSwing, 1, 0, 0); DrawCube((Vector3){ 0, -0.4f, 0 }, 0.25f, 0.9f, 0.25f, (Color){ 15, 15, 20, 255 }); rlPopMatrix();

        rlPushMatrix();
            rlTranslatef(0.55f, 1.7f, 0);
            rlRotatef(swingArc, 0, 1, 0); 
            rlRotatef(15, 1, 0, 0);       
            DrawCube((Vector3){ 0, -0.5f, 0 }, 0.2f, 1.2f, 0.2f, (Color){ 25, 25, 30, 255 });
            rlPushMatrix();
                rlTranslatef(0.0f, -1.1f, 0.5f);
                rlRotatef(-75, 1, 0, 0);
                DrawCube((Vector3){ 0, 0, 0.6f }, 0.1f, 0.2f, 1.4f, (Color){ 160, 0, 0, 255 }); 
                DrawCubeWires((Vector3){ 0, 0, 0.6f }, 0.11f, 0.21f, 1.41f, RED); 
            rlPopMatrix();
        rlPopMatrix();
    rlPopMatrix();
}
#endif
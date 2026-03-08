#ifndef SLAYER_H
#define SLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <deque>

inline std::deque<Vector3> trailPositions;

void DrawSlayer(Vector3 pos, Vector3 dir, bool isMoving, bool isDead, float time) {
    float rotation = atan2f(dir.x, dir.z) * RAD2DEG;
    float limbSwing = (isMoving && !isDead) ? sinf(time * 12.0f) * 35.0f : 0;
    
    // 1. Draw Haori (Cape Trail) - Only if alive
    if (!isDead) {
        for (int i = 0; i < (int)trailPositions.size(); i++) {
            float scale = (float)(15 - i) / 15;
            DrawCubeV(trailPositions[i], (Vector3){ 1.2f * scale, 1.8f * scale, 0.15f }, Fade(RAYWHITE, scale * 0.4f));
        }
    }

    // 2. Draw Body
    rlPushMatrix();
        rlTranslatef(pos.x, pos.y, pos.z);
        rlScalef(1.5f, 1.5f, 1.5f); 
        
        // --- DEATH ANIMATION ---
        if (isDead) {
            rlRotatef(90, 1, 0, 0); // Fall flat
            rlTranslatef(0, -0.5f, 0); 
        } else {
            rlRotatef(rotation, 0, 1, 0);
        }

        Color bodyColor = isDead ? GRAY : DARKGRAY;
        Color skinColor = isDead ? LIGHTGRAY : BEIGE;
        Color limbColor = isDead ? DARKGRAY : BLACK;

        // Torso
        DrawCube((Vector3){ 0, 1.2f, 0 }, 0.8f, 1.0f, 0.4f, bodyColor); 
        // Head
        DrawCube((Vector3){ 0, 2.0f, 0 }, 0.4f, 0.4f, 0.4f, skinColor);
        DrawCube((Vector3){ 0, 2.15f, 0.05f }, 0.45f, 0.25f, 0.5f, BLACK); 

        // --- ARMS (RESTORED) ---
        // Left Arm
        rlPushMatrix();
            rlTranslatef(-0.55f, 1.6f, 0);
            rlRotatef(-limbSwing, 1, 0, 0); // Inverse swing for natural walk
            DrawCube((Vector3){ 0, -0.3f, 0 }, 0.25f, 0.6f, 0.25f, bodyColor);
        rlPopMatrix();

        // Right Arm
        rlPushMatrix();
            rlTranslatef(0.55f, 1.6f, 0);
            rlRotatef(limbSwing, 1, 0, 0);
            DrawCube((Vector3){ 0, -0.3f, 0 }, 0.25f, 0.6f, 0.25f, bodyColor);
        rlPopMatrix();

        // --- LEGS ---
        rlPushMatrix();
            rlTranslatef(-0.25f, 0.7f, 0);
            rlRotatef(limbSwing, 1, 0, 0);
            DrawCube((Vector3){ 0, -0.35f, 0 }, 0.4f, 0.7f, 0.4f, limbColor);
        rlPopMatrix();
        
        rlPushMatrix();
            rlTranslatef(0.25f, 0.7f, 0);
            rlRotatef(-limbSwing, 1, 0, 0);
            DrawCube((Vector3){ 0, -0.35f, 0 }, 0.4f, 0.7f, 0.4f, limbColor);
        rlPopMatrix();
    rlPopMatrix();
}

void DrawSword(Vector3 pos, Vector3 dir, bool isAttacking, bool isDead, float attackTimer) {
    if (isDead) return; 
    float rotation = atan2f(dir.x, dir.z) * RAD2DEG;
    float swing = isAttacking ? ((0.25f - attackTimer) / 0.25f * 160.0f) - 80.0f : 0;
    
    rlPushMatrix();
        rlTranslatef(pos.x, pos.y + 1.8f, pos.z); 
        rlScalef(1.5f, 1.5f, 1.5f);
        rlRotatef(rotation + swing, 0, 1, 0); 
        rlTranslatef(0.6f, 0, 0.2f); 
        
        // Handle
        DrawCube((Vector3){ 0, 0, 0 }, 0.1f, 0.1f, 0.6f, MAROON);
        // Guard
        DrawCube((Vector3){ 0, 0, 0.35f }, 0.25f, 0.25f, 0.05f, GOLD);
        // Blade
        DrawCube((Vector3){ 0, 0, 1.2f }, 0.08f, 0.15f, 1.6f, SKYBLUE);
    rlPopMatrix();
}
#endif
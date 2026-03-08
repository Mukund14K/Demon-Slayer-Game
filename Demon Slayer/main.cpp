#include "raylib.h"
#include "raymath.h"
#include "world.h"
#include "demon.h"
#include "slayer.h"
#include <vector>

typedef enum { STATE_PLAYING, STATE_GAMEOVER } GameState;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Selection: Clean HUD Edition");
    InitWorld();

    Camera3D camera = { 0 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 65.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 playerPos = { 0, 0, 0 }, playerDir = { 0, 0, 1 };
    float health = 100.0f, stamina = 100.0f, cameraDistance = 22.0f;
    float cameraAngleX = 0.0f, cameraAngleY = 25.0f;
    int score = 0;
    bool isAttacking = false, isMoving = false;
    float attackTimer = 0.0f, spawnTimer = 0.0f;
    GameState currentState = STATE_PLAYING;

    std::vector<Demon> demons;
    DisableCursor(); SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime(), time = (float)GetTime();

        if (currentState == STATE_PLAYING) {
            // --- 1. MOVEMENT & STAMINA ---
            float speed = (IsKeyDown(KEY_LEFT_SHIFT) && stamina > 5) ? 19.0f : 10.0f;
            Vector3 forward = { sinf(cameraAngleX * DEG2RAD), 0, cosf(cameraAngleX * DEG2RAD) };
            Vector3 right = { sinf((cameraAngleX - 90) * DEG2RAD), 0, cosf((cameraAngleX - 90) * DEG2RAD) };
            Vector3 moveVec = { 0 };

            if (IsKeyDown(KEY_W)) moveVec = Vector3Subtract(moveVec, forward); 
            if (IsKeyDown(KEY_S)) moveVec = Vector3Add(moveVec, forward);
            if (IsKeyDown(KEY_A)) moveVec = Vector3Add(moveVec, right); 
            if (IsKeyDown(KEY_D)) moveVec = Vector3Subtract(moveVec, right);

            isMoving = (Vector3Length(moveVec) > 0);
            if (isMoving) {
                playerDir = Vector3Normalize(moveVec);
                playerPos = Vector3Add(playerPos, Vector3Scale(playerDir, speed * dt));
                if (speed > 10.0f) stamina -= 40.0f * dt; 
                else stamina = fminf(100, stamina + 60.0f * dt);
            } else stamina = fminf(100, stamina + 95.0f * dt);

            // --- 2. COLLISIONS ---
            playerPos.x = Clamp(playerPos.x, -MAP_LIMIT + 2.0f, MAP_LIMIT - 2.0f);
            playerPos.z = Clamp(playerPos.z, -MAP_LIMIT + 2.0f, MAP_LIMIT - 2.0f);

            for (auto &m : mountains) {
                float dx = playerPos.x - m.x;
                float dz = playerPos.z - m.z;
                float distSq = (dx * dx) + (dz * dz);
                float minD = MOUNTAIN_RADIUS + 2.0f; 
                if (distSq < (minD * minD)) {
                    float dist = sqrtf(distSq);
                    playerPos.x = m.x + (dx / dist) * minD;
                    playerPos.z = m.z + (dz / dist) * minD;
                }
            }

            // --- 3. CAMERA ---
            Vector2 mouseDelta = GetMouseDelta();
            cameraAngleX -= mouseDelta.x * 0.15f; 
            cameraAngleY = Clamp(cameraAngleY + mouseDelta.y * 0.15f, 5.0f, 80.0f);
            Vector3 camOff = { 
                sinf(cameraAngleX * DEG2RAD) * cosf(cameraAngleY * DEG2RAD) * cameraDistance, 
                sinf(cameraAngleY * DEG2RAD) * cameraDistance, 
                cosf(cameraAngleX * DEG2RAD) * cosf(cameraAngleY * DEG2RAD) * cameraDistance 
            };
            camera.position = Vector3Lerp(camera.position, Vector3Add(playerPos, camOff), 0.2f);
            camera.target = Vector3Lerp(camera.target, Vector3Add(playerPos, (Vector3){0, 2.5f, 0}), 0.2f);

            // --- 4. WISTERIA HEALING ---
            for (auto &t : forest) 
                if (t.isWisteria && Vector3Distance(playerPos, t.position) < SAFE_ZONE_RADIUS) 
                    health = fminf(100, health + 20.0f * dt);

            // --- 5. PLAYER ATTACK ---
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !isAttacking && stamina >= 12) {
                isAttacking = true; attackTimer = 0.5f; stamina -= 12;
                for (auto &d : demons) 
                    if (d.active && CheckCollisionSpheres(playerPos, 8.0f, d.position, 1.5f)) { 
                        d.health -= 35; 
                        if (d.health <= 0) { d.active = false; score++; } 
                    }
            }
            if (isAttacking) { attackTimer -= dt; if (attackTimer <= 0) isAttacking = false; }

            // --- 6. BALANCED SPAWNING ---
            spawnTimer += dt;
            float spawnRate = fmaxf(0.3f, 0.7f - (score * 0.003f)); 
            if (spawnTimer >= spawnRate && demons.size() < 45) { 
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float dist = (float)GetRandomValue(45, 65); 
                Vector3 sPos = { playerPos.x + cosf(angle) * dist, 0, playerPos.z + sinf(angle) * dist };
                sPos.x = Clamp(sPos.x, -MAP_LIMIT + 10, MAP_LIMIT - 10);
                sPos.z = Clamp(sPos.z, -MAP_LIMIT + 10, MAP_LIMIT - 10);
                demons.push_back({ sPos, 50.0f, true, (float)GetRandomValue(0,100), 0.0f, false }); 
                spawnTimer = 0; 
            }

            // --- 7. DEMON AI ---
            for (auto &d : demons) {
                if (!d.active) continue;
                bool pushed = false;
                for (auto &t : forest) if (t.isWisteria && Vector3Distance(d.position, t.position) < SAFE_ZONE_RADIUS) { d.position = Vector3Add(t.position, Vector3Scale(Vector3Normalize(Vector3Subtract(d.position, t.position)), SAFE_ZONE_RADIUS + 0.2f)); pushed = true; }
                if (!pushed) {
                    float dist = Vector3Distance(playerPos, d.position);
                    if (d.isAttacking) { 
                        d.attackTimer -= dt; 
                        d.position = Vector3Add(d.position, Vector3Scale(Vector3Normalize(Vector3Subtract(playerPos, d.position)), 12.0f * dt));
                        if (d.attackTimer <= 0) d.isAttacking = false; 
                        if (d.attackTimer < 0.25f && d.attackTimer > 0.20f && dist < 6.5f) health -= 45.0f * dt; 
                    } else { 
                        d.position = Vector3Add(d.position, Vector3Scale(Vector3Normalize(Vector3Subtract(playerPos, d.position)), (5.0f + (score * 0.01f)) * dt)); 
                        if (dist < 8.0f) { d.isAttacking = true; d.attackTimer = 0.5f; } 
                    }
                }
            }
            if (health <= 0.1f) { health = 0; currentState = STATE_GAMEOVER; EnableCursor(); }
        } else if (IsKeyPressed(KEY_R)) { health = 100; score = 0; playerPos = (Vector3){0,0,0}; demons.clear(); currentState = STATE_PLAYING; DisableCursor(); }

       // --- 8. RENDER ---
        BeginDrawing();
            ClearBackground((Color){ 2, 2, 8, 255 });
            BeginMode3D(camera);
                DrawPlane((Vector3){0,-0.01f,0}, (Vector2){1000, 1000}, (Color){10,15,10,255});
                DrawEnvironment(time);
                DrawSlayer(playerPos, playerDir, isMoving, (currentState == STATE_GAMEOVER), time);
                DrawSword(playerPos, playerDir, isAttacking, (currentState == STATE_GAMEOVER), attackTimer);
                for (auto &d : demons) if (d.active) DrawScaryDemon(d, playerPos, time);
            EndMode3D();

            // --- HUD SECTION WITH FLASHING ALERT ---
            
            // Determine Health Bar Color
            Color healthBarColor = MAROON;
            if (health <= 20.0f) {
                // Flash between Maroon and Red every 0.2 seconds
                float flash = sinf(time * 15.0f); 
                if (flash > 0) healthBarColor = RED;
            }

            // Vitality Bar (Health)
            DrawRectangle(30, 30, 300, 25, Fade(BLACK, 0.6f)); // Shadow
            DrawRectangle(30, 30, (int)fmaxf(0, health * 3), 25, healthBarColor);
            DrawText("VITALITY", 40, 35, 18, WHITE);

            // Breath Bar (Stamina)
            DrawRectangle(30, 70, 300, 15, Fade(BLACK, 0.6f)); // Shadow
            DrawRectangle(30, 70, (int)stamina * 3, 15, SKYBLUE);
            DrawText("BREATH", 40, 71, 12, WHITE);

            // Score
            DrawText(TextFormat("PURIFIED: %i", score), 30, 100, 24, GOLD);

            // --- GAME OVER ---
            if (currentState == STATE_GAMEOVER) { 
                DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,Fade(BLACK,0.8f)); 
                DrawText("YOU DIED", SCREEN_WIDTH/2-180, SCREEN_HEIGHT/2 - 20, 25, RED); 
                DrawText("Press 'R' to Restart", SCREEN_WIDTH/2-110, SCREEN_HEIGHT/2 + 20, 20, WHITE); 
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
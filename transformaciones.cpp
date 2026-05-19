// transformaciones.cpp
// Aplicacion Raylib - Transformaciones Matematicas 2D
// HCI - Actividad de Transformaciones
// Autor: Juan Pablo Duran Reyes 

#include "raylib.h"
#include <cmath>
#include <cstdio>

// ─── Constantes ────────────────────────────────────────────────────────────
const int SCREEN_W = 1100;
const int SCREEN_H = 700;


// ─── Estado de cada transformacion ─────────────────────────────────────────

// 1. TRASLACION
struct Traslacion {
    float x = 80.0f, y = 100.0f;   // posicion actual
    float vx = 120.0f, vy = 60.0f; // velocidad (px/s)
};

// 2. ROTACION
struct Rotacion {
    float cx = 200.0f, cy = 430.0f;  // centro
    float angulo = 0.0f;             // angulo acumulado (rad)
    float radio = 40.0f;
    float omega = 1.5f;              // velocidad angular (rad/s)
};

// 3. REBOTE (Bounce)
struct Rebote {
    float x = 700.0f, y = 100.0f;
    float vx = 90.0f, vy = 80.0f;
    float radio = 18.0f;
    // Limites del panel derecho
    float xMin = 560.0f, xMax = 1080.0f;
    float yMin = 30.0f,  yMax = 340.0f;
};

// 4. MOVIMIENTO SENOIDAL
struct Senoidal {
    float t = 0.0f;           // tiempo acumulado
    float amplitud = 70.0f;
    float frecuencia = 1.8f;  // rad/s
    float velocidadX = 80.0f; // px/s
    float x0 = 560.0f;        // origen X
    float yBase = 430.0f;     // linea base Y
    float x = 560.0f;
};

// 5. TRAYECTORIA (proyectil parabolico)
struct Trayectoria {
    float t = 0.0f;
    float x0 = 570.0f, y0 = 630.0f;
    float v0x = 100.0f, v0y = -220.0f; // px/s
    float g = 300.0f;                  // gravedad px/s^2
    float x = 570.0f, y = 630.0f;
    bool activo = true;
};

// 6. ORBITA
struct Orbita {
    float cx = 280.0f, cy = 290.0f; // centro de orbita
    float a = 90.0f, b = 50.0f;    // semiejes
    float theta = 0.0f;             // angulo
    float omega = 0.8f;             // velocidad angular (rad/s)
    // Satelite
    float px = 0.0f, py = 0.0f;
    float omegaS = 3.0f;
    float thetaS = 0.0f;
    float rs = 20.0f;
};

// ─── Funcion auxiliar: dibuja panel con titulo ──────────────────────────────
void DrawPanel(int x, int y, int w, int h, const char* titulo, Color col) {
    DrawRectangle(x, y, w, h, Fade(col, 0.12f));
    DrawRectangleLinesEx({(float)x,(float)y,(float)w,(float)h}, 2, col);
    DrawText(titulo, x+8, y+6, 16, col);
}

// ─── Dibuja una ecuacion (texto) centrado ────────────────────────────────────
void DrawEq(const char* eq, int x, int y, Color col) {
    DrawText(eq, x, y, 13, col);
}

// ───────────────────────────────────────────────────────────────────────────
int main() {
    InitWindow(SCREEN_W, SCREEN_H, "Transformaciones Matematicas 2D - HCI");
    SetTargetFPS(60);

    Traslacion  tras;
    Rotacion    rot;
    Rebote      reb;
    Senoidal    sen;
    Trayectoria tray;
    Orbita      orb;

    // Historial de trayectoria
    const int MAXPTS = 300;
    Vector2 trayPts[MAXPTS];
    int trayCount = 0;

    // Historial senoidal
    Vector2 senPts[MAXPTS];
    int senCount = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f; // clamp

        // ── 1. TRASLACION  P' = P + v*dt ──────────────────────────────────
        tras.x += tras.vx * dt;
        tras.y += tras.vy * dt;
        // Rebote en el panel
        if (tras.x < 10 || tras.x > 520) tras.vx = -tras.vx;
        if (tras.y < 30 || tras.y > 330) tras.vy = -tras.vy;

        // ── 2. ROTACION  x' = cx + r*cos(theta), y' = cy + r*sin(theta) ──
        rot.angulo += rot.omega * dt;
        float rotX = rot.cx + rot.radio * cosf(rot.angulo);
        float rotY = rot.cy + rot.radio * sinf(rot.angulo);

        // ── 3. REBOTE  reflexion de velocidad ─────────────────────────────
        reb.x += reb.vx * dt;
        reb.y += reb.vy * dt;
        if (reb.x - reb.radio < reb.xMin) { reb.x = reb.xMin + reb.radio; reb.vx = -reb.vx; }
        if (reb.x + reb.radio > reb.xMax) { reb.x = reb.xMax - reb.radio; reb.vx = -reb.vx; }
        if (reb.y - reb.radio < reb.yMin) { reb.y = reb.yMin + reb.radio; reb.vy = -reb.vy; }
        if (reb.y + reb.radio > reb.yMax) { reb.y = reb.yMax - reb.radio; reb.vy = -reb.vy; }

        // ── 4. MOVIMIENTO SENOIDAL  y(t) = A*sin(wt) ─────────────────────
        sen.t  += dt;
        sen.x  += sen.velocidadX * dt;
        if (sen.x > 1080.0f) { sen.x = sen.x0; senCount = 0; }
        float senY = sen.yBase + sen.amplitud * sinf(sen.frecuencia * sen.t);
        // guarda historial
        if (senCount < MAXPTS) {
            senPts[senCount++] = { sen.x, senY };
        } else {
            for (int i = 0; i < MAXPTS-1; i++) senPts[i] = senPts[i+1];
            senPts[MAXPTS-1] = { sen.x, senY };
        }

        // ── 5. TRAYECTORIA  x(t)=x0+vx*t  y(t)=y0+vy*t+0.5*g*t^2 ──────
        if (tray.activo) {
            tray.t += dt;
            tray.x = tray.x0 + tray.v0x * tray.t;
            tray.y = tray.y0 + tray.v0y * tray.t + 0.5f * tray.g * tray.t * tray.t;
            if (trayCount < MAXPTS)
                trayPts[trayCount++] = { tray.x, tray.y };
            if (tray.y > 660 || tray.x > 1090) {
                tray.t = 0; tray.x = tray.x0; tray.y = tray.y0;
                trayCount = 0;
            }
        }

        // ── 6. ORBITA  elipse  x=cx+a*cos(th)  y=cy+b*sin(th) ──────────
        orb.theta  += orb.omega  * dt;
        orb.thetaS += orb.omegaS * dt;
        float orbX  = orb.cx + orb.a * cosf(orb.theta);
        float orbY  = orb.cy + orb.b * sinf(orb.theta);
        // satelite gira alrededor del planeta
        orb.px = orbX + orb.rs * cosf(orb.thetaS);
        orb.py = orbY + orb.rs * sinf(orb.thetaS);

        // ── DIBUJADO ──────────────────────────────────────────────────────
        BeginDrawing();
        ClearBackground({ 18, 18, 30, 255 });

        // ─ Titulo principal ─
        DrawText("Transformaciones Matematicas 2D", 300, 5, 22, WHITE);

        // ─ Panel 1: TRASLACION (izq superior) ─
        DrawPanel(5, 25, 535, 320, "1. TRASLACION", GREEN);
        DrawEq("P'(t) = P0 + v * dt", 14, 46, LIME);
        DrawCircle((int)tras.x, (int)tras.y, 14, GREEN);
        // rastro
        DrawText("(x,y)", (int)tras.x+16, (int)tras.y-8, 12, LIME);

        // ─ Panel 2: ROTACION (izq inferior) ─
        DrawPanel(5, 355, 535, 200, "2. ROTACION", ORANGE);
        DrawEq("x' = cx + r*cos(ang)   y' = cy + r*sin(ang)", 14, 376, ORANGE);
        DrawCircle((int)rot.cx, (int)rot.cy, 5, YELLOW);
        DrawLine((int)rot.cx,(int)rot.cy,(int)rotX,(int)rotY, ORANGE);
        DrawCircle((int)rotX, (int)rotY, 12, ORANGE);
        // angulo texto
        char bufAng[40];
        snprintf(bufAng, 40, "ang=%.2f rad", rot.angulo);
        DrawText(bufAng, (int)rot.cx-30, (int)rot.cy+15, 12, YELLOW);

        // ─ Panel 3: REBOTE (der superior) ─
        DrawPanel(555, 25, 535, 320, "3. REBOTE", RED);
        DrawEq("if(x<xMin||x>xMax) vx=-vx", 564, 46, RED);
        DrawCircle((int)reb.x, (int)reb.y, (int)reb.radio, RED);

        // ─ Panel 4: SENOIDAL (der inferior arriba) ─
        DrawPanel(555, 355, 535, 170, "4. MOVIMIENTO SENOIDAL", SKYBLUE);
        DrawEq("y(t) = yBase + A*sin(w*t)", 564, 376, SKYBLUE);
        for (int i = 1; i < senCount; i++)
            DrawLineV(senPts[i-1], senPts[i], SKYBLUE);
        DrawCircle((int)sen.x, (int)senY, 10, WHITE);

        // ─ Panel 5: TRAYECTORIA (izq muy inferior) ─
        DrawPanel(5, 560, 535, 130, "5. TRAYECTORIA (Proyectil)", PURPLE);
        DrawEq("x(t)=x0+vx*t   y(t)=y0+vy*t+0.5g*t^2", 14, 580, VIOLET);
        for (int i = 1; i < trayCount; i++)
            DrawLineV(trayPts[i-1], trayPts[i], PURPLE);
        DrawCircle((int)tray.x, (int)tray.y, 10, PURPLE);

        // ─ Panel 6: ORBITA (der muy inferior) ─
        DrawPanel(555, 530, 535, 160, "6. ORBITA", GOLD);
        DrawEq("x=cx+a*cos(th)  y=cy+b*sin(th)", 564, 550, GOLD);
        // dibuja elipse (aproximacion con lineas)
        for (int i = 0; i < 360; i++) {
            float a1 = i * PI / 180.0f;
            float a2 = (i+1) * PI / 180.0f;
            DrawLine(
                (int)(orb.cx + orb.a * cosf(a1)),
                (int)(orb.cy + orb.b * sinf(a1)),
                (int)(orb.cx + orb.a * cosf(a2)),
                (int)(orb.cy + orb.b * sinf(a2)),
                Fade(GOLD, 0.4f)
            );
        }
        // Sol
        DrawCircle((int)orb.cx, (int)orb.cy, 14, YELLOW);
        // Planeta
        DrawCircle((int)orbX, (int)orbY, 10, BLUE);
        // Satelite
        DrawCircle((int)orb.px, (int)orb.py, 5, GRAY);
        DrawLine((int)orbX,(int)orbY,(int)orb.px,(int)orb.py, Fade(GRAY,0.6f));

        // FPS
        DrawFPS(10, SCREEN_H - 22);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

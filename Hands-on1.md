# Hands-on 1 : System simulation through prediction
Interaccion Humano Computadora
A partir del Modelo Matemático Vectorizado/Matricial, construido en clase, Least Squared Regressor, para simular sistemas a través de Análisis Predictivo:
- Realizar una abstracción orientada a objetos  del modelo..
- Implementar el modelo, orientado a objetos, a través de C++.
- Dado el  Data Set “Least Squared Regressor”, calcular los valores óptimos para los parámetros de un Modelo de Regresión Lineal Múltiple: B_0, B_1 y B_2 (ver sección Data Sets en Google Classroom).
- Compruebe que los valores óptimos, calculadaos para los parámetros, coinciden con los proporcionados en la sección de Datasets. 

<img width="439" height="445" alt="image" src="https://github.com/user-attachments/assets/ea4b9bb4-c1f1-496a-98c9-b31f0f8a657c" />


# ORIENTADO A OBJETOS

|Experiment|
| :---: |
| x1 |
| x2 |
| y |


| LeastSquaredRegressor |
| :---: |
| dataset |  
| B0 |
| B1 | 
| B3 |
| loadData()
calculateBetas()
predict()
printEquation() |

## CODIGO

```
class Experiment{
public:
    double x1;
    double x2;
    double y;
    
    Experiment(double _x1, double _x2, double _y) {
        x1 = _x1;
        x2 = _x2;
        y = _y;
    }
};
```

# CAPTURAS DE PANTALLA DE LA EJECUCION

<img width="1284" height="785" alt="Screenshot 2026-05-18 212054" src="https://github.com/user-attachments/assets/057f09c4-c592-4407-83be-1f3f20e6dd3a" />
<img width="1286" height="787" alt="Screenshot 2026-05-18 212109" src="https://github.com/user-attachments/assets/7302055a-74e6-4c7b-98c8-0c774eaa3ffa" />
<img width="1281" height="785" alt="Screenshot 2026-05-18 212140" src="https://github.com/user-attachments/assets/bac5a578-2b2b-4d97-83ac-b4e346901529" />
<img width="1281" height="786" alt="Screenshot 2026-05-18 212152" src="https://github.com/user-attachments/assets/8280f1fb-8f0b-4c06-bcfc-e0071cf2793e" />
<img width="1285" height="789" alt="Screenshot 2026-05-18 212024" src="https://github.com/user-attachments/assets/6d098c97-ab5d-4dc7-863e-04626ed62c78" />


# CODIGO

```
/*
 * ============================================================
 *  Hands-on 1 – Least Squared Regressor  (Raylib GUI)
 *  Modelo de Regresion Lineal Multiple  y = B0 + B1*x1 + B2*x2
 *  β̂ = (X^T X)^{-1} X^T y
 *
 *  Visualizacion:
 *   - Scatter plot (datos reales + curva del modelo)
 *   - Panel de parametros B0, B1, B2
 *   - Ecuacion animada
 *   - Simulador interactivo con sliders x1, x2
 *   - Tabla de verificacion con scroll
 * ============================================================
 */
 
#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <algorithm>
 
// ─────────────────────────────────────────────────────────────
//  MATRIX CLASS
// ─────────────────────────────────────────────────────────────
class Mat {
    int rows, cols;
    std::vector<std::vector<double>> data;
public:
    Mat(int r, int c) : rows(r), cols(c),
        data(r, std::vector<double>(c, 0.0)) {}
 
    double& at(int r, int c)       { return data[r][c]; }
    double  at(int r, int c) const { return data[r][c]; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
 
    Mat transpose() const {
        Mat T(cols, rows);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                T.at(j, i) = data[i][j];
        return T;
    }
 
    Mat operator*(const Mat& B) const {
        Mat C(rows, B.cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < B.cols; ++j)
                for (int k = 0; k < cols; ++k)
                    C.at(i, j) += data[i][k] * B.data[k][j];
        return C;
    }
 
    Mat inverse() const {
        int n = rows;
        std::vector<std::vector<double>> aug(n, std::vector<double>(2*n, 0.0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) aug[i][j] = data[i][j];
            aug[i][n+i] = 1.0;
        }
        for (int col = 0; col < n; ++col) {
            int pivot = col;
            for (int r = col+1; r < n; ++r)
                if (std::abs(aug[r][col]) > std::abs(aug[pivot][col])) pivot = r;
            std::swap(aug[col], aug[pivot]);
            double piv = aug[col][col];
            for (int j = 0; j < 2*n; ++j) aug[col][j] /= piv;
            for (int r = 0; r < n; ++r) {
                if (r == col) continue;
                double f = aug[r][col];
                for (int j = 0; j < 2*n; ++j) aug[r][j] -= f * aug[col][j];
            }
        }
        Mat inv(n, n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                inv.at(i, j) = aug[i][n+j];
        return inv;
    }
};
 
// ─────────────────────────────────────────────────────────────
//  DATASET CLASS
// ─────────────────────────────────────────────────────────────
class DataSet {
public:
    std::vector<double> x1, x2, y;
    int n;
 
    DataSet() {
        std::vector<std::vector<double>> raw = {
            {41.9,29.1,251.3},{43.4,29.3,251.3},{43.9,29.5,248.3},
            {44.5,29.7,267.5},{47.3,29.9,273.0},{47.5,30.3,276.5},
            {47.9,30.5,270.3},{50.2,30.7,274.9},{52.8,30.8,285.0},
            {53.2,30.9,290.0},{56.7,31.5,297.0},{57.0,31.7,302.5},
            {63.5,31.9,304.5},{65.3,32.0,309.3},{71.1,32.1,321.7},
            {77.0,32.5,330.7},{77.8,32.9,349.0}
        };
        n = (int)raw.size();
        for (auto& r : raw) { x1.push_back(r[0]); x2.push_back(r[1]); y.push_back(r[2]); }
    }
};
 
// ─────────────────────────────────────────────────────────────
//  REGRESSOR CLASS
// ─────────────────────────────────────────────────────────────
class LeastSquaredRegressor {
public:
    double B0, B1, B2;
    bool trained = false;
 
    LeastSquaredRegressor() : B0(0), B1(0), B2(0) {}
 
    void fit(const DataSet& ds) {
        int n = ds.n;
        Mat X(n, 3), Y(n, 1);
        for (int i = 0; i < n; ++i) {
            X.at(i,0)=1; X.at(i,1)=ds.x1[i]; X.at(i,2)=ds.x2[i];
            Y.at(i,0)=ds.y[i];
        }
        Mat Xt   = X.transpose();
        Mat Beta = (Xt*X).inverse() * (Xt*Y);
        B0 = Beta.at(0,0); B1 = Beta.at(1,0); B2 = Beta.at(2,0);
        trained = true;
    }
 
    double predict(double x1v, double x2v) const {
        return B0 + B1*x1v + B2*x2v;
    }
};
 
// ─────────────────────────────────────────────────────────────
//  HELPERS
// ─────────────────────────────────────────────────────────────
static std::string fmt(double v, int dec=4) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(dec) << v;
    return ss.str();
}
 
static void DrawTextC(const char* txt, int x, int y, int fs, Color c) {
    DrawText(txt, x, y, fs, c);
}
 
// Map data value to screen pixel
static float mapX(double v, double vmin, double vmax, float pxMin, float pxMax) {
    return (float)(pxMin + (v-vmin)/(vmax-vmin)*(pxMax-pxMin));
}
static float mapY(double v, double vmin, double vmax, float pyMin, float pyMax) {
    // y axis: data min at bottom, max at top → invert
    return (float)(pyMax - (v-vmin)/(vmax-vmin)*(pyMax-pyMin));
}
 
// ─────────────────────────────────────────────────────────────
//  COLORS / THEME
// ─────────────────────────────────────────────────────────────
static const Color BG       = {13, 17, 23, 255};
static const Color PANEL    = {22, 27, 34, 255};
static const Color ACCENT   = {0, 200, 150, 255};
static const Color ACCENT2  = {255, 180, 0, 255};
static const Color DIMTXT   = {120, 140, 160, 255};
static const Color WHITETXT = {220, 230, 240, 255};
static const Color RED_DOT  = {255, 80, 80, 255};
static const Color BORDER   = {40, 50, 65, 255};
 
// ─────────────────────────────────────────────────────────────
//  DRAW ROUNDED PANEL
// ─────────────────────────────────────────────────────────────
static void DrawPanel(int x, int y, int w, int h) {
    DrawRectangle(x, y, w, h, PANEL);
    DrawRectangleLines(x, y, w, h, BORDER);
}
 
// ─────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────
int main() {
    const int SW = 1280, SH = 760;
    InitWindow(SW, SH, "Least Squared Regressor - Hands-on 1");
    SetTargetFPS(60);
 
    DataSet ds;
    LeastSquaredRegressor model;
    model.fit(ds);
 
    // ── Precompute ranges ──────────────────────────────
    double x1min=35, x1max=82, x2min=28, x2max=34;
    double ymin=230, ymax=360;
 
    // ── Simulation sliders ────────────────────────────
    float sliderX1 = 55.0f;   // current x1 value
    float sliderX2 = 31.0f;   // current x2 value
    bool draggingX1 = false, draggingX2 = false;
 
    // ── Table scroll ──────────────────────────────────
    int tableScroll = 0;
 
    // ── Animation ─────────────────────────────────────
    float timer = 0.0f;
 
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        timer += dt;
 
        // ── Slider logic ──────────────────────────────
        Vector2 mouse = GetMousePosition();
 
        // Slider bars (defined below, but logic here)
        Rectangle slBarX1 = {700, 320, 240, 16};
        Rectangle slBarX2 = {700, 370, 240, 16};
 
        // Handle X1 slider
        float kx1 = (sliderX1 - 35.0f) / (82.0f - 35.0f);
        float thumbX1x = slBarX1.x + kx1 * slBarX1.width;
        Rectangle thumbX1 = {thumbX1x - 10, slBarX1.y - 6, 20, 28};
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, thumbX1))
            draggingX1 = true;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) draggingX1 = false;
        if (draggingX1) {
            float t = (mouse.x - slBarX1.x) / slBarX1.width;
            t = std::max(0.0f, std::min(1.0f, t));
            sliderX1 = 35.0f + t * (82.0f - 35.0f);
        }
 
        // Handle X2 slider
        float kx2 = (sliderX2 - 28.0f) / (34.0f - 28.0f);
        float thumbX2x = slBarX2.x + kx2 * slBarX2.width;
        Rectangle thumbX2 = {thumbX2x - 10, slBarX2.y - 6, 20, 28};
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, thumbX2))
            draggingX2 = true;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) draggingX2 = false;
        if (draggingX2) {
            float t = (mouse.x - slBarX2.x) / slBarX2.width;
            t = std::max(0.0f, std::min(1.0f, t));
            sliderX2 = 28.0f + t * (34.0f - 28.0f);
        }
 
        double predY = model.predict(sliderX1, sliderX2);
 
        // Table scroll
        int wheel = (int)GetMouseWheelMove();
        tableScroll -= wheel;
        tableScroll = std::max(0, std::min(tableScroll, ds.n - 7));
 
        BeginDrawing();
        ClearBackground(BG);
 
        // ── TITLE BAR ──────────────────────────────────
        DrawRectangle(0, 0, SW, 48, PANEL);
        DrawRectangle(0, 48, SW, 1, BORDER);
        DrawTextC("Least Squared Regressor", 20, 12, 22, WHITETXT);
        DrawTextC("Hands-on 1  |  y = B0 + B1*x1 + B2*x2  |  17 Chemical Experiments", 20, 34, 11, DIMTXT);
 
        // ── PARAMETERS PANEL (top-left) ────────────────
        DrawPanel(12, 56, 320, 110);
        DrawTextC("PARAMETROS OPTIMOS", 22, 64, 11, DIMTXT);
        DrawTextC("B0", 22, 84, 13, ACCENT);
        DrawTextC(fmt(model.B0, 6).c_str(), 60, 84, 13, WHITETXT);
        DrawTextC("B1", 22, 104, 13, ACCENT);
        DrawTextC(fmt(model.B1, 6).c_str(), 60, 104, 13, WHITETXT);
        DrawTextC("B2", 22, 124, 13, ACCENT);
        DrawTextC(fmt(model.B2, 6).c_str(), 60, 124, 13, WHITETXT);
 
        // small formula
        DrawTextC("beta = (X'X)^-1 X'y", 180, 90, 12, ACCENT2);
 
        // ── EQUATION PANEL (top-center) ────────────────
        DrawPanel(340, 56, 470, 110);
        DrawTextC("ECUACION DE REGRESION", 352, 64, 11, DIMTXT);
 
        // Animated underline
        float lineW = 200 + 60*sinf(timer*1.2f);
        DrawRectangle(352, 143, (int)lineW, 2, ACCENT);
 
        std::string eq = "yield = " + fmt(model.B0,4)
            + " + " + fmt(model.B1,4) + " * x1"
            + " + " + fmt(model.B2,4) + " * x2";
        DrawTextC(eq.c_str(), 352, 88, 13, WHITETXT);
        DrawTextC("Regresion Lineal Multiple (OLS)", 352, 112, 11, DIMTXT);
 
        // ── METRICS PANEL (top-right) ──────────────────
        DrawPanel(818, 56, 250, 110);
        DrawTextC("METRICAS", 830, 64, 11, DIMTXT);
 
        // Compute SSE/RMSE
        double sse = 0;
        for (int i = 0; i < ds.n; ++i) {
            double e = ds.y[i] - model.predict(ds.x1[i], ds.x2[i]);
            sse += e*e;
        }
        double rmse = sqrt(sse/ds.n);
 
        DrawTextC("SSE", 830, 84, 13, ACCENT);
        DrawTextC(fmt(sse,2).c_str(), 880, 84, 13, WHITETXT);
        DrawTextC("RMSE", 830, 104, 13, ACCENT);
        DrawTextC(fmt(rmse,4).c_str(), 890, 104, 13, WHITETXT);
        DrawTextC("n = 17 observaciones", 830, 124, 11, DIMTXT);
 
        // ─────────────────────────────────────────────────
        //  SCATTER PLOT: Yield vs x1
        // ─────────────────────────────────────────────────
        int px0=30, py0=180, pw=620, ph=270;
        DrawPanel(px0, py0, pw, ph);
        DrawTextC("Scatter: Yield vs x1  (color = x2)", px0+8, py0+6, 11, DIMTXT);
 
        // Axes
        int axL=px0+48, axR=px0+pw-16, axT=py0+20, axB=py0+ph-30;
        DrawLine(axL, axT, axL, axB, BORDER);   // Y axis
        DrawLine(axL, axB, axR, axB, BORDER);   // X axis
 
        // Axis labels
        DrawTextC("x1", axR-12, axB+6, 11, DIMTXT);
        DrawTextC("y", axL-14, axT, 11, DIMTXT);
 
        // Tick labels X
        for (int v = 40; v <= 80; v += 10) {
            float px = mapX(v, x1min, x1max, axL, axR);
            DrawLine((int)px, axB, (int)px, axB+4, DIMTXT);
            DrawTextC(std::to_string(v).c_str(), (int)px-8, axB+6, 10, DIMTXT);
        }
        // Tick labels Y
        for (int v = 250; v <= 350; v += 25) {
            float py = mapY(v, ymin, ymax, axT, axB);
            DrawLine(axL-4, (int)py, axL, (int)py, DIMTXT);
            DrawTextC(std::to_string(v).c_str(), axL-38, (int)py-5, 10, DIMTXT);
        }
 
        // Regression line for x2 = mean(x2)
        double x2mean = 0;
        for (double v : ds.x2) x2mean += v;
        x2mean /= ds.n;
 
        // Draw line
        float px_start = mapX(x1min, x1min, x1max, axL, axR);
        float py_start = mapY(model.predict(x1min, x2mean), ymin, ymax, axT, axB);
        float px_end   = mapX(x1max, x1min, x1max, axL, axR);
        float py_end   = mapY(model.predict(x1max, x2mean), ymin, ymax, axT, axB);
        DrawLine((int)px_start, (int)py_start, (int)px_end, (int)py_end, ACCENT);
 
        // Legend for line
        DrawLine(axR-90, axT+8, axR-60, axT+8, ACCENT);
        DrawTextC("modelo (x2=mean)", axR-58, axT+3, 9, ACCENT);
 
        // Data points (color coded by x2)
        double x2lo = *std::min_element(ds.x2.begin(), ds.x2.end());
        double x2hi = *std::max_element(ds.x2.begin(), ds.x2.end());
        for (int i = 0; i < ds.n; ++i) {
            float px = mapX(ds.x1[i], x1min, x1max, axL, axR);
            float py = mapY(ds.y[i],  ymin,  ymax,  axT, axB);
            float t  = (float)((ds.x2[i]-x2lo)/(x2hi-x2lo));
            Color dot = {(unsigned char)(255*(1-t)), (unsigned char)(80+100*t), (unsigned char)(200*t), 255};
            DrawCircle((int)px, (int)py, 5, dot);
            DrawCircleLines((int)px, (int)py, 5, WHITE);
        }
 
        // Predicted point from simulator
        float spx = mapX(sliderX1, x1min, x1max, axL, axR);
        float spy = mapY(predY, ymin, ymax, axT, axB);
        if (spy >= axT && spy <= axB && spx >= axL && spx <= axR) {
            float pulse = 1.0f + 0.3f*sinf(timer*4);
            DrawCircle((int)spx, (int)spy, (int)(9*pulse), ACCENT2);
            DrawCircleLines((int)spx, (int)spy, (int)(9*pulse), WHITE);
            DrawTextC("pred", (int)spx+10, (int)spy-8, 10, ACCENT2);
        }
 
        // ─────────────────────────────────────────────────
        //  SIMULATOR PANEL
        // ─────────────────────────────────────────────────
        DrawPanel(660, 174, 400, 220);
        DrawTextC("SIMULADOR INTERACTIVO", 672, 182, 11, DIMTXT);
 
        // X1 slider
        DrawTextC("x1 (Factor 1):", 672, 308, 12, WHITETXT);
        DrawTextC(fmt(sliderX1,2).c_str(), 760, 308, 12, ACCENT2);
        DrawRectangle((int)slBarX1.x, (int)slBarX1.y, (int)slBarX1.width, (int)slBarX1.height, BORDER);
        kx1 = (sliderX1 - 35.0f) / (82.0f - 35.0f);
        DrawRectangle((int)slBarX1.x, (int)slBarX1.y, (int)(kx1*slBarX1.width), (int)slBarX1.height, ACCENT);
        thumbX1x = slBarX1.x + kx1 * slBarX1.width;
        DrawCircle((int)thumbX1x, (int)(slBarX1.y+8), 10, WHITETXT);
        DrawTextC("35", (int)slBarX1.x-4, (int)slBarX1.y+18, 9, DIMTXT);
        DrawTextC("82", (int)(slBarX1.x+slBarX1.width)-8, (int)slBarX1.y+18, 9, DIMTXT);
 
        // X2 slider
        DrawTextC("x2 (Factor 2):", 672, 358, 12, WHITETXT);
        DrawTextC(fmt(sliderX2,2).c_str(), 760, 358, 12, ACCENT2);
        DrawRectangle((int)slBarX2.x, (int)slBarX2.y, (int)slBarX2.width, (int)slBarX2.height, BORDER);
        kx2 = (sliderX2 - 28.0f) / (34.0f - 28.0f);
        DrawRectangle((int)slBarX2.x, (int)slBarX2.y, (int)(kx2*slBarX2.width), (int)slBarX2.height, ACCENT);
        thumbX2x = slBarX2.x + kx2 * slBarX2.width;
        DrawCircle((int)thumbX2x, (int)(slBarX2.y+8), 10, WHITETXT);
        DrawTextC("28", (int)slBarX2.x-4, (int)slBarX2.y+18, 9, DIMTXT);
        DrawTextC("34", (int)(slBarX2.x+slBarX2.width)-8, (int)slBarX2.y+18, 9, DIMTXT);
 
        // Prediction result
        DrawRectangle(672, 395, 376, 4, BORDER);
        DrawTextC("YIELD PREDICHO:", 672, 404, 12, DIMTXT);
        std::string predStr = fmt(predY, 4) + " unidades";
        DrawTextC(predStr.c_str(), 672, 422, 20, ACCENT2);
 
        // Equation with current values substituted
        std::string eqInst = fmt(model.B0,2) + " + " + fmt(model.B1,4)
            + "*" + fmt(sliderX1,1) + " + " + fmt(model.B2,4) + "*" + fmt(sliderX2,1);
        DrawTextC(eqInst.c_str(), 672, 350+60, 10, DIMTXT);
 
        // Pulse bar for predicted value
        float barFrac = (float)((predY - ymin) / (ymax - ymin));
        barFrac = std::max(0.0f, std::min(1.0f, barFrac));
        DrawRectangle(672, 455, 376, 8, BORDER);
        DrawRectangle(672, 455, (int)(376*barFrac), 8, ACCENT2);
 
        // ─────────────────────────────────────────────────
        //  VERIFICATION TABLE
        // ─────────────────────────────────────────────────
        DrawPanel(12, 460, 650, 290);
        DrawTextC("VERIFICACION: REAL vs PREDICHO", 24, 468, 11, DIMTXT);
 
        // Header
        int tx = 24, ty = 484;
        DrawRectangle(tx, ty, 628, 18, BORDER);
        DrawTextC("Obs", tx+4,  ty+3, 10, WHITETXT);
        DrawTextC("x1",  tx+50, ty+3, 10, WHITETXT);
        DrawTextC("x2",  tx+110,ty+3, 10, WHITETXT);
        DrawTextC("Real",tx+170,ty+3, 10, WHITETXT);
        DrawTextC("Pred",tx+250,ty+3, 10, WHITETXT);
        DrawTextC("Error",tx+340,ty+3, 10, WHITETXT);
        DrawTextC("Error %",tx+420,ty+3, 10, WHITETXT);
 
        // Rows
        int rowH = 18;
        int visRows = 11;
        for (int i = 0; i < visRows && (i+tableScroll) < ds.n; ++i) {
            int idx = i + tableScroll;
            double yp  = model.predict(ds.x1[idx], ds.x2[idx]);
            double err = ds.y[idx] - yp;
            double errP = (err/ds.y[idx])*100.0;
            int ry = ty + 20 + i*rowH;
 
            // Alternating row bg
            if (i%2==0) DrawRectangle(tx, ry, 628, rowH, {30,38,50,255});
 
            // Error color
            Color errCol = std::abs(err) < 5 ? ACCENT : (std::abs(err) < 10 ? ACCENT2 : RED_DOT);
 
            DrawTextC(std::to_string(idx+1).c_str(),      tx+4,   ry+3, 10, DIMTXT);
            DrawTextC(fmt(ds.x1[idx],1).c_str(),           tx+50,  ry+3, 10, WHITETXT);
            DrawTextC(fmt(ds.x2[idx],1).c_str(),           tx+110, ry+3, 10, WHITETXT);
            DrawTextC(fmt(ds.y[idx],1).c_str(),            tx+170, ry+3, 10, WHITETXT);
            DrawTextC(fmt(yp,2).c_str(),                   tx+250, ry+3, 10, ACCENT);
            DrawTextC(fmt(err,4).c_str(),                  tx+340, ry+3, 10, errCol);
            DrawTextC((fmt(errP,2)+"%").c_str(),           tx+420, ry+3, 10, errCol);
        }
 
        // Scroll hint
        DrawTextC("Scroll para ver mas filas", tx+480, ty+22, 9, DIMTXT);
 
        // SSE/RMSE summary at bottom of table
        int sumY = ty + 22 + visRows*rowH + 2;
        DrawRectangle(tx, sumY, 628, 20, {20,28,40,255});
        std::string sumStr = "SSE = " + fmt(sse,4) + "   |   RMSE = " + fmt(rmse,4)
            + "   |   n = " + std::to_string(ds.n);
        DrawTextC(sumStr.c_str(), tx+8, sumY+4, 10, ACCENT);
 
        // ─────────────────────────────────────────────────
        //  RIGHT PANEL: Yield vs x2 scatter
        // ─────────────────────────────────────────────────
        DrawPanel(670, 400, 390, 350);
        DrawTextC("Scatter: Yield vs x2", 682, 408, 11, DIMTXT);
 
        int bx0=718, bxR=bx0+320, byT=420, byB=bx0+290;
        DrawLine(bx0, byT, bx0, byB, BORDER);
        DrawLine(bx0, byB, bxR, byB, BORDER);
 
        // Ticks
        for (double v = 28.5; v <= 33.5; v+=1.0) {
            float px = mapX(v, 28.0, 34.0, bx0, bxR);
            DrawLine((int)px, byB, (int)px, byB+4, DIMTXT);
            DrawTextC(fmt(v,0).c_str(), (int)px-8, byB+6, 9, DIMTXT);
        }
        for (int v = 250; v <= 350; v+=25) {
            float py = mapY(v, ymin, ymax, byT, byB);
            DrawLine(bx0-4,(int)py, bx0,(int)py, DIMTXT);
            DrawTextC(std::to_string(v).c_str(), bx0-34,(int)py-4, 9, DIMTXT);
        }
 
        // Regression line (x2 axis, fix x1=mean)
        double x1mean=0; for(double v:ds.x1) x1mean+=v; x1mean/=ds.n;
        DrawLine(
            bx0, (int)mapY(model.predict(x1mean,28.0), ymin,ymax, byT,byB),
            bxR, (int)mapY(model.predict(x1mean,34.0), ymin,ymax, byT,byB),
            ACCENT
        );
 
        // Dots
        for (int i = 0; i < ds.n; ++i) {
            float px = mapX(ds.x2[i], 28.0, 34.0, bx0, bxR);
            float py = mapY(ds.y[i],  ymin, ymax,  byT, byB);
            float t  = (float)((ds.x1[i]-x1min)/(x1max-x1min));
            Color dot = {(unsigned char)(255*t), (unsigned char)(180*(1-t)+80), (unsigned char)(200*(1-t)), 255};
            DrawCircle((int)px,(int)py, 5, dot);
            DrawCircleLines((int)px,(int)py, 5, WHITE);
        }
 
        // Simulator point on this chart
        float spx2 = mapX(sliderX2, 28.0, 34.0, bx0, bxR);
        float spy2 = mapY(predY, ymin, ymax, byT, byB);
        if (spy2>=byT && spy2<=byB && spx2>=bx0 && spx2<=bxR) {
            float pulse2 = 1.0f + 0.3f*sinf(timer*4+1);
            DrawCircle((int)spx2,(int)spy2,(int)(9*pulse2), ACCENT2);
            DrawCircleLines((int)spx2,(int)spy2,(int)(9*pulse2), WHITE);
        }
 
        DrawTextC("x2", bxR-12, byB+6, 11, DIMTXT);
        DrawTextC("y",  bx0-14, byT, 11, DIMTXT);
 
        // ── FOOTER ──────────────────────────────────────
        DrawRectangle(0, SH-20, SW, 20, PANEL);
        DrawTextC("ESC para salir  |  Arrastrar sliders para simular  |  Scroll en tabla", 10, SH-16, 10, DIMTXT);
        DrawTextC("OOP C++ + Raylib", SW-130, SH-16, 10, DIMTXT);
 
        EndDrawing();
    }
 
    CloseWindow();
    return 0;
}
```

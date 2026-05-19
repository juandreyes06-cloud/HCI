# Hands-on 1 : System simulation through prediction
Interaccion Humano Computadora
A partir del Modelo Matemático Vectorizado/Matricial, construido en clase, Least Squared Regressor, para simular sistemas a través de Análisis Predictivo:
- Realizar una abstracción orientada a objetos  del modelo..
- Implementar el modelo, orientado a objetos, a través de C++.
- Dado el  Data Set “Least Squared Regressor”, calcular los valores óptimos para los parámetros de un Modelo de Regresión Lineal Múltiple: B_0, B_1 y B_2 (ver sección Data Sets en Google Classroom).
- Compruebe que los valores óptimos, calculadaos para los parámetros, coinciden con los proporcionados en la sección de Datasets. 

<img width="400" height="400" alt="image" src="https://github.com/user-attachments/assets/9939c70c-d86d-4d63-8086-8fd8665bd7d6" />

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

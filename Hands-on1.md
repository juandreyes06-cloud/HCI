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

<img width="1285" height="789" alt="image" src="https://github.com/user-attachments/assets/0003094b-bc79-41f0-bdbe-20f169e46584" />
<img width="1284" height="785" alt="image" src="https://github.com/user-attachments/assets/e3130030-fada-4beb-8790-fc62e64a3510" />
<img width="1286" height="787" alt="image" src="https://github.com/user-attachments/assets/9d30ba68-fdb2-49f3-beab-b021c6e636a3" />
<img width="1281" height="785" alt="image" src="https://github.com/user-attachments/assets/310b91fd-2f85-415f-b2c5-6948e6bc3be6" />
<img width="1281" height="786" alt="image" src="https://github.com/user-attachments/assets/4df713b1-9ae4-459a-9419-0b620748872c" />

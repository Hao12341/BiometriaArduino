#ifndef MEDIDOR_H_INCLUIDO
#define MEDIDOR_H_INCLUIDO
/**
 * class Medidor
 *
 * Esta clase se encarga de medir la concentración de ozono y la temperatura utilizando sensores analógicos.
 */
class Medidor {
private:
  int pinVGas = 2;    // Pin para el sensor de ozono
  int pinVRef = 29;   // Pin para la referencia
  int pinVTemp = 28;  // Pin para el sensor de temperatura
  double ppmOzono;    // Variable privada para almacenar la concentración de ozono

public:
  // Variables públicas para almacenar los valores medidos.
  float vgas;    // Tensión del sensor de ozono
  float vref;    // Tensión de referencia
  float VeTemp;  // Tensión del sensor de temperatura

  float aGas;    // Valor analógico del sensor de ozono
  float aRef;    // Valor analógico de referencia
  float aTemp;   // Valor analógico del sensor de temperatura

  float Temp;     // Temperatura medida
  float Vmedida;  // Tensión medida
  float Cx;       // Concentración de ozono
  float Itemp;    // Variación de temperatura
  float fCx;      // Concentración ajustada de ozono

  // .....................................................
  // Constructor
  //
  // @param {int} numero
  // Inicializa los pines de los sensores como entradas.
  Medidor() {
    pinMode(pinVGas, INPUT);   // Configura pinVGas como entrada
    pinMode(pinVRef, INPUT);   // Configura pinVRef como entrada
    pinMode(pinVTemp, INPUT);  // Configura pinVTemp como entrada
  }  // fin del constructor Medidor()

  // .....................................................
  // Función para convertir un valor digital a voltios.
  //
  // @param {int} Vin
  // @return {float} voltaje correspondiente al valor digital.
  float digToVolt(int Vin) {
    return ((Vin * 3.3) / 1024); // Convierte el valor leído en un voltaje (0-3.3V)
  }

  // .....................................................
  // Función para realizar una calibración lineal.
  //
  // @param {double} valorMedido
  // @param {double &} m
  // @return {double} valor calibrado.
  double calibrarLectura(double valorMedido, double &m) {
    m = 0.3;                                    // Pendiente de la recta, ajustar según sea necesario
    const double b = -1.5;                      // Intersección de la recta, ajustar según sea necesario
    double valorCalibrado = m * valorMedido + b; // Calcula el valor calibrado

    // Asegura que el valor calibrado no sea negativo
    return valorCalibrado > 0 ? valorCalibrado : 0;
  }

  // .....................................................
  // Método para realizar inicializaciones necesarias.
  //
  // Inicializa las variables y configura los pines.
  void iniciarMedidor() {
    vgas = 0;       // Inicializa la tensión del sensor de ozono
    vref = 0;       // Inicializa la tensión de referencia
    ppmOzono = 0;   // Inicializa la concentración de ozono
    pinMode(pinVRef, INPUT);  // Configura pinVRef como entrada
    pinMode(pinVGas, INPUT);  // Configura pinVGas como entrada
  }  // fin de iniciarMedidor()

  // .....................................................
  // Método para medir la concentración de ozono.
  //
  // @return {int} ppm calibrado de ozono.
  int medirCO2() {
    // Lee el valor de los pines del sensor
    int Agas = analogRead(pinVGas);  // Lee el valor analógico del sensor de ozono
    int Aref = analogRead(pinVRef);  // Lee el valor analógico de referencia

    // Convierte el valor digital a voltios
    vgas = digToVolt(Agas); // Convierte la lectura del sensor de ozono a voltios
    vref = digToVolt(Aref); // Convierte la lectura de referencia a voltios

    // Constante de conversión basada en la especificación del sensor
    const double M = -41.96 * 499 * (0.000001); // Coeficiente de conversión específico para el sensor

    // Calcula las ppm de ozono
    double res = ((1 / M) * (vgas - vref)); // Calcula la concentración en partes por millón (ppm)
    ppmOzono = res > 0 ? res : 0; // Asegura que el valor de ppmOzono no sea negativo

    // Almacena el valor de la pendiente para calibración
    double m;

    // Aplica la calibración lineal al valor de ppmOzono
    double ppmCalibrado = calibrarLectura(ppmOzono, m);

    // Calcula el valor de ppmOzono * 10 para imprimir
    int ppm10 = (int)(ppmOzono * 10); // Multiplica por 10 y convierte a entero

    // Imprime valores de referencia y medidos en el monitor serie
    Serial.print("VGAS: "); // Imprime la tensión del sensor de ozono
    Serial.println(vgas);
    Serial.print("VREF: "); // Imprime la tensión de referencia
    Serial.println(vref);
    Serial.print("M: ");     // Imprime la constante de conversión
    Serial.println(M);  
    Serial.print("PPM Ozono * 10: "); // Imprime el valor de ppmOzono * 10
    Serial.println(ppm10);  
    Serial.print("PPM Ozono (calibrado): "); // Imprime el valor calibrado
    Serial.println(ppmCalibrado);

    return ppmCalibrado;  // Devuelve el valor calibrado
  } // fin de medirCO2()

  // .....................................................
  // Método para medir la temperatura.
  //
  // @return {int} temperatura en centésimas.
  int medirTemperatura() {
    return Temp * 100; // Devuelve la temperatura en centésimas
  }  // fin de medirTemperatura()
};//class

#endif

#include <bluefruit.h>

#undef min
#undef max 

// --------------------------------------------------------------
// Inclusión de cabeceras necesarias para el proyecto
// --------------------------------------------------------------
#include "LED.h"          
#include "PuertoSerie.h" 

// --------------------------------------------------------------
// Espacio de nombres Globales para variables y objetos compartidos
// --------------------------------------------------------------
namespace Globales {

LED elLED(/* NUMERO DEL PIN LED = */ 7);  

PuertoSerie elPuerto(/* velocidad = */ 115200); 

};

// --------------------------------------------------------------
// Inclusión de cabeceras adicionales
// --------------------------------------------------------------
#include "EmisoraBLE.h" 
#include "Publicador.h"  
#include "Medidor.h"     

// --------------------------------------------------------------
// Espacio de nombres Globales para otros objetos
// --------------------------------------------------------------
namespace Globales {

Publicador elPublicador;  
Medidor elMedidor;        

};  // namespace Globales

// --------------------------------------------------------------
// Función setup()
// Se ejecuta una vez al inicio del programa para inicializar el sistema.
// --------------------------------------------------------------
void setup() {

  Globales::elPuerto.esperarDisponible();  // Espera a que el puerto serie esté disponible

  // Inicialización de la plaquita, puede incluir más configuraciones necesarias.
  // inicializarPlaquita(); // (descomentado si es necesario)

  // Suspender el loop() para ahorrar energía (opcional)
  // suspendLoop(); // (descomentado si es necesario)

  Globales::elPublicador.encenderEmisora();  // Enciende la emisora para comenzar la transmisión

  // Globales::elPublicador.laEmisora.pruebaEmision(); // (prueba de emisión opcional)

  Globales::elMedidor.iniciarMedidor();  // Inicializa el medidor para comenzar a recoger datos

  esperar(1000);  // Espera un segundo antes de continuar

  Globales::elPuerto.escribir("---- setup(): fin ---- \n ");  // Mensaje de fin de configuración
}  // fin de setup()

// --------------------------------------------------------------
// Función lucecitas()
// Sirve para comprobar que el programa está subido a la placa Sparkfun.
// Realiza un parpadeo del LED para indicar que el sistema está activo.
// --------------------------------------------------------------
inline void lucecitas() {
  using namespace Globales;

  elLED.brillar(100);   // LED encendido (100%)
  esperar(400);         // Espera 400 ms
  elLED.brillar(0);     // LED apagado (0%)
  esperar(400);         // Espera 400 ms
  elLED.brillar(100);   // LED encendido (100%)
  esperar(400);         // Espera 400 ms
  elLED.brillar(0);     // LED apagado (0%)
  esperar(400);         // Espera 400 ms
  elLED.brillar(1000);  // LED encendido (1000%)
  esperar(1000);        // Espera 1000 ms
}  // fin de lucecitas()

// --------------------------------------------------------------
// Función loop()
// Se ejecuta continuamente después de setup().
// Aquí se realizan las mediciones y se publica la información.
// --------------------------------------------------------------
namespace Loop {
uint8_t cont = 0;  // Contador para llevar la cuenta de iteraciones
};

// ..............................................................
// ..............................................................
void loop() {

  using namespace Loop;      // Uso del espacio de nombres Loop
  using namespace Globales;  // Uso del espacio de nombres Globales

  cont++;  // Incrementa el contador

  elPuerto.escribir("\n---- loop(): empieza ");  // Mensaje indicando el inicio del loop
  elPuerto.escribir(cont);                       // Escribe el valor del contador en el puerto serie
  elPuerto.escribir("\n");

  lucecitas();  // Llama a la función para parpadear el LED

  // Medición y publicación del valor de CO2
  int valorCO2 = elMedidor.medirCO2();
  elPublicador.publicarCO2(valorCO2, cont, 1000);  

  // Medición y publicación de la temperatura
  int valorTemperatura = elMedidor.medirTemperatura();  // Medir la temperatura
  // elPublicador.publicarTemperatura( valorTemperatura, cont, 1000 ); // Publicar la temperatura
  /*
  char datos[21] = {
    'H', 'o', 'l', 'a', // Datos de ejemplo
    'H', 'o', 'l', 'a',
    'H', 'o', 'l', 'a',
    'H', 'o', 'l', 'a',
    'H', 'o', 'l', 'a',
    'H'
  };
  // elPublicador.laEmisora.emitirAnuncioIBeaconLibre ( &datos[0], 21 );
  elPublicador.laEmisora.emitirAnuncioIBeaconLibre ( "MolaMolaMolaMolaMolaM", 21 );
  */

  esperar(1000);  // Espera 1000 ms antes de continuar

  elPublicador.laEmisora.detenerAnuncio();  // Detiene la emisión del anuncio

  elPuerto.escribir("---- loop(): acaba **** ");  // Mensaje indicando el fin del loop
  elPuerto.escribir(cont);                        // Escribe el valor del contador en el puerto serie
  elPuerto.escribir("\n");
}  // fin de loop()
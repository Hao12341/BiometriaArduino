// --------------------------------------------------------------
#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

// --------------------------------------------------------------
// --------------------------------------------------------------
class Publicador {

  // ............................................................
  // ............................................................
private:

  uint8_t beaconUUID[16] = { 
	'S', 'E', 'N', 'S', 'O', 'R', '-', 'B', 
	'I', 'O', 'M', 'E', 'T', 'R', 'I', 'A'
	};

  // ............................................................
  // ............................................................
public:
  EmisoraBLE laEmisora {
	"SensorBiometria", //  nombre emisora <-----
	  0x004c, // fabricanteID (Apple)
	  4 // txPower
	  };
  
  const int RSSI = -53; // por poner algo, de momento no lo uso

  // ............................................................
  // ............................................................
public:

  // ............................................................
  // ............................................................
  enum MedicionesID  {
	CO2 = 1,
	TEMPERATURA = 2,
	RUIDO = 3
  };

  // ............................................................
  // ............................................................
  /**
   * Constructor 
   *
   */
  Publicador( ) {
	// ATENCION: no hacerlo aquí. (*this).laEmisora.encenderEmisora();
	// Pondremos un método para llamarlo desde el setup() más tarde
  } // ()

  // ............................................................
  // ............................................................
   /**
   * encenderEmisora()
   *
   */
  void encenderEmisora() {
	(*this).laEmisora.encenderEmisora();
  } // ()

  // ............................................................
  // ............................................................
  /**
   * int16_t,uint8_t,long -> publicarCO2()
   *
   * @param {int16_t} valorCO2 
   * @param {uint8_t} contador 
   * @param {long} tiempoEspera
   *
   */
  void publicarCO2( int16_t valorCO2, uint8_t contador,
					long tiempoEspera ) {
  
  	//
  	// 1. empezamos anuncio
  	//
  	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
  											MedicionesID::CO2,
  											valorCO2, // minor
  											(*this).RSSI // rssi
  									);
    /*
  	Globales::elPuerto.escribir( "   publicarCO2(): valor=" );
  	Globales::elPuerto.escribir( valorCO2 );
  	Globales::elPuerto.escribir( "   contador=" );
  	Globales::elPuerto.escribir( contador );
  	Globales::elPuerto.escribir( "   todo="  );
  	Globales::elPuerto.escribir( major );
  	Globales::elPuerto.escribir( "\n" );
    */
  
  	//
  	// 2. esperamos el tiempo que nos digan
  	//
  	esperar( tiempoEspera );
  
  	//
  	// 3. paramos anuncio
  	//
  	(*this).laEmisora.detenerAnuncio();
  } // ()

  // ............................................................
  // ............................................................
  /**
   * int16_t,uint8_t,long -> publicarTemperatura()
   *
   * @param {int16_t} valorTemperatura 
   * @param {uint8_t} contador
   * @param {long} tiempoEspera
   *
   */
  void publicarTemperatura( int16_t valorTemperatura,
							uint8_t contador, long tiempoEspera ) {

	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
											MedicionesID::TEMPERATURA,
											valorTemperatura, // minor
											(*this).RSSI // rssi
									);
	esperar( tiempoEspera );

	(*this).laEmisora.detenerAnuncio();
  } // ()

}; // class

// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
#endif

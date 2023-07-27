//#include <Arduino.h>
//#include <time.h>

//program includes
//#include "Numero Uno.h"
//#include "Numero Due.h"
//#include "Numero Tre.h"
//#include "Numero quattro.h"
//#include "Morse Code.h"
//#include "Screen.h"
//#include "Numero Cinque.h"
//#include "Numero Sei.h"
#include "LEDs.h"

//namespaces
//using namespace Uno;
//using namespace Due;
//using namespace Tre;
//using namespace Quattro;
//using namespace ScreenTest;
//using namespace Cinque;
//using namespace Sei;


using namespace LEDs;

LED* leds = nullptr;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  leds = new LED(7, 300);
  leds->setup();
  //SeiSetup();
}


void loop()
{
  leds->loop();
}
/*Attiny85_batteryMonitor
  Ideas borrowed from: https://github.com/unixbigot/Flat-Mate
  modified for arduino tiny core by DWhacks
  @@ Voltage trigger levels.

   Battery voltage is read through a voltage divider and compared to the internal voltage reference.

   If
      Vin ----+
              R1
              +----- Vout (BATI)
              R2
              |
              =
              .  (gnd)

   Then Vout = Vin * ( R2 / (R1 + R2) )
   R2 = 1k
   R1 = 16k
   Divider = Vin / 17
  >990 = 16.5v  (charging)
  982 = 16.4v
  838 = 14.0v
  718 = 12.0v
  670 = 11.2v   (drained)*/
#include <DigiUSB.h>

const int batteryPin = 3;                      //V+ from divider P3
const int redPin     = 1;                      //Red led P1 PWM capable
const int greenPin   = 4;                      //Green led P4 PWM capable
float vout           = 0.0;
float vin            = 0.0;
int value            = 0;                      //Value read from ADC

void setup() {
  //P0, P1, and P4 are capable of hardware PWM (analogWrite).
  DigiUSB.begin();
  DigiUSB.delay(3000);
  analogReference(INTERNAL);
  pinMode(batteryPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  value = analogRead(batteryPin);
  //vout = (value * 1.1) / 1024;
  vin = (value * 17.1) / 1024;
  DigiUSB.write("CH3:  ");                    //debug
  DigiUSB.println(value, DEC);                //ADC Value
  DigiUSB.println(vin, DEC);                  //Voltage value calculated
  DigiUSB.delay(500);

  if (value >= 982) {                          //Charging  GREEN BLINK
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);              // turn the LED on (HIGH is the voltage level)
    delay(500);                               // wait for a second
    digitalWrite(greenPin, LOW);               // turn the LED off by making the voltage LOW
    delay(500);                               // wait for a second
  } else if (value <= 982 && value >= 838) {   //16.4 level GREEN ON
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    delay(500);
  } else if (value <= 838 && value >= 718) {   //14.0v level ORANGE ON
    digitalWrite(redPin, 127);
    digitalWrite(greenPin, 127);
    delay(1000);
  } else if (value <= 718 && value >= 658) {   // 12.0v level RED ON
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    delay(500);
  } else  if (value < 658) {                   //11.2v critacal low voltage RED BLINK
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);
    delay(500);
  }
}

/*
Date: 02.06.2020
Author: Glaukon Ariston
URL: https://github.com/glaukon-ariston/Rx2Tx2CarRemote
Abstract:

= Cheap Toy Car Remote Control using Arduino =

Control cheap toy remote cars by hijacking the car's transmitter and sending control codes to the car's receiver using Arduino.

Based on the Silan Semiconductors TX-2B/RX-2B datasheet. Open up your remote car's transmitter to see what chip is being used.

From the datasheet:

REMOTE CONTROLLER WITH FIVE FUNCTIONS 

DESCRIPTION
The TX-2B/RX-2B is a pair of CMOS LSIs designed for remote
controlled car applications. The TX-2B/RX-2B has five control keys
for controlling the motions (i.e. forward, backward, rightward, leftward
and the turbo function) of the remote controlled car.

FEATURES
* Wide operating voltage range (VCC=1.5~5.0V)
* Low stand-by current
* Auto-power-off function for TX-2B
* Few external components are needed

There is a similar project at https://github.com/dimircea/RX2-TX2-RC


IMPORTANT!!!
How can I enable c ++ 14 standard support in Arduino IDE?
https://forum.arduino.cc/index.php?topic=661424.0

C:\app\electro\Arduino\hardware\arduino\avr\platform.txt
Change this line so it reads -std=gnu++17 instead of -std=gnu++11. 
compiler.cpp.flags=-c -g -Os {compiler.warning_flags} -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto


*/

#include <Arduino.h>
#include "Rx2Tx2CarRemote.h"


auto repeat = [](auto n, auto f) {
    decltype(f()) sum=0;
    for (unsigned int i=(n); i > 0 ; --i) sum += f();
    return sum;
};


Rx2Tx2CarRemote::Rx2Tx2CarRemote(unsigned int powerControlPin, unsigned int signalOutPin, unsigned int freq, unsigned int cmdRepeat, unsigned int endCodeRepeat) 
: powerControlPin_(powerControlPin)
, signalOutPin_(signalOutPin)
, freq_(freq)
,  halfPeriodUs_(static_cast<unsigned int>(1000000UL/2/freq)) 
, cmdRepeat_(cmdRepeat)
, endCodeRepeat_(endCodeRepeat) 
{
}


void Rx2Tx2CarRemote::setup() {
  pinMode(powerControlPin_, OUTPUT);
  pinMode(signalOutPin_, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}


Rx2Tx2CarRemote::ElapsedTimeT Rx2Tx2CarRemote::pulseW1() {
  digitalWrite(signalOutPin_, HIGH);
  delayMicroseconds(halfPeriodUs_);
  digitalWrite(signalOutPin_, LOW);
  delayMicroseconds(halfPeriodUs_);
  return 2*halfPeriodUs_;
}


Rx2Tx2CarRemote::ElapsedTimeT Rx2Tx2CarRemote::pulseW2() {
  digitalWrite(signalOutPin_, HIGH);
  delayMicroseconds(3*halfPeriodUs_);
  digitalWrite(signalOutPin_, LOW);
  delayMicroseconds(halfPeriodUs_);
  return 4*halfPeriodUs_;
}


Rx2Tx2CarRemote::ElapsedTimeT Rx2Tx2CarRemote::sendWord(W1 cmd) {
  ElapsedTimeT t = 0;
  // CODE FORMAT
  // W2 W2 W2 W2 (n x W1) W2 W2 W2 W2 (n x W1) W2 W2 W2 W2
  // Start code sequence 4 x W2, @500Hz, 75% Duty Cycle
  t += repeat(4, [this]{return this->pulseW2();});
  // Function code sequence n x W1, @1000Hz, 50% Duty Cycle
  t += repeat(cmd, [this]{return this->pulseW1();});
  return t;
}


Rx2Tx2CarRemote::ElapsedTimeT Rx2Tx2CarRemote::sendCmd(W1 cmd) {
  ElapsedTimeT t = 0;
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(powerControlPin_, HIGH);
  t += repeat(cmdRepeat_, [this, cmd]{return this->sendWord(cmd);});
  t += repeat(endCodeRepeat_, [this]{return this->sendWord(Rx2Tx2CarRemote::ENDCODE);});
  digitalWrite(powerControlPin_, LOW);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  return t;
}


Rx2Tx2CarRemote::ElapsedTimeT Rx2Tx2CarRemote::sendCmd(W1 cmd, ElapsedTimeT us) {
  ElapsedTimeT t = 0;
  for(; t < us; t += sendCmd(cmd))
    continue;
  return t;
}

/*
Date: 02.06.2020
Author: Glaukon Ariston
URL: https://github.com/glaukon-ariston/Rx2Tx2CarRemote
Abstract: Make the car go in a wave-like motion.
*/

#include <Rx2Tx2CarRemote.h>

auto repeat = [](auto n, auto f) {
    for (unsigned int i=(n); i > 0 ; --i) f();
};


Rx2Tx2CarRemote transmitter(/*powerControlPin*/ 9, /*signalOutPin*/ 10, /*freq*/ 1000, /*cmdRepeat*/ 1, /*endCodeRepeat*/ 1);


void setup() {
  transmitter.setup();
}


void loop() {
  repeat(7, [&]{
    transmitter.sendCmd(Rx2Tx2CarRemote::FORWARD_RIGHT, 500*1000UL);
    transmitter.sendCmd(Rx2Tx2CarRemote::FORWARD_LEFT, 500*1000UL);
  });
  
  repeat(7, [&]{
    transmitter.sendCmd(Rx2Tx2CarRemote::BACKWARD_RIGHT, 500*1000UL);
    transmitter.sendCmd(Rx2Tx2CarRemote::BACKWARD_LEFT, 500*1000UL);
  });
}


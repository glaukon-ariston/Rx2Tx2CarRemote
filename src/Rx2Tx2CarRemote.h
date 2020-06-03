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
*/

#pragma once


class Rx2Tx2CarRemote
{
public:
  typedef unsigned long ElapsedTimeT;

  /** Functions (aka W1) - as defined by the IC datasheet */
  enum W1 {
    ENDCODE = 4,
    FORWARD = 10,
    FORWARD_TURBO = 16,
    TURBO = 22,
    FORWARD_LEFT = 28,
    FORWARD_RIGHT = 34,
    BACKWARD = 40,
    BACKWARD_RIGHT = 46,
    BACKWARD_LEFT = 52,
    LEFT = 58,
    RIGHT = 64,
  };

  Rx2Tx2CarRemote(unsigned int powerControlPin, unsigned int signalOutPin, unsigned int freq = 1000, unsigned int cmdRepeat = 3, unsigned int endCodeRepeat = 16);
  void setup();
  ElapsedTimeT sendWord(W1 cmd);
  ElapsedTimeT sendCmd(W1 cmd);
  ElapsedTimeT sendCmd(W1 cmd, ElapsedTimeT us);

private:
  ElapsedTimeT pulseW1();
  ElapsedTimeT pulseW2();

private:
  const unsigned int powerControlPin_;
  const unsigned int signalOutPin_;
  const unsigned int freq_;
  const unsigned int cmdRepeat_;
  const unsigned int endCodeRepeat_;
  const ElapsedTimeT halfPeriodUs_;
};


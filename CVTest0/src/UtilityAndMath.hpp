#include "main.h"

#ifndef UTILITYANDMATH_HPP
#define UTILITYANDMATH_HPP

/*
 Calculates the standard deviation of an array given the array, the mean, and
 the size of the array.Data[] is an array of double precision data points to
 calculate the standatrd devition of, mean is a double precision mean of the
 array data[], size is an integer for the size of teh array. Returns the
 standard deviation as a double.
*/
double calculateSD(double data[], double mean, int size);

/*
 This function is used for testing to get ultrasonic sensor vaules printed to
 terminal. Values can be accessed with putty see pros docs debug for details.
 tInterval is the time in milliseconds between samples, liftArmPos is the
 position to raise the arm too(number of degreees / 5 above the start position)
*/
void ultraScanner(int tInterval, int liftArmPos);

#endif

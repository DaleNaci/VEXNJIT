#include "main.h"
#include "Sensors.hpp"
#include "LiftFunctions.hpp"
#include "UtilityAndMath.hpp"

double calculateSD(double data[], double mean, int size){
    double standardDeviation = 0.0;//initialize teh standardDeviation variable as a double

    for(int i = 0; i < size; ++i)//iterate through every value of array data
        standardDeviation += pow(data[i] - mean, 2);//sum together all of the differences between the array values and the mean squared
    return sqrt(standardDeviation / size);//return the squart root of teh summation divided by size
}

void ultraScanner(int tInterval, int liftArmPos){
	liftPosition(liftArmPos, 50);
	pros::delay(1000);
	while(true)
	 {
		 pros::delay(tInterval);
		 printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	 }
}

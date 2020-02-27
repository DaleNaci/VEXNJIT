#include "main.h"
#include "Vectors2.hpp"

#define HALF_PI 1.5707963
void calculateOffset(ServoGeometry &geo, double theta, double azimuth, vector &offsets)
{
  theta *= -1;
  theta += (HALF_PI);
  azimuth -= HALF_PI;

  //https://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
  //calculate the new vector of m1 after rotating to the given theta
  //uses a rotation matrix rotating about the x-axis
  double tempx = geo.pitch_x;
  double tempy = geo.pitch_y * cos(theta) - geo.pitch_z * sin(theta);
  double tempz = geo.pitch_y * sin(theta) + geo.pitch_z * cos(theta);
  //combine vectors
  tempx += geo.yaw_x;
  tempy += geo.yaw_y;
  tempz += geo.yaw_z;
  //calculate the new vector of m2 after rotating to the given azimuth
  //uses a rotation matrix rotating about the z-axis
  double temp = tempx * cos(azimuth) - tempy * sin(azimuth);
  double temp2 = tempx * sin(azimuth) - tempy * cos(azimuth);
  tempx = temp;
  tempy = temp2;
  //add hard offsets from reference frame and store it to the vector offsets
  offsets.x = (tempx + geo.x);
  offsets.y = tempy + geo.y;
  offsets.z = (tempz + geo.z);
}
vector sphereToVector(double r, double theta, double azimuth, vector &offsets)
{
  Vector data;
  theta *= -1;
  theta += (HALF_PI);
  azimuth -= HALF_PI;
  double temp1 = sin(theta) * r;
  data.x = temp1 * cos(azimuth) + offsets.x;
  data.y = temp1 * sin(azimuth) + offsets.z;
  data.z = r * cos(theta) + offsets.y;
  return data;
}

#ifndef VECTORS2_HPP
#define VECTORS2_HPP

struct vector
{
  double x;
  double y;
  double z;
};
typedef struct vector Vector;

struct sphere
{
  double r;
  double theta;
  double azimuth;
};
typedef struct sphere Sphere;
struct servoGeometry
{
  //coordinates of a vector starting at the center of the axis of rotation of the servo controlling pitch(m1) and ending at the sensor where measurments are taken from
  double pitch_x;
  double pitch_y;
  double pitch_z;
  //coordinates of a vector starting at the center of the axis of rotation of the servo controlling yaw(m2) and ending at the center of the axis of rotation of the servo controlling pitch
  double yaw_x;
  double yaw_y;
  double yaw_z;
  //the coordinates of the center of the axis of rotation of the servo controlling yaw relative to space
  double x;
  double y;
  double z;
};
typedef struct servoGeometry ServoGeometry;

void calculateOffset(ServoGeometry &geo, double theta, double azimuth, vector &offsets);

vector sphereToVector(double r, double theta, double azimuth, vector &offsets);



#endif

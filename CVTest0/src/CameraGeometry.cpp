#import "Vectors2.hpp"

//http://www.cse.psu.edu/~rtc12/CSE486/lecture13.pdf
//http://www.cse.psu.edu/~rtc12/CSE486/lecture12.pdf
struct Camera{
  double focalLength;
  double sx;
  double sy;
  double ox;
  double oy;
};

struct pixelCoord{
 double u;
 double v;
};

struct filmCoord{
  double x;
  double y;
};

struct cameraCoord{
  double x;
  double y;
  double z;
};

struct worldCoord{
  double u;
  double v;
  double w;
};

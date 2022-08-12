#include "header.h"

void ba_project_point(
 double K[3*3],
 double R[3*3],
 double T[3],
 double X3[3],
 double x3[3]
)

/*
Compute the projection of 3D point X3
onto image pixel plane (in image pixel homogeneous coordinates) 
*/

{

 double X3b[3];
 int i;

 /*
 Project onto the camera coordinate system
 proj(X3)=R*X3+T
 where R is the camera rotation and T is the camera translation
 */

 math_matrix_vector_product(
  R,
  3,
  3,
  X3,
  3,
  X3b
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    X3b[i]+= T[i];
 }

 /*
 Go from camera coordinates to
 image pixel homogeneous coordinates
 */

 math_matrix_vector_product(
  K,
  3,
  3,
  X3b,
  3,
  x3
 );

}

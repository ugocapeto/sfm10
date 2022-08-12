#include "header.h"

void ba_create_quaternion_from_rotation_matrix(
 double R[3*3],
 double q[4]
)

/*
Get the quaternion from the rotation matrix
*/

/*
There are 2 methods available:
ba_create_quaternion_from_rotation_matrix_0()
ba_create_quaternion_from_rotation_matrix_1()
*/

{

 ba_create_quaternion_from_rotation_matrix_0(
  R,
  q
 );

 /*
 ba_create_quaternion_from_rotation_matrix_1(
  R,
  q
 );
 */

}

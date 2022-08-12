#include "header.h"

void compute_1st_camera_matrix(
 double P1[3*4]
)

/*
This computes the 1st camera matrix [I|0]
*/

{

 double I33[3*3];
 int i;
 int j;

 /*
 Compute the 3x3 identity matrix
 */

 I33[0*3+0]= 1;
 I33[0*3+1]= 0;
 I33[0*3+2]= 0;

 I33[1*3+0]= 0;
 I33[1*3+1]= 1;
 I33[1*3+2]= 0;

 I33[2*3+0]= 0;
 I33[2*3+1]= 0;
 I33[2*3+2]= 1;

 /*
 Compute P1, the 1st camera matrix
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P1[i*4+j]= I33[i*3+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P1[i*4+j]= 0;
 }

}

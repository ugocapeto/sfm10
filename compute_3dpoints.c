#include "header.h"

void compute_3dpoints(
 double P1[3*4],
 double P2[3*4],
 match_struct *match_arr,
 int match_nbr,
 double *points_arr
)

/*
Compute 3D points in projective space (points are homogeneous 4-vectors)
given the the camera matrices
*/

{

 double X4[4];
 double X3[3];
 int match_ind;
 double x1;
 double y1;
 double x2;
 double y2;

 for ( match_ind= 0 ; match_ind< match_nbr ; match_ind++ ) {
    x1= match_arr[match_ind].x1;
    y1= match_arr[match_ind].y1;
    x2= match_arr[match_ind].x2;
    y2= match_arr[match_ind].y2;

    compute_linear_triangulation_two_cameras(
     P1,
     P2,
     x1,
     y1,
     x2,
     y2,
     X4
    );

    /*
    Go from homogeneous to inhomogeneous
    */

    X3[0]= X4[0]/X4[3];
    X3[1]= X4[1]/X4[3];
    X3[2]= X4[2]/X4[3];

    /*
    Store the 3d point
    */

    points_arr[3*match_ind+0]= X3[0];
    points_arr[3*match_ind+1]= X3[1];
    points_arr[3*match_ind+2]= X3[2];
 }

}

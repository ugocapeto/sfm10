#include "header.h"

double sfm_compute_average_separation_angle(
 double P1[3*4],
 double K1[3*3],
 double R1[3*3],
 double t1[3],
 double P2[3*4],
 double K2[3*3],
 double R2[3*3],
 double t2[3],
 match_struct *match_arr,
 int match_nbr
)

{

 double X4[4];
 double X3[3];
 int match_ind;
 double x1;
 double y1;
 double x2;
 double y2;
 double C1[3];
 double C2[3];
 double avg_angle;
 int feat1_ind;
 int feat2_ind;
 int i;
 double xyz[3];
 double vec[3];
 double norm2;
 double norm;
 double vec1[3];
 double vec2[3];
 double dotp;
 double cosine;
 double angle_rad;
 double angle;
 double pi=acos(-1.);

 /*
 Compute camera center for camera1
 */

 sfm_compute_C_from_R_t(
  R1,
  t1,
  C1
 );

 /*
 Compute camera center for camera2
 */

 sfm_compute_C_from_R_t(
  R2,
  t2,
  C2
 );

 avg_angle= 0;

 for ( match_ind= 0 ; match_ind< match_nbr ; match_ind++ ) {
    feat1_ind= match_arr[match_ind].feat1_ind;
    x1= match_arr[match_ind].x1;
    y1= match_arr[match_ind].y1;
    feat2_ind= match_arr[match_ind].feat2_ind;
    x2= match_arr[match_ind].x2;
    y2= match_arr[match_ind].y2;

    /*
    Compute the location of the 3D point
    */

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

    for ( i= 0 ; i< 3 ; i++ )
     xyz[i]= X3[i];

    /*
    Consider the normalized vector
    that goes from the 3D point to the camera center
    of camera 1
    */

    for ( i= 0 ; i< 3 ; i++ )
     vec[i]= C1[i]-xyz[i];

    norm2= math_vector_vector_product(
     vec,
     3,
     vec,
     3
    );
    norm= sqrt(norm2);

    for ( i= 0 ; i< 3 ; i++ )
     vec[i]/= norm;

    for ( i= 0 ; i< 3 ; i++ )
     vec1[i]= vec[i];

    /*
    Consider the normalized vector
    that goes from the 3D point to the camera center
    of camera 2
    */

    for ( i= 0 ; i< 3 ; i++ )
     vec[i]= C2[i]-xyz[i];

    norm2= math_vector_vector_product(
     vec,
     3,
     vec,
     3
    );
    norm= sqrt(norm2);

    for ( i= 0 ; i< 3 ; i++ )
     vec[i]/= norm;

    for ( i= 0 ; i< 3 ; i++ )
     vec2[i]= vec[i];

    dotp= math_vector_vector_product(
     vec1,
     3,
     vec2,
     3
    );

    cosine= dotp;

    angle_rad= acos(cosine);

    angle= angle_rad*180/pi;

    avg_angle+= angle;
 }

 avg_angle/= (double)match_nbr;

 return avg_angle;

}

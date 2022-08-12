#include "header.h"

int select_correct_2nd_camera_matrix(
 double P1[3*4],
 double P24[4][3*4],
 match_struct *match_normalized_arr,
 match_struct *match_arr,
 int match_nbr,
 double P2[3*4]
)

/*
P1 and P2 are normalized camera matrices
They are of the form P=[R|t] (P=K[r|t] where K is the identity matrix)
*/

{

 double K1[3*3];
 double R1[3*3];
 double C1[3];
 double K2[3*3];
 double R2[3*3];
 double C2[3];
 int i;
 int j;
 int ind;
 int count;
 int match_ind;
 double x1_normalized;
 double y1_normalized;
 double x2_normalized;
 double y2_normalized;
 double x1;
 double y1;
 double x2;
 double y2;
 double depth1;
 double depth2;
 int max_ind;
 int max_count;
 double X4[4];
 double X3[3];
 int err_flag;
 int nbr;
 int count4[4];
 double t1[3];
 double t2[3];

 /*
 Decompose the 1st camera matrix
 */

 K1[0*3+0]= 1;
 K1[0*3+1]= 0;
 K1[0*3+2]= 0;

 K1[1*3+0]= 0;
 K1[1*3+1]= 1;
 K1[1*3+2]= 0;

 K1[2*3+0]= 0;
 K1[2*3+1]= 0;
 K1[2*3+2]= 1;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R1[3*i+j]= P1[4*i+j]; 
    }
 }

 j= 3;
 for ( i= 0 ; i< 3 ; i++ ) {
    t1[i]= P1[4*i+j];
 }

 sfm_compute_C_from_R_t(
  R1,
  t1,
  C1
 );

 /*
 Loop on each possible 2nd camera matrix
 */

 for ( ind= 0 ; ind< 4 ; ind++ ) {

    /*
    Get the 2nd camera matrix
    */

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 4 ; j++ ) {
          P2[i*4+j]= P24[ind][i*4+j];
       }
    }

    /*
    Decompose the 2nd camera matrix
    */

    K2[0*3+0]= 1;
    K2[0*3+1]= 0;
    K2[0*3+2]= 0;

    K2[1*3+0]= 0;
    K2[1*3+1]= 1;
    K2[1*3+2]= 0;

    K2[2*3+0]= 0;
    K2[2*3+1]= 0;
    K2[2*3+2]= 1;

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          R2[3*i+j]= P2[4*i+j];
       }
    }

    j= 3;
    for ( i= 0 ; i< 3 ; i++ ) {
       t2[i]= P2[4*i+j];
    }

    sfm_compute_C_from_R_t(
     R2,
     t2,
     C2
    );

    /*
    Initialize counter that keeps track of
    how many 3d points are in front of both cameras
    */

    count= 0;

    /*
    Loop on each match
    */

    for ( match_ind= 0 ; match_ind< match_nbr ; match_ind++ ) {

       /*
       Get normalized coordinates
       */

       x1_normalized= match_normalized_arr[match_ind].x1;
       y1_normalized= match_normalized_arr[match_ind].y1;
       x2_normalized= match_normalized_arr[match_ind].x2;
       y2_normalized= match_normalized_arr[match_ind].y2;

       /*
       Get non-normalized coordinates (for debugging purposes)
       */

       x1= match_arr[match_ind].x1;
       y1= match_arr[match_ind].y1;
       x2= match_arr[match_ind].x2;
       y2= match_arr[match_ind].y2;

       /*
       Compute the 3d point 
       */

       compute_linear_triangulation_two_cameras(
        P1,
        P2,
        x1_normalized,
        y1_normalized,
        x2_normalized,
        y2_normalized,
        X4
       );

       /*
       Go from homogeneous to inhomogeneous
       */

       X3[0]= X4[0]/X4[3];
       X3[1]= X4[1]/X4[3];
       X3[2]= X4[2]/X4[3];

       /*
       Compute the depth for the 1st camera
       */

       X4[0]= X3[0];
       X4[1]= X3[1];
       X4[2]= X3[2];
       X4[3]= 1;

       depth1= compute_3dpoint_depth(
        X4,
        P1,
        K1,
        R1,
        C1
       );

       /*
       Compute the depth for the 2nd camera
       */

       X4[0]= X3[0];
       X4[1]= X3[1];
       X4[2]= X3[2];
       X4[3]= 1;

       depth2= compute_3dpoint_depth(
        X4,
        P2,
        K2,
        R2,
        C2
       );

       if ( depth1 > 0 && depth2 > 0 )
        count++;
    }
    count4[ind]= count; 
 }

 /*
 The correct camera is the one
 with the largest number of 3d points that are in front of both cameras
 */

 max_ind=-1;
 max_count= 0;

 for ( ind= 0 ; ind< 4 ; ind++ ) {
    count= count4[ind];
    fprintf(stdout,"Number of 3D points in front of both cameras for solution %d = %d\n",
     ind,count);
    if ( count > max_count ) {
       max_count= count;
       max_ind= ind;
    }
 }

 count= max_count;
 ind= max_ind;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 4 ; j++ ) {
       P2[i*4+j]= P24[ind][i*4+j];
    }
 }

 /*
 It's probably a good idea to give an error
 if there is more than one camera P2 for which 3D points are in front of both cameras
 */

 err_flag= 0;

 nbr= 0;
 for ( ind= 0 ; ind< 4 ; ind++ ) {
    count= count4[ind];
    if ( count > 0 )
     nbr++;
 }

 if ( nbr > 1 )
  err_flag= 1; 

 return err_flag;

}

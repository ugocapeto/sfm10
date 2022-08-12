#include "header.h"

double ba_reprojection_error(
 ba_measurement_struct *measurement_arr,
 int measurement_nbr,
 ba_camera_internal_struct *camera_internal_arr,
 double *p_camera_arr,
 double *p_point_arr
)

/*
p_camera_arr is the array of external camera parameters
p_camera_arr[camera_ind*6+0]= omega3_x
p_camera_arr[camera_ind*6+1]= omega3_y
p_camera_arr[camera_ind*6+2]= omega3_z
p_camera_arr[camera_ind*6+3+0]= T_x
p_camera_arr[camera_ind*6+3+1]= T_y
p_camera_arr[camera_ind*6+3+2]= T_z
*/

/*
p_point_arr is the array of point parameters
p_point_arr[point_ind*3+0]= X
p_point_arr[point_ind*3+1]= Y
p_point_arr[point_ind*3+2]= Z
*/

/*
Compute the reprojection error as
the sum of distances between measurement and projected point
divided by the number of measurements
In other words, it is the average reprojection error
Not the same as what is done in ba_evaluate()
but the same as what is done in epnp_reprojection_error()
*/

{

 ba_camera_internal_struct camera_internal;
 double sum2;
 int measurement_ind;
 ba_measurement_struct measurement;
 int camera_ind;
 int point_ind;
 double x3[3];
 double x2[2];
 double fx;
 double skew;
 double cx;
 double fy;
 double cy;
 double k1;
 double k2;
 double p1;
 double p2;
 double K[3*3];
 double R[3*3];
 int i;
 int j;
 double omega3[3];
 double T[3];
 double x3_proj[3];
 double x2_proj[2];
 double norm2;
 double deriv2x6[2*6];
 double deriv6[6];
 double deriv2x3[2*3];
 double deriv3[3];
 double X3[3];
 double vec2[2];

 /*
 Initialize function value
 */

 sum2= 0;

 /*
 Go over each measurement
 Add contribution to
 sum2
 */

 for ( measurement_ind= 0 ;
       measurement_ind< measurement_nbr ;
       measurement_ind++ ) {

    measurement= measurement_arr[measurement_ind];
    camera_ind= measurement.camera_ind;
    point_ind= measurement.point_ind;
    x3[0]= measurement.x3[0];
    x3[1]= measurement.x3[1];
    x3[2]= measurement.x3[2];

    /*
    Go from homogeneous pixel coordinates to
    inhomogeneous pixel coordinates
    */

    x2[0]= x3[0]/x3[2];
    x2[1]= x3[1]/x3[2];

    /*
    Get the internal parameters for that camera
    */

    camera_internal= camera_internal_arr[camera_ind];
    fx= camera_internal.fx;
    skew= camera_internal.skew;
    cx= camera_internal.cx;
    fy= camera_internal.fy;
    cy= camera_internal.cy;
    k1= camera_internal.k1;
    k2= camera_internal.k2;
    p1= camera_internal.p1;
    p2= camera_internal.p2;

    /*
    Build the calibration matrix K
    */

    K[0*3+0]= fx;
    K[0*3+1]= skew;
    K[0*3+2]= cx;
    K[1*3+0]= 0;
    K[1*3+1]= fy;
    K[1*3+2]= cy;
    K[2*3+0]= 0;
    K[2*3+1]= 0;
    K[2*3+2]= 1;

    /*
    Get the current rotation vector for that camera
    */

    for ( i= 0 ; i< 3 ; i++ ) {
       omega3[i]= p_camera_arr[camera_ind*6+i];
    }

    /*
    Compute the rotation matrix
    */

    ba_create_rotation_matrix_rodrigues(
     omega3,
     R
    );

    /*
    Get the current translation for that camera
    */

    for ( i= 0 ; i< 3 ; i++ ) {
       T[i]= p_camera_arr[camera_ind*6+3+i];
    }

    /*
    Get the current location of the 3d point
    */

    X3[0]= p_point_arr[point_ind*3+0];
    X3[1]= p_point_arr[point_ind*3+1];
    X3[2]= p_point_arr[point_ind*3+2];

    /*
    Project the point onto the camera
    */

    ba_project_point(
     K,
     R,
     T,
     X3,
     x3_proj 
    );

    /*
    Go from homogeneous pixel coordinates to
    inhomogeneous pixel coordinates
    */

    x2_proj[0]= x3_proj[0]/x3_proj[2];
    x2_proj[1]= x3_proj[1]/x3_proj[2];

    /*
    Let's take care of function sum2
    (sum of the residuals)
    */

    for ( i= 0 ; i< 2 ; i++ )
     vec2[i]= x2_proj[i]-x2[i];

    /*
    Compute norm squared
    */

    norm2= math_vector_vector_product(
     vec2,
     2,
     vec2,
     2
    );

    /*
    Add to function value
    */

    sum2+= sqrt(norm2);
 }

 sum2/= (double)measurement_nbr;

 return sum2;

}

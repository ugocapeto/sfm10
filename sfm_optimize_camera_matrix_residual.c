#include "header.h"

void sfm_optimize_camera_matrix_residual(
 double *X_arr,
 double *x_arr,
 int n, 
 double P[3*4],
 double *res_vec
)

/*
Compute the residual vector
y - f(beta)
where beta are the parameters
y is the vector of measured 2d image points
f(beta) is the vector of projected 3d points onto the image
*/

{

 int ind;
 double x[3];
 double X[4];
 double xproj[3];

 for ( ind= 0 ; ind< n ; ind++ ) {
    x[0]= x_arr[3*ind+0];
    x[1]= x_arr[3*ind+1];
    x[2]= x_arr[3*ind+2];

    X[0]= X_arr[4*ind+0];
    X[1]= X_arr[4*ind+1];
    X[2]= X_arr[4*ind+2];
    X[3]= X_arr[4*ind+3];

    /*
    Compute the projection of the 3D point onto the image
    x = P*X
    */

    math_matrix_vector_product(
     P,
     3,
     4,
     X,
     4,
     xproj
    );

    res_vec[2*ind+0]= x[0]/x[2] - xproj[0]/xproj[2];
    res_vec[2*ind+1]= x[1]/x[2] - xproj[1]/xproj[2];
 }

}

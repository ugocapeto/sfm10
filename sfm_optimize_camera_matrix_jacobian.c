#include "header.h"

void sfm_optimize_camera_matrix_jacobian(
 double *X_arr,
 double *x_arr,
 int n, 
 double P[3*4],
 double *J
)

/*
Compute the Jacobian matrix of f
Row (2*i+0) contains the derivative of (PXi)[1]/(PXi)[3]
w/r to the parameters
Row (2*i+1) contains the derivative of (PXi)[2]/(PXi)[3]
w/r to the parameters
*/

/*
J has 2*n rows and 12 columns
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

    /*
    You really don't want xproj[3-1] to be 0!
    */

    if ( xproj[3-1] == 0 ) {
       sfm10_error_handler("sfm_optimize_camera_matrix_jacobian");
    }

    J[(2*ind+0)*12+ 0]= X[1-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+0)*12+ 1]= X[2-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+0)*12+ 2]= X[3-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+0)*12+ 3]= X[4-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+0)*12+ 4]= 0;
    J[(2*ind+0)*12+ 5]= 0;
    J[(2*ind+0)*12+ 6]= 0;
    J[(2*ind+0)*12+ 7]= 0;
    J[(2*ind+0)*12+ 8]=-X[1-1]*xproj[1-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+0)*12+ 9]=-X[2-1]*xproj[1-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+0)*12+10]=-X[3-1]*xproj[1-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+0)*12+11]=-X[4-1]*xproj[1-1]/(xproj[3-1]*xproj[3-1]);

    J[(2*ind+1)*12+ 0]= 0;
    J[(2*ind+1)*12+ 1]= 0;
    J[(2*ind+1)*12+ 2]= 0;
    J[(2*ind+1)*12+ 3]= 0;
    J[(2*ind+1)*12+ 4]= X[1-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+1)*12+ 5]= X[2-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+1)*12+ 6]= X[3-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+1)*12+ 7]= X[4-1]*xproj[3-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+1)*12+ 8]=-X[1-1]*xproj[2-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+1)*12+ 9]=-X[2-1]*xproj[2-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+1)*12+10]=-X[3-1]*xproj[2-1]/(xproj[3-1]*xproj[3-1]);
    J[(2*ind+1)*12+11]=-X[4-1]*xproj[2-1]/(xproj[3-1]*xproj[3-1]);
 }

}

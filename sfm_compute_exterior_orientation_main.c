#include "header.h"

void sfm_compute_exterior_orientation_main(
 double *xyz_arr,
 double *xy_arr,
 int n,
 double K[3*3],
 double R[3*3],
 double t[3],
 int lbfgs_max_iterations
)

{

 ba_measurement_struct *measurement_arr;
 int measurement_nbr;
 ba_camera_internal_struct *camera_internal_arr;
 double *p_camera_arr;
 double *p_point_arr;
 int *p_camera2_arr;
 int *p_point2_arr;
 double *p_grad_camera_arr;
 double *p_grad_point_arr;
 int p_camera_nbr;
 int p_point_nbr;
 double omega[3];
 int camera_matrix_ind;
 int point_ind;
 double xyz[3];
 double x;
 double y;
 int measurement_ind;
 double R_check[3*3];
 int p_point_ind;
 int xyz_ind;
 int xy_ind;
 double xy[2];

 p_camera_nbr= 1;

 /*
 Let's fill up camera_internal_arr
 */

 if ( p_camera_nbr > 0 )
  camera_internal_arr= (ba_camera_internal_struct *)calloc(p_camera_nbr,sizeof(ba_camera_internal_struct));

 camera_matrix_ind= 0;

 camera_internal_arr[camera_matrix_ind].fx  = K[0*3+0];
 camera_internal_arr[camera_matrix_ind].skew= K[0*3+1];
 camera_internal_arr[camera_matrix_ind].cx  = K[0*3+2];
 camera_internal_arr[camera_matrix_ind].fy  = K[1*3+1];
 camera_internal_arr[camera_matrix_ind].cy  = K[1*3+2];
 camera_internal_arr[camera_matrix_ind].k1  = 0;
 camera_internal_arr[camera_matrix_ind].k2  = 0;
 camera_internal_arr[camera_matrix_ind].p1  = 0;
 camera_internal_arr[camera_matrix_ind].p2  = 0;

 /*
 Let's fill up p_camera_arr
 */

 if ( p_camera_nbr > 0 )
  p_camera_arr= (double *)calloc(p_camera_nbr,6*sizeof(double));

 camera_matrix_ind= 0;

 /*
 Get the rotation vector
 */

 ba_create_rodrigues_param_from_rotation_matrix(
  R,
  omega
 ); 

 /*
 Let's make sure we get the same R
 when creating the rotation matrix from the rotation vector
 */

 ba_create_rotation_matrix_rodrigues(
  omega,
  R_check
 );

 p_camera_arr[camera_matrix_ind*6+0]= omega[0];
 p_camera_arr[camera_matrix_ind*6+1]= omega[1];
 p_camera_arr[camera_matrix_ind*6+2]= omega[2];
 p_camera_arr[camera_matrix_ind*6+3+0]= t[0];
 p_camera_arr[camera_matrix_ind*6+3+1]= t[1];
 p_camera_arr[camera_matrix_ind*6+3+2]= t[2];

 p_point_nbr= n;

 /*
 Let's fill up p_point_arr
 */

 if ( p_point_nbr > 0 )
  p_point_arr= (double *)calloc(p_point_nbr,3*sizeof(double));

 for ( p_point_ind= 0 ;
       p_point_ind< p_point_nbr ;
       p_point_ind++ ) {

    xyz_ind= p_point_ind;

    xyz[0]= xyz_arr[3*xyz_ind+0];
    xyz[1]= xyz_arr[3*xyz_ind+1];
    xyz[2]= xyz_arr[3*xyz_ind+2];

    p_point_arr[p_point_ind*3+0]= xyz[0];
    p_point_arr[p_point_ind*3+1]= xyz[1];
    p_point_arr[p_point_ind*3+2]= xyz[2];
 }

 /*
 Let's fill up p_camera2_arr
 p_camera2_arr != 0 constrains the corresponding camera parameter
 */

 if ( p_camera_nbr > 0 )
  p_camera2_arr= (int *)calloc(p_camera_nbr,6*sizeof(int));

 /*
 Let's fill up p_point2_arr
 p_point2_arr != 0 constrains the corresponding point parameter
 */

 if ( p_point_nbr > 0 )
  p_point2_arr= (int *)calloc(p_point_nbr,3*sizeof(int));

 /*
 Constrain the points
 */

 for ( p_point_ind= 0 ;
       p_point_ind< p_point_nbr ;
       p_point_ind++ ) {
    p_point2_arr[p_point_ind*3+0]= 1;
    p_point2_arr[p_point_ind*3+1]= 1;
    p_point2_arr[p_point_ind*3+2]= 1;
 }

 /*
 Force camera to have some arbitrary orientation/position
 for debugging
 */

 /*
 camera_matrix_ind= 0;
 p_camera_arr[camera_matrix_ind*6+0]=-1;
 p_camera_arr[camera_matrix_ind*6+1]= 0;
 p_camera_arr[camera_matrix_ind*6+2]=-2;
 p_camera_arr[camera_matrix_ind*6+3+0]=-1;
 p_camera_arr[camera_matrix_ind*6+3+1]=-2;
 p_camera_arr[camera_matrix_ind*6+3+2]=-1;
 */

 /*
 Let's fill measurement_arr
 */

 measurement_nbr= n;

 if ( measurement_nbr > 0 )
  measurement_arr= (ba_measurement_struct *)calloc(measurement_nbr+1,sizeof(ba_measurement_struct));

 for ( measurement_ind= 0 ;
       measurement_ind< measurement_nbr ;
       measurement_ind++ ) {

    xyz_ind= measurement_ind;

    xyz[0]= xyz_arr[3*xyz_ind+0];
    xyz[1]= xyz_arr[3*xyz_ind+1];
    xyz[2]= xyz_arr[3*xyz_ind+2];

    xy_ind= measurement_ind;

    xy[0]= xy_arr[2*xy_ind+0];
    xy[1]= xy_arr[2*xy_ind+1];

    /*
    Store
    camera/view index w/r to camera_matrix_arr
    3D point index
    image coordinates in homogeneous coordinates
    */

    camera_matrix_ind= 0;
    point_ind= xyz_ind;
    x= xy[0];
    y= xy[1];

    measurement_arr[measurement_ind].camera_ind= camera_matrix_ind;
    measurement_arr[measurement_ind].point_ind= point_ind;
    measurement_arr[measurement_ind].x3[0]= x;
    measurement_arr[measurement_ind].x3[1]= y;
    measurement_arr[measurement_ind].x3[2]= 1;
 }

 /*
 Allocate memory for p_grad_camera_arr
 Derivatives of function to minimize
 w/r to camera parameters
 */

 if ( p_camera_nbr > 0 )
  p_grad_camera_arr= (double *)calloc(p_camera_nbr,6*sizeof(double));

 /*
 Allocate memory for p_grad_point_arr
 Derivatives of function to minimize
 w/r to point parameters
 */

 if ( p_point_nbr > 0 )
  p_grad_point_arr= (double *)calloc(p_point_nbr,3*sizeof(double));

 ba_lbfgs_main(
  measurement_arr,
  measurement_nbr,
  camera_internal_arr,
  p_camera_arr,
  p_point_arr,
  p_camera2_arr,
  p_point2_arr,
  p_grad_camera_arr,
  p_grad_point_arr,
  p_camera_nbr,
  p_point_nbr,
  lbfgs_max_iterations
 );

 /*
 Update the camera matrices
 */

 camera_matrix_ind= 0;

 /*
 Get new rotation vector and translation vector
 as found by the solver
 */

 omega[0]= p_camera_arr[camera_matrix_ind*6+0];
 omega[1]= p_camera_arr[camera_matrix_ind*6+1];
 omega[2]= p_camera_arr[camera_matrix_ind*6+2];
 t[0]= p_camera_arr[camera_matrix_ind*6+3+0];
 t[1]= p_camera_arr[camera_matrix_ind*6+3+1];
 t[2]= p_camera_arr[camera_matrix_ind*6+3+2];

 /*
 Compute rotation matrix from rotation vector
 */

 ba_create_rotation_matrix_rodrigues(
  omega,
  R
 );

 /*
 Free camera_internal_arr
 */

 if ( p_camera_nbr > 0 )
  free(camera_internal_arr);

 /*
 Free p_camera_arr
 */

 if ( p_camera_nbr > 0 )
  free(p_camera_arr);

 /*
 Free p_camera2_arr
 */

 if ( p_camera_nbr > 0 )
  free(p_camera2_arr);

 /*
 Free p_point_arr
 */

 if ( p_point_nbr > 0 )
  free(p_point_arr);

 /*
 Free p_point2_arr
 */

 if ( p_point_nbr > 0 )
  free(p_point2_arr);

 /*
 Free p_grad_camera_arr
 */

 if ( p_camera_nbr > 0 )
  free(p_grad_camera_arr);

 /*
 Free p_grad_point_arr
 */

 if ( p_point_nbr > 0 )
  free(p_grad_point_arr);

 /*
 Free measurement_arr
 */

 if ( measurement_nbr > 0 )
  free(measurement_arr);

}

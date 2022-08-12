#include "header.h"

ba_measurement_struct *global_measurement_arr;
int global_measurement_nbr;
ba_camera_internal_struct *global_camera_internal_arr;
double *global_p_camera_arr;
double *global_p_point_arr;
int *global_p_camera2_arr;
int *global_p_point2_arr;
double *global_p_grad_camera_arr;
double *global_p_grad_point_arr;
int global_p_camera_nbr;
int global_p_point_nbr;

void ba_lbfgs_main(
 ba_measurement_struct *measurement_arr,
 int measurement_nbr,
 ba_camera_internal_struct *camera_internal_arr,
 double *p_camera_arr,
 double *p_point_arr,
 int *p_camera2_arr,
 int *p_point2_arr,
 double *p_grad_camera_arr,
 double *p_grad_point_arr,
 int p_camera_nbr,
 int p_point_nbr,
 int max_iterations
)

/*
p_camera_arr contains the initial rotation vectors and translation vectors
p_point_arr contains the point coordinates in 3d euclidean space
if p_camera2_arr != 0, the corresponding camera parameter is constrained
if p_point2_arr != 0, the corresponding point parameter is constrained
p_grad_camera_arr is allocated in calling function (contains nothing)
p_grad_point_arr is allocated in calling function (contains nothing)
*/

{

 double avg_reproj_error;

 global_measurement_arr= measurement_arr;
 global_measurement_nbr= measurement_nbr;
 global_camera_internal_arr= camera_internal_arr;
 global_p_camera_arr= p_camera_arr;
 global_p_point_arr= p_point_arr;
 global_p_camera2_arr= p_camera2_arr;
 global_p_point2_arr= p_point2_arr;
 global_p_grad_camera_arr= p_grad_camera_arr;
 global_p_grad_point_arr= p_grad_point_arr;
 global_p_camera_nbr= p_camera_nbr;
 global_p_point_nbr= p_point_nbr;

 /*
 Get average reprojection error
 */

 avg_reproj_error= ba_reprojection_error(
  measurement_arr,
  measurement_nbr,
  camera_internal_arr,
  p_camera_arr,
  p_point_arr
 );

 fprintf(stdout,"Average reprojection error (before lbfgs optimization) = %g\n",
  avg_reproj_error);

 /*
 Optimize the camera rotation and translation
 Optimize the 3d point locations
 */

 ba_lbfgs(
  max_iterations
 );

 /*
 Get average reprojection error
 */

 avg_reproj_error= ba_reprojection_error(
  measurement_arr,
  measurement_nbr,
  camera_internal_arr,
  p_camera_arr,
  p_point_arr
 );

 fprintf(stdout,"Average reprojection error (after lbfgs optimization) = %g\n",
  avg_reproj_error);

}

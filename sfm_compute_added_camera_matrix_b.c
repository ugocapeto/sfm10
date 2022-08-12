#include "header.h"

void sfm_compute_added_camera_matrix_b(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 int camera_ind,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double t[3],
 int lbfgs_max_iterations
)

/*
Constrain the calibration matrix and
recompute R and t (exterior orientation)
*/

{

 double focal_length;
 int width;
 int height;
 double KR[3*3];
 double Kt[3];
 int i;
 int j;
 char image_filename[80];
 int err_flag;
 int *image_arr;

 /*
 Get filename for image
 */

 strcpy(image_filename,image_filename_arr[camera_ind]);

 /*
 Load image
 */

 err_flag= load_rgb_image(
  image_filename,
  &image_arr,
  &width,
  &height
 );

 free(image_arr);

 /*
 Get the focal length
 */

 focal_length= focal_length_arr[camera_ind];

 /*
 Compute the calibration matrix
 */

 /*
 It is assumed that the origin of the image coordinate system
 is at top-left with y axis going down
 It is also assumed that the y axis of the camera coordinate system
 points downward like in VisualSFM
 */

 K[0*3+0]= focal_length;
 K[0*3+1]= 0;
 K[0*3+2]= (double)width/2;

 K[1*3+0]= 0;
 K[1*3+1]= focal_length;
 K[1*3+2]= (double)height/2;

 K[2*3+0]= 0;
 K[2*3+1]= 0;
 K[2*3+2]= 1;

 /*
 Use bundle adjustment to adjust R and t
 with K constrained
 */

 sfm_compute_added_camera_matrix_exterior_orientation(
  image_filename_arr,
  camera_nbr,
  camera_ind,
  threedpoint_arr,
  threedpoint_nbr,
  K,
  R,
  t,
  lbfgs_max_iterations
 );

 /*
 Compute P = K[R|t]
 */

 math_matrix_matrix_product(
  K,
  3,
  3,
  R,
  3,
  3,
  KR
 );

 math_matrix_vector_product(
  K,
  3,
  3,
  t,
  3,
  Kt
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P[i*4+j]= KR[i*3+j];
    }
    j= 3;
    P[i*4+j]= Kt[i];
 }

}

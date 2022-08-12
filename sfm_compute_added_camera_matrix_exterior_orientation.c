#include "header.h"

void sfm_compute_added_camera_matrix_exterior_orientation(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double K[3*3],
 double R[3*3],
 double t[3],
 int lbfgs_max_iterations
)

/*
K = calibration matrix (input)
R = camera rotation matrix (input/output) 
t = camera translation vector (input/output) 
*/

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double *xyz_arr;
 double *xy_arr;
 int n;
 double xyz[3];
 int rgb[3];
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int camera_ind;
 int threedpoint_ind;
 int feat_ind;
 double x;
 double y;
 int ind;
 char image_filename[80];
 int err_flag;
 int *image_arr;
 int width;
 int height;

 /*
 Get filename for input image
 */

 strcpy(image_filename,image_filename_arr[inp_camera_ind]);

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
 Initialize 3D points and image points
 for input image
 */

 xyz_arr= 0;
 xy_arr= 0;
 n= 0;

 /*
 Loop through the 3D points
 */

 for ( threedpoint_ind= 0 ;
       threedpoint_ind< threedpoint_nbr ;
       threedpoint_ind++ ) {

    xyz[0]= threedpoint_arr[threedpoint_ind].xyz[0];
    xyz[1]= threedpoint_arr[threedpoint_ind].xyz[1];
    xyz[2]= threedpoint_arr[threedpoint_ind].xyz[2];
    rgb[0]= threedpoint_arr[threedpoint_ind].rgb[0];
    rgb[1]= threedpoint_arr[threedpoint_ind].rgb[1];
    rgb[2]= threedpoint_arr[threedpoint_ind].rgb[2];
    imagepoint_arr= threedpoint_arr[threedpoint_ind].imagepoint_arr;
    imagepoint_nbr= threedpoint_arr[threedpoint_ind].imagepoint_nbr;

    for ( imagepoint_ind= 0 ;
          imagepoint_ind< imagepoint_nbr ;
          imagepoint_ind++ ) {
       imagepoint= imagepoint_arr[imagepoint_ind];
       camera_ind= imagepoint.camera_ind;
       feat_ind= imagepoint.feat_ind;
       x= imagepoint.x;
       y= imagepoint.y;

       if ( camera_ind != inp_camera_ind )
        continue;

       /*
       Add 3D point and image position
       */

       if ( n == 0 ) {
          xyz_arr= (double *)calloc(n+1,3*sizeof(double));
          xy_arr= (double *)calloc(n+1,2*sizeof(double));
       }
       else {
          xyz_arr= (double *)realloc(xyz_arr,(n+1)*3*sizeof(double));
          xy_arr= (double *)realloc(xy_arr,(n+1)*2*sizeof(double));
       }
       ind= n;
       n++;
       xyz_arr[3*ind+0]= xyz[0];
       xyz_arr[3*ind+1]= xyz[1];
       xyz_arr[3*ind+2]= xyz[2];
       xy_arr[2*ind+0]= x;
       xy_arr[2*ind+1]= y;
    }
 }

 /*
 Compute the camera pose (R and t) aka exterior orientation
 using bundle adjustment
 */

 sfm_compute_exterior_orientation_main(
  xyz_arr,
  xy_arr,
  n,
  K,
  R,
  t,
  lbfgs_max_iterations
 );

 /*
 Free memory
 */

 if ( n > 0 ) {
    free(xyz_arr);
    free(xy_arr);
 }

}

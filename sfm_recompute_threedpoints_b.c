#include "header.h"

void sfm_recompute_threedpoints_b(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

/*
Difference with sfm_recompute_threedpoints() is that here
the image point coordinates and the camera matrices are normalized
using the calibration matrix
*/

{

 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 char image_filename[80];
 int width0;
 int height0;
 int **all_image_arr;
 int camera_ind;
 int err_flag;
 int *image_arr;
 int width;
 int height;

 /*
 Load up all the images
 */

 width0= 0;
 height0= 0;

 all_image_arr= (int **)calloc(camera_nbr,sizeof(int *));

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {

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

    if ( width0 == 0 )
     width0= width;
    else if ( width != width0 ) {
       sfm10_error_handler("sfm_recompute_threedpoints");
    } 
    if ( height0 == 0 )
     height0= height;
    else if ( height != height0 ) {
       sfm10_error_handler("sfm_recompute_threedpoints");
    }

    all_image_arr[camera_ind]= image_arr;
 }

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

    sfm_recompute_threedpoint_b(
     image_filename_arr,
     camera_nbr,
     camera_matrix_arr,
     camera_matrix_nbr,
     width0,
     height0,
     all_image_arr,
     threedpoint_arr,
     threedpoint_nbr,
     threedpoint_ind
    );
 }

 /*
 Free the images
 */

 for ( camera_ind= 0 ; 
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    free(all_image_arr[camera_ind]);
 }
 if ( camera_nbr > 0 )
  free(all_image_arr);

}

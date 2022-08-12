#include "header.h"

void sfm_get_next_camera(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int *pcamera_ind,
 int *pn
)

/*
Get the next camera to add
It's the one with the most matches with current 3D reconstruction
*/

{

 int max_n;
 int max_camera_ind;
 int camera_ind;
 int flag;
 int camera_matrix_ind;
 double *xyz_arr;
 double *xy_arr;
 int n;
 int camera_ind2;

 max_n= 0;
 max_camera_ind=-1;

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {

    /*
    See if camera has already been added to 3D reconstruction
    */

    flag= 0;
    for ( camera_matrix_ind= 0 ;
          camera_matrix_ind< camera_matrix_nbr ;
          camera_matrix_ind++ ) {
       camera_ind2= camera_matrix_arr[camera_matrix_ind].camera_ind;
       if ( camera_ind2 == camera_ind )
        flag= 1;
    }

    if ( flag == 1 )
     continue;

    /*
    If here,
    camera has not been added yet to the 3d reconstruction
    */

    /*
    Get the image points for which 3D points exist
    */

    sfm_get_next_camera_2(
     image_filename_arr,
     camera_nbr,
     camera_ind,
     camera_matrix_arr,
     camera_matrix_nbr,
     threedpoint_arr,
     threedpoint_nbr,
     &xyz_arr,
     &xy_arr,
     &n
    );

    if ( n > max_n ) {
       max_camera_ind= camera_ind;
       max_n= n;
    }

    /*
    Free the image points and the 3D points
    */

    if ( n > 0 ) {
       free(xyz_arr);
       free(xy_arr);
    }
 }

 camera_ind= max_camera_ind;
 n= max_n;

 (*pcamera_ind)= camera_ind;
 (*pn)= n;

}

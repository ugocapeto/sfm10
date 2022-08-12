#include "header.h"

void sfm_get_camera_matrix_ind(
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int inp_camera_ind,
 int *pcamera_matrix_ind
)

{

 int camera_matrix_ind; 
 int camera_ind;

 for ( camera_matrix_ind= 0 ;
       camera_matrix_ind< camera_matrix_nbr ;
       camera_matrix_ind++ ) {
    camera_ind= camera_matrix_arr[camera_matrix_ind].camera_ind; 
    if ( camera_ind == inp_camera_ind )
     break;
 }

 if ( !( camera_matrix_ind < camera_matrix_nbr ) ) {
    sfm10_error_handler("sfm_get_camera_matrix_ind");
 }

 (*pcamera_matrix_ind)= camera_matrix_ind;

}

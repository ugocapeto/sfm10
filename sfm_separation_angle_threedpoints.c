#include "header.h"

void sfm_separation_angle_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double min_separation_angle
)

{

 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 int err_flag;

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

    err_flag= sfm_separation_angle_threedpoint(
     image_filename_arr,
     camera_nbr,
     camera_matrix_arr,
     camera_matrix_nbr,
     threedpoint_arr,
     threedpoint_nbr,
     threedpoint_ind,
     min_separation_angle
    );
 }

}

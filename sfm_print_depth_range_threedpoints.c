#include "header.h"

void sfm_print_depth_range_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

{

 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double min_z;
 double max_z;
 double avg_z;
 double z;

 min_z=+1.0e32;
 max_z=-1.0e32;
 avg_z= 0;

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

    z= xyz[2]; 

    if ( z < min_z ) {
       min_z= z;
    }
    if ( z > max_z ) {
       max_z= z;
    }
    avg_z+= z;

 }

 avg_z/= (double)threedpoint_nbr;

 fprintf(stdout,"Average depth = %g min depth = %g max depth = %g\n",
  avg_z,min_z,max_z);

}

#include "header.h"

int sfm_reprojection_error_threedpoint(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int threedpoint_ind,
 double max_reproj_error
)

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double xyz[3];
 int rgb[3];
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int feat_ind;
 double x;
 double y;
 int camera_ind;
 double P[3*4];
 double K[3*3];
 double R[3*3];
 double t[3];
 int camera_matrix_ind;
 double x3[3];
 double x_proj;
 double y_proj;
 double dist2;
 double dist;
 int err_flag;

 xyz[0]= threedpoint_arr[threedpoint_ind].xyz[0];
 xyz[1]= threedpoint_arr[threedpoint_ind].xyz[1];
 xyz[2]= threedpoint_arr[threedpoint_ind].xyz[2];
 rgb[0]= threedpoint_arr[threedpoint_ind].rgb[0];
 rgb[1]= threedpoint_arr[threedpoint_ind].rgb[1];
 rgb[2]= threedpoint_arr[threedpoint_ind].rgb[2];
 imagepoint_arr= threedpoint_arr[threedpoint_ind].imagepoint_arr;
 imagepoint_nbr= threedpoint_arr[threedpoint_ind].imagepoint_nbr;

 err_flag= 0;

 /*
 Loop over the image points the 3D point projects onto
 */

 for ( imagepoint_ind= 0 ;
       imagepoint_ind< imagepoint_nbr ;
       imagepoint_ind++ ) {
    imagepoint= imagepoint_arr[imagepoint_ind];
    camera_ind= imagepoint.camera_ind;
    feat_ind= imagepoint.feat_ind;
    x= imagepoint.x;
    y= imagepoint.y;

    /*
    Get camera matrix for camera
    */

    sfm_get_camera_matrix_ind(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera_ind,
     &camera_matrix_ind
    );

    sfm_get_camera_matrix(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera_matrix_ind,
     &camera_ind,
     P,
     K,
     R,
     t
    );

    /*
    Compute the projection of the 3D point onto the image
    */

    ba_project_point(
     K,
     R,
     t,
     xyz,
     x3
    );

    x_proj= x3[0]/x3[2];
    y_proj= x3[1]/x3[2];

    dist2= (x_proj-x)*(x_proj-x)+
           (y_proj-y)*(y_proj-y);
    dist= sqrt(dist2);

    /*
    Check the reprojection error
    */

    if ( dist > max_reproj_error ) {
       err_flag= 1;
    }
 }

 return err_flag;

}

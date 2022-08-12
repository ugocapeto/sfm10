#include "header.h"

void sfm_print_reprojection_error_threedpoints(
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
 int nbr;
 double avg_reproj_error;
 double max_reproj_error;
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int camera_ind;
 int feat_ind;
 double x;
 double y;
 int camera_matrix_ind;
 double P[3*4];
 double K[3*3];
 double R[3*3];
 double t[3];
 double x3[3];
 double x_proj;
 double y_proj;
 double dist2;
 double dist;

 nbr= 0;
 avg_reproj_error= 0;
 max_reproj_error= 0;

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

       avg_reproj_error+= dist;

       if ( dist > max_reproj_error ) {
          max_reproj_error= dist;
       }

       nbr++;
    }
 }

 avg_reproj_error/= (double)nbr;

 fprintf(stdout,"Number of 3D points = %d\n",
  threedpoint_nbr);
 fprintf(stdout,"Average reprojection error = %g\n",
  avg_reproj_error);
 fprintf(stdout,"Max reprojection error = %g\n",
  max_reproj_error);

}

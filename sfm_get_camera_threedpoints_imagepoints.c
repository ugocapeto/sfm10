#include "header.h"

void sfm_get_camera_threedpoints_imagepoints(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double **pxyz_arr,
 double **pxy_arr,
 int *pn
)

/*
Get the 3d points and projections
*/

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double *xyz_arr;
 double *xy_arr;
 int n;
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int camera_ind;
 int threedpoint_ind;
 int feat_ind;
 double x;
 double y;
 int ind;
 double xyz[3];
 int rgb[3];
 int keep_imagepoint_ind;

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

    /*
    Find the imagepoint for the input camera
    */

    keep_imagepoint_ind=-1;

    for ( imagepoint_ind= 0 ;
          imagepoint_ind< imagepoint_nbr ;
          imagepoint_ind++ ) {
       imagepoint= imagepoint_arr[imagepoint_ind];
       camera_ind= imagepoint.camera_ind;
       feat_ind= imagepoint.feat_ind;
       x= imagepoint.x;
       y= imagepoint.y;
       if ( camera_ind == inp_camera_ind ) {
          if ( keep_imagepoint_ind == -1 ) {
             keep_imagepoint_ind= imagepoint_ind;
          } 
          else {
             sfm10_error_handler("sfm_get_camera_threedpoints_imagepoints");
          }
       }
    }

    imagepoint_ind= keep_imagepoint_ind;

    if ( imagepoint_ind == -1 )
     continue;

    imagepoint= imagepoint_arr[imagepoint_ind];
    camera_ind= imagepoint.camera_ind;
    feat_ind= imagepoint.feat_ind;
    x= imagepoint.x;
    y= imagepoint.y;

    /*
    Add 3D point and image point
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

 (*pxyz_arr)= xyz_arr;
 (*pxy_arr)= xy_arr;
 (*pn)= n;

}

#include "header.h"

void sfm_get_next_camera_2(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double **pxyz_arr,
 double **pxy_arr,
 int *pn
)

/*
Same as sfm_get_added_camera_points_projections()
except that here threedpoint_arr is not modified
*/

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double *xyz_arr;
 double *xy_arr;
 int n;
 match_struct **cam_match_arr;
 int *cam_match_nbr;
 char filename_match[80];
 double xyz[3];
 int rgb[3];
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int camera_ind;
 match_struct *match_arr;
 int match_nbr;
 int threedpoint_ind;
 int feat_ind;
 double x;
 double y;
 int match_ind;
 match_struct match;
 int feat1_ind;
 double x1;
 double y1;
 int feat2_ind;
 double x2;
 double y2;
 int ind;
 char image_filename[80];
 int err_flag;
 int *image_arr;
 int width;
 int height;
 int camera1_ind;
 int camera2_ind;
 int inp_feat_ind;
 double inp_x;
 double inp_y;

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

 xyz_arr= 0;
 xy_arr= 0;
 n= 0;

 /*
 For each camera that's not the input camera,
 retrieve the matches it has with input camera
 */

 cam_match_arr= (match_struct **)calloc(camera_nbr,sizeof(match_struct *));
 cam_match_nbr= (int *)calloc(camera_nbr,sizeof(int));

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {

    if ( camera_ind == inp_camera_ind )
     continue;

    /*
    Sort (camera_ind,inp_camera_ind)
    in ascending order
    */

    if ( camera_ind < inp_camera_ind ) {
       camera1_ind= camera_ind;
       camera2_ind= inp_camera_ind;
    }
    if ( inp_camera_ind < camera_ind ) {
       camera1_ind= inp_camera_ind;
       camera2_ind= camera_ind;
    }

    sprintf(filename_match,"best_match_%d_%d.sfm",
     camera1_ind,camera2_ind);

    sfm_read_matches(
     filename_match,
     &match_arr,
     &match_nbr
    );

    cam_match_arr[camera_ind]= match_arr;
    cam_match_nbr[camera_ind]= match_nbr;
 }

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
    Let's go through the image points for that 3D point
    We want to find an image point that
    matches an image point in the input camera
    */

    inp_feat_ind=-1;
    inp_x= 0;
    inp_y= 0;

    for ( imagepoint_ind= 0 ;
          imagepoint_ind< imagepoint_nbr ;
          imagepoint_ind++ ) {
       imagepoint= imagepoint_arr[imagepoint_ind];
       camera_ind= imagepoint.camera_ind;
       feat_ind= imagepoint.feat_ind;
       x= imagepoint.x;
       y= imagepoint.y;

       /*
       Let's see if feat_ind of camera_ind
       shows up in the matches between inp_camera_ind and camera_ind
       */

       match_arr= cam_match_arr[camera_ind];
       match_nbr= cam_match_nbr[camera_ind];

       for ( match_ind= 0 ;
             match_ind< match_nbr ;
             match_ind++ ) {
          match= match_arr[match_ind];
          feat1_ind= match.feat1_ind;
          x1= match.x1;
          y1= match.y1;
          feat2_ind= match.feat2_ind;
          x2= match.x2;
          y2= match.y2;

          if ( camera_ind < inp_camera_ind ) {

             /*
             feat1_ind, x1, y1 correspond to camera_ind
             feat2_ind, x2, y2 correspond to inp_camera_ind
             */

             if ( feat1_ind == feat_ind ) {
                inp_feat_ind= feat2_ind;
                inp_x= x2;
                inp_y= y2;
                break;
             }
          }

          if ( inp_camera_ind < camera_ind ) {
             
             /*
             feat1_ind, x1, y1 correspond to inp_camera_ind
             feat2_ind, x2, y2 correspond to camera_ind
             */

             if ( feat2_ind == feat_ind ) {
                inp_feat_ind= feat1_ind;
                inp_x= x1;
                inp_y= y1;
                break;
             }
          }
       } /* loop on match_ind */

       if ( inp_feat_ind != -1 )
        break;
    } /* loop on imagepoint_ind */

    if ( inp_feat_ind == -1 )
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
    xy_arr[2*ind+0]= inp_x;
    xy_arr[2*ind+1]= inp_y;
 }

 /*
 Free memory
 */

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    if ( cam_match_nbr[camera_ind] > 0 )
     free(cam_match_arr[camera_ind]);
 }
 if ( camera_nbr > 0 ) {
    free(cam_match_arr);
    free(cam_match_nbr);
 }

 (*pxyz_arr)= xyz_arr;
 (*pxy_arr)= xy_arr;
 (*pn)= n;

}

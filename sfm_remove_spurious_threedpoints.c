#include "header.h"

void sfm_remove_spurious_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int ***pfeat2threedpoint_arr,
 int **pfeat2threedpoint_nbr,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr,
 double min_separation_angle,
 double max_reproj_error
)

/*
We're gonna rebuild feat2threedpoint_arr and threedpoint_arr,
delete the passed feat2threedpoint_arr and threedpoint_arr, and
return the newly built feat2threedpoint_arr and threedpoint_arr
*/

/*
feat2threedpoint_arr is passed by reference
because it's gonna be replaced
*/

/*
threedpoint_arr is passed by reference
because it's gonna be replaced
*/

{

 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 int **inp_feat2threedpoint_arr;
 int *inp_feat2threedpoint_nbr;
 sfm_threedpoint_struct *inp_threedpoint_arr;
 int inp_threedpoint_nbr;
 sfm_threedpoint_struct *threedpoint_arr;
 int threedpoint_nbr;
 int **feat2threedpoint_arr;
 int *feat2threedpoint_nbr;
 int camera_ind;
 int feat_nbr;
 int feat_ind;
 int inp_threedpoint_ind;
 sfm_imagepoint_struct *inp_imagepoint_arr;
 int err_flag;
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 double x;
 double y;
 int count0;
 int count1;
 int count2;
 int count3;

 inp_feat2threedpoint_arr= *pfeat2threedpoint_arr;
 inp_feat2threedpoint_nbr= *pfeat2threedpoint_nbr;

 inp_threedpoint_arr= *pthreedpoint_arr;
 inp_threedpoint_nbr= *pthreedpoint_nbr;

 /*
 Initialize the 3D points
 */

 threedpoint_arr= 0;
 threedpoint_nbr= 0; 

 /*
 Initialize feat2threedpoint_arr
 */

 if ( camera_nbr > 0 ) {
    feat2threedpoint_arr= (int **)calloc(camera_nbr,sizeof(int *));
    feat2threedpoint_nbr= (int *)calloc(camera_nbr,sizeof(int));
 }
 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    feat_nbr= inp_feat2threedpoint_nbr[camera_ind];
    feat2threedpoint_nbr[camera_ind]= feat_nbr;
    if ( feat_nbr > 0 )
     feat2threedpoint_arr[camera_ind]= (int *)calloc(feat_nbr,sizeof(int));
    for ( feat_ind= 0 ;
          feat_ind< feat_nbr ;
          feat_ind++ )
     feat2threedpoint_arr[camera_ind][feat_ind]=-1;
 }

 /*
 We are gonna keep track of 3D points that are rejected because:
 - they don't exist anymore
 - they are behind the reference camera
 - the separation angle is too low
 - the reprojection error is too high
 */

 count0= 0;
 count1= 0;
 count2= 0;
 count3= 0;

 /*
 Loop through the current 3D points
 */

 for ( inp_threedpoint_ind= 0 ;
       inp_threedpoint_ind< inp_threedpoint_nbr ;
       inp_threedpoint_ind++ ) {

    xyz[0]= inp_threedpoint_arr[inp_threedpoint_ind].xyz[0];
    xyz[1]= inp_threedpoint_arr[inp_threedpoint_ind].xyz[1];
    xyz[2]= inp_threedpoint_arr[inp_threedpoint_ind].xyz[2];
    rgb[0]= inp_threedpoint_arr[inp_threedpoint_ind].rgb[0];
    rgb[1]= inp_threedpoint_arr[inp_threedpoint_ind].rgb[1];
    rgb[2]= inp_threedpoint_arr[inp_threedpoint_ind].rgb[2];
    inp_imagepoint_arr= inp_threedpoint_arr[inp_threedpoint_ind].imagepoint_arr;
    imagepoint_nbr= inp_threedpoint_arr[inp_threedpoint_ind].imagepoint_nbr;

    /*
    Check if 3D point actually exists
    */

    if ( imagepoint_nbr == 0 ) {

       /*
       Do not keep this 3D point
       */

       count0++;
       continue;
    }

    /*
    Reject any 3D point that's behind the camera
    */

    if ( !(xyz[2] > 0) ) {

       /*
       Do not keep this 3D point
       */

       count1++;
       continue;
    }

    /*
    See if this 3D point is low confidence
    */

    /*
    According to Noah Snavely's thesis,
    a 3D point is low confidence if:
    1) the 3D point is seen by fewer than three cameras
    2) the max angle over all pairs of rays that see the 3D point
       is less than a threshold theta_max (he uses 1.5 degrees)
    */

    /*
    Reject any 3D point with a separation angle that's too small
    */

    err_flag= sfm_separation_angle_threedpoint(
     image_filename_arr,
     camera_nbr,
     camera_matrix_arr,
     camera_matrix_nbr,
     inp_threedpoint_arr,
     inp_threedpoint_nbr,
     inp_threedpoint_ind,
     min_separation_angle
    );

    if ( err_flag == 1 ) {

       /*
       Do not keep this 3D point
       */

       count2++;
       continue;
    }

    /*
    Reject any 3D point with a reprojection error that's too large
    */

    err_flag= sfm_reprojection_error_threedpoint(
     image_filename_arr,
     camera_nbr,
     camera_matrix_arr,
     camera_matrix_nbr,
     inp_threedpoint_arr,
     inp_threedpoint_nbr,
     inp_threedpoint_ind,
     max_reproj_error
    );

    if ( err_flag == 1 ) {

       /*
       Do not keep this 3D point
       */

       count3++;
       continue;
    }

    /*
    Store the 3D point in threedpoint_arr
    */

    if ( threedpoint_nbr == 0 ) {
       threedpoint_arr= (sfm_threedpoint_struct *)calloc(threedpoint_nbr+1,sizeof(sfm_threedpoint_struct));
    }
    else {
       threedpoint_arr= (sfm_threedpoint_struct *)realloc(threedpoint_arr,(threedpoint_nbr+1)*sizeof(sfm_threedpoint_struct));
    }
    threedpoint_ind= threedpoint_nbr;
    threedpoint_nbr++;

    imagepoint_arr= (sfm_imagepoint_struct *)calloc(imagepoint_nbr,sizeof(sfm_imagepoint_struct));

    for ( imagepoint_ind= 0 ;
          imagepoint_ind< imagepoint_nbr ;
          imagepoint_ind++ ) {
       imagepoint= inp_imagepoint_arr[imagepoint_ind];
       camera_ind= imagepoint.camera_ind;
       feat_ind= imagepoint.feat_ind;
       x= imagepoint.x;
       y= imagepoint.y;
       imagepoint_arr[imagepoint_ind]= imagepoint;

       /*
       Indicate fact that 3D point has been created
       for feature in image
       */

       feat2threedpoint_arr[camera_ind][feat_ind]= threedpoint_ind;
    }

    threedpoint_arr[threedpoint_ind].xyz[0]= xyz[0];
    threedpoint_arr[threedpoint_ind].xyz[1]= xyz[1];
    threedpoint_arr[threedpoint_ind].xyz[2]= xyz[2];
    threedpoint_arr[threedpoint_ind].rgb[0]= rgb[0];
    threedpoint_arr[threedpoint_ind].rgb[1]= rgb[1];
    threedpoint_arr[threedpoint_ind].rgb[2]= rgb[2];
    threedpoint_arr[threedpoint_ind].imagepoint_arr= imagepoint_arr;
    threedpoint_arr[threedpoint_ind].imagepoint_nbr= imagepoint_nbr;
 }

 fprintf(stdout,"Number of 3D points that don't exist anymore = %d\n",
  count0);
 fprintf(stdout,"Number of 3D points behind reference camera = %d\n",
  count1);
 fprintf(stdout,"Number of 3D points with separation angle too low = %d\n",
  count2);
 fprintf(stdout,"Number of 3D points with reprojection error too high = %d\n",
  count3);

 /*
 Free inp_threedpoint_arr
 */

 sfm_free_threedpoints(
  inp_threedpoint_arr,
  inp_threedpoint_nbr
 );

 /*
 Free inp_feat2threedpoint_arr
 */

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    if ( inp_feat2threedpoint_nbr[camera_ind] > 0 )
     free(inp_feat2threedpoint_arr[camera_ind]);
 }
 if ( camera_nbr > 0 ) {
    free(inp_feat2threedpoint_arr);
    free(inp_feat2threedpoint_nbr);
 }

 *pfeat2threedpoint_arr= feat2threedpoint_arr;
 *pfeat2threedpoint_nbr= feat2threedpoint_nbr;

 *pthreedpoint_arr= threedpoint_arr;
 *pthreedpoint_nbr= threedpoint_nbr;

}

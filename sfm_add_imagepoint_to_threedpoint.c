#include "header.h"

void sfm_add_imagepoint_to_threedpoint(
 int inp_camera_ind,
 int inp_feat_ind,
 double inp_x,
 double inp_y,
 int threedpoint_ind,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

/*
Add image point to 3D point
unless already there
When I say "already there", I mean the exact same image point
since it can happen that
I am trying to add an image point that was added previously
Recall that in sfm, in a track, there can only be one image point per camera
*/

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int camera_ind;
 int feat_ind;
 double x;
 double y;
 int count;

 imagepoint_arr= threedpoint_arr[threedpoint_ind].imagepoint_arr;
 imagepoint_nbr= threedpoint_arr[threedpoint_ind].imagepoint_nbr;

 /*
 Check if image point already there
 */

 /*
 It is assumed that
 if an image point is found on inp_camera_ind,
 it better be (inp_camera_ind,inp_feat_ind,inp_x,inp_y)
 */

 count= 0;
 for ( imagepoint_ind= 0 ;
       imagepoint_ind< imagepoint_nbr ;
       imagepoint_ind++ ) {
    imagepoint= imagepoint_arr[imagepoint_ind];
    camera_ind= imagepoint.camera_ind;
    feat_ind= imagepoint.feat_ind;
    x= imagepoint.x;
    y= imagepoint.y;
    if ( camera_ind == inp_camera_ind ) {
       if ( feat_ind != inp_feat_ind ||
            x != inp_x ||
            y != inp_y ) {
          sfm10_error_handler("sfm_add_imagepoint_to_threedpoint");
       }
       count++;
    }
 }
 if ( count > 0 ) {
    if ( count > 1 ) {
       sfm10_error_handler("sfm_add_imagepoint_to_threedpoint");
    }

    /*
    Do not add to image points of 3D point
    */

    goto END;
 }

 /*
 Need to add
 (inp_camera_ind,inp_feat_ind,inp_x,inp_y)
 to the list of image points for 3D point
 */

 if ( imagepoint_nbr == 0 ) {
    imagepoint_arr= (sfm_imagepoint_struct *)calloc(imagepoint_nbr+1,sizeof(sfm_imagepoint_struct));
 }
 else {
    imagepoint_arr= (sfm_imagepoint_struct *)realloc(imagepoint_arr,(imagepoint_nbr+1)*sizeof(sfm_imagepoint_struct));
 }
 imagepoint_arr[imagepoint_nbr].camera_ind= inp_camera_ind;
 imagepoint_arr[imagepoint_nbr].feat_ind= inp_feat_ind;
 imagepoint_arr[imagepoint_nbr].x= inp_x;
 imagepoint_arr[imagepoint_nbr].y= inp_y;
 imagepoint_nbr++;
 threedpoint_arr[threedpoint_ind].imagepoint_arr= imagepoint_arr;
 threedpoint_arr[threedpoint_ind].imagepoint_nbr= imagepoint_nbr;

 /*
 Make sure image point references 3D point
 */

 if ( feat2threedpoint_arr[inp_camera_ind][inp_feat_ind] != -1 ) {
    sfm10_error_handler("sfm_add_imagepoint_to_threedpoint");
 }
 feat2threedpoint_arr[inp_camera_ind][inp_feat_ind]= threedpoint_ind;

 END:;

}

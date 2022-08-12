#include "header.h"

void sfm_merge_threedpoints(
 int inp_threedpoint_ind1,
 int inp_threedpoint_ind2,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

/*
Merge 3D point 2 onto 3D point 1
*/

{

 sfm_imagepoint_struct *imagepoint_arr1;
 int imagepoint_nbr1;
 sfm_imagepoint_struct *imagepoint_arr2;
 int imagepoint_nbr2;
 sfm_imagepoint_struct imagepoint1;
 sfm_imagepoint_struct imagepoint2;
 int imagepoint_ind1;
 int imagepoint_ind2;
 int camera1_ind;
 int camera2_ind;
 int feat1_ind;
 int feat2_ind;
 double x1;
 double x2;
 double y1;
 double y2;
 int count;
 int threedpoint_ind1;
 int threedpoint_ind2;
 int debug_flag=0;

 threedpoint_ind1= inp_threedpoint_ind1;
 threedpoint_ind2= inp_threedpoint_ind2;

 imagepoint_arr1= threedpoint_arr[threedpoint_ind1].imagepoint_arr;
 imagepoint_nbr1= threedpoint_arr[threedpoint_ind1].imagepoint_nbr;

 imagepoint_arr2= threedpoint_arr[threedpoint_ind2].imagepoint_arr;
 imagepoint_nbr2= threedpoint_arr[threedpoint_ind2].imagepoint_nbr;

 if ( imagepoint_nbr2 > imagepoint_nbr1 ) {

    /*
    We want 3D point 2 to stay,
    so switch the 3D points
    */

    threedpoint_ind1= inp_threedpoint_ind2;
    threedpoint_ind2= inp_threedpoint_ind1;
 }
 else if ( imagepoint_nbr2 == imagepoint_nbr1 ) {
    if ( threedpoint_ind2 < threedpoint_ind1 ) {

       /*
       We want 3D point 2 to stay,
       so switch the 3D points
       */

       threedpoint_ind1= inp_threedpoint_ind2;
       threedpoint_ind2= inp_threedpoint_ind1;
    }
 }

 imagepoint_arr1= threedpoint_arr[threedpoint_ind1].imagepoint_arr;
 imagepoint_nbr1= threedpoint_arr[threedpoint_ind1].imagepoint_nbr;

 imagepoint_arr2= threedpoint_arr[threedpoint_ind2].imagepoint_arr;
 imagepoint_nbr2= threedpoint_arr[threedpoint_ind2].imagepoint_nbr;

 if ( debug_flag == 1 ) {
 /*
 Let's do some printing ...
 */

 fprintf(stdout,"Merging 3D point 2 = %d onto 3D point 1 = %d (before) ...\n",
  threedpoint_ind2,threedpoint_ind1);

 for ( imagepoint_ind1= 0 ;
       imagepoint_ind1< imagepoint_nbr1 ;
       imagepoint_ind1++ ) {
    imagepoint1= imagepoint_arr1[imagepoint_ind1];
    camera1_ind= imagepoint1.camera_ind;
    feat1_ind= imagepoint1.feat_ind;
    x1= imagepoint1.x;
    y1= imagepoint1.y;
    fprintf(stdout,"camera1_ind = %d feat1_ind = %d x1 = %g y1 = %g\n",
     camera1_ind,feat1_ind,x1,y1);
 }

 for ( imagepoint_ind2= 0 ;
       imagepoint_ind2< imagepoint_nbr2 ;
       imagepoint_ind2++ ) {
    imagepoint2= imagepoint_arr2[imagepoint_ind2];
    camera2_ind= imagepoint2.camera_ind;
    feat2_ind= imagepoint2.feat_ind;
    x2= imagepoint2.x;
    y2= imagepoint2.y;
    fprintf(stdout,"camera2_ind = %d feat2_ind = %d x2 = %g y2 = %g\n",
     camera2_ind,feat2_ind,x2,y2);
 }

 /*
 Let's do some printing ... done.
 */
 }

 /*
 Add image points from 3D point 2 to 3D point 1
 */

 for ( imagepoint_ind2= 0 ;
       imagepoint_ind2< imagepoint_nbr2 ;
       imagepoint_ind2++ ) {
    imagepoint2= imagepoint_arr2[imagepoint_ind2];
    camera2_ind= imagepoint2.camera_ind;
    feat2_ind= imagepoint2.feat_ind;
    x2= imagepoint2.x;
    y2= imagepoint2.y;

    /*
    Check if 3D point 1
    already has an image point in camera2_ind
    There should not be any!
    */

    count= 0;
    for ( imagepoint_ind1= 0 ;
          imagepoint_ind1< imagepoint_nbr1 ;
          imagepoint_ind1++ ) {
       imagepoint1= imagepoint_arr1[imagepoint_ind1];
       camera1_ind= imagepoint1.camera_ind;
       feat1_ind= imagepoint1.feat_ind;
       x1= imagepoint1.x;
       y1= imagepoint1.y;
       if ( camera1_ind == camera2_ind ) {
          count++;
       }
    }
    if ( count > 0 ) {
       sfm10_error_handler("sfm_merge_threedpoints");
    }

    /*
    Add image point from 3D point 2
    to the image points of 3D point 1
    */

    if ( imagepoint_nbr1 == 0 ) {
       imagepoint_arr1= (sfm_imagepoint_struct *)calloc(imagepoint_nbr1+1,sizeof(sfm_imagepoint_struct));
    }
    else {
       imagepoint_arr1= (sfm_imagepoint_struct *)realloc(imagepoint_arr1,(imagepoint_nbr1+1)*sizeof(sfm_imagepoint_struct));
    }
    imagepoint_ind1= imagepoint_nbr1;
    imagepoint_nbr1++;
    imagepoint1.camera_ind= camera2_ind;
    imagepoint1.feat_ind= feat2_ind;
    imagepoint1.x= x2;
    imagepoint1.y= y2;
    imagepoint_arr1[imagepoint_ind1]= imagepoint1;

    /*
    Make sure feature from image 2 references 3D point from image 1
    instead of 3D point from image 2
    */

    if ( feat2threedpoint_arr[camera2_ind][feat2_ind] != threedpoint_ind2 ) {
       sfm10_error_handler("sfm_merge_threedpoints");
    }
    feat2threedpoint_arr[camera2_ind][feat2_ind]= threedpoint_ind1;
 }

 threedpoint_arr[threedpoint_ind1].imagepoint_arr= imagepoint_arr1;
 threedpoint_arr[threedpoint_ind1].imagepoint_nbr= imagepoint_nbr1;

 /*
 Remove image points from 3D point 2
 (all of them)
 */

 if ( imagepoint_nbr2 > 0 )
  free(imagepoint_arr2);

 threedpoint_arr[threedpoint_ind2].imagepoint_arr= 0;
 threedpoint_arr[threedpoint_ind2].imagepoint_nbr= 0;

 imagepoint_arr1= threedpoint_arr[threedpoint_ind1].imagepoint_arr;
 imagepoint_nbr1= threedpoint_arr[threedpoint_ind1].imagepoint_nbr;

 imagepoint_arr2= threedpoint_arr[threedpoint_ind2].imagepoint_arr;
 imagepoint_nbr2= threedpoint_arr[threedpoint_ind2].imagepoint_nbr;

 if ( debug_flag == 1 ) {
 /*
 Let's do some printing ...
 */

 fprintf(stdout,"Merging 3D point 2 = %d onto 3D point 1 = %d (after) ...\n",
  threedpoint_ind2,threedpoint_ind1);

 for ( imagepoint_ind1= 0 ;
       imagepoint_ind1< imagepoint_nbr1 ;
       imagepoint_ind1++ ) {
    imagepoint1= imagepoint_arr1[imagepoint_ind1];
    camera1_ind= imagepoint1.camera_ind;
    feat1_ind= imagepoint1.feat_ind;
    x1= imagepoint1.x;
    y1= imagepoint1.y;
    fprintf(stdout,"camera1_ind = %d feat1_ind = %d x1 = %g y1 = %g\n",
     camera1_ind,feat1_ind,x1,y1);
 }

 for ( imagepoint_ind2= 0 ;
       imagepoint_ind2< imagepoint_nbr2 ;
       imagepoint_ind2++ ) {
    imagepoint2= imagepoint_arr2[imagepoint_ind2];
    camera2_ind= imagepoint2.camera_ind;
    feat2_ind= imagepoint2.feat_ind;
    x2= imagepoint2.x;
    y2= imagepoint2.y;
    fprintf(stdout,"camera2_ind = %d feat2_ind = %d x2 = %g y2 = %g\n",
     camera2_ind,feat2_ind,x2,y2);
 }

 /*
 Let's do some printing ... done.
 */
 }

}

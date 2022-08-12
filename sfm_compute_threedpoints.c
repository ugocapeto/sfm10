#include "header.h"

void sfm_compute_threedpoints(
 int width,
 int height,
 int camera1_ind,
 int camera2_ind,
 int *image1_arr,
 int *image2_arr,
 double P1[3*4],
 double P2[3*4],
 match_struct *match_arr,
 int match_nbr,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr
)

/*
Compute 3D points in projective space (points are homogeneous 4-vectors)
given the camera matrices
*/

/*
Note that a match is considered for the creation of a new 3D point
only if both features do not reference a 3D point in feat2threedpoint_arr
*/

{

 double X4[4];
 double X3[3];
 int match_ind;
 double x1;
 double y1;
 double x2;
 double y2;
 sfm_threedpoint_struct *threedpoint_arr;
 int threedpoint_nbr;
 int feat1_ind;
 int feat2_ind;
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 int j1;
 int i1;
 int j2;
 int i2;
 int pixel1;
 int pixel2;
 int rgb1[3];
 int rgb2[3];
 int rgb[3];
 int cind;
 double val;
 int val_int;
 int threedpoint_ind;
 int threedpoint1_ind;
 int threedpoint2_ind;

 threedpoint_arr= (*pthreedpoint_arr);
 threedpoint_nbr= (*pthreedpoint_nbr);

 for ( match_ind= 0 ; match_ind< match_nbr ; match_ind++ ) {
    feat1_ind= match_arr[match_ind].feat1_ind;
    x1= match_arr[match_ind].x1;
    y1= match_arr[match_ind].y1;
    feat2_ind= match_arr[match_ind].feat2_ind;
    x2= match_arr[match_ind].x2;
    y2= match_arr[match_ind].y2;

    /*
    Let's see if a 3D point has already been created
    for feature in image 1
    */

    threedpoint1_ind= feat2threedpoint_arr[camera1_ind][feat1_ind];

    /*
    Let's see if a 3D point has already been created
    for feature in image 2
    */

    threedpoint2_ind= feat2threedpoint_arr[camera2_ind][feat2_ind];

    /*
    There are a bunch of cases to consider
    depending on where the image points reference 3D points
    */

    if ( threedpoint1_ind != -1 &&
         threedpoint2_ind == -1 ) {

       /*
       Add (camera2_ind,feat2_ind,x2,y2) to 3D point 1
       unless already there
       */

       sfm_add_imagepoint_to_threedpoint(
        camera2_ind,
        feat2_ind,
        x2,
        y2,
        threedpoint1_ind,
        feat2threedpoint_arr,
        feat2threedpoint_nbr,
        threedpoint_arr,
        threedpoint_nbr
       );

       continue;
    }

    if ( threedpoint1_ind == -1 &&
         threedpoint2_ind != -1 ) {

       /*
       Add (camera1_ind,feat1_ind,x1,y1) to 3D point 2
       unless already there
       */

       sfm_add_imagepoint_to_threedpoint(
        camera1_ind,
        feat1_ind,
        x1,
        y1,
        threedpoint2_ind,
        feat2threedpoint_arr,
        feat2threedpoint_nbr,
        threedpoint_arr,
        threedpoint_nbr
       );

       continue;
    }

    if ( threedpoint1_ind != -1 &&
         threedpoint2_ind != -1 ) {

       if ( threedpoint2_ind == threedpoint1_ind ) {

          /*
          We have 2 matching image points and 1 3D point
          That's golden!
          */

          continue;
       }

       if ( threedpoint2_ind != threedpoint1_ind ) {

          /*
          Merge the two 3D points
          */

          sfm_merge_threedpoints(
           threedpoint1_ind,
           threedpoint2_ind,
           feat2threedpoint_arr,
           feat2threedpoint_nbr,
           threedpoint_arr,
           threedpoint_nbr
          );

          continue;
       }
    }

    /*
    if ( threedpoint1_ind == -1 &&
         threedpoint2_ind == -1 ) {
    }
    */

    /*
    If here,
    this is the case where the 2 image points do not reference 3D points
    it's ok to create a new 3D point using the 2 image points
    */

    /*
    Get color for image point 1
    */

    j1= (int)(x1 + 0.5);

    if ( j1 < 0 )
     j1= 0;
    if ( j1 > width-1 )
     j1= width-1;

    i1= (int)(y1 + 0.5);

    if ( i1 < 0 )
     i1= 0;
    if ( i1 > height-1 )
     i1= height-1;

    pixel1= i1*width+j1;

    rgb1[0]= image1_arr[pixel1*3+0];
    rgb1[1]= image1_arr[pixel1*3+1];
    rgb1[2]= image1_arr[pixel1*3+2];

    /*
    Get color for image point 2
    */

    j2= (int)(x2 + 0.5);

    if ( j2 < 0 ) 
     j2= 0;
    if ( j2 > width-1 )
     j2= width-1;

    i2= (int)(y2 + 0.5);

    if ( i2 < 0 ) 
     i2= 0;
    if ( i2 > height-1 )
     i2= height-1;

    pixel2= i2*width+j2;

    rgb2[0]= image2_arr[pixel2*3+0];
    rgb2[1]= image2_arr[pixel2*3+1];
    rgb2[2]= image2_arr[pixel2*3+2];

    /*
    Compute the location of the 3D point
    */

    compute_linear_triangulation_two_cameras(
     P1,
     P2,
     x1,
     y1,
     x2,
     y2,
     X4
    );

    /*
    Go from homogeneous to inhomogeneous
    */

    X3[0]= X4[0]/X4[3];
    X3[1]= X4[1]/X4[3];
    X3[2]= X4[2]/X4[3];

    /*
    The color is the average color of the 2 image points
    */

    for ( cind= 0 ; cind< 3 ; cind++ ) {
       val= .5 * ( (double)rgb1[cind] + (double)rgb2[cind] );
       val_int= (int)(val + 0.5);
       if ( val_int < 0 )
        val_int= 0;
       if ( val_int > 255 )
        val_int= 255;
       rgb[cind]= val_int;
    }

    /*
    Store the 3d point
    */

    if ( threedpoint_nbr == 0 ) {
       threedpoint_arr= (sfm_threedpoint_struct *)calloc(threedpoint_nbr+1,sizeof(sfm_threedpoint_struct));
    }
    else {
       threedpoint_arr= (sfm_threedpoint_struct *)realloc(threedpoint_arr,(threedpoint_nbr+1)*sizeof(sfm_threedpoint_struct));
    }
    threedpoint_ind= threedpoint_nbr;
    threedpoint_nbr++;
    imagepoint_nbr= 2;
    imagepoint_arr= (sfm_imagepoint_struct *)calloc(imagepoint_nbr,sizeof(sfm_imagepoint_struct));
    imagepoint_arr[1-1].camera_ind= camera1_ind;
    imagepoint_arr[1-1].feat_ind= feat1_ind;
    imagepoint_arr[1-1].x= x1;
    imagepoint_arr[1-1].y= y1;
    imagepoint_arr[2-1].camera_ind= camera2_ind;
    imagepoint_arr[2-1].feat_ind= feat2_ind;
    imagepoint_arr[2-1].x= x2;
    imagepoint_arr[2-1].y= y2;

    threedpoint_arr[threedpoint_ind].xyz[0]= X3[0];
    threedpoint_arr[threedpoint_ind].xyz[1]= X3[1];
    threedpoint_arr[threedpoint_ind].xyz[2]= X3[2];
    threedpoint_arr[threedpoint_ind].rgb[0]= rgb[0];
    threedpoint_arr[threedpoint_ind].rgb[1]= rgb[1];
    threedpoint_arr[threedpoint_ind].rgb[2]= rgb[2];
    threedpoint_arr[threedpoint_ind].imagepoint_arr= imagepoint_arr;
    threedpoint_arr[threedpoint_ind].imagepoint_nbr= imagepoint_nbr;

    /*
    Indicate fact that 3D point has been created
    for feature in image 1
    */

    if ( feat2threedpoint_arr[camera1_ind][feat1_ind] != -1 ) {
       sfm10_error_handler("sfm_compute_threedpoints");
    }
    feat2threedpoint_arr[camera1_ind][feat1_ind]= threedpoint_ind;

    /*
    Indicate fact that 3D point has been created
    for feature in image 2
    */

    if ( feat2threedpoint_arr[camera2_ind][feat2_ind] != -1 ) {
       sfm10_error_handler("sfm_compute_threedpoints");
    }
    feat2threedpoint_arr[camera2_ind][feat2_ind]= threedpoint_ind;
 }

 *pthreedpoint_arr= threedpoint_arr;
 *pthreedpoint_nbr= threedpoint_nbr;

}

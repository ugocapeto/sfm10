#include "header.h"
#include "proto.h"

int main(
 int argc,
 char* argv[]
)

{

 char **image_filename_arr;
 int camera_nbr;
 FILE *fp;
 char filename[80];
 int camera_ind;
 int camera1_ind;
 int camera2_ind;
 sfm_threedpoint_struct *threedpoint_arr;
 int threedpoint_nbr;
 int **feat2threedpoint_arr;
 int *feat2threedpoint_nbr;
 sfm_feat_struct *feat_arr;
 int feat_nbr;
 double P1[3*4];
 double P2[3*4];
 double P[3*4];
 int feat_ind;
 int camera_matrix_nbr;
 sfm_camera_matrix_struct *camera_matrix_arr;
 double K1[3*3];
 double R1[3*3];
 double C1[3];
 double R1C1[3];
 double t1[3];
 double K2[3*3];
 double R2[3*3];
 double C2[3];
 double R2C2[3];
 double t2[3];
 int i;
 double K[3*3];
 double R[3*3];
 double t[3];
 int camera_matrix_ind1;
 int camera_matrix_ind2;
 int camera_matrix_ind;
 char image_filename[80];
 int err_flag;
 int *image_arr;
 int width;
 int height;
 int j;
 double det1;
 double det2;
 double det;
 int lbfgs_max_iterations;
 double min_separation_angle;
 double focal_length;
 double *focal_length_arr;
 double *xyz_arr;
 double *xy_arr;
 int number_of_correspondences;
 double fu;
 double fv;
 double uc;
 double vc;
 double reproj_error;
 double R33[3][3];
 int ntrials;
 double max_reproj_error;
 int n;
 int radius_gif;
 double max_theta_deg;
 int width0;
 int height0;
 int err_flag1;

 /*
 Let's read in the input file
 */

 fp= fopen("sfm10_input.txt","r");

 /*
 Get number of images/cameras
 */

 fscanf(fp,"%d",&camera_nbr);

 fprintf(stdout,"Number of cameras = %d\n",
  camera_nbr);

 if ( camera_nbr > 0 )
  image_filename_arr= (char **)calloc(camera_nbr,sizeof(char *));

 /*
 Get the image filenames
 */

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    fscanf(fp,"%s",image_filename);

    fprintf(stdout,"Image name for camera %d = %s\n",
     camera_ind,image_filename);

    image_filename_arr[camera_ind]= (char *)calloc(80,sizeof(char));
    strcpy(image_filename_arr[camera_ind],image_filename);
 }

 /*
 Get the focal length
 */

 fscanf(fp,"%lg",&focal_length);

 fprintf(stdout,"Focal length = %g\n",
  focal_length);

 /*
 Store the focal length in focal_length_arr
 */

 if ( camera_nbr > 0 )
  focal_length_arr= (double *)calloc(camera_nbr,sizeof(double));

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {

    focal_length_arr[camera_ind]= focal_length;
 }

 /*
 The image dimensions should be the same
 */

 width0= 0;
 height0= 0;

 err_flag= 0;
 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {

    /*
    Get corresponding image file name
    */

    strcpy(image_filename,image_filename_arr[camera_ind]);

    /*
    Load the corresponding image to get width and height
    */

    err_flag1= load_rgb_image(
     image_filename,
     &image_arr,
     &width,
     &height
    );

    if ( err_flag1 == 1 ) {
       sfm10_error_handler("main");
    }

    /*
    Free image_arr
    */

    free(image_arr);

    if ( width0 == 0 ) {
       width0= width;
    }
    else if ( width != width0 ) {
       err_flag= 1;
    }

    if ( height0 == 0 ) {
       height0= height;
    }
    else if ( height != height0 ) {
       err_flag= 1;
    }
 }
 if ( err_flag == 1 ) {
    fprintf(stdout,"Image dimensions must be the same.\n");
    return 1;
 }

 /*
 Get initial camera pair
 */

 fscanf(fp,"%d %d",&camera1_ind,&camera2_ind);

 fprintf(stdout,"initial camera pair = %d %d\n",
  camera1_ind,camera2_ind);

 if ( !(camera1_ind >= 0) ) {
    fprintf(stdout,"camera1_ind must be greater or equal to 0.\n");
    return 1;
 }
 if ( !(camera1_ind <= (camera_nbr-1)) ) {
    fprintf(stdout,"camera1_ind must be less or equal to %d.\n",camera_nbr-1);
    return 1;
 }
 if ( !(camera2_ind >= 0) ) {
    fprintf(stdout,"camera2_ind must be greater or equal to 0.\n");
    return 1;
 }
 if ( !(camera2_ind <= (camera_nbr-1)) ) {
    fprintf(stdout,"camera2_ind must be less or equal to %d.\n",camera_nbr-1);
    return 1;
 }
 if ( !(camera1_ind < camera2_ind) ) {
    fprintf(stdout,"camera1_ind must be less than camera2_ind.\n");
    return 1;
 }

 /*
 Get the number of trials used to get the good matches
 */

 fscanf(fp,"%d",&ntrials);

 fprintf(stdout,"Number of trials (good matches) = %d\n",
  ntrials);

 /*
 Get the maximum number of iterations for lbfgs (Bundle Adjustment)
 */

 fscanf(fp,"%d",&lbfgs_max_iterations);

 fprintf(stdout,"Max number of iterations (Bundle Adjustment) = %d\n",
  lbfgs_max_iterations);

 /*
 Get the minimum separation angle (removal of low-confidence 3D points)
 */

 fscanf(fp,"%lg",&min_separation_angle);

 fprintf(stdout,"Min separation angle (low-confidence 3D points) = %g\n",
  min_separation_angle);

 /*
 Get the maximum reprojection error (removal of low-confidence 3D points)
 */

 fscanf(fp,"%lg",&max_reproj_error);

 fprintf(stdout,"Max reprojection error (low-confidence 3D points) = %g\n",
  max_reproj_error);

 /*
 Get the radius for the animated gif frames
 */

 fscanf(fp,"%d",&radius_gif);

 fprintf(stdout,"Radius (animated gif frames) = %d\n",
  radius_gif);

 /*
 Get the angle amplitude for the animated gif frames
 */

 fscanf(fp,"%lg",&max_theta_deg);

 fprintf(stdout,"Angle amplitude (animated gif frames) = %g\n",
  max_theta_deg);

 /*
 Done reading the input file
 */

 fclose(fp);

 /*
 For each image, 
 compute the features (keypoints) and the descriptors for each feature
 Print the features and descriptors on disk
 */

 fprintf(stdout,"Getting the features for each image ...\n");

 sfm_compute_features_main(
  image_filename_arr,
  camera_nbr
 );

 fprintf(stdout,"Getting the features for each image ... done.\n");

 /*
 For each pair of images,
 compute the matches
 Print the matches on disk
 */

 fprintf(stdout,"Getting the matches for each image pair ...\n");

 sfm_compute_matches_main(
  image_filename_arr,
  camera_nbr
 );

 fprintf(stdout,"Getting the matches for each image pair ... done.\n");

 /*
 For each pair of images,
 compute the good matches using A Contrario RANSAC
 Print the good matches on disk
 */

 fprintf(stdout,"Removing the outliers for each image pair and computing the fundamental matrix ...\n");

 sfm_compute_good_matches_main(
  image_filename_arr,
  camera_nbr,
  ntrials
 );

 fprintf(stdout,"Removing the outliers for each image pair and computing the fundamental matrix ... done.\n");

 /*
 Merge duplicate features (same image location but different descriptor)
 Update the matches by merging duplicate features into the feature that remains
 Print the gooder matches on disk
 */

 fprintf(stdout,"Merging duplicate features and updating matches ...\n");

 sfm_merge_duplicate_features(
  image_filename_arr,
  camera_nbr
 );

 fprintf(stdout,"Merging duplicate features and updating matches ... done.\n");

 /*
 Remove the spurious matches from the good matches
 Print the best matches on disk
 */

 /*
 This basically builds the tracks
 which will in turn become the 3D points
 If a track is such that it has 2 or more image points on the same camera,
 the corresponding matches are removed
 */

 fprintf(stdout,"Removing the matches inconsistent with 3D reconstruction ...\n");

 sfm_remove_spurious_matches(
  image_filename_arr,
  camera_nbr
 );

 fprintf(stdout,"Removing the matches inconsistent with 3D reconstruction ... done.\n");

 /*
 Compute the focal length from the fundamental matrix
 Do this for every camera pair and take an average of the focal length
 */

 /*
 fprintf(stdout,"Computing the focal length ...\n");

 sfm_compute_focal_length_main(
  image_filename_arr,
  camera_nbr,
  &focal_length
 );

 fprintf(stdout,"Computing the focal length ... done.\n");
 */

 /*
 Check the initial pair of cameras
 Needs the best matches
 */

 fprintf(stdout,"Checking the initial pair ...\n");

 sfm_check_initial_camera_pair(
  image_filename_arr,
  focal_length_arr,
  camera_nbr,
  camera1_ind,
  camera2_ind,
  &err_flag
 );

 fprintf(stdout,"Checking the initial pair ... done.\n");

 if ( err_flag == 1 ) {
    fprintf(stdout,"Initial camera pair is not acceptable!\n");
    return 1;
 }

 /*
 Initialize feat2threedpoint_arr
 feat2threedpoint_arr[camera_ind][feat_ind]= 3D point
 */

 if ( camera_nbr > 0 ) {
    feat2threedpoint_arr= (int **)calloc(camera_nbr,sizeof(int *));
    feat2threedpoint_nbr= (int *)calloc(camera_nbr,sizeof(int));
 }
 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    sprintf(filename,"feat_%d.sfm",camera_ind);
    sfm_read_features_and_descriptors(
     filename,
     &feat_arr,
     &feat_nbr
    );
    sfm_free_features_and_descriptors(
     feat_arr,
     feat_nbr
    );
    feat2threedpoint_nbr[camera_ind]= feat_nbr;
    if ( feat_nbr > 0 )
     feat2threedpoint_arr[camera_ind]= (int *)calloc(feat_nbr,sizeof(int));
    for ( feat_ind= 0 ;
          feat_ind< feat_nbr ;
          feat_ind++ )
     feat2threedpoint_arr[camera_ind][feat_ind]=-1;
 }

 /*
 Initialize camera_matrix_arr
 which keeps track of the camera matrices of processed cameras
 */

 camera_matrix_arr= 0;
 camera_matrix_nbr= 0;

 /*
 Compute 3D points using a pair of cameras
 to initialize the reconstruction process
 */

 fprintf(stdout,"Initializing the 3D reconstruction with initial pair ...\n");

 sfm_reconstruct_3d_from_2cameras(
  image_filename_arr,
  focal_length_arr,
  camera_nbr,
  camera1_ind,
  camera2_ind,
  P1,
  R1,
  t1,
  P2,
  R2,
  t2,
  feat2threedpoint_arr,
  feat2threedpoint_nbr,
  &threedpoint_arr,
  &threedpoint_nbr
 );

 fprintf(stdout,"Initializing the 3D reconstruction with initial pair ... done.\n");

 /*
 If all went well,
 the determinant of the rotation matrix should be +1
 */

 det1= math_matrix3x3_determinant(
  R1
 );

 /*
 If all went well,
 the determinant of the rotation matrix should be +1
 */

 det2= math_matrix3x3_determinant(
  R2
 );

 /*
 Store the camera matrix for camera1
 */

 width= width0;
 height= height0;

 K1[0*3+0]= focal_length;
 K1[0*3+1]= 0;
 K1[0*3+2]= (double)width/2;

 K1[1*3+0]= 0;
 K1[1*3+1]= focal_length;
 /*
 K1[1*3+1]=-focal_length;
 */
 K1[1*3+2]= (double)height/2;

 K1[2*3+0]= 0;
 K1[2*3+1]= 0;
 K1[2*3+2]= 1;

 /*
 Add P1 to camera_matrix_arr
 */

 sfm_add_camera_matrix(
  camera1_ind,
  P1,
  K1,
  R1,
  t1,
  &camera_matrix_arr,
  &camera_matrix_nbr,
  &camera_matrix_ind1
 );

 /*
 if ( camera_matrix_ind1 != camera1_ind ) {
    error_handler("main");
 }
 */

 /*
 Store the camera matrix for camera2
 */

 width= width0;
 height= height0;

 K2[0*3+0]= focal_length;
 K2[0*3+1]= 0;
 K2[0*3+2]= (double)width/2;

 K2[1*3+0]= 0;
 K2[1*3+1]= focal_length;
 /*
 K2[1*3+1]=-focal_length;
 */
 K2[1*3+2]= (double)height/2;

 K2[2*3+0]= 0;
 K2[2*3+1]= 0;
 K2[2*3+2]= 1;

 /*
 Add P2 to camera_matrix_arr
 */

 sfm_add_camera_matrix(
  camera2_ind,
  P2,
  K2,
  R2,
  t2,
  &camera_matrix_arr,
  &camera_matrix_nbr,
  &camera_matrix_ind2
 );

 /*
 if ( camera_matrix_ind2 != camera2_ind ) {
    error_handler("main");
 }
 */

 /*
 Recompute the 3D points
 from all measurements
 */

 fprintf(stdout,"Recomputing the 3D reconstruction ...\n");

 sfm_recompute_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 fprintf(stdout,"Recomputing the 3D reconstruction ... done.\n");

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Remove spurious 3D points
 */

 fprintf(stdout,"Removing the low-confidence 3D points ...\n");

 fprintf(stdout,"Number of 3D points (before) = %d\n",threedpoint_nbr);

 sfm_remove_spurious_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  &feat2threedpoint_arr,
  &feat2threedpoint_nbr,
  &threedpoint_arr,
  &threedpoint_nbr,
  min_separation_angle,
  max_reproj_error
 );

 fprintf(stdout,"Number of 3D points (after) = %d\n",threedpoint_nbr);

 fprintf(stdout,"Removing the low-confidence 3D points ... done.\n");

 if ( threedpoint_nbr == 0 ) {
    fprintf(stdout,"There are no 3D points remaining.\n");
    return 1;
 }

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Perform Bundle Adjustment
 Note that camera_matrix_arr[0] is constrained
 */

 fprintf(stdout,"Performing Bundle Adjustment on all cameras and 3D points ...\n");

 /*
 sfm_lbfgs_bundle_adjustment(
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr,
  lbfgs_max_iterations
 );
 */
 sfm_pba_bundle_adjustment(
  image_filename_arr,
  focal_length_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 fprintf(stdout,"Performing Bundle Adjustment on all cameras and 3D points ... done.\n");

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Add the remaining cameras
 */

 ADD_CAMERA_START:

 /*
 Get the next camera to add
 It's the one with the most image points/3D points
 */

 fprintf(stdout,"Looking for next camera to add to 3D reconstruction ...\n");

 sfm_get_next_camera(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr,
  &camera_ind,
  &n
 );

 fprintf(stdout,"Looking for next camera to add to 3D reconstruction ... done.\n");

 if ( camera_ind == -1 ) {

    /*
    No more cameras to add
    */

    fprintf(stdout,"No more cameras to add to the 3D reconstruction\n");

    /*
    We are done here!
    */

    goto ADD_CAMERA_END;
 }

 /*
 For EPnP to work,
 we need at least 4 image points/3D points
 */

 if ( !(n >= 4) ) {

    /*
    No more cameras with enough correspondences to add
    */

    fprintf(stdout,"No more cameras with enough correspondences to add to the 3D reconstruction\n");

    /*
    We are done here!
    */

    goto ADD_CAMERA_END;
 }

 fprintf(stdout,"Adding camera %d to the 3D reconstruction ...\n",camera_ind);

 fprintf(stdout,"Number of 3D points with image points = %d\n",n);

 /*
 Get filename for image
 */

 strcpy(image_filename,image_filename_arr[camera_ind]);

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

 /*
 Get the focal length
 */

 focal_length= focal_length_arr[camera_ind];

 /*
 Set the calibration matrix
 */

 /*
 It is assumed that the origin of the image coordinate system
 is at top-left with y axis going down
 It is also assumed that the y axis of the camera coordinate system
 points downward like in VisualSFM
 */

 K[0*3+0]= focal_length;
 K[0*3+1]= 0;
 K[0*3+2]= (double)width/2;

 K[1*3+0]= 0;
 K[1*3+1]= focal_length;
 K[1*3+2]= (double)height/2;

 K[2*3+0]= 0;
 K[2*3+1]= 0;
 K[2*3+2]= 1;

 /*
 Get the 3D points and 2D image projections for camera_ind
 Will add measurements coming from camera_ind
 to threedpoint_arr
 Be careful because the camera has not been added yet
 to camera_matrix_arr
 */

 fprintf(stdout,"Adding image points to existing 3D points ...\n");

 sfm_add_camera_imagepoints(
  image_filename_arr,
  camera_nbr,
  camera_ind,
  feat2threedpoint_arr,
  feat2threedpoint_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 fprintf(stdout,"Adding image points to existing 3D points ... done.\n");

 /*
 Get the 3D points and image points for camera_ind
 */

 fprintf(stdout,"Getting the camera 3D points and image points ...\n");

 sfm_get_camera_threedpoints_imagepoints(
  image_filename_arr,
  camera_nbr,
  camera_ind,
  threedpoint_arr,
  threedpoint_nbr,
  &xyz_arr,
  &xy_arr,
  &number_of_correspondences
 );

 fprintf(stdout,"Getting the camera 3D points and image points ... done.\n");

 /*
 Compute the pose (rotation and translation)
 */

 fprintf(stdout,"Computing the camera pose ...\n");

 fu= focal_length;
 fv= focal_length;
 uc= (double)width/2;
 vc= (double)height/2;
 reproj_error= epnp_compute_pose(
  fu,
  fv,
  uc,
  vc,
  xyz_arr,
  xy_arr,
  number_of_correspondences,
  R33,
  t
 );

 fprintf(stdout,"Average reprojection error = %g\n",reproj_error);

 fprintf(stdout,"Computing the camera pose ... done.\n");

 /*
 Free the 3D points and corresponding 2D image projections
 */

 if ( number_of_correspondences > 0 ) {
    free(xyz_arr);
    free(xy_arr);
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i*3+j]= R33[i][j];
    }
 }

 /*
 If all went well,
 the determinant of the rotation matrix should be +1
 */

 det= math_matrix3x3_determinant(
  R
 );

 /*
 Print out the camera rotation and translation
 */

 fprintf(stdout,"rotation:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fprintf(stdout," %g",R[i*3+j]);
    }
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"translation:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    fprintf(stdout," %g",t[i]);
 }
 fprintf(stdout,"\n");

 /*
 Use bundle adjustment to adjust R and t
 with K constrained
 Be careful because the camera has not been added yet
 to camera_matrix_arr
 */

 fprintf(stdout,"Performing Bundle Adjustment on camera pose ...\n");

 sfm_compute_added_camera_matrix_exterior_orientation(
  image_filename_arr,
  camera_nbr,
  camera_ind,
  threedpoint_arr,
  threedpoint_nbr,
  K,
  R,
  t,
  lbfgs_max_iterations
 );

 fprintf(stdout,"Performing Bundle Adjustment on camera pose ... done.\n");

 /*
 If all went well,
 the determinant of the rotation matrix should be +1
 */

 det= math_matrix3x3_determinant(
  R
 );

 /*
 Print out the camera rotation and translation
 */

 fprintf(stdout,"rotation:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fprintf(stdout," %g",R[i*3+j]);
    }
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"translation:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    fprintf(stdout," %g",t[i]);
 }
 fprintf(stdout,"\n");

 /*
 Compute P=K[R|t]
 */

 sfm_compute_P_from_K_R_t(
  K,
  R,
  t,
  P
 );

 /*
 Add P to camera_matrix_arr
 */

 fprintf(stdout,"Adding camera to camera matrix array ...\n");

 sfm_add_camera_matrix(
  camera_ind,
  P,
  K,
  R,
  t,
  &camera_matrix_arr,
  &camera_matrix_nbr,
  &camera_matrix_ind
 );

 fprintf(stdout,"Adding camera to camera matrix array ... done.\n");

 /*
 if ( camera_matrix_ind != camera_ind ) {
    error_handler("main");
 }
 */

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Add 3D points
 */

 fprintf(stdout,"Adding the 3D points to 3D reconstruction ...\n");

 sfm_add_camera(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  camera_ind,
  feat2threedpoint_arr,
  feat2threedpoint_nbr,
  &threedpoint_arr,
  &threedpoint_nbr
 );

 fprintf(stdout,"Adding the 3D points to 3D reconstruction ... done.\n");

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Recompute the 3D points
 from all measurements
 */

 fprintf(stdout,"Recomputing the 3D reconstruction ...\n");

 sfm_recompute_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 fprintf(stdout,"Recomputing the 3D reconstruction ... done.\n");

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Remove spurious 3D points
 */

 fprintf(stdout,"Removing the low-confidence 3D points ...\n");

 fprintf(stdout,"Number of 3D points (before) = %d\n",threedpoint_nbr);

 sfm_remove_spurious_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  &feat2threedpoint_arr,
  &feat2threedpoint_nbr,
  &threedpoint_arr,
  &threedpoint_nbr,
  min_separation_angle,
  max_reproj_error
 );

 fprintf(stdout,"Number of 3D points (after) = %d\n",threedpoint_nbr);

 fprintf(stdout,"Removing the low-confidence 3D points ... done.\n");

 if ( threedpoint_nbr == 0 ) {
    fprintf(stdout,"There are no 3D points remaining.\n");
    return 1;
 }

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Perform Bundle Adjustment
 Note that camera_matrix_arr[0] is constrained
 */

 fprintf(stdout,"Performing Bundle Adjustment on all cameras and 3D points ...\n");

 /*
 sfm_lbfgs_bundle_adjustment(
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr,
  lbfgs_max_iterations
 );
 */
 sfm_pba_bundle_adjustment(
  image_filename_arr,
  focal_length_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 fprintf(stdout,"Performing Bundle Adjustment on all cameras and 3D points ... done.\n");

 /*
 Compute and print out the average and maximum reprojection error
 */

 sfm_print_reprojection_error_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 fprintf(stdout,"Adding camera %d to the 3D reconstruction ... done.\n",camera_ind);

 goto ADD_CAMERA_START;

 ADD_CAMERA_END:

 /*
 Check the 3D points
 */

 sfm_check_threedpoints(
  camera_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Print the 3D reconstruction
 */

 sfm_print_threedpoints_in(
  "duh.in",
  threedpoint_arr,
  threedpoint_nbr
 );

 sfm_print_threedpoints_nvm(
  "duh.nvm",
  image_filename_arr,
  focal_length_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 sfm_print_threedpoints_ply(
  "duh.ply",
  threedpoint_arr,
  threedpoint_nbr
 );

 sfm_print_threedpoints_gif(
  image_filename_arr,
  focal_length_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr,
  radius_gif,
  max_theta_deg
 );

 /*
 Print out the depth range of the 3D points
 */

 sfm_print_depth_range_threedpoints(
  image_filename_arr,
  camera_nbr,
  camera_matrix_arr,
  camera_matrix_nbr,
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Free image_filename_arr
 */

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    free(image_filename_arr[camera_ind]);
 }
 if ( camera_nbr > 0 )
  free(image_filename_arr);

 /*
 Free focal_length_arr
 */

 if ( camera_nbr > 0 )
  free(focal_length_arr);

 /*
 Free threedpoint_arr
 */

 sfm_free_threedpoints(
  threedpoint_arr,
  threedpoint_nbr
 );

 /*
 Free camera_matrix_arr
 */

 if ( camera_matrix_nbr > 0 )
  free(camera_matrix_arr);

 /*
 Free feat2threedpoint_arr
 */

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    if ( feat2threedpoint_nbr[camera_ind] > 0 )
     free(feat2threedpoint_arr[camera_ind]); 
 }
 if ( camera_nbr > 0 ) {
    free(feat2threedpoint_arr);
    free(feat2threedpoint_nbr);
 }

 return 0;

}

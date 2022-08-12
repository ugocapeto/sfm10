#include "header.h"

void sfm_print_threedpoints_nvm(
 char *filename,
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

{

 FILE *fp;
 int camera_ind;
 char image_filename[80];
 int err_flag;
 int *image_arr;
 int width;
 int height;
 double f;
 double K[3*3];
 double R[3*3];
 double C[3];
 double q[4];
 int qind;
 int Cind;
 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double P[3*4];
 int i;
 int cind;
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int feat_ind;
 double x;
 double y;
 int camera_matrix_ind;
 double t[3];
 int width0;
 int height0;

 fp= fopen(filename,"w");

 /*
 Print file version header
 */

 fprintf(fp,"NVM_V3\n");

 /*
 Skip a line
 */

 fprintf(fp,"\n");

 /*
 Print number of cameras
 */

 fprintf(fp,"%d\n",camera_matrix_nbr);

 /*
 Assume all images have the same widht and height
 */

 width0= 0;
 height0= 0;

 /*
 For each camera, print:
 File name, focal length, quaternion WXYZ, camera center, radial distortion, 0
 */

 for ( camera_matrix_ind= 0 ;
       camera_matrix_ind< camera_matrix_nbr ;
       camera_matrix_ind++ ) {

    /*
    Get the camera matrix
    */

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

    if ( width0 == 0 ) {
       width0= width;
    }
    else if ( width != width0 ) {
       sfm10_error_handler("sfm_print_threedpoints_nvm");
    }

    if ( height0 == 0 ) {
       height0= height;
    }
    else if ( height != height0 ) {
       sfm10_error_handler("sfm_print_threedpoints_nvm");
    }

    /*
    Get the focal length
    */

    f= focal_length_arr[camera_ind];

    /*
    Print the filename for the image
    */

    fprintf(fp,"%s ",image_filename);

    /*
    Print the focal length
    */

    fprintf(fp,"%g ",f);

    /*
    Extract the quaternion from the rotation matrix
    */

    compute_quaternion_from_rotation(
     R,
     q
    );

    /*
    Print the quaternion
    */

    for ( qind= 0 ; qind< 4 ; qind++ )
     fprintf(fp,"%g ",q[qind]);

    /*
    Get the camera center from R and t
    */

    sfm_compute_C_from_R_t(
     R,
     t,
     C
    );

    /*
    Print the camera center coordinates
    */

    for ( Cind= 0 ; Cind< 3 ; Cind++ )
     fprintf(fp,"%g ",C[Cind]);

    /*
    Print the radial distortion
    */

    fprintf(fp,"0 0 ");

    fprintf(fp,"\n");
 }

 /*
 Skip a line
 */

 fprintf(fp,"\n");

 /*
 Print number of 3D points
 */

 fprintf(fp,"%d\n",threedpoint_nbr);

 /*
 For each 3D point, print:
 XYZ, RGB, number of measurements, list of measurements
 Measurement = image index, feature index, xy
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

    if ( imagepoint_nbr == 0 ) {
       sfm10_error_handler("sfm_print_threedpoints_in");
    }

    /*
    Print the 3D point coordinates
    */

    for ( i= 0 ; i< 3 ; i++ )
     fprintf(fp,"%g ",xyz[i]);

    /*
    Print the rgb for the 3D point
    */

    for ( cind= 0 ; cind< 3 ; cind++ )
     fprintf(fp,"%d ",rgb[cind]);

    /*
    Print number of measurements
    */

    fprintf(fp,"%d ",imagepoint_nbr);

    /*
    For each measurement, print:
    image index, feature index, xy
    Note that xy is expected to be w/r to image center
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
       Get camera index w/t to camera_matrix_arr
       */ 

       sfm_get_camera_matrix_ind(
        camera_matrix_arr,
        camera_matrix_nbr,
        camera_ind,
        &camera_matrix_ind
       );

       /*
       We need the width and height of the image
       but it would be too expensive to load the image
       to get that info
       */

       width= width0; 
       height= height0; 

       /*
       Print camera index w/r to camera_matrix_arr
       */

       fprintf(fp,"%d ",camera_matrix_ind);

       /*
       Print feature index
       */

       fprintf(fp,"%d ",feat_ind);

       /*
       Print xy
       */

       fprintf(fp,"%g ",x-(double)width/2);
       fprintf(fp,"%g ",y-(double)height/2);
    }

    fprintf(fp,"\n");
 }

 /*
 Skip a line
 */

 fprintf(fp,"\n");

 /*
 Print 0
 */

 fprintf(fp,"0\n");

 /*
 Print some verbose
 */

 fprintf(fp,"#the last part of NVM file points to the PLY files\n");
 fprintf(fp,"#the first number is the number of associated PLY files\n");
 fprintf(fp,"#each following number gives a model-index that has PLY\n");

 /*
 Print 0
 */

 fprintf(fp,"0\n");

 fclose(fp);

}

#include "header.h"

void sfm_print_threedpoints_gif_b(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

/*
The idea is to print a bunch of frames
which can be used to make an animated gif
*/

/*
We are gonna assume the camera coordinate system for camera_ind=0
is the world coordinate system, in other words,
R = identity matrix
t = null vector
This is actually what was assumed
when reconstructing the 3D scene from the initial pair
*/

{

 char image_filename[80];
 double pi=acos(-1);
 int camera_ind;
 int err_flag;
 int *image_arr;
 int width;
 int height;
 double f;
 int camera_matrix_ind;
 double P[3*4];
 double K[3*3];
 double R[3*3];
 double t[3];
 int frame_nbr=12;
 int frame_ind;
 double theta_deg;
 double theta;
 double C[3];
 double radius=1.0;
 int radius2=4;
 int i;
 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr; 
 double x3[3];
 double x;
 double y;
 int j;
 int cind;
 char filename[80];
 int i_offset;
 int j_offset;
 int i2;
 int j2;
 int pixel2;
 double cent[3];
 double tangent_x;
 double tangent_y;
 double angle_x;
 double angle_y;
 double R_x[3*3];
 double R_y[3*3];
 double RC[3];

 if ( frame_nbr > 99 ) {
    sfm10_error_handler("sfm_print_threedpoints_gif");
 }

 camera_ind= 0;

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

 f= focal_length_arr[camera_ind];

 /*
 Get camera matrix
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
 K should be f 0 width/2 ; 0 f height/2 ; 0 0 1
 R should be the identity matrix
 t should the null vector
 */

 for ( i= 0 ; i< 3 ; i++ )
  cent[i]= 0;

 /*
 Compute the centroid of the 3D scene
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

    for ( i= 0 ; i< 3 ; i++ )
     cent[i]+= xyz[i];
 }

 for ( i= 0 ; i< 3 ; i++ )
  cent[i]/= (double)threedpoint_nbr;

 /*
 We are gonna move a camera around a circle
 to get the frames
 Note that the camera is not rotated w/r to the world coordinates
 although it could be turned towards the 3D scene if desired
 */

 for ( frame_ind= 0 ;
       frame_ind< frame_nbr ;
       frame_ind++ ) {

    theta_deg= ((double)frame_ind/(double)frame_nbr)*360;
    theta= theta_deg*2*pi/360;
    C[0]= 0+cos(theta)*radius;
    C[1]= 0+sin(theta)*radius;
    C[2]= 0;

    /*
    Compute R
    */

    tangent_x= C[0]/cent[2];
    angle_y=-atan(tangent_x);
    tangent_y= C[1]/cent[2];
    angle_x= atan(tangent_y);
    R_x[0*3+0]= 1;
    R_x[0*3+1]= 0;
    R_x[0*3+2]= 0;
    R_x[1*3+0]= 0;
    R_x[1*3+1]= cos(angle_x);
    R_x[1*3+2]=-sin(angle_x);
    R_x[2*3+0]= 0;
    R_x[2*3+1]= sin(angle_x);
    R_x[2*3+2]= cos(angle_x);
    R_y[0*3+0]= cos(angle_y);
    R_y[0*3+1]= 0;
    R_y[0*3+2]= sin(angle_y);
    R_y[1*3+0]= 0;
    R_y[1*3+1]= 1;
    R_y[1*3+2]= 0;
    R_y[2*3+0]=-sin(angle_y);
    R_y[2*3+1]= 0;
    R_y[2*3+2]= cos(angle_y);

    math_matrix_matrix_product(
     R_x,
     3,
     3,
     R_y,
     3,
     3,
     R 
    );

    /*
    Compute t, the translation vector
    t = -R*C
    */

    math_matrix_vector_product(
     R,
     3,
     3,
     C,
     3,
     RC 
    );

    for ( i= 0 ; i< 3 ; i++ )
     t[i]=-RC[i];

    /*
    Allocate memoru for the image
    */

    image_arr= (int *)calloc(width*height,3*sizeof(int));

    /*
    Project the 3D points onto the image plane
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
       Project the 3D point onto the image plane
       */

       ba_project_point(
        K,
        R,
        t,
        xyz,
        x3
       );

       x= x3[0]/x3[2];
       y= x3[1]/x3[2];

       if ( y >= 0 )
        i= (int)(y+0.5);
       else 
        i= (int)(y-0.5);

       if ( x >= 0 )
        j= (int)(x+0.5);
       else
        j= (int)(x-0.5);

       /*
       Store the 3D point color in the image
       */

       for ( i_offset= -radius2 ;
             i_offset<=+radius2 ;
             i_offset++ ) {

          i2= i+i_offset;

          if ( i2 < 0 )
           continue;
          if ( i2 > height-1 )
           continue;

          for ( j_offset= -radius2 ;
                j_offset<=+radius2 ;
                j_offset++ ) {

             j2= j+j_offset;

             if ( j2 < 0 )
              continue;
             if ( j2 > width-1 )
              continue;

             pixel2=i2*width+j2;

             for ( cind= 0 ; cind< 3 ; cind++ )
              image_arr[3*pixel2+cind]= rgb[cind];
          }
       }
    }

    /*
    Print the image (as a jpg file)
    */

    sprintf(filename,"duh_%02d.jpg",frame_ind);

    err_flag= write_rgb_image(
     filename,
     image_arr,
     width,
     height
    );

    free(image_arr);
 }

}

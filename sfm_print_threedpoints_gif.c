#include "header.h"

void sfm_print_threedpoints_gif(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int radius,
 double max_theta_deg
)

/*
The idea is to print a bunch of frames
which can be used to make an animated gif
*/

/*
We are gonna rotate the 3D scene and
keep the camera (camera_matrix_ind=0) still
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
 double R_x[3*3];
 double R_y[3*3];
 double xyz2[3];
 double theta_deg_y;
 double theta_deg_x;
 double theta_y;
 double theta_x;
 int *image2_arr;
 int pixel;
 int threedpoint_ind2;

 if ( frame_nbr > 99 ) {
    sfm10_error_handler("sfm_print_threedpoints_gif");
 }

 camera_matrix_ind= 0;

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

 /*
 Get the focal length
 */

 f= focal_length_arr[camera_ind];

 /*
 K should be f 0 width/2 ; 0 f height/2 ; 0 0 1
 R should be the identity matrix
 t should the null vector
 */

 /*
 Compute the centroid of the 3D scene
 */

 for ( i= 0 ; i< 3 ; i++ )
  cent[i]= 0;

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

    for ( i= 0 ; i< 3 ; i++ )
     cent[i]+= xyz[i];
 }

 for ( i= 0 ; i< 3 ; i++ )
  cent[i]/= (double)threedpoint_nbr;

 for ( frame_ind = 0 ;
       frame_ind< frame_nbr ;
       frame_ind++ ) {
    theta_deg= ((double)frame_ind/(double)frame_nbr)*360;
    theta= theta_deg*pi/180;
    theta_deg_y= cos(theta)*max_theta_deg;
    theta_deg_x= sin(theta)*max_theta_deg;
    theta_y= theta_deg_y*pi/180;
    theta_x= theta_deg_x*pi/180;

    /*
    Compute the rotation around x
    */

    R_x[0*3+0]= 1;
    R_x[0*3+1]= 0;
    R_x[0*3+2]= 0;
    R_x[1*3+0]= 0;
    R_x[1*3+1]= cos(theta_x);
    R_x[1*3+2]=-sin(theta_x);
    R_x[2*3+0]= 0;
    R_x[2*3+1]= sin(theta_x);
    R_x[2*3+2]= cos(theta_x);

    /*
    Compute the rotation around y
    */

    R_y[0*3+0]= cos(theta_y);
    R_y[0*3+1]= 0;
    R_y[0*3+2]= sin(theta_y);
    R_y[1*3+0]= 0;
    R_y[1*3+1]= 1;
    R_y[1*3+2]= 0;
    R_y[2*3+0]=-sin(theta_y);
    R_y[2*3+1]= 0;
    R_y[2*3+2]= cos(theta_y);

    /*
    Compute combined rotation R = R_y * R_x
    */

    math_matrix_matrix_product(
     R_y,
     3,
     3,
     R_x,
     3,
     3,
     R
    );

    /*
    Allocate memoru for the image
    */

    image_arr= (int *)calloc(width*height,3*sizeof(int));

    /*
    Keep track of pixels that have already been painted on
    */

    image2_arr= (int *)calloc(width*height,sizeof(int));

    for ( i= 0 ; i< height ; i++ ) {
       for ( j= 0 ; j< width ; j++ ) {
          pixel= i*width+j;
          image2_arr[pixel]=-1;
       }
    }

    /*
    Project the 3D points onto the image plane
    after having rotated them around the centroid of the 3D scene
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
       Check if 3D point actually exists
       */

       if ( imagepoint_nbr == 0 )
        continue;

       /*
       Compute 3D point location
       relative to centroid of the 3D scene
       */

       for ( i= 0 ; i< 3 ; i++ )
        xyz2[i]= xyz[i]-cent[i];

       /*
       Rotate the 3d point
       */

       math_matrix_vector_product(
        R,
        3,
        3,
        xyz2,
        3,
        xyz
       );

       /*
       Compute rotated 3D point location
       relative to world coordinates origin
       */

       for ( i= 0 ; i< 3 ; i++ )
        xyz[i]+= cent[i];

       /*
       Project the 3D point onto the image plane
       */

       math_matrix_vector_product(
        K,
        3,
        3,
        xyz,
        3,
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
       using a disk centered on the 2D image point
       instead of just the 2D image point
       */

       for ( i_offset= -radius ;
             i_offset<=+radius ;
             i_offset++ ) {

          i2= i+i_offset;

          if ( i2 < 0 )
           continue;
          if ( i2 > height-1 )
           continue;

          for ( j_offset= -radius ;
                j_offset<=+radius ;
                j_offset++ ) {

             j2= j+j_offset;

             if ( j2 < 0 )
              continue;
             if ( j2 > width-1 )
              continue;

             pixel2=i2*width+j2;

             /*
             See if pixel has already been painted
             */

             threedpoint_ind2= image2_arr[pixel2];

             if ( threedpoint_ind2 != -1 ) { 

                /*
                See if that 3D point is in front of
                currently processed 3D point
                in the 3D scene
                */

                xyz2[0]= threedpoint_arr[threedpoint_ind2].xyz[0];
                xyz2[1]= threedpoint_arr[threedpoint_ind2].xyz[1];
                xyz2[2]= threedpoint_arr[threedpoint_ind2].xyz[2];

                if ( xyz2[2] < xyz[2] ) {

                   /*
                   That 3D point is in front of
                   currently processed 3D point
                   Currently processed 3D point should not be visible
                   */

                   continue;
                }
             }

             /*
             Store the color of the 3D point
             */

             for ( cind= 0 ; cind< 3 ; cind++ )
              image_arr[3*pixel2+cind]= rgb[cind];

             /*
             Keep track of 3D point
             */

             image2_arr[pixel2]= threedpoint_ind;
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

    free(image2_arr);
 }

}

#include "header.h"

void sfm_add_camera_matrix(
 int camera_ind,
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double t[3],
 sfm_camera_matrix_struct **pcamera_matrix_arr,
 int *pcamera_matrix_nbr,
 int *pcamera_matrix_ind
)

{

 sfm_camera_matrix_struct *camera_matrix_arr;
 int camera_matrix_nbr;
 int camera_matrix_ind;
 int i;
 int j;

 camera_matrix_arr= (*pcamera_matrix_arr);
 camera_matrix_nbr= (*pcamera_matrix_nbr);

 if ( camera_matrix_nbr == 0 ) {
    camera_matrix_arr= (sfm_camera_matrix_struct *)calloc(camera_matrix_nbr+1,sizeof(sfm_camera_matrix_struct));
 }
 else {
    camera_matrix_arr= (sfm_camera_matrix_struct *)realloc(camera_matrix_arr,(camera_matrix_nbr+1)*sizeof(sfm_camera_matrix_struct));
 }

 camera_matrix_ind= camera_matrix_nbr;
 camera_matrix_nbr++;

 camera_matrix_arr[camera_matrix_ind].camera_ind= camera_ind;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 4 ; j++ ) {
       camera_matrix_arr[camera_matrix_ind].P[i*4+j]= P[i*4+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       camera_matrix_arr[camera_matrix_ind].K[i*3+j]= K[i*3+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       camera_matrix_arr[camera_matrix_ind].R[i*3+j]= R[i*3+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    camera_matrix_arr[camera_matrix_ind].t[i]= t[i];
 }

 (*pcamera_matrix_arr)= camera_matrix_arr;
 (*pcamera_matrix_nbr)= camera_matrix_nbr;
 (*pcamera_matrix_ind)= camera_matrix_ind;

}

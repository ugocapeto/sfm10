#include "header.h"

void sfm_read_threedpoints(
 char *filename,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr
)

{

 FILE *fp;
 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 int camera_ind;
 int feat_ind;
 double x;
 double y;
 int imagepoint_ind;
 sfm_threedpoint_struct *threedpoint_arr;
 int threedpoint_nbr;

 threedpoint_arr= 0;
 threedpoint_nbr= 0;

 fp= fopen(filename,"r");

 while ( fscanf(fp,"%lg %lg %lg %d %d %d %d",
          &xyz[0],&xyz[1],&xyz[2],&rgb[0],&rgb[1],&rgb[2],&imagepoint_nbr) == 7 ) {
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
       fscanf(fp,"%d %d %lg %lg",
        &camera_ind,&feat_ind,&x,&y);
       imagepoint_arr[imagepoint_ind].camera_ind= camera_ind;
       imagepoint_arr[imagepoint_ind].feat_ind= feat_ind;
       imagepoint_arr[imagepoint_ind].x= x;
       imagepoint_arr[imagepoint_ind].y= y;
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

 fclose(fp);

 *pthreedpoint_arr= threedpoint_arr;
 *pthreedpoint_nbr= threedpoint_nbr;

}

#include "header.h"

void sfm_print_threedpoints_ply(
 char *filename,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

{

 FILE *fp;
 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 sfm_imagepoint_struct imagepoint;
 int camera_ind;
 int feat_ind;
 double x;
 double y;
 int imagepoint_ind;

 fp= fopen(filename,"w");

 fprintf(fp,"ply\n");
 fprintf(fp,"format ascii 1.0\n");
 fprintf(fp,"element vertex %d\n",threedpoint_nbr);
 fprintf(fp,"property float x\n");
 fprintf(fp,"property float y\n");
 fprintf(fp,"property float z\n");
 fprintf(fp,"property uchar red\n");
 fprintf(fp,"property uchar green\n");
 fprintf(fp,"property uchar blue\n");
 fprintf(fp,"end_header\n");

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

    fprintf(fp,"%g %g %g %d %d %d\n",
     xyz[0],xyz[1],xyz[2],
     rgb[0],rgb[1],rgb[2]);

    for ( imagepoint_ind= 0 ;
          imagepoint_ind< imagepoint_nbr ;
          imagepoint_ind++ ) {
       imagepoint= imagepoint_arr[imagepoint_ind];
       camera_ind= imagepoint.camera_ind;
       feat_ind= imagepoint.feat_ind;
       x= imagepoint.x;
       y= imagepoint.y;
    }
 }

 fclose(fp);

}

#include "header.h"

void sfm_read_image_filenames(
 char *filename,
 char ***pimage_filename_arr,
 int *pcamera_nbr
)

{

 FILE *fp;
 char image_filename[80];
 char **image_filename_arr;
 int camera_nbr;
 int camera_ind;

 fp= fopen(filename,"r");

 image_filename_arr= 0;
 camera_nbr= 0;

 while ( fscanf(fp,"%s",
          image_filename) == 1 ) {

    if ( camera_nbr == 0 ) {
       image_filename_arr= (char **)calloc(camera_nbr+1,sizeof(char *));
    }
    else {
       image_filename_arr= (char **)realloc(image_filename_arr,(camera_nbr+1)*sizeof(char *));
    }

    camera_ind= camera_nbr;
    camera_nbr++;
    image_filename_arr[camera_ind]= (char *)calloc(80,sizeof(char));

    strcpy(image_filename_arr[camera_ind],image_filename);
 }

 fclose(fp);

 (*pimage_filename_arr)= image_filename_arr;
 (*pcamera_nbr)= camera_nbr;

}

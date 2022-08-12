#include "header.h"

void sfm_read_P(
 char *filename,
 double P[3*4]
)

{

 FILE *fp;
 int i;
 int j;

 fp= fopen(filename,"r");

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 4 ; j++ ) {
       fscanf(fp,"%lg",
        &P[i*4+j]);
    }
 }

 fclose(fp);

}

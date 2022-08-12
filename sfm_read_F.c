#include "header.h"

void sfm_read_F(
 char *filename,
 double F[3*3]
)

{

 FILE *fp;
 int i;
 int j;

 fp= fopen(filename,"r");

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fscanf(fp,"%lg",
        &F[i*3+j]);
    }
 }

 fclose(fp);

}

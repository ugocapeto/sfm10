#include "header.h"

int sfm_common(
 int n1,
 int arr1[],
 int n2,
 int arr2[]
)

{

 int i;
 int j;
 int count;

 count= 0;
 for ( i= 0 ; i< n1 ; i++ ) {
    for ( j= 0 ; j< n2 ; j++ ) {
       if ( arr1[i] == arr2[j] ) {
          count++;
          break;
       }
    }
 }

 return count;

}

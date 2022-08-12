#include "header.h"
#include "proto.h"
#include "tiff_proto.h"

int *global_image_arr;
int global_width;
int global_height;
int global_min_d;
int global_max_d;
mesh_struct *global_mesh;
matrix_int_crs_struct *global_splat_matrix;
int *global_blur_matrix[5];
double *global_Cn_matrix;
int *global_Cs_matrix;
double global_lambda;
int *global_disp_lower_bound;
int *global_disp_upper_bound;

void stereo_compute_disparity_map(
 int width,
 int height,
 int *image_arr,
 int *image2_arr,
 int min_d,
 int max_d,
 int sample_rate_spatial,
 int sample_rate_range,
 int radius,
 int epsilon,
 double lambda,
 int max_iterations,
 int hash_table_size,
 int *disp_arr
)

{

 mesh_struct *mesh;
 matrix_int_crs_struct *splat_matrix;
 int *blur_matrix[5];
 double *Cn_matrix;
 int *Cs_matrix;
 int *disp_lower_bound;
 int *disp_upper_bound;
 int dim;
 double *v_arr;
 int nbr_pixels;
 arrayHeader *mesh_verts;
 int maxInd;
 int nbr_verts;
 int pixel;
 int vert;
 double d_dbl;
 int d_int;
 int *temp_disp_arr;
 int *temp_disp_normalized_arr;
 int *pixel2vert_arr;
 int ind;
 int *disp_lower_bound_old;
 int *disp_upper_bound_old;
 int i;
 int j;
 int *con_image_arr;

 /*
 Get nbr of pixels
 */

 nbr_pixels= width*height;

 /*
 Compute the disparity lower and upper bounds
 */

 fprintf(stdout,"Computing the disparity lower and upper bounds ...\n");

 /*
 stereo_compute_disparity_bounds_old(
  image_arr,
  image2_arr,
  width,
  height,
  min_d,
  max_d,
  radius,
  &disp_lower_bound_old,
  &disp_upper_bound_old
 );
 */

 stereo_compute_disparity_bounds(
  image_arr,
  image2_arr,
  width,
  height,
  min_d,
  max_d,
  radius,
  epsilon,
  &disp_lower_bound,
  &disp_upper_bound
 );

 /*
 for ( i= 0 ; i< height ; i++ ) {
    for ( j= 0 ; j< width ; j++ ) {
       ind= i*width+j;
       if ( disp_lower_bound[ind] !=
            disp_lower_bound_old[ind] ) {
          sfm10_error_handler("stereo_compute_disparity_map");
       }
       if ( disp_upper_bound[ind] !=
            disp_upper_bound_old[ind] ) {
          sfm10_error_handler("stereo_compute_disparity_map");
       }
    }
 }
 */

 fprintf(stdout,"Computing the disparity lower and upper bounds ... done.\n");

 /*
 I think it's a good idea to write a depth map
 based on data cost only
 */

 temp_disp_arr= (int *)calloc(width*height,sizeof(int));

 stereo_compute_disparity_map_unsmoothed(
  width,
  height,
  image_arr,
  min_d,
  max_d,
  disp_lower_bound,
  disp_upper_bound,
  temp_disp_arr
 );

 temp_disp_normalized_arr= (int *)calloc(width*height,sizeof(int));

 image_normalize(
  temp_disp_arr,
  temp_disp_normalized_arr,
  width*height,
  min_d,
  max_d
 );

 write_image_arr_into_tiff("duh_disp.tiff",width,height,temp_disp_normalized_arr);

 free(temp_disp_arr);

 free(temp_disp_normalized_arr);

 /*
 I think it's a good idea to write a confidence map
 based on data cost only
 */

 con_image_arr= (int *)calloc(width*height,sizeof(int));

 stereo_compute_confidence_map(
  width,
  height,
  image_arr,
  min_d,
  max_d,
  disp_lower_bound,
  disp_upper_bound,
  con_image_arr
 );

 write_image_arr_into_tiff("duh_conf.tiff",width,height,con_image_arr);

 free(con_image_arr);

 /*
 Create the bilateral space verts
 */

 fprintf(stdout,"Creating the bilateral space verts ...\n");

 stereo_create_verts(
  image_arr,
  width,
  height,
  sample_rate_spatial,
  sample_rate_range,
  hash_table_size,
  &mesh,
  &splat_matrix,
  blur_matrix
 );

 fprintf(stdout,"Creating the bilateral space verts ... done.\n");

 /*
 Do some detective work with the verts
 */

 /*
 stereo_print_verts(
  image_arr,
  width,
  height,
  min_d,
  max_d,
  disp_lower_bound,
  disp_upper_bound,
  mesh,
  splat_matrix
 );
 */

 /*
 Build the Cn and Cs matrix
 */

 fprintf(stdout,"Building the Cn and Cs matrix ...\n");

 stereo_build_Cn_Cs_matrix(
  image_arr,
  width,
  height,
  mesh,
  splat_matrix,
  blur_matrix,
  &Cn_matrix,
  &Cs_matrix
 );

 fprintf(stdout,"Building the Cn and Cs matrix ... done.\n");

 /*
 Perform the L-BFGS optimization
 */

 fprintf(stdout,"Performing the L-BFGS optimization ...\n");

 global_image_arr= image_arr;
 global_width= width;
 global_height= height;
 global_min_d= min_d;
 global_max_d= max_d;
 global_mesh= mesh;
 global_splat_matrix= splat_matrix;
 for ( dim= 0 ; dim< 5 ; dim++ )
  global_blur_matrix[dim]= blur_matrix[dim];
 global_Cn_matrix= Cn_matrix;
 global_Cs_matrix= Cs_matrix;
 global_lambda= lambda;
 global_disp_lower_bound= disp_lower_bound;
 global_disp_upper_bound= disp_upper_bound;

 stereo_lbfgs(
  max_iterations,
  &v_arr
 );

 fprintf(stdout,"Performing the L-BFGS optimization ... done.\n");

 /*
 All that's left to do is transfer the disparities
 from bilateral space to pixel space
 */

 fprintf(stdout,"Transfering the disparities to pixel-space ...\n");

 /*
 Get nbr of pixels
 */

 nbr_pixels= width*height;

 /*
 Get nbr of verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 /*
 Go from bilateral space solution v_arr
 to pixel space solution disp_arr
 */

 myCalloc(pixel2vert_arr,int,nbr_pixels,sizeof(int));

 for ( vert= 0 ; vert< nbr_verts ; vert++ ) {
    for ( ind= splat_matrix->row_ptr[vert] ;
          ind< splat_matrix->row_ptr[vert+1] ;
          ind++ ) {
       pixel= splat_matrix->col_ind[ind];
       pixel2vert_arr[pixel]= vert;
    }
 }

 for ( pixel= 0 ; pixel< nbr_pixels ; pixel++ ) {
    vert= pixel2vert_arr[pixel];

    /*
    Get the disparity for that vert
    Note that it's a double and it could be out of bounds
    */

    d_dbl= v_arr[vert];

    if ( d_dbl >= 0 )
     d_int= (int)(d_dbl+0.5);
    else
     d_int= (int)(d_dbl-0.5);

    /*
    If not in bounds, put in bounds
    */

    if ( d_int < min_d )
     d_int= min_d;
    if ( d_int > max_d )
     d_int= max_d;

    /*
    Store the pixel disparity
    */

    disp_arr[pixel]= d_int;
 }

 fprintf(stdout,"Transfering the disparities to pixel-space ... done.\n");

 /*
 Free the mesh
 */

 mesh_free(mesh);

 /*
 Free the splat matrix
 */

 myFree(splat_matrix->val);
 myFree(splat_matrix->col_ind);
 myFree(splat_matrix->row_ptr);
 myFree(splat_matrix);

 /*
 Free the blur matrix
 */

 for ( dim= 0 ; dim< 5 ; dim++ ) {
    myFree(blur_matrix[dim]);
 }

 /*
 Free the Cn matrix
 */

 myFree(Cn_matrix);

 /*
 Free the Cs matrix
 */

 myFree(Cs_matrix);

 /*
 Free the disparity lower bound
 */

 myFree(disp_lower_bound);

 /*
 Free the disparity upper bound
 */

 myFree(disp_upper_bound);

 /*
 Free v_arr
 */

 myFree(v_arr);

 /*
 Free pixel2vert_arr
 */

 myFree(pixel2vert_arr);

}

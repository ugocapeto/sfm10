#include "header.h"
#include "proto.h"

extern int *global_image_arr;
extern int global_width;
extern int global_height;
extern int global_min_d;
extern int global_max_d;
extern mesh_struct *global_mesh;
extern matrix_int_crs_struct *global_splat_matrix;
extern int *global_blur_matrix[5];
extern double *global_Cn_matrix;
extern int *global_Cs_matrix;
extern double global_lambda;
extern int *global_disp_lower_bound;
extern int *global_disp_upper_bound;

static lbfgsfloatval_t evaluate(
    void *instance,
    const lbfgsfloatval_t *x,
    lbfgsfloatval_t *g,
    const int n,
    const lbfgsfloatval_t step
    )
{

 int *image_arr;
 int width;
 int height;
 mesh_struct *mesh;
 matrix_int_crs_struct *splat_matrix;
 int *blur_matrix[5];
 double *Cn_matrix;
 int *Cs_matrix;
 double lambda;
 int *disp_lower_bound;
 int *disp_upper_bound;
 int dim;
 double cost;
 double *v_arr;
 arrayHeader *mesh_verts;
 int maxInd;
 int nbr_verts;
 int vert;
 double *cost_grad;
 int min_d;
 int max_d;

 image_arr= global_image_arr;
 width= global_width;
 height= global_height;
 min_d= global_min_d;
 max_d= global_max_d;
 mesh= global_mesh;
 splat_matrix= global_splat_matrix;
 for ( dim= 0 ; dim< 5 ; dim++ )
  blur_matrix[dim]= global_blur_matrix[dim];
 Cn_matrix= global_Cn_matrix;
 Cs_matrix= global_Cs_matrix;
 lambda= global_lambda;
 disp_lower_bound= global_disp_lower_bound;
 disp_upper_bound= global_disp_upper_bound;

 /*
 Get nbr of verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 if ( nbr_verts != n ) {
    sfm10_error_handler("stereo_lbfgs");
 }

 /*
 Allocate memory for v_arr (bilateral space disparities)
 */

 myCalloc(v_arr,double,nbr_verts,sizeof(double));

 /*
 Copy x into v_arr
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ )
  v_arr[vert]= (double)x[vert];

 /*
 Compute the cost and the cost gradient
 */

 stereo_compute_cost(
  image_arr,
  width,
  height,
  min_d,
  max_d,
  mesh,
  splat_matrix,
  blur_matrix,
  Cn_matrix,
  Cs_matrix,
  lambda,
  v_arr,
  disp_lower_bound,
  disp_upper_bound,
  &cost,
  &cost_grad
 );

 /*
 Copy the cost gradient into g
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ )
  g[vert]= (lbfgsfloatval_t)cost_grad[vert];

 /*
 Free v_arr
 */

 if ( nbr_verts > 0 )
  myFree(v_arr);

 /*
 Free cost_grad
 */

 if ( nbr_verts > 0 )
  free(cost_grad);

 /*
 Return the cost
 */

 return (lbfgsfloatval_t)cost;

}

static int progress(
    void *instance,
    const lbfgsfloatval_t *x,
    const lbfgsfloatval_t *g,
    const lbfgsfloatval_t fx,
    const lbfgsfloatval_t xnorm,
    const lbfgsfloatval_t gnorm,
    const lbfgsfloatval_t step,
    int n,
    int k,
    int ls
    )
{
    /*
    printf("Iteration %d:\n", k);
    printf("  fx = %f\n", fx);
    printf("  xnorm = %f, gnorm = %f, step = %f\n", xnorm, gnorm, step);
    printf("\n");
    */
    return 0;
}

void stereo_lbfgs(
 int max_iterations,
 double **ptr_v_arr
)

{

 lbfgsfloatval_t *x;
 lbfgs_parameter_t param;
 arrayHeader *mesh_verts;
 mesh_struct *mesh;
 int maxInd;
 int nbr_verts;
 int ret;
 int i;
 lbfgsfloatval_t fx; 
 int N;
 double *v_arr;
 int vert;
 int min_d;
 int max_d;

 mesh= global_mesh;
 min_d= global_min_d;
 max_d= global_max_d;

 /*
 Get nbr of verts
 */

 mesh_verts= mesh->mesh_verts;
 maxInd= arrayMaxId(*mesh_verts);
 nbr_verts= maxInd+1;

 /*
 Set nbr of unknowns N
 */

 N = nbr_verts;

 /*
 Allocate the vector of unknowns x
 */

 x = lbfgs_malloc(N);

 if (x == NULL) {
     printf("ERROR: Failed to allocate a memory block for variables.\n");
 }

 /* Initialize the variables. */
 for ( i= 0 ; i< N ; i+= 1 ) {
     x[i] = 0.5*(min_d+max_d);
 }

 /* Initialize the parameters for the L-BFGS optimization. */
 lbfgs_parameter_init(&param);
 /*param.linesearch = LBFGS_LINESEARCH_BACKTRACKING;*/

 /*
 Set parameters for the L-BFGS optimization
 */

 param.max_iterations = max_iterations;

 /*
     Start the L-BFGS optimization; this will invoke the callback functions
     evaluate() and progress() when necessary.
 */
 ret = lbfgs(N, x, &fx, evaluate, progress, NULL, &param);

 /* Report the result. */
 printf("L-BFGS optimization terminated with status code = %d\n", ret);
 printf("  fx = %f\n", fx);

 /*
 Allocate memory for v_arr
 */

 myCalloc(v_arr,double,nbr_verts,sizeof(double));

 /*
 Copy the x vector of solved unknowns onto v_arr
 */

 for ( vert= 0 ; vert< nbr_verts ; vert++ )
  v_arr[vert]= (double)x[vert];

 lbfgs_free(x);

 (*ptr_v_arr)= v_arr;

}

#include "header.h"

extern ba_measurement_struct *global_measurement_arr;
extern int global_measurement_nbr;
extern ba_camera_internal_struct *global_camera_internal_arr;
extern double *global_p_camera_arr;
extern double *global_p_point_arr;
extern int *global_p_camera2_arr;
extern int *global_p_point2_arr;
extern double *global_p_grad_camera_arr;
extern double *global_p_grad_point_arr;
extern int global_p_camera_nbr;
extern int global_p_point_nbr;

static lbfgsfloatval_t evaluate(
    void *instance,
    const lbfgsfloatval_t *x,
    lbfgsfloatval_t *g,
    const int n,
    const lbfgsfloatval_t step
    )
{

 ba_measurement_struct *measurement_arr;
 int measurement_nbr;
 ba_camera_internal_struct *camera_internal_arr;
 double *p_camera_arr;
 double *p_point_arr;
 int *p_camera2_arr;
 int *p_point2_arr;
 double *p_grad_camera_arr;
 double *p_grad_point_arr;
 int p_camera_nbr;
 int p_point_nbr;
 int x_ind;
 int p_camera_ind;
 int i;
 int p_point_ind;
 double E;

 measurement_arr= global_measurement_arr;
 measurement_nbr= global_measurement_nbr;
 camera_internal_arr= global_camera_internal_arr;
 p_camera_arr= global_p_camera_arr;
 p_point_arr= global_p_point_arr;
 p_camera2_arr= global_p_camera2_arr;
 p_point2_arr= global_p_point2_arr;
 p_grad_camera_arr= global_p_grad_camera_arr;
 p_grad_point_arr= global_p_grad_point_arr;
 p_camera_nbr= global_p_camera_nbr;
 p_point_nbr= global_p_point_nbr;

 /*
 Go from x to p_camera_arr and p_point_arr
 */

 x_ind= 0;
 for ( p_camera_ind= 0 ;
       p_camera_ind< p_camera_nbr ;
       p_camera_ind++ ) {
    for ( i= 0 ; i< 6 ; i++ ) {
       p_camera_arr[p_camera_ind*6+i]= x[x_ind];
       x_ind++;
    }
 }
 for ( p_point_ind= 0 ;
       p_point_ind< p_point_nbr ;
       p_point_ind++ ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       p_point_arr[p_point_ind*3+i]= x[x_ind];
       x_ind++;
    }
 }

 E= ba_evaluate(
  measurement_arr,
  measurement_nbr,
  camera_internal_arr,
  p_camera_arr,
  p_point_arr,
  p_grad_camera_arr,
  p_grad_point_arr,
  p_camera_nbr,
  p_point_nbr
 );

 /*
 Copy the gradient of E
 w/r to the camera parameters and point parameters
 into g
 */

 x_ind= 0;
 for ( p_camera_ind= 0 ;
       p_camera_ind< p_camera_nbr ;
       p_camera_ind++ ) {
    for ( i= 0 ; i< 6 ; i++ ) {
       g[x_ind]= p_grad_camera_arr[p_camera_ind*6+i];
       if ( p_camera2_arr[p_camera_ind*6+i] != 0 ) {

          /*
          Constrain the camera parameter not to change
          by forcing the gradient to be 0
          */

          g[x_ind]= 0;
       }
       x_ind++;
    }
 }
 for ( p_point_ind= 0 ;
       p_point_ind< p_point_nbr ;
       p_point_ind++ ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       g[x_ind]= p_grad_point_arr[p_point_ind*3+i];
       if ( p_point2_arr[p_point_ind*3+i] != 0 ) {

          /*
          Constrain the point parameter not to change
          by forcing the gradient to be 0
          */

          g[x_ind]= 0;
       } 
       x_ind++;
    }
 }

 /*
 Return the value of the function
 */

 return (lbfgsfloatval_t)E;

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

void ba_lbfgs(
 int max_iterations
)

{

 ba_measurement_struct *measurement_arr;
 int measurement_nbr;
 ba_camera_internal_struct *camera_internal_arr;
 double *p_camera_arr;
 double *p_point_arr;
 double *p_grad_camera_arr;
 double *p_grad_point_arr;
 int p_camera_nbr;
 int p_point_nbr;
 lbfgsfloatval_t *x;
 lbfgs_parameter_t param;
 lbfgsfloatval_t fx;
 int N;
 int ret;
 int x_ind;
 int p_camera_ind;
 int i;
 int p_point_ind;

 measurement_arr= global_measurement_arr;
 measurement_nbr= global_measurement_nbr;
 camera_internal_arr= global_camera_internal_arr;
 p_camera_arr= global_p_camera_arr;
 p_point_arr= global_p_point_arr;
 p_grad_camera_arr= global_p_grad_camera_arr;
 p_grad_point_arr= global_p_grad_point_arr;
 p_camera_nbr= global_p_camera_nbr;
 p_point_nbr= global_p_point_nbr;

 /*
 Set nbr of unknowns N
 */

 N = p_camera_nbr*6+p_point_nbr*3;

 /*
 Allocate the vector of unknowns x
 */

 x = lbfgs_malloc(N);

 if (x == NULL) {
     printf("ERROR: Failed to allocate a memory block for variables.\n");
 }

 /* Initialize the variables. */

 x_ind= 0;
 for ( p_camera_ind= 0 ;
       p_camera_ind< p_camera_nbr ;
       p_camera_ind++ ) {
    for ( i= 0 ; i< 6 ; i++ ) {
       x[x_ind]= p_camera_arr[p_camera_ind*6+i];
       x_ind++;
    }
 }
 for ( p_point_ind= 0 ;
       p_point_ind< p_point_nbr ;
       p_point_ind++ ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       x[x_ind]= p_point_arr[p_point_ind*3+i];
       x_ind++;
    }
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
 Update p_camera_arr and p_point_arr
 */

 x_ind= 0;
 for ( p_camera_ind= 0 ;
       p_camera_ind< p_camera_nbr ;
       p_camera_ind++ ) {
    for ( i= 0 ; i< 6 ; i++ ) {
       p_camera_arr[p_camera_ind*6+i]= x[x_ind];
       x_ind++;
    }
 }
 for ( p_point_ind= 0 ;
       p_point_ind< p_point_nbr ;
       p_point_ind++ ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       p_point_arr[p_point_ind*3+i]= x[x_ind];
       x_ind++;
    }
 }

 lbfgs_free(x);

}

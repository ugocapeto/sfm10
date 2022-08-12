void ba_create_quaternion_from_rotation_matrix_0(
 double R_[3*3],
 double q[4]
);

void ba_create_quaternion_from_rotation_matrix_1(
 double R[3*3],
 double q[4]
);

void ba_create_quaternion_from_rotation_matrix(
 double R[3*3],
 double q[4]
);

void ba_create_rodrigues_param_from_rotation_matrix(
 double R[3*3],
 double omega[3]
);

void ba_create_rotation_matrix_rodrigues(
 double omega[3],
 double R[3*3]
);

double ba_evaluate(
 ba_measurement_struct *measurement_arr,
 int measurement_nbr,
 ba_camera_internal_struct *camera_internal_arr,
 double *p_camera_arr,
 double *p_point_arr,
 double *p_grad_camera_arr,
 double *p_grad_point_arr,
 int p_camera_nbr,
 int p_point_nbr
);

void ba_evaluate_grad_camera(
 double K[3*3],
 double omega3[3],
 double T[3],
 double X3[3],
 double x3_proj[3],
 double deriv2x6[2*6]
);

void ba_evaluate_grad_camera(
 double K[3*3],
 double omega3[3],
 double T[3],
 double X3[3],
 double deriv2x6[2*6]
);

void ba_evaluate_grad_point(
 double K[3*3],
 double omega3[3],
 double T[3],
 double X3[3],
 double x3_proj[3],
 double deriv2x3[2*3]
);

void ba_lbfgs(
 int max_iterations
);

void ba_lbfgs_main(
 ba_measurement_struct *measurement_arr,
 int measurement_nbr,
 ba_camera_internal_struct *camera_internal_arr,
 double *p_camera_arr,
 double *p_point_arr,
 int *p_camera2_arr,
 int *p_point2_arr,
 double *p_grad_camera_arr,
 double *p_grad_point_arr,
 int p_camera_nbr,
 int p_point_nbr,
 int max_iterations
);

void ba_project_point(
 double K[3*3],
 double R[3*3],
 double T[3],
 double X3[3],
 double x3[3]
);

double ba_reprojection_error(
 ba_measurement_struct *measurement_arr,
 int measurement_nbr,
 ba_camera_internal_struct *camera_internal_arr,
 double *p_camera_arr,
 double *p_point_arr
);

void ba_rodrigues_rotation_derivatives(
 double omega3[3],
 double dR[3][3*3]
);

void compute_1st_camera_matrix(
 double P1[3*4]
);

void compute_2nd_camera_matrix_from_essential_matrix(
 double E[3*3],
 double P24[4][3*4]
);

void compute_2nd_camera_matrix_from_essential_matrix_horn(
 double E[3*3],
 double P24[4][3*4]
);

void compute_2nd_camera_matrix_from_fundamental_matrix(
 double F[3*3],
 double P2[3*4]
);

double compute_3dpoint_depth(
 double X[4],
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double C[3]
);

void compute_3dpoints(
 double P1[3*4],
 double P2[3*4],
 match_struct *match_arr,
 int match_nbr,
 double *points_arr
);

void compute_essential_matrix(
 double F[3*3],
 double K1[3*3],
 double K2[3*3],
 double E[3*3]
);

void compute_givens_rotation_matrix(
 double A[3*3],
 int axis,
 double Q[3*3]
);

void compute_linear_triangulation(
 double *P_arr,
 double *x_arr,
 double *y_arr,
 int imagepoint_nbr,
 double X[4]
);

void compute_linear_triangulation_two_cameras(
 double P1[3*4],
 double P2[3*4],
 double x1,
 double y1,
 double x2,
 double y2,
 double X[4]
);

void compute_quaternion_from_rotation(
 double M[3*3],
 double q[4]
);

void compute_rotation_from_quaternion(
 double q[4],
 double M[3*3]
);

void compute_RQ_decomposition(
 double A[3*3],
 double R[3*3],
 double Q[3*3]
);

void convert_features_and_descriptors_to_sfm(
 feat_struct *feat_arr,
 int feat_nbr,
 sfm_feat_struct **psfm_feat_arr
);

void decompose_camera_matrix(
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double C[3]
);

void epnp_choose_control_points(
 double *pws,
 int number_of_correspondences,
 double cws[4][3]
);

void epnp_compute_A_and_b_gauss_newton(
 double *l_6x10,
 double *rho,
 double betas[4],
 double *A,
 double *b
);

void epnp_compute_barycentric_coordinates(
 double *pws,
 double *alphas,
 int number_of_correspondences,
 double cws[4][3]
);

void epnp_compute_ccs(
 double *betas,
 double *ut,
 double ccs[4][3]
);

void epnp_compute_L_6x10(
 double *ut,
 double *l_6x10
);

void epnp_compute_pcs(
 double *alphas,
 double *pcs,
 int number_of_correspondences,
 double ccs[4][3]
);

double epnp_compute_pose(
 double fu,
 double fv,
 double uc,
 double vc,
 double *pws,
 double *us,
 int number_of_correspondences,
 double R[3][3],
 double t[3]
);

double epnp_compute_R_and_t(
 double fu,
 double fv,
 double uc,
 double vc,
 double *pws,
 double *us,
 double *alphas,
 double *pcs,
 int number_of_correspondences,
 double *ut,
 double *betas,
 double R[3][3],
 double t[3]
);

void epnp_compute_rho(
 double cws[4][3],
 double *rho
);

void epnp_copy_R_and_t(
 double R_src[3][3],
 double t_src[3],
 double R_dst[3][3],
 double t_dst[3]
);

double epnp_dist2(
 double *p1,
 double *p2
);

double epnp_dot(
 double *v1,
 double *v2
);

void epnp_estimate_R_and_t(
 double *pws,
 double *pcs,
 int number_of_correspondences,
 double R[3][3],
 double t[3]
);

void epnp_fill_M(
 double fu,
 double fv,
 double uc,
 double vc,
 double *M,
 int row,
 double *as,
 double u,
 double v
);

void epnp_find_betas_approx_1(
 double *L_6x10,
 double *Rho,
 double *betas
);

void epnp_find_betas_approx_2(
 double *L_6x10,
 double *Rho,
 double *betas
);

void epnp_find_betas_approx_3(
 double *L_6x10,
 double *Rho,
 double *betas
);

void epnp_gauss_newton(
 double *L_6x10,
 double *Rho,
 double betas[4]
);

void epnp_normal_equations_solve(
 double *A,
 int nr,
 int nc,
 double *b,
 double *x
);

void epnp_qr_solve(
 double *A,
 int nr,
 int nc,
 double *b,
 double *x
);

double epnp_reprojection_error(
 double fu,
 double fv,
 double uc,
 double vc,
 double *pws,
 double *us,
 int number_of_correspondences,
 double R[3][3],
 double t[3]
);

void epnp_solve_for_sign(
 double *pcs,
 int number_of_correspondences,
 double ccs[4][3]
);

void free_features_and_descriptors(
 feat_struct *feat_arr,
 int feat_nbr
);

void normalize_coordinates(
 double K1[3*3],
 double K2[3*3],
 match_struct *match_arr,
 int match_nbr
);

int select_correct_2nd_camera_matrix(
 double P1[3*4],
 double P24[4][3*4],
 match_struct *match_normalized_arr,
 match_struct *match_arr,
 int match_nbr,
 double P2[3*4]
);

void sfm10_error_handler(
 char* from
);

void sfm_add_camera(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int inp_camera_ind,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr
);

void sfm_add_camera_imagepoints(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_add_camera_matrix(
 int camera_ind,
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double t[3],
 sfm_camera_matrix_struct **pcamera_matrix_arr,
 int *pcamera_matrix_nbr,
 int *pcamera_matrix_ind
);

void sfm_add_imagepoint_to_threedpoint(
 int inp_camera_ind,
 int inp_feat_ind,
 double inp_x,
 double inp_y,
 int threedpoint_ind,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_check_initial_camera_pair(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 int camera1_ind,
 int camera2_ind,
 int *perr_flag
);

void sfm_check_threedpoints(
 int camera_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

int sfm_common(
 int n1,
 int arr1[],
 int n2,
 int arr2[]
);

void sfm_compute_added_camera_matrix_b(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 int camera_ind,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double t[3],
 int lbfgs_max_iterations
);

void sfm_compute_added_camera_matrix(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr,
 double P[3*4]
);

void sfm_compute_added_camera_matrix_exterior_orientation(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double K[3*3],
 double R[3*3],
 double t[3],
 int lbfgs_max_iterations
);

void sfm_compute_camera_matrix_main(
 double *xyz_arr,
 double *xy_arr,
 int n,
 double P[3*4]
);

void sfm_compute_camera_matrix(
 double *X_arr,
 double *x_arr,
 int n,
 double P[3*4]
);

void sfm_compute_C_from_R_t(
 double R[3*3],
 double t[3],
 double C[3]
);

void sfm_compute_exterior_orientation_main(
 double *xyz_arr,
 double *xy_arr,
 int n,
 double K[3*3],
 double R[3*3],
 double t[3],
 int lbfgs_max_iterations
);

void sfm_compute_features_main(
 char **image_filename_arr,
 int camera_nbr
);

void sfm_compute_focal_length(
 double F[3*3],
 int width,
 int height,
 double *pfocal_length
);

void sfm_compute_focal_length_main(
 char **image_filename_arr,
 int camera_nbr,
 double *pfocal_length
);

void sfm_compute_good_matches(
 int width,
 int height,
 match_struct *match_arr,
 int match_nbr,
 int ntrials,
 match_struct **pgood_match_arr,
 int *pgood_match_nbr,
 double F[3*3]
);

void sfm_compute_good_matches_main(
 char **image_filename_arr,
 int camera_nbr,
 int ntrials
);

void sfm_compute_match(
 sfm_feat_struct *feat1_arr,
 int feat1_ind,
 sfm_feat_struct *feat2_arr,
 int feat2_nbr,
 int *pfeat2_ind
);

void sfm_compute_matches(
 sfm_feat_struct *feat1_arr,
 int feat1_nbr,
 sfm_feat_struct *feat2_arr,
 int feat2_nbr,
 match_struct **pmatch_arr,
 int *pmatch_nbr
);

void sfm_compute_matches_main(
 char **image_filename_arr,
 int camera_nbr
);

void sfm_compute_P_from_K_R_t(
 double K[3*3],
 double R[3*3],
 double t[3],
 double P[3*4]
);

int sfm_compute_relative_camera_pose(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 int camera1_ind,
 int camera2_ind,
 double P1[3*4],
 double P2[3*4]
);

double sfm_compute_average_separation_angle(
 double P1[3*4],
 double K1[3*3],
 double R1[3*3],
 double t1[3],
 double P2[3*4],
 double K2[3*3],
 double R2[3*3],
 double t2[3],
 match_struct *match_arr,
 int match_nbr
);

void sfm_compute_threedpoints(
 int width,
 int height,
 int camera1_ind,
 int camera2_ind,
 int *image1_arr,
 int *image2_arr,
 double P1[3*4],
 double P2[3*4],
 match_struct *match_arr,
 int match_nbr,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr
);

void sfm_free_features_and_descriptors(
 sfm_feat_struct *feat_arr,
 int feat_nbr
);

void sfm_free_threedpoints(
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_get_camera_matrix(
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int camera_matrix_ind,
 int *pcamera_ind,
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double t[3]
);

void sfm_get_camera_matrix_ind(
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int inp_camera_ind,
 int *pcamera_matrix_ind
);

void sfm_get_camera_threedpoints_imagepoints(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double **pxyz_arr,
 double **pxy_arr,
 int *pn
);

void sfm_get_next_camera_2(
 char **image_filename_arr,
 int camera_nbr,
 int inp_camera_ind,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double **pxyz_arr,
 double **pxy_arr,
 int *pn
);

void sfm_get_next_camera(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int *pcamera_ind,
 int *pn
);

int sfm_get_threedpoints_nbr(
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_lbfgs_bundle_adjustment(
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int lbfgs_max_iterations
);

double sfm_match_distance_squared(
 double *descr1_arr,
 int descr1_nbr,
 double *descr2_arr,
 int descr2_nbr
);

void sfm_merge_duplicate_features(
 char **image_filename_arr,
 int camera_nbr
);

void sfm_merge_threedpoints(
 int inp_threedpoint_ind1,
 int inp_threedpoint_ind2,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_normalize_2dpoints(
 double *x3_arr,
 int x3_nbr,
 double T[3*3],
 double *normalized_x3_arr
);

void sfm_normalize_3dpoints(
 double *x4_arr,
 int x4_nbr,
 double T[4*3],
 double *normalized_x4_arr
);

void sfm_optimize_camera_matrix(
 double *X_arr,
 double *x_arr,
 int n,
 double P[3*4]
);

void sfm_optimize_camera_matrix_jacobian(
 double *X_arr,
 double *x_arr,
 int n,
 double P[3*4],
 double *J
);

void sfm_optimize_camera_matrix_residual(
 double *X_arr,
 double *x_arr,
 int n,
 double P[3*4],
 double *res_vec
);

void sfm_pba_bundle_adjustment(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_print_depth_range_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_print_F(
 char *filename,
 double F[3*3]
);

void sfm_print_features_and_descriptors(
 char *filename,
 sfm_feat_struct *feat_arr,
 int feat_nbr
);

void sfm_print_matches(
 char *filename,
 match_struct *match_arr,
 int match_nbr
);

void sfm_print_P(
 char *filename,
 double P[3*4]
);

void sfm_print_reprojection_error_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_print_threedpoints(
 char *filename,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_print_threedpoints_gif_b(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_print_threedpoints_gif(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int radius,
 double max_theta_deg
);

void sfm_print_threedpoints_in(
 char *filename,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_print_threedpoints_nvm(
 char *filename,
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_print_threedpoints_ply(
 char *filename,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_read_F(
 char *filename,
 double F[3*3]
);

void sfm_read_features_and_descriptors(
 char *filename,
 sfm_feat_struct **pfeat_arr,
 int *pfeat_nbr
);

void sfm_read_image_filenames(
 char *filename,
 char ***pimage_filename_arr,
 int *pcamera_nbr
);

void sfm_read_matches(
 char *filename,
 match_struct **pmatch_arr,
 int *pmatch_nbr
);

void sfm_read_P(
 char *filename,
 double P[3*4]
);

void sfm_read_threedpoints(
 char *filename,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr
);

void sfm_recompute_threedpoint_b(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int width,
 int height,
 int **all_image_arr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int threedpoint_ind
);

void sfm_recompute_threedpoint(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int width,
 int height,
 int **all_image_arr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int threedpoint_ind
);

void sfm_recompute_threedpoints_b(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_recompute_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
);

void sfm_reconstruct_3d_from_2cameras(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 int camera1_ind,
 int camera2_ind,
 double P1[3*4],
 double R1[3*3],
 double t1[3],
 double P2[3*4],
 double R2[3*3],
 double t2[3],
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr
);

void sfm_remove_spurious_matches_2(
 char **image_filename_arr,
 int camera_nbr,
 match_struct ***cam_match_arr,
 int **cam_match_nbr,
 sfm_temp_struct **ptemp_arr,
 int *ptemp_nbr
);

int sfm_remove_spurious_matches_3(
 char **image_filename_arr,
 int camera_nbr,
 sfm_temp_struct *temp_arr,
 int temp_nbr
);

void sfm_remove_spurious_matches_4(
 char **image_filename_arr,
 int camera_nbr,
 match_struct ***cam_match_arr,
 int **cam_match_nbr,
 sfm_temp_struct *temp_arr,
 int temp_nbr
);

void sfm_remove_spurious_matches(
 char **image_filename_arr,
 int camera_nbr
);

void sfm_remove_spurious_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int ***pfeat2threedpoint_arr,
 int **pfeat2threedpoint_nbr,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr,
 double min_separation_angle,
 double max_reproj_error
);

int sfm_reprojection_error_threedpoint(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int threedpoint_ind,
 double max_reproj_error
);

void sfm_reprojection_error_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double max_reproj_error
);

int sfm_separation_angle_threedpoint(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int threedpoint_ind,
 double min_separation_angle
);

void sfm_separation_angle_threedpoints(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 double min_separation_angle
);

void sfm_update_camera_matrix(
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int camera_matrix_ind,
 int camera_ind,
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double t[3]
);

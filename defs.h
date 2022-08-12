typedef struct sfm_feat_struct {
 double x;
 double y;
 double *descr_arr;
 int descr_nbr;
} sfm_feat_struct;

typedef struct sfm_imagepoint_struct {
 int camera_ind;
 int feat_ind;
 double x;
 double y;
} sfm_imagepoint_struct;

typedef struct sfm_threedpoint_struct {
 double xyz[3];
 int rgb[3];
 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
} sfm_threedpoint_struct;

typedef struct sfm_camera_matrix_struct {
 int camera_ind;
 double P[3*4];
 double K[3*3];
 double R[3*3];
 double t[3];
} sfm_camera_matrix_struct;

typedef struct sfm_temp_struct {
 int camera_ind;
 int feat_ind;
 int camera1_ind;
 int camera2_ind;
 int match_ind;
} sfm_temp_struct;

typedef struct ba_camera_internal_struct {
 double fx;
 double skew;
 double cx;
 double fy;
 double cy;
 double k1;
 double k2;
 double p1;
 double p2;
} ba_camera_internal_struct;

typedef struct ba_camera_external_struct {
 double R[3][3];
 double T[3];
} ba_camera_external_struct;

typedef struct ba_point_struct {
 double X3[3]; /* Point in Euclidean coordinates */
} ba_point_struct;

typedef struct ba_measurement_struct {
 int camera_ind;
 int point_ind;
 double x3[3]; /* Measurement in homogeneous coordinates */
} ba_measurement_struct;

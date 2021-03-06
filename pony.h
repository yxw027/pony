// Feb-2020
//
// PONY core declarations
#define pony_bus_version 4		// current bus version

// TIME EPOCH
typedef struct 		// Julian-type time epoch
{
	int Y;			// Year
	int M;			// Month
	int D;			// Day
	int h;			// hour
	int m;			// minute
	double s;		// seconds
} pony_time_epoch;

// SOL
typedef struct			// navigation solution structure
{
	double x[3];		// cartesian coordinates, meters
	char x_valid;		// validity flag (0/1)
	double x_cov;		// coordinate RMS deviation estimate, meters
	
	double llh[3];		// geodetic coordinates: longitude (rad), latitude (rad), height (meters)
	char llh_valid;		// validity flag (0/1)

	double v[3];		// relative-to-Earth velocity vector coordinates in local-level geodetic or cartesian frame, meters per second
	char v_valid;		// validity flag (0/1)
	double v_cov;		// velocity RMS deviation estimate, meters per second

	double q[4];		// attitude quaternion, relative to local-level or cartesian frame
	char q_valid;		// validity flag (0/1)

	double L[9];		// attitude matrix for the transition from local-level or cartesian frame, row-wise: L[0] = L_11, L[1] = L_12, ..., L[8] = L[33]
	char L_valid;		// validity flag (0/1)

	double rpy[3];		// attitude angles relative to local-level frame: roll (rad), pitch (rad), yaw = true heading (rad)
	char rpy_valid;		// validity flag (0/1)

	double dt;			// clock bias
	char dt_valid;		// validity flag (0/1)
} pony_sol;




// IMU
	// IMU const
typedef struct		// inertial navigation constants
{
	double 
		pi,			// pi
		rad2deg,	// 180/pi
		// Earth parameters as in GRS-80 by H. Moritz // Journal of Geodesy (2000) 74 (1): pp. 128�162
		u,			// Earth rotation rate, rad/s
		a,			// Earth ellipsoid semi-major axis, m
		e2,			// Earth ellipsoid first eccentricity squared
		ge,			// Earth normal gravity at the equator, m/s^2
		fg;			// Earth normal gravity flattening
} pony_imu_const;

	// IMU
typedef struct		// inertial measurement unit
{
	char* cfg;		// pointer to IMU configuration string
	int cfglength;	// IMU configuration string length

	double t;		// time of measurement update

	double w[3];	// up to 3 gyroscope measurements
	char w_valid;	// validity flag (0/1)

	double f[3];	// up to 3 accelerometer measurements
	char f_valid;	// validity flag (0/1)

	double W[3];	// angular velocity of the local level reference frame
	char W_valid;	// validity flag (0/1)

	double g[3];	// current gravity acceleration vector
	char g_valid;	// validity flag (0/1)

	pony_sol sol;	// inertial solution
} pony_imu;




// GNSS
	// SAT
typedef struct 				// GNSS satellite data
{
	double *eph;			// array of satellite ephemeris as defined by RINEX format (starting with toc: year, month, day, hour, min, sec, clock bias, etc., system-dependent)
	char eph_valid;			// validity flag (0/1)

	double Deltatsv;		// SV PRN code phase time offset (seconds), SV slock correction term to be subtracted: 
								// GPS as in Section 20.3.3.3.3.1 of IS-GPS-200J (22 May 2018) p. 96
								// GLONASS as in Section 3.3.3 of ICD GLONASS Edition 5.1 2008, minus sign, tau_c if present in pony_gnss_glo.clock_corr[0]

	double t_em;			// time of signal emission
	char t_em_valid;		// validity flag (0/1)
	double x[3];			// satellite coordinates
	char x_valid;			// validity flag (0/1)
	double v[3];			// satellite velocity vector
	char v_valid;			// validity flag (0/1)
	
	double sinEl;			// sine of satellite elevation angle
	char sinEl_valid;		// validity flag (0/1)

	double *obs;			// satellite observables array, defined at runtime
	char *obs_valid;		// satellite observables validity flag array (0/1)
} pony_gnss_sat;

	// GPS const
typedef struct		// GPS system constants
{
	double 
		mu,			// Earth grav constant as in GPS interface specs, m^3/s^2
		u,			// Earth rotation rate as in GPS interface specs, rad/s
		a,			// Earth ellipsoid semi-major axis, m
		e2,			// Earth ellipsoid first eccentricity squared
		F,			// relativistic correction constant as in GPS interface specs, sec/sqrt(m)
		F1, L1,		// nominal frequency and wavelength for L1 signal as in GPS interface specs, Hz and m
		F2, L2;		// nominal frequency and wavelength for L2 signal as in GPS interface specs, Hz and m
} pony_gps_const;

	// GLONASS const
typedef struct		// GLONASS system constants
{
	double 
		mu,			// Earth grav constant as in GLONASS ICD, m^3/s^2
		J02,		// second zonal harmonic of geopotential
		u,			// Earth rotation rate as in GLONASS ICD, rad/s
		a,			// Earth ellipsoid semi-major axis, m
		e2,			// Earth ellipsoid first eccentricity squared as in GLONASS ICD
		F01, dF1,	// nominal centre frequency and channel separation for L1 signal as in GLONASS ICD, Hz
		F02, dF2;	// nominal centre frequency and channel wavelength for L2 signal as in GLONASS ICD, Hz
} pony_glo_const;

	// GALILEO const
typedef struct		// Galileo system constants
{
	double 
		mu,			// Earth grav constant as in Galileo interface specs, m^3/s^2
		u,			// Earth rotation rate as in Galileo interface specs, rad/s
		a,			// Earth ellipsoid semi-major axis, m
		e2,			// Earth ellipsoid first eccentricity squared
		F,			// relativistic correction constant as in Galileo interface specs, sec/sqrt(m)
		F1, L1,		// nominal frequency and wavelength for E1 signal as in Galileo interface specs, Hz and m
		F5a, L5a,	// nominal frequency and wavelength for E5a signal as in Galileo interface specs, Hz and m
		F5b, L5b,	// nominal frequency and wavelength for E5b signal as in Galileo interface specs, Hz and m
		F6, L6;		// nominal frequency and wavelength for E6 signal as in Galileo interface specs, Hz and m
} pony_gal_const;

	// BEIDOU const
typedef struct		// BeiDou system constants
{
	double 
		mu,			// Earth grav constant as in BeiDou interface specs, m^3/s^2
		u,			// Earth rotation rate as in BeiDou interface specs, rad/s
		a,			// Earth ellipsoid semi-major axis as in CGCS2000, m
		e2,			// Earth ellipsoid first eccentricity squared, as in CGCS2000
		F,			// relativistic correction constant as in BeiDou interface specs, sec/sqrt(m)
		leap_sec,	// leap seconds between BeiDou time and GPS time as of 01-Jan-2006
		B1, L1,		// nominal frequency and wavelength for B1 signal as in BeiDou interface specs, Hz and m
		B2, L2;		// nominal frequency and wavelength for B2 signal as in BeiDou interface specs, Hz and m
} pony_bds_const;

typedef struct		// GNSS constants
{
	double 
		pi,			// circumference-to-diameter ratio
		c,			// speed of light, m/s
		sec_in_w,	// seconds in a week
		sec_in_d;	// seconds in a day
	// constellation-specific constants
	pony_gps_const gps;		// GPS constants
	pony_glo_const glo;		// GLONASS constants
	pony_gal_const gal;		// Galileo constants
	pony_bds_const bds;		// BeiDou constants
} pony_gnss_const;

	// GPS
typedef struct				// GPS constellation data
{
	char* cfg;				// GPS configuration string
	int cfglength;			// configuration string length

	int max_sat_count;		// maximum supported number of satellites
	int max_eph_count;		// maximum supported number of ephemeris

	pony_gnss_sat *sat;		// GPS satellites
	char **obs_types;		// observation types according to RINEX: C1C, etc.; an array of 3-character null-terminated strings in the same order as in satellites
	int obs_count;			// number of observation types

	double iono_a[4];		// ionospheric model parameters from GPS almanac
	double iono_b[4];		
	char iono_valid;		// validity flag (0/1)

	double clock_corr[4];	// clock correction parameters from GPS almanac: e.g. a0, a1, gps_second, gps_week for GPS to UTC, optional
	char clock_corr_to[2];	// time system, which the correction results into: GP - GPS, UT - UTC, GA - Galileo, etc.
	char clock_corr_valid;	// validity flag (0/1)
} pony_gnss_gps;

	// GLONASS
typedef struct				// GLONASS constellation data
{
	char* cfg;				// GLONASS configuration string
	int cfglength;			// configuration string length

	int max_sat_count;		// maximum supported number of satellites
	int max_eph_count;		// maximum supported number of ephemeris

	pony_gnss_sat *sat;		// GLONASS satellites
	int *freq_slot;			// frequency numbers
	char **obs_types;		// observation types according to RINEX: C1C, etc.; an array of 3-character null-terminated strings in the same order as in satellites
	int obs_count;			// number of observation types

	double clock_corr[4];	// clock correction parameters from GLONASS almanac: e.g. -tauC, zero, Na_day_number, N4_four_year_interval for GLONASS to UTC, optional
	char clock_corr_to[2];	// time system, which the correction results into: GP - GPS, UT - UTC, GA - Galileo, etc.
	char clock_corr_valid;	// validity flag (0/1)
} pony_gnss_glo;

	// GALILEO
typedef struct				// Galileo constellation data
{
	char* cfg;				// Galileo configuration string
	int cfglength;			// configuration string length

	int max_sat_count;		// maximum supported number of satellites
	int max_eph_count;		// maximum supported number of ephemeris

	pony_gnss_sat *sat;		// Galileo satellites
	char **obs_types;		// observation types according to RINEX: C1C, etc.; an array of 3-character null-terminated strings in the same order as in satellites
	int obs_count;			// number of observation types

	double iono[3];			// ionospheric model parameters from Galileo almanac
	char iono_valid;		// validity flag (0/1)

	double clock_corr[4];	// clock correction parameters from Galileo almanac: e.g. a0, a1, gal_second, gal_week for GAL to UTC, optional
	char clock_corr_to[2];	// time system, which the correction results into: GP - GPS, UT - UTC, GA - Galileo, etc.
	char clock_corr_valid;	// validity flag (0/1)
} pony_gnss_gal;

		// BEIDOU
typedef struct				// BeiDou constellation data
{
	char* cfg;				// BeiDou configuration string
	int cfglength;			// configuration string length

	int max_sat_count;		// maximum supported number of satellites
	int max_eph_count;		// maximum supported number of ephemeris

	pony_gnss_sat *sat;		// BeiDou satellites
	char **obs_types;		// observation types according to RINEX: C1C, etc.; an array of 3-character null-terminated strings in the same order as in satellites
	int obs_count;			// number of observation types

	double iono_a[4];		// ionospheric model parameters from BeiDou almanac
	double iono_b[4];		
	char iono_valid;		// validity flag (0/1)

	double clock_corr[4];	// clock correction parameters from BeiDou almanac: e.g. a0, a1, bds_second, bds_week for BDS to UTC, optional
	char clock_corr_to[2];	// time system, which the correction results into: GP - GPS, UT - UTC, GA - Galileo, etc.
	char clock_corr_valid;	// validity flag (0/1)
} pony_gnss_bds;

	// SETTINGS
typedef struct // GNSS operation settings
{
	double sinEl_mask;			// elevation angle mask, sine of

	double code_sigma;			// pseudorange measurement rmsdev (sigma), meters
	double phase_sigma;			// carrier phase measurement rmsdev (sigma), cycles

	double ant_pos[3];				// antenna coordinates in the instrumental frame
	double ant_pos_tol;			// antenna position tolerance (-1 if undefined)

	double leap_sec_def;		// default value of leap seconds ( <= 0 if undefined)
} pony_gnss_settings;

	// GNSS
typedef struct						// global navigation satellite systems data
{
	char* cfg;						// full GNSS configuration string pointer, NULL if gnss is not used
	int cfglength;					// full GNSS configuration string length

	char* cfg_settings;				// pointer to a part of GNSS configuration string common to all systems
	int settings_length;			// length of the part of GNSS configuration string common to all systems

	pony_gnss_settings settings;	// GNSS operation settings

	pony_gnss_gps* gps;				// GPS constellation data pointer
	pony_gnss_glo* glo;				// GLONASS constellation data pointer
	pony_gnss_gal* gal;				// Galileo constellation data pointer
	pony_gnss_bds* bds;				// BeiDou constellation data pointer

	pony_time_epoch epoch;			// current GNSS time epoch
	int leap_sec;					// current number of leap seconds (for UTC by default, but may also be used for BDS leap second for BDS-only processing)
	char leap_sec_valid;			// validity flag (0/1)

	pony_sol sol;					// current GNSS solution
	int obs_count;					// total observations used in solution
} pony_gnss;





// BUS
	// PLUGIN
typedef struct	// scheduled plugin structure
{
	void(*func)(void);	// pointer to plugin function to execute
	int cycle;			// tick cycle (period) to execute
	int shift;			// tick within a cycle to execute at (shift)
	int tick;			// current tick
} pony_plugin;
	// CORE
typedef struct	// core structure
{
	pony_plugin *plugins;	// plugin array pointer
	int plugin_count;		// number of plugins
	int current_plugin_id;	// current plugin in plugin execution list
	int exit_plugin_id;		// index of a plugin that initiated termination
	char host_termination;	// identifier of termination being called by host
} pony_core;

typedef struct					// bus data to be used in host application
{
	int ver;								// bus version to be used at runtime	

	// main functions to be used in host app
		// basic
	char(*add_plugin)	(void(*func)(void)	);	// add plugin to the plugin execution list,	input: pointer to plugin function,				output: OK/not OK (1/0)
	char(*init)			(char *cfg			);	// initialize the bus, except for core,		input: configuration string (see description),	output: OK/not OK (1/0)
	char(*step)			(void				);	// step through the plugin execution list,													output: OK/not OK (1/0)
	char(*terminate)	(void				);	// terminate operation,																		output: OK/not OK (1/0)
		// advanced scheduling
	char(*remove_plugin)		(void(*func)(void)							);	// remove all instances of a plugin from the plugin execution list,		input: pointer to plugin function to be removed,			output: OK/not OK (1/0)
	char(*replace_plugin)		(void(*oldfunc)(void), void(*newfunc)(void)	);	// replace all instances of the plugin by another one,					input: pointers to old and new plugin functions,			output: OK/not OK (1/0)
	char(*schedule_plugin)		(void(*func)(void), int cycle, int shift	);	// add scheduled plugin to the plugin execution list,					input: pointer to plugin function, cycle, shift,			output: OK/not OK (1/0)
	char(*reschedule_plugin)	(void(*func)(void), int cycle, int shift	);	// reschedule all instances of the plugin in the plugin execution list,	input: pointer to plugin function, new cycle, new shift,	output: OK/not OK (1/0)
	char(*suspend_plugin)		(void(*func)(void)							);	// suspend all instances of the plugin in the plugin execution list,	input: pointer to plugin function,							output: OK/not OK (1/0)
	char(*resume_plugin)		(void(*func)(void)							);	// resume all instances of the plugin in the plugin execution list,		input: pointer to plugin function,							output: OK/not OK (1/0)
	pony_core core;								// core instances

	char* cfg;									// full configuration string
	int cfglength;								// full configuration string length

	char* cfg_settings;							// pointer to a part of the configuration string common to all subsystems
	int settings_length;						// length of the part of the configuration string common to all subsystems

	pony_imu_const imu_const;					// inertial navigation constants, initialized independent of imu structure
	pony_imu* imu;								// inertial measurement unit data pointer

	pony_gnss_const gnss_const;					// global navigation satellite system constants, initialized independent of gnss structure
	pony_gnss* gnss;							// global navigation satellite system data pointer
	int gnss_count;								// number of gnss instances

	double t;									// system time
	int mode;									// operation mode: 0 - init, <0 termination, >0 normal operation
	pony_sol sol;								// navigation solution
} pony_struct;

extern pony_struct *pony;







// basic parsing
char * pony_locate_token(const char *token, char *src, const int len, const char delim); // locate a token (and delimiter, when given) within a configuration string







// time routines
int pony_time_days_between_dates(pony_time_epoch epoch_from, pony_time_epoch epoch_to);	// days elapsed from one date to another, based on Rata Die serial date from day one on 0001/01/01 







// linear algebra functions
	// conventional operations
double pony_linal_dot(double *u, double *v, const int m); // dot product
double pony_linal_vnorm(double *u, const int m); // l2 vector norm, i.e. sqrt(u^T*u)
void pony_linal_cross3x1(double *res, double *u, double *v); // cross product for 3x1 vectors res = u x v
void pony_linal_mmul  (double *res,  double *a, double *b, const int n, const int n1, const int m); // matrix multiplication res = a*b, a is n x n1, b is n1 x m, res is n x m
void pony_linal_mmul2T(double *res,  double *a, double *b, const int n, const int m, const int n1); // matrix multiplication with the second argument transposed res = a*b^T, a is n x m, b is n1 x m, res is n x n1
void pony_linal_qmul(double *res, double *q, double *r); // quaternion multiplication for 4x1 quaternions res = q x r, with res0, q0, r0 being scalar parts

	// space rotation representation
/*void pony_linal_mat2quat(double *q, double *R); // 3x3 attitude matrix R to quaternion q with q0 being scalar part*/

	// routines for m x m upper-triangular matrices U lined up in a single-dimension array u
		// index conversion
void pony_linal_u_ij2k(int *k,  const int i, const int j, const int m);	// upper-triangular matrix lined up in a single-dimension array index conversion: (i,j) -> k
void pony_linal_u_k2ij(int *i,  int *j, const int k, const int m);		// upper-triangular matrix lined up in a single-dimension array index conversion: k -> (i,j)
		// conventional matrix operations
void pony_linal_u_mul(double *res,  double *u, double *v, const int n, const int m);	// upper-triangular matrix lined up in a single-dimension array multiplication: res = U*v
void pony_linal_uT_mul_v(double *res,  double *u, double *v, const int m);	// upper-triangular matrix lined up in a single-dimension array transposed multiplication by vector: res = U^T*v
void pony_linal_u_inv(double *res,  double *u, const int m); // inversion of upper-triangular matrix lined up in a single-dimension array: res = U^-1
void pony_linal_uuT(double *res,  double *u, const int m); // square (with transposition) of upper-triangular matrix lined up in a single-dimension array: res = U U^T
	
	// matrix factorizations
void pony_linal_chol(double *S,  double *P, const int m); // Cholesky upper-triangular factorization P = S*S^T, where P is symmetric positive-definite matrix

	// square root Kalman filtering
double pony_linal_kalman_update(double *x, double *S, double *K,  double z, double *h, double sigma, const int m);


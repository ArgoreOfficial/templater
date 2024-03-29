/* 
 * LibPSn00b Example Programs
 *
 * GTE Graphics Example
 * 2019 - 2021 Meido-Tek Productions / PSn00bSDK Project
 *
 * Renders a spinning 3D cube with light source calculation
 * using GTE macros.
 *
 *
 * Example by Lameguy64
 *
 * Changelog:
 *
 *  Aug 10, 2022		- Added texture to cube faces.
 *
 *  May 10, 2021		- Variable types updated for psxgpu.h changes.
 *
 *  Jan 26, 2019		- Initial version.
 *
 */
 
#include <stdint.h>
#include <stdio.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <inline_c.h>

/* OT and Packet Buffer sizes */
#define OT_LEN			256
#define PACKET_LEN		1024

/* Screen resolution */
#define SCREEN_XRES		320
#define SCREEN_YRES		240

/* Screen center position */
#define CENTERX			SCREEN_XRES>>1
#define CENTERY			SCREEN_YRES>>1


/* Double buffer structure */
typedef struct {
	DISPENV		disp;			/* Display environment */
	DRAWENV		draw;			/* Drawing environment */
	uint32_t	ot[OT_LEN];		/* Ordering table */
	char		p[PACKET_LEN];	/* Packet buffer */
} DB;

/* Double buffer variables */
DB		db[2];
int		db_active = 0;
char	*db_nextpri;


/* For easier handling of vertex indices */
typedef struct {
	short v0,v1,v2,v3;
} INDEX;

/* Cube vertices */
SVECTOR cube_verts[] = {
	{ 57, 36, -34, 0 },{ 57, 36, -12, 0 },{ 55, 42, -10, 0 },{ 53, 48, -12, 0 },{ -57, 36, -12, 0 },{ -57, 36, -34, 0 },{ -53, 48, -34, 0 },{ -23, 25, -34, 0 },{ -19, 38, -34, 0 },{ -23, 25, -12, 0 },{ 21, 32, -36, 0 },{ -64, -83, -1, 0 },{ -63, -80, 8, 0 },{ -93, -74, -1, 0 },{ -92, -71, -10, 0 },{ -53, -49, 8, 0 },{ -53, -46, -1, 0 },{ -82, -40, -10, 0 },{ 63, -80, 8, 0 },{ 53, -46, -1, 0 },{ 93, -74, -1, 0 },{ 82, -38, -1, 0 },{ -64, -9, -22, 0 },{ 77, -52, 4, 0 },{ -66, -59, -7, 0 },{ 52, 53, -17, 0 },{ -53, 48, -12, 0 },{ -52, 53, -17, 0 },{ -59, 30, -23, 0 },{ -51, 54, -23, 0 },{ -52, 53, -30, 0 },{ -58, 31, -30, 0 },{ -24, 21, -30, 0 },{ -17, 44, -23, 0 },{ -25, 19, -23, 0 },{ -24, 21, -17, 0 },{ 23, 25, -12, 0 },{ 24, 21, -17, 0 },{ 25, 19, -23, 0 },{ 17, 44, -23, 0 },{ 24, 21, -30, 0 },{ 18, 42, -30, 0 },{ 19, 38, -34, 0 },{ 12, 86, -7, 0 },{ -12, 86, -7, 0 },{ -58, -65, -20, 0 },{ -63, -80, -10, 0 },{ -61, -74, 15, 0 },{ -55, -55, -17, 0 },{ -55, -55, 15, 0 },{ -85, -47, 15, 0 },{ -85, -47, -17, 0 },{ -87, -56, 17, 0 },{ -90, -65, 15, 0 },{ -87, -56, -20, 0 },{ -73, -85, -18, 0 },{ -68, -79, -10, 0 },{ -72, -80, -25, 0 },{ -79, -94, -18, 0 },{ -85, -66, -17, 0 },{ -66, -72, -17, 0 },{ -86, -81, -18, 0 },{ -86, -92, -18, 0 },{ -87, -94, -20, 0 },{ -85, -76, -25, 0 },{ -85, -76, 22, 0 },{ -73, -85, 16, 0 },{ -79, -94, 15, 0 },{ -80, -96, 18, 0 },{ -72, -80, 22, 0 },{ -68, -79, 8, 0 },{ -66, -72, 15, 0 },{ -86, -81, 16, 0 },{ -87, -73, 8, 0 },{ -86, -92, 15, 0 },{ -85, -66, 15, 0 },{ -92, -71, 8, 0 },{ -87, -73, -10, 0 },{ -90, -65, -17, 0 },{ -61, -74, -17, 0 },{ -53, -49, -10, 0 },{ -82, -38, -1, 0 },{ -82, -40, 8, 0 },{ -58, -65, 17, 0 },{ -80, -96, -20, 0 },{ -87, -94, 18, 0 },{ 61, -74, 15, 0 },{ 55, -55, -17, 0 },{ 53, -49, -10, 0 },{ 90, -65, -17, 0 },{ 87, -56, 17, 0 },{ 87, -56, -20, 0 },{ 90, -65, 15, 0 },{ 82, -40, 8, 0 },{ 79, -94, -18, 0 },{ 86, -81, -18, 0 },{ 87, -73, -10, 0 },{ 72, -80, -25, 0 },{ 73, -85, -18, 0 },{ 68, -79, -10, 0 },{ 80, -96, -20, 0 },{ 87, -94, -20, 0 },{ 66, -72, -17, 0 },{ 86, -92, -18, 0 },{ 85, -76, -25, 0 },{ 85, -66, -17, 0 },{ 85, -76, 22, 0 },{ 72, -80, 22, 0 },{ 79, -94, 15, 0 },{ 86, -81, 16, 0 },{ 73, -85, 16, 0 },{ 87, -94, 18, 0 },{ 85, -66, 15, 0 },{ 58, -65, -20, 0 },{ 64, -83, -1, 0 },{ 63, -80, -10, 0 },{ 61, -74, -17, 0 },{ 85, -47, -17, 0 },{ 82, -40, -10, 0 },{ 92, -71, -10, 0 },{ 53, -49, 8, 0 },{ 55, -55, 15, 0 },{ 85, -47, 15, 0 },{ 58, -65, 17, 0 },{ 66, -72, 15, 0 },{ 68, -79, 8, 0 },{ 87, -73, 8, 0 },{ 92, -71, 8, 0 },{ 86, -92, 15, 0 },{ 80, -96, 18, 0 },{ 56, 41, 20, 0 },{ 75, 9, 19, 0 },{ 48, 58, 5, 0 },{ 42, 78, -43, 0 },{ 42, 78, -3, 0 },{ 40, 102, -36, 0 },{ 68, 9, -47, 0 },{ 56, 41, -67, 0 },{ 75, 9, -67, 0 },{ 66, 31, -79, 0 },{ 78, 38, -74, 0 },{ 61, 62, -50, 0 },{ 61, 62, 3, 0 },{ 54, 103, -35, 0 },{ 54, 103, -12, 0 },{ 86, 17, -63, 0 },{ 86, 17, 15, 0 },{ 78, 38, 26, 0 },{ 66, 31, 31, 0 },{ 69, 47, 16, 0 },{ 56, 81, -5, 0 },{ 40, 102, -11, 0 },{ 56, 81, -42, 0 },{ 48, 58, -53, 0 },{ 69, 47, -63, 0 },{ 80, 18, -45, 0 },{ 77, 19, -23, 0 },{ 65, 10, -23, 0 },{ 80, 18, -2, 0 },{ 68, 9, 0, 0 },{ 45, -13, -26, 0 },{ 43, -7, -25, 0 },{ 42, -2, -20, 0 },{ 43, -7, -2, 0 },{ 68, 0, -25, 0 },{ 66, 4, -20, 0 },{ 66, 6, -13, 0 },{ 73, -18, -13, 0 },{ 72, -12, -2, 0 },{ 70, -5, 0, 0 },{ 65, -14, -2, 0 },{ 67, -20, -13, 0 },{ 65, -56, 4, 0 },{ 66, -59, -7, 0 },{ 54, -22, -20, 0 },{ 78, -56, -7, 0 },{ 53, -17, -2, 0 },{ 47, -19, -2, 0 },{ 45, -13, 0, 0 },{ 73, -16, -7, 0 },{ 68, 0, -2, 0 },{ 66, 4, -7, 0 },{ 42, -2, -7, 0 },{ 41, -1, -13, 0 },{ 70, -5, -26, 0 },{ 72, -12, -25, 0 },{ 47, -19, -25, 0 },{ 67, -18, -20, 0 },{ 73, -16, -20, 0 },{ 67, -18, -7, 0 },{ 48, -24, -20, 0 },{ 55, -24, -13, 0 },{ 49, -25, -13, 0 },{ 54, -22, -7, 0 },{ 48, -24, -7, 0 },{ 52, 28, -23, 0 },{ 51, 34, -12, 0 },{ 63, -5, -4, 0 },{ 50, -9, -4, 0 },{ 52, -12, -22, 0 },{ 64, -9, -22, 0 },{ 38, 30, -12, 0 },{ 40, 26, -17, 0 },{ 21, 32, -10, 0 },{ 58, 31, -17, 0 },{ 19, 38, -12, 0 },{ 18, 42, -17, 0 },{ 51, 54, -23, 0 },{ 52, 53, -30, 0 },{ 53, 48, -34, 0 },{ 55, 42, -36, 0 },{ 23, 25, -34, 0 },{ 52, 29, -30, 0 },{ 58, 31, -30, 0 },{ 59, 30, -23, 0 },{ 52, 29, -17, 0 },{ 40, 26, -30, 0 },{ 40, 24, -23, 0 },{ -56, 41, 20, 0 },{ -68, 9, 0, 0 },{ -65, 10, -23, 0 },{ -68, 9, -47, 0 },{ -66, 31, -79, 0 },{ -42, 78, -43, 0 },{ -40, 102, -11, 0 },{ -78, 38, -74, 0 },{ -69, 47, -63, 0 },{ -80, 18, -2, 0 },{ -69, 47, 16, 0 },{ -78, 38, 26, 0 },{ -56, 81, -42, 0 },{ -66, 31, 31, 0 },{ -61, 62, 3, 0 },{ -48, 58, 5, 0 },{ -56, 81, -5, 0 },{ -42, 78, -3, 0 },{ -54, 103, -12, 0 },{ -40, 102, -36, 0 },{ -54, 103, -35, 0 },{ -61, 62, -50, 0 },{ -48, 58, -53, 0 },{ -56, 41, -67, 0 },{ -75, 9, -67, 0 },{ -86, 17, -63, 0 },{ -80, 18, -45, 0 },{ -77, 19, -23, 0 },{ -75, 9, 19, 0 },{ -86, 17, 15, 0 },{ -43, -7, -25, 0 },{ -47, -19, -25, 0 },{ -42, -2, -20, 0 },{ -49, -25, -13, 0 },{ -48, -24, -7, 0 },{ -45, -13, 0, 0 },{ -73, -16, -20, 0 },{ -72, -12, -25, 0 },{ -68, 0, -25, 0 },{ -66, 4, -20, 0 },{ -68, 0, -2, 0 },{ -77, -52, 4, 0 },{ -67, -18, -7, 0 },{ -67, -20, -13, 0 },{ -65, -56, 4, 0 },{ -53, -17, -2, 0 },{ -55, -24, -13, 0 },{ -78, -56, -7, 0 },{ -47, -19, -2, 0 },{ -65, -14, -2, 0 },{ -70, -5, 0, 0 },{ -43, -7, -2, 0 },{ -66, 4, -7, 0 },{ -42, -2, -7, 0 },{ -66, 6, -13, 0 },{ -41, -1, -13, 0 },{ -72, -12, -2, 0 },{ -73, -16, -7, 0 },{ -70, -5, -26, 0 },{ -45, -13, -26, 0 },{ -67, -18, -20, 0 },{ -54, -22, -20, 0 },{ -48, -24, -20, 0 },{ -54, -22, -7, 0 },{ -73, -18, -13, 0 },{ -63, -5, -4, 0 },{ -51, 34, -12, 0 },{ -50, -9, -4, 0 },{ -38, 30, -12, 0 },{ -52, -12, -22, 0 },{ -40, 26, -17, 0 },{ -21, 32, -10, 0 },{ -55, 42, -10, 0 },{ -19, 38, -12, 0 },{ -18, 42, -17, 0 },{ -58, 31, -17, 0 },{ -18, 42, -30, 0 },{ -55, 42, -36, 0 },{ -21, 32, -36, 0 },{ -52, 29, -30, 0 },{ -40, 26, -30, 0 },{ -40, 24, -23, 0 },{ -52, 28, -23, 0 },{ -52, 29, -17, 0 },{ -19, -81, 25, 0 },{ 23, -82, 20, 0 },{ -48, -103, 41, 0 },{ -35, -103, -22, 0 },{ -48, -103, -32, 0 },{ -51, -99, -35, 0 },{ -51, -71, -39, 0 },{ -51, -59, 43, 0 },{ -48, -67, -34, 0 },{ -48, -56, 37, 0 },{ -28, -103, -28, 0 },{ -38, -102, -41, 0 },{ -41, -98, -45, 0 },{ -41, -72, -49, 0 },{ -35, -68, -46, 0 },{ -42, -55, 33, 0 },{ 38, -102, -41, 0 },{ 41, -98, -45, 0 },{ 41, -72, -49, 0 },{ 35, -68, -46, 0 },{ 48, -103, -32, 0 },{ 51, -99, -35, 0 },{ 51, -71, -39, 0 },{ 48, -67, -34, 0 },{ 42, -66, -30, 0 },{ 35, -103, 31, 0 },{ 48, -103, 41, 0 },{ 51, -98, 43, 0 },{ 51, -59, 43, 0 },{ 48, -56, 37, 0 },{ 42, -55, 33, 0 },{ 32, -53, 44, 0 },{ 28, -103, 37, 0 },{ 38, -102, 50, 0 },{ 41, -97, 53, 0 },{ 41, -56, 53, 0 },{ 35, -52, 50, 0 },{ -35, -52, 50, 0 },{ -32, -53, 44, 0 },{ -38, -102, 50, 0 },{ -41, -97, 53, 0 },{ -41, -56, 53, 0 },{ -35, -103, 31, 0 },{ -51, -98, 43, 0 },{ -12, 55, -33, 0 },{ 12, 89, -28, 0 },{ -12, 89, -28, 0 },{ -12, 52, -12, 0 },{ 24, 47, -8, 0 },{ 12, 52, -12, 0 },{ 12, 55, -33, 0 },{ 24, 51, -38, 0 },{ -24, 51, -38, 0 },{ -24, 47, -8, 0 },{ 14, 45, 1, 0 },{ 15, 27, 2, 0 },{ -15, 27, 2, 0 },{ -14, 45, 1, 0 },{ 27, 7, -1, 0 },{ 18, 6, 10, 0 },{ -18, 6, 10, 0 },{ -27, 7, -1, 0 },{ 35, -16, 15, 0 },{ 25, -15, 27, 0 },{ -25, -15, 27, 0 },{ 39, -34, 47, 0 },{ -39, -34, 47, 0 },{ 35, -38, 47, 0 },{ -35, -38, 47, 0 },{ -48, -40, 35, 0 },{ 42, -42, 31, 0 },{ 19, -85, -20, 0 },{ -23, -85, -15, 0 },{ 23, -85, -15, 0 },{ 25, 28, -8, 0 },{ 24, 34, -43, 0 },{ 25, 11, -47, 0 },{ 35, -22, -47, 0 },{ 49, -36, 37, 0 },{ 15, 35, -53, 0 },{ 15, 11, -56, 0 },{ 24, -24, -56, 0 },{ 39, -55, -51, 0 },{ 49, -53, -41, 0 },{ 48, -40, 35, 0 },{ 14, 53, -48, 0 },{ -15, 35, -53, 0 },{ -15, 11, -56, 0 },{ -24, -24, -56, 0 },{ -39, -55, -51, 0 },{ -36, -59, -47, 0 },{ 36, -59, -47, 0 },{ -14, 53, -48, 0 },{ -24, 34, -43, 0 },{ -25, 11, -47, 0 },{ -35, -22, -47, 0 },{ -49, -53, -41, 0 },{ -25, 28, -8, 0 },{ -35, -16, 15, 0 },{ -49, -36, 37, 0 },{ -48, -56, -34, 0 },{ -42, -42, 31, 0 },{ -42, -56, -31, 0 },{ -32, -59, -41, 0 },{ 48, -56, -34, 0 },{ -32, -67, -40, 0 },{ 32, -67, -40, 0 },{ 32, -59, -41, 0 },{ 42, -56, -31, 0 },{ 32, -40, 42, 0 },{ -32, -40, 42, 0 },{ -42, -66, -30, 0 },{ 0, 141, -57, 0 },{ -20, 135, -55, 0 },{ 0, 137, -56, 0 },{ 20, 135, -55, 0 },{ -23, 130, -54, 0 },{ 0, 131, -54, 0 },{ -24, 110, -48, 0 },{ 0, 110, -48, 0 },{ 24, 110, -48, 0 },{ 0, 88, -41, 0 },{ -23, 89, -41, 0 },{ 0, 82, -39, 0 },{ 20, 84, -40, 0 },{ 14, 80, 19, 0 },{ 14, 76, 40, 0 },{ 20, 80, 41, 0 },{ 20, 83, 20, 0 },{ 14, 82, -1, 0 },{ 20, 85, -1, 0 },{ 20, 86, -23, 0 },{ 23, 85, 42, 0 },{ 23, 89, 21, 0 },{ 23, 91, -1, 0 },{ 23, 91, -24, 0 },{ 23, 89, -41, 0 },{ 26, 110, 25, 0 },{ 26, 113, 0, 0 },{ 26, 112, -27, 0 },{ 23, 131, 29, 0 },{ 23, 134, 0, 0 },{ 23, 133, -30, 0 },{ 23, 130, -54, 0 },{ 23, 126, 54, 0 },{ 20, 136, 30, 0 },{ 20, 139, 0, 0 },{ 20, 138, -30, 0 },{ 14, 142, -31, 0 },{ 14, 138, -57, 0 },{ 14, 134, 57, 0 },{ 14, 139, 31, 0 },{ 14, 142, 0, 0 },{ 0, 144, -31, 0 },{ -14, 138, -57, 0 },{ -14, 142, -31, 0 },{ -20, 138, -30, 0 },{ 0, 142, 31, 0 },{ 0, 144, 0, 0 },{ -14, 142, 0, 0 },{ -20, 139, 0, 0 },{ -23, 133, -30, 0 },{ -14, 139, 31, 0 },{ -14, 134, 57, 0 },{ -20, 136, 30, 0 },{ -23, 131, 29, 0 },{ -23, 134, 0, 0 },{ -26, 112, -27, 0 },{ -23, 126, 54, 0 },{ -26, 110, 25, 0 },{ -26, 113, 0, 0 },{ -23, 91, -24, 0 },{ -24, 105, 48, 0 },{ -23, 89, 21, 0 },{ -23, 91, -1, 0 },{ -20, 85, -1, 0 },{ -20, 86, -23, 0 },{ -20, 84, -40, 0 },{ -14, 82, -22, 0 },{ -14, 81, -38, 0 },{ -23, 85, 42, 0 },{ -20, 83, 20, 0 },{ -14, 80, 19, 0 },{ -14, 82, -1, 0 },{ 0, 80, -22, 0 },{ 0, 79, -38, 0 },{ 0, 78, 19, 0 },{ 0, 80, -1, 0 },{ 14, 82, -22, 0 },{ 14, 81, -38, 0 },{ 30, -100, 26, 0 },{ 30, -100, -19, 0 },{ 19, -81, 25, 0 },{ 24, -99, 32, 0 },{ -24, -99, 32, 0 },{ -28, -103, 37, 0 },{ 35, -103, -22, 0 },{ 24, -100, -24, 0 },{ 28, -103, -28, 0 },{ -24, -100, -24, 0 },{ -30, -100, -19, 0 },{ -19, -85, -20, 0 },{ -30, -100, 26, 0 },{ -23, -82, 20, 0 },{ -20, 105, 47, 0 },{ -18, 120, 52, 0 },{ -11, 109, 37, 0 },{ -10, 98, 34, 0 },{ -18, 91, 44, 0 },{ -15, 86, 42, 0 },{ -20, 80, 41, 0 },{ -15, 125, 54, 0 },{ -10, 119, 40, 0 },{ 0, 109, 36, 0 },{ 0, 97, 34, 0 },{ -10, 83, 41, 0 },{ -20, 131, 55, 0 },{ -10, 127, 54, 0 },{ 0, 128, 54, 0 },{ 0, 120, 40, 0 },{ 10, 119, 40, 0 },{ 11, 109, 37, 0 },{ 0, 136, 57, 0 },{ 10, 127, 54, 0 },{ 15, 125, 54, 0 },{ 18, 120, 52, 0 },{ 20, 105, 47, 0 },{ 18, 91, 44, 0 },{ 10, 98, 34, 0 },{ 20, 131, 55, 0 },{ 24, 105, 48, 0 },{ 15, 86, 42, 0 },{ 10, 83, 41, 0 },{ 0, 81, 41, 0 },{ -14, 76, 40, 0 },{ 0, 74, 39, 0 }
};

/* Cube face normals */
SVECTOR cube_norms[] = {
	{ 0, 0, -ONE, 0 },
	{ 0, 0, ONE, 0 },
	{ 0, -ONE, 0, 0 },
	{ 0, ONE, 0, 0 },
	{ -ONE, 0, 0, 0 },
	{ ONE, 0, 0, 0 }
};

/* Cube vertex indices */
INDEX cube_indices[] = {
	{ 0, 209, 210 },{ 1, 2, 3 },{ 4, 26, 290 },{ 5, 295, 6 },{ 7, 8, 296 },{ 9, 289, 291 },{ 36, 205, 203 },{ 211, 10, 42 },{ 11, 12, 46 },{ 13, 14, 76 },{ 80, 15, 16 },{ 81, 82, 17 },{ 114, 115, 18 },{ 120, 88, 19 },{ 20, 127, 119 },{ 93, 21, 118 },{ 197, 200, 199 },{ 199, 198, 197 },{ 285, 287, 22 },{ 22, 283, 285 },{ 173, 172, 23 },{ 23, 175, 173 },{ 265, 259, 262 },{ 262, 24, 265 },{ 209, 0, 213 },{ 213, 208, 209 },{ 208, 213, 214 },{ 214, 207, 208 },{ 207, 214, 204 },{ 204, 25, 207 },{ 25, 204, 1 },{ 1, 3, 25 },{ 26, 4, 293 },{ 293, 27, 26 },{ 27, 293, 28 },{ 28, 29, 27 },{ 29, 28, 31 },{ 31, 30, 29 },{ 30, 31, 5 },{ 5, 6, 30 },{ 8, 7, 32 },{ 32, 294, 8 },{ 294, 32, 34 },{ 34, 33, 294 },{ 33, 34, 35 },{ 35, 292, 33 },{ 292, 35, 9 },{ 9, 291, 292 },{ 205, 36, 37 },{ 37, 206, 205 },{ 206, 37, 38 },{ 38, 39, 206 },{ 39, 38, 40 },{ 40, 41, 39 },{ 41, 40, 211 },{ 211, 42, 41 },{ 347, 43, 44 },{ 44, 348, 347 },{ 45, 79, 47 },{ 47, 83, 45 },{ 79, 46, 12 },{ 12, 47, 79 },{ 48, 45, 83 },{ 83, 49, 48 },{ 80, 48, 49 },{ 49, 15, 80 },{ 53, 76, 14 },{ 14, 78, 53 },{ 50, 52, 54 },{ 54, 51, 50 },{ 52, 53, 78 },{ 78, 54, 52 },{ 17, 82, 50 },{ 50, 51, 17 },{ 55, 58, 62 },{ 62, 61, 55 },{ 56, 55, 61 },{ 61, 77, 56 },{ 57, 84, 58 },{ 58, 55, 57 },{ 60, 57, 55 },{ 55, 56, 60 },{ 64, 63, 84 },{ 84, 57, 64 },{ 59, 64, 57 },{ 57, 60, 59 },{ 61, 62, 63 },{ 63, 64, 61 },{ 77, 61, 64 },{ 64, 59, 77 },{ 69, 68, 85 },{ 85, 65, 69 },{ 71, 69, 65 },{ 65, 75, 71 },{ 66, 67, 68 },{ 68, 69, 66 },{ 70, 66, 69 },{ 69, 71, 70 },{ 72, 74, 67 },{ 67, 66, 72 },{ 73, 72, 66 },{ 66, 70, 73 },{ 65, 85, 74 },{ 74, 72, 65 },{ 75, 65, 72 },{ 72, 73, 75 },{ 12, 11, 70 },{ 73, 70, 11 },{ 11, 13, 73 },{ 73, 13, 76 },{ 13, 11, 56 },{ 56, 77, 13 },{ 77, 14, 13 },{ 14, 77, 59 },{ 59, 78, 14 },{ 78, 59, 54 },{ 46, 56, 11 },{ 56, 46, 79 },{ 79, 60, 56 },{ 79, 45, 60 },{ 59, 60, 45 },{ 45, 54, 59 },{ 54, 45, 48 },{ 48, 51, 54 },{ 51, 48, 80 },{ 80, 17, 51 },{ 17, 80, 16 },{ 16, 81, 17 },{ 81, 16, 15 },{ 15, 82, 81 },{ 82, 15, 49 },{ 49, 50, 82 },{ 50, 49, 83 },{ 83, 52, 50 },{ 75, 52, 83 },{ 75, 53, 52 },{ 53, 75, 73 },{ 73, 76, 53 },{ 83, 71, 75 },{ 47, 71, 83 },{ 71, 47, 12 },{ 12, 70, 71 },{ 62, 58, 84 },{ 84, 63, 62 },{ 85, 68, 67 },{ 67, 74, 85 },{ 86, 18, 115 },{ 115, 116, 86 },{ 123, 86, 116 },{ 116, 113, 123 },{ 120, 121, 87 },{ 87, 88, 120 },{ 89, 119, 127 },{ 127, 92, 89 },{ 117, 91, 90 },{ 90, 122, 117 },{ 91, 89, 92 },{ 92, 90, 91 },{ 118, 117, 122 },{ 122, 93, 118 },{ 103, 94, 98 },{ 98, 95, 103 },{ 95, 98, 99 },{ 99, 96, 95 },{ 94, 100, 97 },{ 97, 98, 94 },{ 98, 97, 102 },{ 102, 99, 98 },{ 100, 101, 104 },{ 104, 97, 100 },{ 97, 104, 105 },{ 105, 102, 97 },{ 101, 103, 95 },{ 95, 104, 101 },{ 104, 95, 96 },{ 96, 105, 104 },{ 111, 129, 107 },{ 107, 106, 111 },{ 106, 107, 124 },{ 124, 112, 106 },{ 129, 108, 110 },{ 110, 107, 129 },{ 107, 110, 125 },{ 125, 124, 107 },{ 108, 128, 109 },{ 109, 110, 108 },{ 110, 109, 126 },{ 126, 125, 110 },{ 128, 111, 106 },{ 106, 109, 128 },{ 109, 106, 112 },{ 112, 126, 109 },{ 121, 123, 113 },{ 113, 87, 121 },{ 114, 18, 125 },{ 114, 125, 126 },{ 126, 20, 114 },{ 99, 114, 20 },{ 99, 115, 114 },{ 116, 115, 99 },{ 99, 102, 116 },{ 113, 116, 102 },{ 20, 126, 127 },{ 113, 102, 105 },{ 105, 91, 113 },{ 87, 113, 91 },{ 91, 117, 87 },{ 88, 87, 117 },{ 117, 118, 88 },{ 19, 88, 118 },{ 105, 89, 91 },{ 119, 89, 105 },{ 105, 96, 119 },{ 119, 96, 20 },{ 20, 96, 99 },{ 118, 21, 19 },{ 120, 19, 21 },{ 21, 93, 120 },{ 121, 120, 93 },{ 93, 122, 121 },{ 123, 121, 122 },{ 122, 90, 123 },{ 123, 90, 112 },{ 112, 124, 123 },{ 124, 86, 123 },{ 18, 86, 124 },{ 124, 125, 18 },{ 92, 112, 90 },{ 126, 112, 92 },{ 92, 127, 126 },{ 103, 101, 100 },{ 100, 94, 103 },{ 111, 128, 108 },{ 108, 129, 111 },{ 130, 148, 131 },{ 131, 159, 130 },{ 130, 159, 157 },{ 157, 132, 130 },{ 157, 153, 132 },{ 132, 153, 133 },{ 133, 134, 132 },{ 134, 133, 135 },{ 135, 151, 134 },{ 153, 157, 136 },{ 136, 137, 153 },{ 137, 136, 138 },{ 138, 139, 137 },{ 140, 145, 155 },{ 155, 154, 140 },{ 154, 155, 156 },{ 156, 141, 154 },{ 141, 156, 142 },{ 152, 141, 142 },{ 142, 150, 152 },{ 152, 150, 144 },{ 144, 143, 152 },{ 142, 156, 158 },{ 158, 149, 142 },{ 149, 158, 146 },{ 146, 147, 149 },{ 143, 144, 151 },{ 151, 135, 143 },{ 145, 140, 139 },{ 139, 138, 145 },{ 147, 146, 131 },{ 131, 148, 147 },{ 147, 148, 130 },{ 130, 149, 147 },{ 142, 149, 130 },{ 130, 132, 142 },{ 150, 142, 132 },{ 132, 134, 150 },{ 150, 134, 151 },{ 151, 144, 150 },{ 143, 135, 133 },{ 133, 152, 143 },{ 141, 152, 133 },{ 133, 153, 141 },{ 154, 141, 153 },{ 153, 137, 154 },{ 154, 137, 139 },{ 139, 140, 154 },{ 145, 138, 136 },{ 136, 155, 145 },{ 156, 155, 136 },{ 136, 157, 156 },{ 158, 156, 157 },{ 157, 159, 158 },{ 158, 159, 131 },{ 131, 146, 158 },{ 192, 190, 186 },{ 192, 186, 194 },{ 194, 186, 160 },{ 194, 160, 177 },{ 177, 160, 161 },{ 177, 161, 178 },{ 178, 161, 162 },{ 178, 162, 163 },{ 163, 162, 183 },{ 183, 182, 163 },{ 164, 184, 185 },{ 164, 185, 165 },{ 165, 185, 188 },{ 165, 188, 166 },{ 166, 188, 167 },{ 166, 167, 181 },{ 181, 167, 179 },{ 181, 179, 180 },{ 180, 179, 168 },{ 168, 169, 180 },{ 189, 23, 170 },{ 175, 23, 189 },{ 189, 171, 175 },{ 187, 175, 171 },{ 176, 170, 23 },{ 23, 172, 176 },{ 176, 172, 193 },{ 191, 193, 172 },{ 172, 173, 191 },{ 191, 173, 174 },{ 187, 174, 173 },{ 173, 175, 187 },{ 176, 193, 194 },{ 194, 177, 176 },{ 176, 177, 178 },{ 170, 176, 178 },{ 178, 169, 170 },{ 169, 168, 170 },{ 189, 170, 168 },{ 168, 179, 189 },{ 180, 169, 178 },{ 178, 163, 180 },{ 181, 180, 163 },{ 163, 182, 181 },{ 166, 181, 182 },{ 182, 183, 166 },{ 165, 166, 183 },{ 183, 162, 165 },{ 164, 165, 162 },{ 162, 161, 164 },{ 184, 164, 161 },{ 161, 160, 184 },{ 185, 184, 160 },{ 160, 186, 185 },{ 187, 185, 186 },{ 188, 185, 187 },{ 167, 188, 187 },{ 187, 171, 167 },{ 179, 167, 171 },{ 171, 189, 179 },{ 186, 174, 187 },{ 186, 190, 174 },{ 191, 174, 190 },{ 190, 192, 191 },{ 193, 191, 192 },{ 192, 194, 193 },{ 215, 197, 196 },{ 200, 197, 215 },{ 215, 195, 200 },{ 212, 200, 195 },{ 201, 196, 197 },{ 197, 198, 201 },{ 201, 198, 202 },{ 217, 202, 198 },{ 198, 199, 217 },{ 217, 199, 216 },{ 212, 216, 199 },{ 199, 200, 212 },{ 201, 202, 37 },{ 37, 36, 201 },{ 201, 36, 203 },{ 196, 201, 203 },{ 203, 2, 196 },{ 2, 1, 196 },{ 215, 196, 1 },{ 1, 204, 215 },{ 3, 2, 203 },{ 203, 205, 3 },{ 25, 3, 205 },{ 205, 206, 25 },{ 207, 25, 206 },{ 206, 39, 207 },{ 208, 207, 39 },{ 39, 41, 208 },{ 209, 208, 41 },{ 41, 42, 209 },{ 210, 209, 42 },{ 42, 10, 210 },{ 0, 210, 10 },{ 10, 211, 0 },{ 212, 0, 211 },{ 213, 0, 212 },{ 214, 213, 212 },{ 212, 195, 214 },{ 204, 214, 195 },{ 195, 215, 204 },{ 211, 216, 212 },{ 211, 40, 216 },{ 217, 216, 40 },{ 40, 38, 217 },{ 202, 217, 38 },{ 38, 37, 202 },{ 246, 231, 218 },{ 218, 219, 246 },{ 220, 219, 218 },{ 218, 233, 220 },{ 240, 220, 233 },{ 221, 220, 240 },{ 240, 241, 221 },{ 242, 221, 241 },{ 241, 222, 242 },{ 223, 240, 233 },{ 233, 235, 223 },{ 237, 223, 235 },{ 235, 224, 237 },{ 244, 243, 225 },{ 225, 226, 244 },{ 245, 244, 226 },{ 226, 239, 245 },{ 245, 239, 232 },{ 227, 245, 232 },{ 232, 228, 227 },{ 247, 227, 228 },{ 228, 229, 247 },{ 232, 239, 230 },{ 230, 234, 232 },{ 236, 234, 230 },{ 230, 238, 236 },{ 238, 237, 224 },{ 224, 236, 238 },{ 243, 242, 222 },{ 222, 225, 243 },{ 229, 231, 246 },{ 246, 247, 229 },{ 218, 231, 229 },{ 229, 228, 218 },{ 218, 228, 232 },{ 232, 233, 218 },{ 233, 232, 234 },{ 234, 235, 233 },{ 224, 235, 234 },{ 234, 236, 224 },{ 223, 237, 238 },{ 238, 230, 223 },{ 223, 230, 239 },{ 239, 240, 223 },{ 240, 239, 226 },{ 226, 241, 240 },{ 222, 241, 226 },{ 226, 225, 222 },{ 221, 242, 243 },{ 243, 244, 221 },{ 221, 244, 245 },{ 245, 220, 221 },{ 220, 245, 227 },{ 227, 219, 220 },{ 246, 219, 227 },{ 227, 247, 246 },{ 248, 277, 249 },{ 248, 249, 250 },{ 250, 249, 280 },{ 250, 280, 273 },{ 273, 280, 251 },{ 273, 251, 271 },{ 271, 251, 252 },{ 271, 252, 269 },{ 269, 252, 266 },{ 266, 253, 269 },{ 282, 254, 255 },{ 282, 255, 275 },{ 275, 255, 276 },{ 275, 276, 274 },{ 274, 276, 256 },{ 274, 256, 268 },{ 268, 256, 257 },{ 268, 257, 258 },{ 258, 257, 272 },{ 272, 270, 258 },{ 259, 260, 267 },{ 260, 259, 265 },{ 265, 261, 260 },{ 265, 278, 261 },{ 263, 262, 259 },{ 259, 267, 263 },{ 262, 263, 281 },{ 262, 281, 264 },{ 264, 24, 262 },{ 24, 264, 279 },{ 278, 265, 24 },{ 24, 279, 278 },{ 252, 281, 263 },{ 263, 266, 252 },{ 266, 263, 253 },{ 253, 263, 267 },{ 267, 268, 253 },{ 253, 268, 258 },{ 258, 269, 253 },{ 269, 258, 270 },{ 270, 271, 269 },{ 271, 270, 272 },{ 272, 273, 271 },{ 274, 268, 267 },{ 274, 267, 260 },{ 260, 275, 274 },{ 273, 272, 257 },{ 257, 250, 273 },{ 250, 257, 256 },{ 256, 248, 250 },{ 248, 256, 276 },{ 276, 277, 248 },{ 277, 276, 255 },{ 255, 249, 277 },{ 249, 255, 278 },{ 278, 279, 249 },{ 280, 249, 279 },{ 280, 279, 264 },{ 264, 251, 280 },{ 251, 264, 281 },{ 281, 252, 251 },{ 255, 254, 278 },{ 278, 254, 282 },{ 282, 261, 278 },{ 261, 282, 275 },{ 275, 260, 261 },{ 283, 301, 284 },{ 301, 283, 22 },{ 22, 300, 301 },{ 22, 297, 300 },{ 286, 285, 283 },{ 283, 284, 286 },{ 285, 286, 288 },{ 285, 288, 299 },{ 299, 287, 285 },{ 287, 299, 298 },{ 297, 22, 287 },{ 287, 298, 297 },{ 35, 288, 286 },{ 286, 9, 35 },{ 9, 286, 289 },{ 289, 286, 284 },{ 284, 290, 289 },{ 289, 290, 26 },{ 26, 291, 289 },{ 291, 26, 27 },{ 27, 292, 291 },{ 292, 27, 29 },{ 29, 33, 292 },{ 4, 290, 284 },{ 4, 284, 301 },{ 301, 293, 4 },{ 33, 29, 30 },{ 30, 294, 33 },{ 294, 30, 6 },{ 6, 8, 294 },{ 8, 6, 295 },{ 295, 296, 8 },{ 296, 295, 5 },{ 5, 7, 296 },{ 7, 5, 297 },{ 297, 298, 7 },{ 32, 7, 298 },{ 32, 298, 299 },{ 299, 34, 32 },{ 34, 299, 288 },{ 288, 35, 34 },{ 5, 31, 297 },{ 297, 31, 28 },{ 28, 300, 297 },{ 300, 28, 293 },{ 293, 301, 300 },{ 374, 505, 303 },{ 303, 375, 374 },{ 505, 302, 494 },{ 494, 303, 505 },{ 304, 344, 305 },{ 305, 306, 304 },{ 304, 306, 307 },{ 307, 345, 304 },{ 345, 307, 308 },{ 308, 309, 345 },{ 309, 308, 310 },{ 310, 311, 309 },{ 306, 305, 312 },{ 312, 313, 306 },{ 306, 313, 314 },{ 314, 307, 306 },{ 308, 307, 314 },{ 314, 315, 308 },{ 310, 308, 315 },{ 315, 316, 310 },{ 413, 310, 316 },{ 311, 310, 413 },{ 413, 317, 311 },{ 318, 313, 312 },{ 314, 313, 318 },{ 318, 319, 314 },{ 315, 314, 319 },{ 319, 320, 315 },{ 315, 320, 321 },{ 321, 316, 315 },{ 407, 316, 321 },{ 316, 407, 413 },{ 312, 500, 318 },{ 318, 500, 498 },{ 498, 322, 318 },{ 318, 322, 323 },{ 323, 319, 318 },{ 320, 319, 323 },{ 323, 324, 320 },{ 320, 324, 325 },{ 325, 321, 320 },{ 321, 325, 326 },{ 326, 408, 321 },{ 321, 408, 407 },{ 322, 498, 327 },{ 327, 328, 322 },{ 322, 328, 329 },{ 329, 323, 322 },{ 324, 323, 329 },{ 329, 330, 324 },{ 325, 324, 330 },{ 330, 331, 325 },{ 326, 325, 331 },{ 331, 332, 326 },{ 333, 332, 331 },{ 328, 327, 334 },{ 334, 335, 328 },{ 328, 335, 336 },{ 336, 329, 328 },{ 330, 329, 336 },{ 336, 337, 330 },{ 330, 337, 338 },{ 338, 331, 330 },{ 331, 338, 333 },{ 333, 338, 339 },{ 339, 340, 333 },{ 341, 335, 334 },{ 336, 335, 341 },{ 341, 342, 336 },{ 336, 342, 343 },{ 343, 337, 336 },{ 337, 343, 339 },{ 339, 338, 337 },{ 334, 497, 341 },{ 341, 497, 344 },{ 344, 304, 341 },{ 341, 304, 345 },{ 345, 342, 341 },{ 343, 342, 345 },{ 345, 309, 343 },{ 339, 343, 309 },{ 309, 311, 339 },{ 340, 339, 311 },{ 311, 317, 340 },{ 348, 346, 352 },{ 352, 347, 348 },{ 351, 43, 347 },{ 347, 352, 351 },{ 44, 43, 351 },{ 351, 349, 44 },{ 346, 348, 44 },{ 44, 349, 346 },{ 350, 351, 352 },{ 352, 353, 350 },{ 352, 387, 353 },{ 394, 387, 352 },{ 352, 346, 394 },{ 346, 354, 394 },{ 354, 346, 349 },{ 349, 355, 354 },{ 351, 350, 356 },{ 349, 351, 356 },{ 356, 359, 349 },{ 349, 359, 355 },{ 356, 350, 376 },{ 376, 357, 356 },{ 356, 357, 358 },{ 358, 359, 356 },{ 359, 358, 399 },{ 399, 355, 359 },{ 357, 376, 360 },{ 360, 361, 357 },{ 357, 361, 362 },{ 362, 358, 357 },{ 358, 362, 363 },{ 363, 399, 358 },{ 361, 360, 364 },{ 364, 365, 361 },{ 361, 365, 366 },{ 366, 362, 361 },{ 362, 366, 400 },{ 400, 363, 362 },{ 365, 364, 380 },{ 380, 367, 365 },{ 365, 367, 368 },{ 368, 366, 365 },{ 366, 368, 401 },{ 401, 400, 366 },{ 367, 380, 386 },{ 386, 369, 367 },{ 368, 367, 369 },{ 369, 370, 368 },{ 368, 370, 371 },{ 371, 401, 368 },{ 369, 386, 372 },{ 372, 411, 369 },{ 369, 411, 412 },{ 412, 370, 369 },{ 370, 412, 403 },{ 403, 371, 370 },{ 373, 503, 374 },{ 374, 375, 373 },{ 376, 350, 353 },{ 353, 377, 376 },{ 360, 376, 377 },{ 377, 378, 360 },{ 364, 360, 378 },{ 378, 379, 364 },{ 380, 364, 379 },{ 381, 377, 353 },{ 377, 381, 382 },{ 382, 378, 377 },{ 378, 382, 383 },{ 383, 379, 378 },{ 379, 383, 384 },{ 384, 385, 379 },{ 379, 385, 380 },{ 386, 380, 385 },{ 353, 387, 381 },{ 388, 381, 387 },{ 382, 381, 388 },{ 388, 389, 382 },{ 383, 382, 389 },{ 389, 390, 383 },{ 384, 383, 390 },{ 390, 391, 384 },{ 384, 391, 392 },{ 392, 393, 384 },{ 385, 384, 393 },{ 387, 394, 388 },{ 388, 394, 354 },{ 354, 395, 388 },{ 389, 388, 395 },{ 395, 396, 389 },{ 390, 389, 396 },{ 396, 397, 390 },{ 391, 390, 397 },{ 397, 398, 391 },{ 392, 391, 398 },{ 399, 395, 354 },{ 395, 399, 363 },{ 363, 396, 395 },{ 396, 363, 400 },{ 400, 397, 396 },{ 397, 400, 401 },{ 401, 398, 397 },{ 398, 401, 371 },{ 371, 402, 398 },{ 398, 402, 392 },{ 354, 355, 399 },{ 402, 371, 403 },{ 403, 404, 402 },{ 402, 404, 405 },{ 405, 392, 402 },{ 392, 405, 409 },{ 409, 393, 392 },{ 406, 393, 409 },{ 393, 406, 385 },{ 385, 406, 386 },{ 372, 386, 406 },{ 406, 410, 372 },{ 409, 410, 406 },{ 405, 407, 408 },{ 408, 409, 405 },{ 410, 409, 408 },{ 408, 326, 410 },{ 410, 326, 332 },{ 332, 372, 410 },{ 411, 372, 332 },{ 332, 333, 411 },{ 411, 333, 340 },{ 340, 412, 411 },{ 412, 340, 317 },{ 317, 403, 412 },{ 404, 403, 317 },{ 317, 413, 404 },{ 404, 413, 407 },{ 407, 405, 404 },{ 414, 456, 415 },{ 415, 416, 414 },{ 414, 416, 417 },{ 417, 451, 414 },{ 416, 415, 418 },{ 418, 419, 416 },{ 416, 419, 445 },{ 445, 417, 416 },{ 419, 418, 420 },{ 420, 421, 419 },{ 419, 421, 422 },{ 422, 445, 419 },{ 423, 421, 420 },{ 421, 423, 438 },{ 438, 422, 421 },{ 420, 424, 423 },{ 423, 424, 479 },{ 479, 425, 423 },{ 438, 423, 425 },{ 425, 426, 438 },{ 426, 425, 487 },{ 487, 491, 426 },{ 425, 479, 481 },{ 481, 487, 425 },{ 427, 428, 429 },{ 429, 430, 427 },{ 431, 427, 430 },{ 430, 432, 431 },{ 490, 431, 432 },{ 432, 433, 490 },{ 491, 490, 433 },{ 433, 426, 491 },{ 430, 429, 434 },{ 434, 435, 430 },{ 432, 430, 435 },{ 435, 436, 432 },{ 432, 436, 437 },{ 437, 433, 432 },{ 433, 437, 438 },{ 438, 426, 433 },{ 439, 435, 434 },{ 435, 439, 440 },{ 440, 436, 435 },{ 436, 440, 441 },{ 441, 437, 436 },{ 437, 441, 422 },{ 422, 438, 437 },{ 434, 532, 439 },{ 442, 439, 532 },{ 439, 442, 443 },{ 443, 440, 439 },{ 440, 443, 444 },{ 444, 441, 440 },{ 441, 444, 445 },{ 445, 422, 441 },{ 532, 446, 442 },{ 447, 442, 446 },{ 442, 447, 448 },{ 448, 443, 442 },{ 444, 443, 448 },{ 448, 449, 444 },{ 445, 444, 449 },{ 449, 417, 445 },{ 417, 449, 450 },{ 450, 451, 417 },{ 414, 451, 450 },{ 446, 531, 447 },{ 447, 531, 452 },{ 452, 453, 447 },{ 448, 447, 453 },{ 453, 454, 448 },{ 449, 448, 454 },{ 454, 450, 449 },{ 455, 450, 454 },{ 450, 455, 414 },{ 456, 414, 455 },{ 455, 457, 456 },{ 456, 457, 458 },{ 458, 415, 456 },{ 459, 453, 452 },{ 453, 459, 460 },{ 460, 454, 453 },{ 454, 460, 455 },{ 457, 455, 460 },{ 460, 461, 457 },{ 457, 461, 462 },{ 462, 458, 457 },{ 463, 458, 462 },{ 458, 463, 418 },{ 418, 415, 458 },{ 452, 524, 459 },{ 464, 459, 524 },{ 459, 464, 461 },{ 461, 460, 459 },{ 524, 465, 464 },{ 464, 465, 518 },{ 518, 466, 464 },{ 461, 464, 466 },{ 466, 462, 461 },{ 462, 466, 467 },{ 467, 468, 462 },{ 462, 468, 463 },{ 469, 463, 468 },{ 463, 469, 420 },{ 420, 418, 463 },{ 466, 518, 470 },{ 470, 467, 466 },{ 471, 467, 470 },{ 467, 471, 472 },{ 472, 468, 467 },{ 468, 472, 469 },{ 473, 469, 472 },{ 469, 473, 424 },{ 424, 420, 469 },{ 470, 474, 471 },{ 475, 471, 474 },{ 471, 475, 476 },{ 476, 472, 471 },{ 472, 476, 473 },{ 473, 476, 477 },{ 477, 478, 473 },{ 424, 473, 478 },{ 478, 479, 424 },{ 479, 478, 480 },{ 480, 481, 479 },{ 474, 482, 475 },{ 483, 475, 482 },{ 475, 483, 477 },{ 477, 476, 475 },{ 482, 512, 483 },{ 483, 512, 536 },{ 536, 484, 483 },{ 477, 483, 484 },{ 484, 485, 477 },{ 478, 477, 485 },{ 485, 480, 478 },{ 486, 480, 485 },{ 480, 486, 487 },{ 487, 481, 480 },{ 488, 484, 536 },{ 484, 488, 489 },{ 489, 485, 484 },{ 485, 489, 486 },{ 490, 486, 489 },{ 486, 490, 491 },{ 491, 487, 486 },{ 536, 537, 488 },{ 427, 488, 537 },{ 488, 427, 431 },{ 431, 489, 488 },{ 489, 431, 490 },{ 537, 428, 427 },{ 334, 327, 492 },{ 493, 492, 327 },{ 303, 492, 493 },{ 492, 303, 494 },{ 494, 495, 492 },{ 492, 495, 334 },{ 334, 495, 496 },{ 496, 497, 334 },{ 344, 497, 496 },{ 327, 498, 493 },{ 499, 493, 498 },{ 375, 493, 499 },{ 493, 375, 303 },{ 498, 500, 499 },{ 499, 500, 312 },{ 312, 501, 499 },{ 373, 499, 501 },{ 499, 373, 375 },{ 502, 501, 312 },{ 503, 501, 502 },{ 501, 503, 373 },{ 312, 305, 502 },{ 504, 502, 305 },{ 374, 502, 504 },{ 502, 374, 503 },{ 305, 344, 504 },{ 496, 504, 344 },{ 505, 504, 496 },{ 504, 505, 374 },{ 496, 302, 505 },{ 302, 496, 495 },{ 495, 494, 302 },{ 482, 474, 506 },{ 506, 474, 470 },{ 470, 507, 506 },{ 508, 506, 507 },{ 506, 508, 509 },{ 509, 510, 506 },{ 506, 510, 482 },{ 482, 510, 511 },{ 511, 512, 482 },{ 513, 507, 470 },{ 507, 513, 514 },{ 507, 514, 508 },{ 515, 508, 514 },{ 508, 515, 516 },{ 516, 509, 508 },{ 517, 509, 516 },{ 517, 511, 509 },{ 509, 511, 510 },{ 470, 518, 513 },{ 513, 518, 465 },{ 465, 519, 513 },{ 514, 513, 519 },{ 514, 519, 520 },{ 520, 521, 514 },{ 514, 521, 515 },{ 515, 521, 522 },{ 522, 523, 515 },{ 516, 515, 523 },{ 520, 519, 465 },{ 465, 524, 520 },{ 520, 524, 452 },{ 452, 525, 520 },{ 521, 520, 525 },{ 525, 522, 521 },{ 525, 526, 522 },{ 522, 526, 527 },{ 522, 527, 528 },{ 528, 523, 522 },{ 523, 528, 529 },{ 529, 530, 523 },{ 523, 530, 516 },{ 525, 452, 531 },{ 531, 526, 525 },{ 526, 531, 446 },{ 446, 527, 526 },{ 527, 446, 532 },{ 532, 528, 527 },{ 528, 532, 434 },{ 434, 529, 528 },{ 533, 529, 434 },{ 529, 533, 530 },{ 530, 533, 534 },{ 530, 534, 535 },{ 535, 516, 530 },{ 516, 535, 517 },{ 536, 517, 535 },{ 517, 536, 512 },{ 512, 511, 517 },{ 434, 429, 533 },{ 533, 429, 428 },{ 428, 534, 533 },{ 534, 428, 537 },{ 537, 535, 534 },{ 535, 537, 536 }
};

/* Number of faces of cube */
#define CUBE_FACES 100


/* Light color matrix */
/* Each column represents the color matrix of each light source and is */
/* used as material color when using gte_ncs() or multiplied by a */
/* source color when using gte_nccs(). 4096 is 1.0 in this matrix */
/* A column of zeroes disables the light source. */
MATRIX color_mtx = {
	ONE * 3/4, 0, 0,	/* Red   */
	ONE * 3/4, 0, 0,	/* Green */
	ONE * 3/4, 0, 0	/* Blue  */
};

/* Light matrix */
/* Each row represents a vector direction of each light source. */
/* An entire row of zeroes disables the light source. */
MATRIX light_mtx = {
	/* X,  Y,  Z */
	-2048 , -2048 , -2048,
	0	  , 0	  , 0,
	0	  , 0	  , 0
};


/* Reference texture data */
extern const uint32_t tim_nicklas[];

/* TPage and CLUT values */
uint16_t texture_tpage;		/* For the scrolling blending pattern */
uint16_t texture_clut;

/* Function declarations */
void init();
void display();


/* Main function */
int main() {

	int i,p,xy_temp;
	
	SVECTOR	rot = { 0 };			/* Rotation vector for Rotmatrix */
	VECTOR	pos = { 0, 0, 400 };	/* Translation vector for TransMatrix */
	MATRIX	mtx,lmtx;				/* Rotation matrices for geometry and lighting */
	
	POLY_FT3 *pol4;					/* Flat shaded textured quad primitive pointer */
	
	
	/* Init graphics and GTE */
	init();
	
	
	/* Main loop */
	while( 1 ) {
		
		/* Set rotation and translation to the matrix */
		RotMatrix( &rot, &mtx );
		TransMatrix( &mtx, &pos );
		
		/* Multiply light matrix by rotation matrix so light source */
		/* won't appear relative to the model's rotation */
		MulMatrix0( &light_mtx, &mtx, &lmtx );
		
		/* Set rotation and translation matrix */
		gte_SetRotMatrix( &mtx );
		gte_SetTransMatrix( &mtx );
		
		/* Set light matrix */
		gte_SetLightMatrix( &lmtx );
		
		/* Make the cube SPEEN */
		rot.vx += 16;
		rot.vz += 16;
		
		
		/* Draw the cube */
		pol4 = (POLY_FT3*)db_nextpri;	
		
		for( i=0; i<CUBE_FACES; i++ ) {
			
			/* Load the first 3 vertices of a quad to the GTE */
			gte_ldv3( 
				&cube_verts[cube_indices[i].v0], 
				&cube_verts[cube_indices[i].v1], 
				&cube_verts[cube_indices[i].v2] );
				
			/* Rotation, Translation and Perspective Triple */
			gte_rtpt();
			
			/* Compute normal clip for backface culling */
			gte_nclip();
			
			/* Get result*/
			gte_stopz( &p );
			
			/* Skip this face if backfaced */
			if( p < 0 )
				continue;
			
			/* Calculate average Z for depth sorting */
			gte_avsz4();
			gte_stotz( &p );
			
			/* Skip if clipping off */
			/* (the shift right operator is to scale the depth precision) */
			if( (p>>2) > OT_LEN )
				continue;
			
			/* Initialize a quad primitive */
			setPolyFT3( pol4 );
			
			/* Set the projected vertices to the primitive */
			gte_stsxy0( &pol4->x0 );
			gte_stsxy1( &pol4->x1 );
			gte_stsxy2( &pol4->x2 );
			
			/* Compute the last vertex and set the result */
			// gte_ldv0( &cube_verts[cube_indices[i].v3] );
			// gte_rtps();
			// gte_stsxy( &pol4->x3 );
			
			/* Load primitive color even though gte_ncs() doesn't use it. */
			/* This is so the GTE will output a color result with the */
			/* correct primitive code. */
			gte_ldrgb( &pol4->r0 );
			
			/* Load the face normal */
			//gte_ldv0( &cube_norms[i] );
			
			/* Normal Color Single */
			gte_ncs();
			
			/* Store result to the primitive */
			gte_strgb( &pol4->r0 );
			
			/* Set face texture */
			//setUVWH( pol4, 0, 1, 128, 128 );
			setUV3( pol4, 0, 0, 0, 128, 128, 128 );
			pol4->tpage = texture_tpage;
			pol4->clut = texture_clut;
			
			/* Sort primitive to the ordering table */
			addPrim( db[db_active].ot+(p>>2), pol4 );
			
			/* Advance to make another primitive */
			pol4++;
			
		}
		
		/* Update nextpri variable */
		/* (IMPORTANT if you plan to sort more primitives after this) */
		db_nextpri = (char*)pol4;
		
		/* Swap buffers and draw the primitives */
		display();
		
	}
	
	return 0;
	
}

void init() {
	TIM_IMAGE tim;

	/* Reset the GPU, also installs a VSync event handler */
	ResetGraph( 0 );

	/* Set display and draw environment areas */
	/* (display and draw areas must be separate, otherwise hello flicker) */
	SetDefDispEnv( &db[0].disp, 0, 0, SCREEN_XRES, SCREEN_YRES );
	SetDefDrawEnv( &db[0].draw, SCREEN_XRES, 0, SCREEN_XRES, SCREEN_YRES );
	
	/* Enable draw area clear and dither processing */
	setRGB0( &db[0].draw, 63, 0, 127 );
	db[0].draw.isbg = 1;
	db[0].draw.dtd = 1;
	
	
	/* Define the second set of display/draw environments */
	SetDefDispEnv( &db[1].disp, SCREEN_XRES, 0, SCREEN_XRES, SCREEN_YRES );
	SetDefDrawEnv( &db[1].draw, 0, 0, SCREEN_XRES, SCREEN_YRES );
	
	setRGB0( &db[1].draw, 63, 0, 127 );
	db[1].draw.isbg = 1;
	db[1].draw.dtd = 1;
	
	
	/* Apply the drawing environment of the first double buffer */
	PutDrawEnv( &db[0].draw );
	
	
	/* Clear both ordering tables to make sure they are clean at the start */
	ClearOTagR( db[0].ot, OT_LEN );
	ClearOTagR( db[1].ot, OT_LEN );
	
	/* Set primitive pointer address */
	db_nextpri = db[0].p;
	
	/* Initialize the GTE */
	InitGeom();
	
	/* Set GTE offset (recommended method  of centering) */
	gte_SetGeomOffset( CENTERX, CENTERY );
	
	/* Set screen depth (basically FOV control, W/2 works best) */
	gte_SetGeomScreen( CENTERX );
	
	/* Set light ambient color and light color matrix */
	gte_SetBackColor( 63, 63, 63 );
	gte_SetColorMatrix( &color_mtx );

	/* Load .TIM file */
	GetTimInfo(tim_nicklas, &tim);
	if( tim.mode & 0x8 )
		LoadImage( tim.crect, tim.caddr );	/* Upload CLUT if present */
	LoadImage( tim.prect, tim.paddr );		/* Upload texture to VRAM */
	
	texture_tpage	= getTPage(tim.mode, 1, tim.prect->x, tim.prect->y);
	texture_clut	= getClut(tim.crect->x, tim.crect->y); 
}

void display() {
	
	/* Wait for GPU to finish drawing and vertical retrace */
	DrawSync( 0 );
	VSync( 0 );
	
	/* Swap buffers */
	db_active ^= 1;
	db_nextpri = db[db_active].p;
	
	/* Clear the OT of the next frame */
	ClearOTagR( db[db_active].ot, OT_LEN );
	
	/* Apply display/drawing environments */
	PutDrawEnv( &db[db_active].draw );
	PutDispEnv( &db[db_active].disp );
	
	/* Enable display */
	SetDispMask( 1 );
	
	/* Start drawing the OT of the last buffer */
	DrawOTag( db[1-db_active].ot+(OT_LEN-1) );
	
}

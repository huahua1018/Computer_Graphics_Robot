#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include "stb_image.h"
// Define polygon drwaing models, filled or outlined.
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define FILL 1
#define LINE 2

#define LEFT 1
#define RIGHT 2

#define LEFT_UP 0
#define LEFT_DOWN 1
#define RIGHT_UP 2
#define RIGHT_DOWN 3

#define table_width 15.0
#define table_length 25.0
#define table_thickness 1.5
#define table_height 4.0
#define table_feet_width 1

#define polished_silver 0
#define obsidian 1
#define red_rubber 2
#define white_plastic 3
#define turquoise 4
#define perl 5
#define glass 6
#define polished_bronze 7
#define white_diffuse 8
#define green_diffuse 9
#define brass 10
#define billboard 11

#define lit_white 0
#define lit_red 1
#define lit_orange 2
#define lit_yellow 3
#define lit_green 4
#define lit_blue 5
#define lit_purple 6

#define TRUE 1
#define FALSE 0

#define BIG 1
#define SMALL 0

#define TRANS 0
#define ROT 1

#define STEP_MOD 10
#define TOTAL_STEP_MOD 209
#define PI acos(-1.0)

/* window shape */
int width = 700, height = 700;
// declare the rotational angle.
float rot_ang[3] = {0, 0, 0};
float left_thigh_ang = 0;
float right_thigh_ang = 0;

float left_calf_ang = 0;
float right_calf_ang = 0;

float left_arm_ang = 0;
float right_arm_ang = 0;

float left_forearm_ang = 0;
float right_forearm_ang = 0;

float left_feet_ang = 0;
float right_feet_ang = 0;

float left_hand_ang = 0;
float right_hand_ang = 0;

// Quodri objects for drawing the world coordinate system.
/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj *sphere = NULL, *cylind = NULL, *mycircle = NULL;

// setting
int polygonMode = FILL;
int view_mode = 0;
int thigh_state = 0;
float head_ang = 0;
float walk_state_ang[8][4] = {{0, 0, 0, 0}, {5, 0, 0, -50}, {-15, 0, 45, -50}, {-20, 0, 45, -20}, {0, -50, 5, 0}, {45, -50, -15, 0}, {45, -20, -20, 0}, {5, 0, 0, -50}}; // 左大左小右大右小
float walk_state_time[9] = {0, 3, 9, 18, 24, 27, 33, 42, 48};
float walk_hand_state[8] = {0, 1, 1, 2, 3, 3, 0, 0};
float walk_hand_state_ang[4][4] = {{0, 15, 0, 15}, {18, 25, -18, 25}, {0, 15, 0, 15}, {-18, 25, 18, 25}};
float walk_cnt = 5;
float turn_feet_state_time[4] = {0, 4, 8, 12};
float turn_feet_state_ang[4][4] = {{0, 0, 0, 0}, {10, -10, 0, 0}, {0, 0, 0, 0}, {0, 0, 10, -10}};
float turn_cnt = 0;
float jump_feet_state_time[7] = {0, 64, 128, 230, 332, 396, 460};
float jump_time_slice = 3.0 / 204;
float jump_feet_state_ang[7][5] = {{0, 0, 0, 0, 0}, {-5, -1.5, 70, -90, 20}, {0, 0, 0, 0, 0}, {0, 11.025, 0, 0, 0}, {0, 0, 0, 0, 0}, {-5, -1.5, 70, -90, -90}, {0, 0, 0, 0, 0}}; // 前傾,y座標,大腿,小腿,腳掌
float jump_cnt = 0;
float sayhi_state_time[7] = {0, 128, 212, 296, 380, 474, 592};
float sayhi_state_ang[7][3] = {{0, 0, 0}, {175, 0, 50}, {175, -60, 56}, {175, 0, 50}, {175, -60, 56}, {175, 0, 50}, {0, 0, 0}}; // left_arm,left_forearm,tpang
float sayhi_cnt = 0;
int sayhi = FALSE;
int take_coffee = FALSE;
int table_coffee = TRUE;
float robot_range = 4.5;
int watch_mode = BIG;
int coffee_overthrow = FALSE;
int coffee_onfloor = FALSE;
int step_on_coffee = FALSE;
float coffee_onfloor_info[6] = {0.0};    // cos,sin,ang,x,y,z
float step_on_coffee_info[6][6] = {0.0}; // cos,sin,ang,x,y,z
int idx_cnt = -1;
int step_cnt = 0;
int view_volume = FALSE;

// pos
float robot_pos[3] = {10.0, 0.0, 10.0};
float view_pos[3][9] = {{79.000000, 23.000000, 25.000000, -170.000000, 9.000000, 35.000000, 0.000000, 1.000000, 0.000000},
                        {88.000000, 26.000000, 77.000000, -170.000000, -65.000000, -144.000000, 0.000000, 1.000000, 0.000000},
                        {85.000000, 55.000000, 79.000000, -170.000000, -95.000000, -142.000000, 0.000000, 1.000000, 0.000000}};
float table_pos[3] = {25, 0, 25};
float eyes_pos[3] = {3.0, 0.7, 1.7};
GLfloat ear_tri_pos[5][3] = {{-0.7, 0.0, 0.7}, {0.0, 1.35, 0.0}, {-0.7, 0.0, -0.7}, {0.7, 0.0, -0.7}, {0.7, 0.0, 0.7}};
float coffee_pos[3] = {10, 0, 14.5};
float table_top_point[8][3] = {{0, 0, 0}, {table_length, 0, 0}, {table_length, 0, table_width}, {0, 0, table_width}, {0, -table_thickness, 0}, {table_length, -table_thickness, 0}, {table_length, -table_thickness, table_width}, {0, -table_thickness, table_width}};
int table_faces[6][4] = {{0, 1, 2, 3}, {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6}, {0, 3, 7, 4}, {4, 5, 6, 7}};
float table_normal[6][4] = {{0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
float table_feet_point[8][3] = {{0, 0, 0}, {table_feet_width, 0, 0}, {table_feet_width, 0, table_feet_width}, {0, 0, table_feet_width}, {0, -table_height, 0}, {table_feet_width, -table_height, 0}, {table_feet_width, -table_height, table_feet_width}, {0, -table_height, table_feet_width}};
float table_feet_trans[4][3] = {{0, 0, 0}, {table_length - table_feet_width, 0, 0}, {table_length - table_feet_width, 0, table_width - table_feet_width}, {0, 0, table_width - table_feet_width}};
float coffee_overthrow_pos[3] = {0.0, 0.1, 0.0};

float points[][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
int face[][4] = {{3, 2, 1,0}, {5,4,0,1}, {2,6,5,1}, {2,3,7,6}, { 6, 7,4,5}, { 7, 3,0,4}};
//int face[][4] = {{ 7, 3,0,4}, {5,4,0,1},{3, 2, 1,0} , {2,3,7,6}, { 6, 7,4,5},{2,6,5,1}};
float normal[][4] = {{0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},{0.0, 0.0, 1.0}, {-1.0, 0.0, 0.0}};
int face_map[6] = {2, 4, 1, 3, 5,0};  

// color
// float background_color[3] = { 1.0,0.98,0.8 };
float background_color[2][3] = {{0.83, 0.90, 0.95}, {0.05, 0.05, 0.05}};
float eyes_color[3] = {1.0, 0.84, 0.0};
float nose_color[3] = {0.73, 0.73, 0.73};

float tpx, tpy, tpz, tpang = 0;

float eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double Eye[3] = {4.0, 20, 16.5}, Focus[3] = {0.0, 0.0, 0.0},
       Vup[3] = {0.0, 1.0, 0.0};

float u[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
float eye[3];
float cv, sv; /* cos(5.0) and sin(5.0) */
float zoom_fac = 1;

// light
int light0_on = 1;
int light1_on = 1;
int light2_on = 1;
int light3_on = 1;

float light_color[7][3] = {{1.0, 1.0, 1.0},
                           {1.0, 0.18, 0.18},
                           {1.0, 0.5, 0.15},
                           {1.0, 1.0, 0.57},
                           {0.63, 0.98, 0.56},
                           {0.49, 0.52, 0.97},
                           {0.71, 0.16, 0.91}};

float global_ambient[] = {0.2, 0.2, 0.2, 1.0};
float lit_specular[] = {0.7, 0.7, 0.7, 1.0};
float lit_diffuse[] = {0.8, 0.8, 0.8, 1.0};
float lit_direction[] = {0.0, 50, 50.0, 0.0};
float lit_intensity = 0.7;
int lit_color = lit_white;

float lit1_specular[] = {0.7, 0.7, 0.7, 1.0};
float lit1_diffuse[] = {0.5, 0.5, 0.5, 1.0};
float lit1_direction[] = {0.0, 0.0, 1.0, 0.0};
float lit1_position[] = {0.0, 35.0, 0.0, 1.0};
float lit1_emission[] = {0.9, 0.9, 0.7, 1.0};
float lit1_close_emission[] = {0, 0, 0, 1.0};
float lit1_intensity = 0.5;
int lit1_color = lit_white;

float lit2_position[] = {0.0, 0.0, 0.0, 1.0};
float lit2_direction[] = {1.0, 0.0, 0.0, 0.0};
float lit2_diffuse[] = {0.5, 0.5, 0.5, 1.0};
float lit2_specular[] = {0.7, 0.7, 0.7, 1.0};
float lit2_emission[] = {0.780392f, 0.568627f, 0.313725f, 1.0};
float lit2_cutoff = 15.0;
float lit2_exponent = 2.0;
float lit2_intensity = 0.5;
int lit2_color = lit_white;

float lit_temp_specular[] = {0.7, 0.7, 0.7, 1.0};

float polished_silver_ambient[] = {0.23125f, 0.23125f, 0.23125f, 1.0f};
float polished_silver_diffuse[] = {0.6775f, 0.6775f, 0.6775f, 1.0f};
float polished_silver_specular[] = {0.773911f, 0.773911f, 0.773911f, 1.0f};
float polished_silver_shininess = 89.6f;

float obsidian_ambient[] = {0.05375f, 0.05f, 0.06625f, 1.0f};
float obsidian_diffuse[] = {0.2275f, 0.27f, 0.32525f, 1.0f};
float obsidian_specular[] = {0.332741f, 0.328634f, 0.346435f, 1.0f};
float obsidian_shininess = 38.4f;

float red_rubber_ambient[] = {0.05f, 0.0f, 0.0f, 1.0f};
float red_rubber_diffuse[] = {0.6f, 0.4f, 0.3f, 1.0f};
float red_rubber_specular[] = {0.7f, 0.04f, 0.04f, 1.0f};
float red_rubber_shininess = 10.0f;

float white_plastic_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
float white_plastic_diffuse[] = {0.55f, 0.55f, 0.55f, 1.0f};
float white_plastic_specular[] = {0.70f, 0.70f, 0.70f, 1.0f};
float white_plastic_shininess = 32.0f;

float turquoise_ambient[] = {0.1f, 0.18725f, 0.1745f, 1.0f};
float turquoise_diffuse[] = {0.396f, 0.74151f, 0.69102f, 1.0f};
float turquoise_specular[] = {0.297254f, 0.30829f, 0.306678f, 1.0f};
float turquoise_shininess = 12.8f;

float perl_ambient[] = {0.25f, 0.20725f, 0.20725f, 1.0f};
float perl_diffuse[] = {1.0f, 0.829f, 0.829f, 1.0f};
float perl_specular[] = {0.296648f, 0.296648f, 0.296648f, 1.0f};

float glass_ambient[] = {0.2f, 0.2f, 0.2f, 0.6f};
float glass_diffuse[] = {0.95f, 0.95f, 0.95f, 0.7f};
float glass_specular[] = {1.0f, 1.0f, 1.0f, 0.6f};

float polished_bronze_ambient[] = {0.25f, 0.148f, 0.06475f, 1.0f};
float polished_bronze_diffuse[] = {0.4f, 0.2368f, 0.1036f, 1.0f};
float polished_bronze_specular[] = {0.774597f, 0.458561f, 0.200621f, 1.0f};
float polished_bronze_shininess = 89.6f;

float white_diffuse_ambient[] = {0.05f, 0.05f, 0.05f, 1.0f};
float white_diffuse_diffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
float white_diffuse_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
float white_diffuse_shininess = 10.0f;

float green_diffuse_diffuse[] = {0.5, 0.65, 0.6, 1.0f};

float brass_ambient[] = {0.329412f, 0.223529f, 0.027451f, 1.0f};
float brass_diffuse[] = {0.780392f, 0.568627f, 0.313725f, 1.0f};
float brass_specular[] = {0.992157f, 0.941176f, 0.807843f, 1.0f};
float brass_shininess = 27.8974f;

float footprint_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
float footprint_diffuse[] = {0.8f, 0.9f, 0.8f, 1.0f};
float footprint_specular[] = {0.7f, 0.9f, 0.9f, 1.0f};

float billboard_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
float billboard_diffuse[] = {0.99f, 0.99f, 0.99f, 1.0f};
float billboard_specular[] = {0.7f, 0.7f, 0.7f, 1.0f};

float sun_time = 9;
float sun_ang = 45;

float star_emission[] = {1.0, 0.9f, 0.87f, 1.0};
float star_pos[8][3] = {{-27.5, 51, 6}, {2.5, 46, 20}, {-8.5, 52, -13}, {-1.500000, 48, 12}, {12.500000, 53, -23}, {-15.5, 42, -1}, {30, 44, -27}};

// texture
unsigned int TextureID[4];
int image_width, image_height, nrChannels;
unsigned char *image_data;

float wallpaper_dis = 0;
float wallpaper_ang;
float wallpaper_mode = TRANS;

float vmtx[16];
float billboard_a[3];
float billboard_b[3] = {0, 1, 0};

unsigned int birdID[13];
int bird_idx = 1;
int bird_pos_x = 50;
int bird_pos_y = 20;

unsigned int tree1ID[15];
int tree1_idx = 1;

unsigned int tree2ID[119];
int tree2_idx = 1;

unsigned int skyboxID[6];

int fog_color=lit_white;
int fog_mode=0;
float fog_density=0.05;
float fog_dis=120;
int fog_on = TRUE;

/*------------------------------------------------------------------
 * Procedure to draw a cylinder.
 */
void draw_cylinder(void)
{
    gluCylinder(cylind, 0.3, 0.3, /* radius of top and bottom circle */
                16.0,             /* height of the cylinder */
                12,               /* use 12-side polygon approximating circle*/
                4);               /* Divide it into 3 sections */
}

/*------------------------------------------------------------------
 * Procedure to draw an arrow .
 */
void draw_arrow(void)
{
    gluCylinder(cylind, 0.6, 0.0, /* radius of top and bottom circle */
                1.0,              /* height of the cylinder */
                12,               /* use 12-side polygon approximating circle*/
                4);               /* Divide it into 3 sections */
}
/*----------------------------------------------------------------------
 * Procedure to draw a coordinate system, RGB bars = XYZ axes
 * White ball = origin.
 */
void draw_coord_sys(void)
{

    // draw coord. sys. in line or fill mode.
    if (polygonMode == LINE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glColor3f(1.0, 1.0, 1.0);
    gluSphere(sphere, 0.5, /* radius=0.5 */
              12,          /* composing of 12 slices*/
              12);         /* composing of 12 stacks */
    // Draw the x-axis in RED.
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0, 1, 0); // The x-axis
    draw_cylinder();
    glTranslatef(0, 0, 16);
    draw_arrow();
    glPopMatrix();
    // Draw the Y-axis in GREEN.
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix(); // The y-axis
    glRotatef(-90.0, 1, 0, 0);
    draw_cylinder();
    glTranslatef(0, 0, 16);
    draw_arrow();
    glPopMatrix();
    // Draw the z-axis in BLUE.
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    draw_cylinder(); // The z-axis
    glTranslatef(0, 0, 16);
    draw_arrow();
    glPopMatrix();
}

void walk_end();
void set_material(int mode)
{
    switch (mode)
    {
    case polished_silver:
        glMaterialfv(GL_FRONT, GL_AMBIENT, polished_silver_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, polished_silver_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, polished_silver_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, polished_silver_shininess);
        break;
    case obsidian:
        glMaterialfv(GL_FRONT, GL_AMBIENT, obsidian_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, obsidian_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, obsidian_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, obsidian_shininess);
        break;
    case red_rubber:
        glMaterialfv(GL_FRONT, GL_AMBIENT, red_rubber_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, red_rubber_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red_rubber_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, red_rubber_shininess);
        break;
    case white_plastic:
        glMaterialfv(GL_FRONT, GL_AMBIENT, white_plastic_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white_plastic_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, white_plastic_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, white_plastic_shininess);
        break;
    case turquoise:
        glMaterialfv(GL_FRONT, GL_AMBIENT, turquoise_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, turquoise_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, turquoise_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, turquoise_shininess);
        break;
    case perl:
        glMaterialfv(GL_FRONT, GL_AMBIENT, perl_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, perl_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, perl_diffuse);
        break;
    case glass:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glass_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glass_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glass_diffuse);
        break;
    case polished_bronze:
        glMaterialfv(GL_FRONT, GL_AMBIENT, polished_bronze_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, polished_bronze_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, polished_bronze_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, polished_bronze_shininess);
        break;
    case white_diffuse:
        glMaterialfv(GL_FRONT, GL_AMBIENT, white_diffuse_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white_diffuse_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, white_diffuse_shininess);
        break;
    case green_diffuse:
        glMaterialfv(GL_FRONT, GL_AMBIENT, white_diffuse_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white_diffuse_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, white_diffuse_shininess);
        break;
    case brass:
        glMaterialfv(GL_FRONT, GL_AMBIENT, brass_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, brass_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, brass_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, brass_shininess);
        break;
    case billboard:
        glMaterialfv(GL_FRONT, GL_AMBIENT, billboard_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, billboard_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, billboard_diffuse);
        break;
    }
}
void draw_cube()
{
    for (int i = 0; i < 6; i++)
    { /* draw the six faces one by one */
        glNormal3fv(normal[i]);
        glBegin(GL_POLYGON); /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}


void do_texture(int idx)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    if (idx == 4)
        glBindTexture(GL_TEXTURE_2D, birdID[bird_idx]);
    else if (idx == 5)
        glBindTexture(GL_TEXTURE_2D, tree1ID[tree1_idx]);
    else if (idx == 6)
        glBindTexture(GL_TEXTURE_2D, tree2ID[tree2_idx]);
    else
        glBindTexture(GL_TEXTURE_2D, TextureID[idx]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}
void no_texture()
{
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void draw_skybox()
{
    // glEnable(GL_TEXTURE_2D);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GREATER, 0.5);

    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // glBindTexture(GL_TEXTURE_2D, skyboxID[1]);

    // glMatrixMode(GL_TEXTURE);
    // glLoadIdentity();
    // glMatrixMode(GL_MODELVIEW);
    // glPushMatrix();
    glDisable(GL_LIGHTING);
    do_texture(3);
    glColor4f(1,1,1,1);
    //glBindTexture(GL_TEXTURE_2D, skyboxID[3]);
    for (int i = 0; i < 6; i++)
    { /* draw the six faces one by one */
        glBindTexture(GL_TEXTURE_2D, skyboxID[i+1]);
        glNormal3f(-normal[face_map[i]][0],-normal[face_map[i]][1],-normal[face_map[i]][2]);
        glBegin(GL_POLYGON); /* Draw the face */
        glTexCoord2f(0.0,0.0);
        glVertex3fv(points[face[face_map[i]][0]]);
        glTexCoord2f(1.0,0.0);
        glVertex3fv(points[face[face_map[i]][1]]);
        glTexCoord2f(1.0,1.0);
        glVertex3fv(points[face[face_map[i]][2]]);
        glTexCoord2f(0.0,1.0);
        glVertex3fv(points[face[face_map[i]][3]]);
        glEnd();
    }
    no_texture();
    glEnable(GL_LIGHTING);
}

void draw_billboard(float x, float y, float z, float w, float h)
{
    float v0[3], v1[3], v2[3], v3[3];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/

    v0[0] = x - (w / 2) * billboard_a[0];
    v0[1] = y - (w / 2) * billboard_a[1];
    v0[2] = z - (w / 2) * billboard_a[2];
    v1[0] = x + (w / 2) * billboard_a[0];
    v1[1] = y + (w / 2) * billboard_a[1];
    v1[2] = z + (w / 2) * billboard_a[2];
    v2[0] = v1[0] + h * billboard_b[0];
    v2[1] = v1[1] + h * billboard_b[1];
    v2[2] = v1[2] + h * billboard_b[2];
    v3[0] = v0[0] + h * billboard_b[0];
    v3[1] = v0[1] + h * billboard_b[1];
    v3[2] = v0[2] + h * billboard_b[2];
    ;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(v2);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(v3);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(v0);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(v1);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void draw_wallpaper()
{

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    if (wallpaper_mode == TRANS)
        glTranslatef(wallpaper_dis, 0, 0);
    if (wallpaper_mode == ROT)
        glRotatef(wallpaper_ang, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glNormal3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON); /* Draw the face */
    glTexCoord2f(3.0, 2.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(3.0, 0.0);
    glVertex3f(0.0, 40.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 40.0, 65.0);
    glTexCoord2f(0.0, 2.0);
    glVertex3f(0.0, 0.0, 65.0);
    glEnd();
    no_texture();
}

void draw_wall(int a, int b, int tran1, int tran2)
{

    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            draw_cube();
            switch (tran2)
            {
            case 1:
                glTranslatef(1, 0, 0);
                break;
            case 2:
                glTranslatef(0, 1, 0);
                break;
            case 3:
                glTranslatef(0, 0, 1);
                break;
            }
        }
        switch (tran2)
        {
        case 1:
            glTranslatef(-b, 0, 0);
            break;
        case 2:
            glTranslatef(0, -b, 0);
            break;
        case 3:
            glTranslatef(0, 0, -b);
            break;
        }
        switch (tran1)
        {
        case 1:
            glTranslatef(1, 0, 0);
            break;
        case 2:
            glTranslatef(0, 1, 0);
            break;
        case 3:
            glTranslatef(0, 0, 1);
            break;
        }
    }
}

void draw_footprint()
{
    glPushMatrix();
    glScalef(1.3, 0.8, 1.0);
    // glColor3f(0.58, 0.32, 0.16);
    gluDisk(mycircle,
            0.0, /* inner radius=0.0 */
            3.0, /* outer radius=10.0 */
            16,  /* 16-side polygon */
            3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 1.5, 0);
    glScalef(1.1, 1.0, 1.0);
    gluDisk(mycircle,
            0.0, /* inner radius=0.0 */
            2.4, /* outer radius=10.0 */
            16,  /* 16-side polygon */
            3);
    glPopMatrix();

    // 腳趾
    glPushMatrix();
    glTranslatef(-4.2, 3.5, 0);
    // draw_coord_sys();
    glRotatef(21, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.2, 1.0);
    gluDisk(mycircle,
            0.0, /* inner radius=0.0 */
            1.7, /* outer radius=10.0 */
            16,  /* 16-side polygon */
            3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.7, 6.0, 0);
    glRotatef(12, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.2, 1.0);
    gluDisk(mycircle,
            0.0, /* inner radius=0.0 */
            1.7, /* outer radius=10.0 */
            16,  /* 16-side polygon */
            3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.7, 6.0, 0);
    glRotatef(-12, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.2, 1.0);
    gluDisk(mycircle,
            0.0, /* inner radius=0.0 */
            1.7, /* outer radius=10.0 */
            16,  /* 16-side polygon */
            3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.2, 3.5, 0);
    glRotatef(-21, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.2, 1.0);
    gluDisk(mycircle,
            0.0, /* inner radius=0.0 */
            1.7, /* outer radius=10.0 */
            16,  /* 16-side polygon */
            3);
    glPopMatrix();
}

void draw_floor()
{
    int i, j;

    if (polygonMode == LINE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (i = 0; i < 65; i++)
    {
        for (j = 0; j < 65; j++)
        {
            if ((i / 5 + j / 5) % 2 == 0)
                // glColor3f(0.95, 0.95, 0.95);
                set_material(white_diffuse);
            else
                set_material(green_diffuse);
            glNormal3f(0.0, 1.0, 0.0);
            glBegin(GL_POLYGON);
            glVertex3f(i, 0.0, j);
            glVertex3f(i, 0.0, j + 1);
            glVertex3f(i + 1, 0.0, j + 1);
            glVertex3f(i + 1, 0.0, j);
            glEnd();
        }
    }

    // 灑在地上的咖啡
    // glColor3f(0.8, 0.2, 0.2);
    if (coffee_overthrow == TRUE)
    {
        float cos_ = cos(rot_ang[1] * PI / 180);
        float sin_ = -sin(rot_ang[1] * PI / 180);
        coffee_onfloor_info[0] = cos_;
        coffee_onfloor_info[1] = sin_;
        coffee_onfloor_info[2] = rot_ang[1];
        coffee_onfloor_info[3] = robot_pos[0] + 5 * cos_;
        coffee_onfloor_info[4] = 0.1;
        coffee_onfloor_info[5] = robot_pos[2] + 5 * sin_;
        coffee_overthrow = FALSE;
        take_coffee = FALSE;
        coffee_onfloor = TRUE;
    }
    if (coffee_onfloor == TRUE)
    {
        glPushMatrix();
        glTranslatef(coffee_onfloor_info[3], coffee_onfloor_info[4], coffee_onfloor_info[5]);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glRotatef(coffee_onfloor_info[2], 0.0, 0.0, 1.0);
        // glColor3f(0.58, 0.32, 0.16);
        set_material(polished_bronze);
        glScalef(1.0, 1.4, 1.0);
        glRotatef(coffee_onfloor_info[2], 0.0, 0.0, 1.0);
        gluDisk(mycircle,
                0.0, /* inner radius=0.0 */
                3.0, /* outer radius=10.0 */
                16,  /* 16-side polygon */
                3);
        glPopMatrix();
    }

    // 腳印
    if (step_on_coffee == TRUE)
    {
        for (int i = 0; i < min(idx_cnt, 6); i++)
        {
            glPushMatrix();
            glTranslatef(step_on_coffee_info[i][3], step_on_coffee_info[i][4], step_on_coffee_info[i][5]);
            glScalef(0.2, 0.2, 0.2);
            glRotatef(-90, 1.0, 0.0, 0.0);
            glRotatef(-90, 0.0, 0.0, 1.0);
            glRotatef(step_on_coffee_info[i][2], 0.0, 0.0, 1.0);
            set_material(polished_bronze);
            draw_footprint();
            glPopMatrix();
        }
    }

    // 左牆(門牆)
    // glColor3f(0.7, 0.8, 0.75);
    glPushMatrix();
    glTranslatef(1.02, 0.0, 0.0);
    draw_wallpaper();
    glPopMatrix();

    glPushMatrix();
    set_material(turquoise);
    draw_wall(65, 40, 3, 2);
    glPopMatrix();

    if (lit2_color == lit_purple && light2_on && !light0_on && !light1_on)
    {
        // glScalef(10,10,10);
        glPushMatrix();
        glTranslatef(1.2, 20, 32.5);
        glRotatef(90, 0.0, 1.0, 0.0);
        // set_material(white_plastic);
        //   gluSphere(sphere, 5.0, /* radius=2.0 */
        //        12,          /* composing of 12 slices*/
        //        12);         /* composing of 12 stacks */
        glMaterialfv(GL_FRONT, GL_AMBIENT, footprint_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, footprint_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, footprint_diffuse);
        draw_footprint();
        glPopMatrix();
    }

    // 門
    // glColor4f(0.6, 0.4, 0.3, 1.0);
    glPushMatrix();
    glTranslatef(-0.1, 0, 5);
    glScalef(1.2, 1, 1);
    set_material(red_rubber);
    draw_wall(25, 15, 2, 3);
    glPopMatrix();

    // 門把
    // glColor3fv(nose_color);
    glPushMatrix();
    glTranslatef(1.1, 10.3, 8);
    glRotatef(90, 0.0, 1.0, 0.0);
    set_material(polished_silver);
    gluSphere(sphere, 1.0, /* radius=2.0 */
              12,          /* composing of 12 slices*/
              12);         /* composing of 12 stacks */
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.8, 10.3, 8);
    glRotatef(90, 0.0, 1.0, 0.0);
    gluSphere(sphere, 1.0, /* radius=2.0 */
              12,          /* composing of 12 slices*/
              12);         /* composing of 12 stacks */
    glPopMatrix();

    // 右牆
    // glColor3f(0.7, 0.8, 0.75);
    glPushMatrix();
    glTranslatef(65 - 0.02, 0.0, 65.0);
    glRotatef(-180, 0.0, 1.0, 0.0);
    draw_wallpaper();
    glPopMatrix();

    set_material(turquoise);
    glPushMatrix();
    glTranslatef(65, 0, 0);
    draw_wall(40, 65, 2, 3);
    glPopMatrix();

    // 天花板
    // glColor3f(0.7, 0.8, 0.75);
    glPushMatrix();
    glTranslatef(0, 40, 0);
    // glScalef(67, 1, 65);
    // draw_cube();
    draw_wall(66, 65, 1, 3);
    glPopMatrix();

    // 前牆(窗戶牆)
    // glColor3f(0.7, 0.8, 0.75);
    glPushMatrix();
    draw_wall(40, 8, 2, 1);
    // glScalef(8, 40, 1);
    // draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(57, 0, 0);
    // glScalef(8, 40, 1);
    draw_wall(40, 8, 2, 1);
    // draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 32, 0);
    // glScalef(65, 8, 1);
    // draw_cube();
    draw_wall(8, 65, 2, 1);
    glPopMatrix();

    glPushMatrix();
    // glScalef(65, 8, 1);
    // draw_cube();
    draw_wall(8, 65, 2, 1);
    glPopMatrix();

    // 窗戶
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0, 1.0, 1.0, 0.5);
    glPushMatrix();
    glTranslatef(10, 10, 0);
    set_material(glass);
    // glScalef(45, 20, 1);
    // draw_cube();
    draw_wall(45, 20, 1, 2);
    glPopMatrix();
    glDisable(GL_BLEND);

    // 窗戶框
    // glColor3f(0.9, 0.9, 0.9);
    set_material(perl);
    glPushMatrix();
    glTranslatef(8, 8, 0);
    // glScalef(2, 24, 1);
    // draw_cube();
    draw_wall(2, 24, 1, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, 8, 0);
    // glScalef(49, 2, 1);
    // draw_cube();
    draw_wall(49, 2, 1, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(55, 8, 0);
    // glScalef(2, 24, 1);
    // draw_cube();
    draw_wall(2, 24, 1, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, 30, 0);
    // glScalef(49, 2, 1);
    // draw_cube();
    draw_wall(49, 2, 1, 2);
    glPopMatrix();
}
void draw_ear(int a, int b, int c)
{
    float v1[3];
    float v2[3];
    float n[4] = {0};
    for (int i = 0; i < 3; i++)
    {
        v1[i] = ear_tri_pos[b][i] - ear_tri_pos[a][i];
        v2[i] = ear_tri_pos[c][i] - ear_tri_pos[a][i];
    }
    n[0] = v1[1] * v2[2] - v1[2] * v2[1];
    n[1] = -(v1[0] * v2[2] - v1[2] * v2[0]);
    n[2] = v1[0] * v2[1] - v1[1] * v2[0];

    set_material(polished_silver);
    do_texture(1);
    glNormal3fv(n);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(ear_tri_pos[a]);
    glTexCoord2f(0.25, 0.5);
    glVertex3fv(ear_tri_pos[b]);
    glTexCoord2f(0.5, 0.0);
    glVertex3fv(ear_tri_pos[c]);
    glEnd();
    no_texture();
    set_material(obsidian);
    glNormal3fv(n);
    glBegin(GL_TRIANGLES);
    glVertex3fv(ear_tri_pos[a]);
    glVertex3fv(ear_tri_pos[b]);
    glVertex3fv(ear_tri_pos[c]);
    glEnd();
}
void draw_head()
{
    glTranslatef(0.0, 4.2, 0.0);
    glRotatef(head_ang, 0.0, 0.0, 1.0); // 頭旋轉
    glPushMatrix();                     // 存下M4
    // 畫頭
    glScalef(1.0, 1.0, 1.16);
    // glColor3f(0.0, 0.0, 0.0);
    do_texture(1);
    gluSphere(sphere, 3.15, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    no_texture();
    set_material(obsidian);
    gluSphere(sphere, 3.15, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();          // 回到M4

    // 畫左耳朵
    glPushMatrix(); // 存下M4
    glTranslatef(0.0, 2.0, 1.8);
    glRotatef(26, 1.0, 0.0, 0.0);
    glScalef(2.0, 2.0, 2.0);
    glColor3f(0.0, 0.0, 0.0);
    draw_ear(0, 1, 2);
    draw_ear(2, 1, 3);
    draw_ear(3, 1, 4);
    draw_ear(4, 1, 0);
    glPopMatrix(); // 回到M4

    // 畫右耳朵
    glPushMatrix(); // 存下M4
    glTranslatef(0.0, 2.0, -1.8);
    glRotatef(-26, 1.0, 0.0, 0.0);
    glScalef(2.0, 2.0, 2.0);
    glColor3f(0.0, 0.0, 0.0);
    draw_ear(0, 1, 2);
    draw_ear(2, 1, 3);
    draw_ear(3, 1, 4);
    draw_ear(4, 1, 0);
    glPopMatrix(); // 回到M4

    // 畫左眼睛

    glPushMatrix(); // 存下M4
    glTranslatef(eyes_pos[0] + 0.2, eyes_pos[1], eyes_pos[2]);
    glScalef(0.3, 1.0, 1.0);
    // glColor3f(0.0, 0.0, 0.0);
    set_material(obsidian);
    gluSphere(sphere, 0.68, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();          // 回到M4

    // 畫右眼睛

    glPushMatrix(); // 存下M4
    glTranslatef(eyes_pos[0] + 0.2, eyes_pos[1], -eyes_pos[2]);
    glScalef(0.3, 1.0, 1.0);
    // glColor3f(0.0, 0.0, 0.0);
    set_material(obsidian);
    gluSphere(sphere, 0.68, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();          // 回到M4

    // 畫鼻子
    glPushMatrix(); // 存下M4
    glTranslatef(3.2, -0.2, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    // glColor3f(1.0,1.0,1.0);
    // glColor3fv(nose_color);
    set_material(polished_silver);
    // gluCylinder(cylind, 0.6, 0.6, /* radius of top and bottom circle */
    //             0.6,              /* height of the cylinder */
    //             12,               /* use 12-side polygon approximating circle*/
    //             3);               /* Divide it into 3 sections */
    gluSphere(sphere, 0.35, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();          // 回到M4

    // 畫鼻子鬍鬚
    glPushMatrix(); // 存下M4
    glTranslatef(3.0, -0.45, 0.0);
    glRotatef(55, 1.0, 0.0, 0.0);
    glColor3fv(nose_color);
    gluCylinder(cylind, 0.12, 0.12, /* radius of top and bottom circle */
                0.9,                /* height of the cylinder */
                12,                 /* use 12-side polygon approximating circle*/
                3);                 /* Divide it into 3 sections */
    glPopMatrix();                  // 回到M4

    glPushMatrix(); // 存下M4
    glTranslatef(3.0, -0.45, 0.0);
    glRotatef(-235, 1.0, 0.0, 0.0);
    glColor3fv(nose_color);
    gluCylinder(cylind, 0.12, 0.12, /* radius of top and bottom circle */
                0.9,                /* height of the cylinder */
                12,                 /* use 12-side polygon approximating circle*/
                3);                 /* Divide it into 3 sections */
    glPopMatrix();                  // 回到M4

    // 畫左臉鬍鬚
    glPushMatrix(); // 存下M4
    glTranslatef(2.0, -0.6, 2.3);
    glPushMatrix(); // 存下M5

    glColor3fv(nose_color);
    gluCylinder(cylind, 0.12, 0.12, /* radius of top and bottom circle */
                2.0,                /* height of the cylinder */
                12,                 /* use 12-side polygon approximating circle*/
                3);                 /* Divide it into 3 sections */
    glPopMatrix();                  // 回到M5

    glPushMatrix(); // 存下M5
    glTranslatef(0.0, -0.4, 0.0);
    glRotatef(15, 1.0, 0.0, 0.0);
    glColor3fv(nose_color);
    gluCylinder(cylind, 0.12, 0.12, /* radius of top and bottom circle */
                2.0,                /* height of the cylinder */
                12,                 /* use 12-side polygon approximating circle*/
                3);                 /* Divide it into 3 sections */
    glPopMatrix();                  // 回到M5
    glPopMatrix();                  // 回到M4

    // 畫右臉鬍鬚
    glTranslatef(2.0, -0.6, -2.3);
    glRotatef(180, 0.0, 1.0, 0.0);
    glPushMatrix(); // 存下M6

    glColor3fv(nose_color);
    gluCylinder(cylind, 0.12, 0.12, /* radius of top and bottom circle */
                2.0,                /* height of the cylinder */
                12,                 /* use 12-side polygon approximating circle*/
                3);                 /* Divide it into 3 sections */
    glPopMatrix();                  // 回到M6

    glPushMatrix(); // 存下M6
    glTranslatef(0.0, -0.4, 0.0);
    glRotatef(15, 1.0, 0.0, 0.0);
    glColor3fv(nose_color);
    gluCylinder(cylind, 0.12, 0.12, /* radius of top and bottom circle */
                2.0,                /* height of the cylinder */
                12,                 /* use 12-side polygon approximating circle*/
                3);                 /* Divide it into 3 sections */

    glPopMatrix(); // 回到M6
    glPopMatrix(); // 回到M1
}

void draw_hand(int mode, float arm_ang, float forearm_ang, float hand_ang)
{

    glPushMatrix(); // 存下M1
    if (mode == LEFT)
    {
        glTranslatef(0.0, 1.5, 3.6);   // 移到M2
        glRotatef(-20, 1.0, 0.0, 0.0); // 手臂和身體分開一點
                                       //  draw_coord_sys();
    }
    else
    {
        glTranslatef(0.0, 1.5, -3.6); // 移到M2
        glRotatef(20, 1.0, 0.0, 0.0); // 手臂和身體分開一點
                                      //  draw_coord_sys();
    }
    glRotatef(arm_ang, 0.0, 0.0, 1.0); // 整隻手臂旋轉
    if (sayhi == TRUE && mode == RIGHT)
        glRotatef(tpang, 1.0, 0.0, 0.0); // 整隻手臂旋轉

    // 肩膀圓
    glPushMatrix(); // 存下M2
    // glScalef(1.0, 1.1, 1.0);
    // glColor3fv(nose_color);
    set_material(polished_silver);
    gluSphere(sphere, 0.8, 12, 12);
    glPopMatrix(); // 回到M2

    // 手臂
    glPushMatrix(); // 存下M2
    glRotatef(90, 1.0, 0.0, 0.0);
    // glColor3f(0.0, 0.0, 0.0);
    do_texture(1);
    gluCylinder(cylind, 0.7, 0.45, /* radius of top and bottom circle */
                1.6,               /* height of the cylinder */
                12,                /* use 12-side polygon approximating circle*/
                3);                /* Divide it into 3 sections */
    no_texture();
    set_material(obsidian);
    gluCylinder(cylind, 0.7, 0.45, /* radius of top and bottom circle */
                1.6,               /* height of the cylinder */
                12,                /* use 12-side polygon approximating circle*/
                3);                /* Divide it into 3 sections */
    glPopMatrix();                 // 回到M2

    /*--------------前臂-----------------*/
    glPushMatrix();               // 存下M2
    glTranslatef(0.0, -2.0, 0.0); // 移到M3
    if (sayhi == TRUE)
    {
        if (mode == RIGHT)
        {
            glRotatef(forearm_ang, 1.0, 0.0, 0.0); // 前臂旋轉
            glRotatef(90, 0.0, 1.0, 0.0);          // 前臂旋轉
        }
    }
    else
        glRotatef(forearm_ang, 1.0, 0.0, 0.0); // 前臂旋轉
    // glRotatef(90, 0.0, 1.0, 0.0);	//前臂旋轉
    // draw_coord_sys();

    // 前臂圓
    glPushMatrix(); // 存下M3
    // glColor3fv(nose_color);
    set_material(polished_silver);
    gluSphere(sphere, 0.65, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();          // 回到M3

    // 前臂
    glPushMatrix(); // 存下M3
    glRotatef(90, 1.0, 0.0, 0.0);
    do_texture(1);
    gluCylinder(cylind, 0.6, 0.4, /* radius of top and bottom circle */
                1.6,              /* height of the cylinder */
                12,               /* use 12-side polygon approximating circle*/
                3);               /* Divide it into 3 sections */
    no_texture();
    set_material(obsidian);
    gluCylinder(cylind, 0.6, 0.4, /* radius of top and bottom circle */
                1.6,              /* height of the cylinder */
                12,               /* use 12-side polygon approximating circle*/
                3);               /* Divide it into 3 sections */
    glPopMatrix();                // 回到M3

    /*--------------手掌-----------------*/
    glPushMatrix();               // 存下M3
    glTranslatef(0.0, -2.0, 0.0); // 移到M5
    // glRotatef(90, 0.0, 0.0,1.0);	//前臂旋轉

    // 手掌圓
    glPushMatrix(); // 存下M5
    glScalef(1.0, 1.4, 0.5);
    set_material(polished_silver);
    gluSphere(sphere, 0.76, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */

    glPopMatrix(); // 回到M5

    // 四手指
    glPushMatrix();
    glTranslatef(0.0, -0.55, 0.0);
    // draw_coord_sys();
    if (take_coffee == TRUE && mode == LEFT)
    {
        glTranslatef(0.0, -0.3, -0.55);
        glRotatef(75, 1.0, 0.0, 0.0);
    }
    else
        glRotatef(hand_ang, 1.0, 0.0, 0.0);

    glScalef(1.0, 1.2, 0.3);

    // glColor3f(0.0,0.0,0.0);
    gluSphere(sphere, 0.76, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();

    // 大拇指
    //  draw_coord_sys();
    glPushMatrix();
    if (mode == LEFT)
        glTranslatef(0.0, 0.3, -0.6); // 移到M8
    else
        glTranslatef(0.0, 0.3, 0.6); // 移到M8
    glRotatef(100, 1.0, 0.0, 0.0);
    // draw_coord_sys();
    glScalef(1.0, 0.8, 0.3);
    // glColor3fv(nose_color);
    gluSphere(sphere, 0.76, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();

    // 手拿水
    if (take_coffee == TRUE && mode == LEFT)
    {
        glPushMatrix();
        glTranslatef(-0.6, -0.2, -1.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        // 咖啡
        // glColor4f(0.58, 0.32, 0.16, 1.0);
        set_material(polished_bronze);
        glTranslatef(0.0, 0.0, 0.2);
        gluCylinder(cylind, 0.42, 0.80, /* radius of top and bottom circle */
                    1.8,                /* height of the cylinder */
                    12,                 /* use 12-side polygon approximating circle*/
                    3);                 /* Divide it into 3 sections */
        glTranslatef(0.0, 0.0, 1.8);
        // glColor4f(0.58, 0.32, 0.16, 1.0);
        gluDisk(mycircle,
                0.0, /* inner radius=0.0 */
                0.8, /* outer radius=10.0 */
                16,  /* 16-side polygon */
                3);  /* Divide it into 3 sections */
        // 杯子
        glDisable(GL_CULL_FACE);
        glTranslatef(0.0, 0.0, -2.0);
        set_material(glass);
        gluCylinder(cylind, 0.6, 1.0, /* radius of top and bottom circle */
                    2.7,              /* height of the cylinder */
                    12,               /* use 12-side polygon approximating circle*/
                    3);               /* Divide it into 3 sections */
        glPopMatrix();
        glEnable(GL_CULL_FACE);
    }

    glPopMatrix(); // 回到M3
    glPopMatrix(); // 回到M2
    glPopMatrix(); // 回到M1
}

void draw_feet(int mode, float thigh_ang, float calf_ang, float feet_ang)
{
    glPushMatrix(); // 存下M1
    if (mode == LEFT)
        glTranslatef(0.0, -2.8, 1.2); // 移到M6
    else
        glTranslatef(0.0, -2.8, -1.2);   // 移到M6
    glRotatef(thigh_ang, 0.0, 0.0, 1.0); // 整隻腳旋轉
    /*---------------------大腿----------------------*/
    // 大腿圓
    glPushMatrix(); // 存下M6
    // glColor3fv(nose_color);
    set_material(polished_silver);
    gluSphere(sphere, 1.2, /* radius=2.0 */
              12,          /* composing of 12 slices*/
              12);         /* composing of 12 stacks */
    glPopMatrix();         // 回到M6

    // 大腿
    glPushMatrix(); // 存下M6
    glRotatef(90, 1.0, 0.0, 0.0);
    // glColor3f(0.0, 0.0, 0.0);
    do_texture(1);
    if (cylind == NULL)
    {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    gluCylinder(cylind, 1.0, 0.7, /* radius of top and bottom circle */
                1.8,              /* height of the cylinder */
                12,               /* use 12-side polygon approximating circle*/
                3);               /* Divide it into 3 sections */
    no_texture();
    set_material(obsidian);
    gluCylinder(cylind, 1.0, 0.7, /* radius of top and bottom circle */
                1.8,              /* height of the cylinder */
                12,               /* use 12-side polygon approximating circle*/
                3);               /* Divide it into 3 sections */
    glPopMatrix();                // 回到M6

    /*---------------------小腿----------------------*/
    glPushMatrix();               // 存下M6
    glTranslatef(0.0, -2.0, 0.0); // 移到M7
    glRotatef(calf_ang, 0.0, 0.0, 1.0);

    // 小腿圓
    glPushMatrix(); // 存下M7
    // glColor3fv(nose_color);
    set_material(polished_silver);
    gluSphere(sphere, 0.7, /* radius=2.0 */
              12,          /* composing of 12 slices*/
              12);         /* composing of 12 stacks */
    glPopMatrix();         // 回到M6

    // 小腿
    glPushMatrix(); // 存下M7
    glRotatef(90, 1.0, 0.0, 0.0);
    // glColor3f(0.0, 0.0, 0.0);
    do_texture(1);
    gluCylinder(cylind, 0.65, 0.4, /* radius of top and bottom circle */
                1.8,               /* height of the cylinder */
                12,                /* use 12-side polygon approximating circle*/
                3);                /* Divide it into 3 sections */
    no_texture();
    set_material(obsidian);
    gluCylinder(cylind, 0.65, 0.4, /* radius of top and bottom circle */
                1.8,               /* height of the cylinder */
                12,                /* use 12-side polygon approximating circle*/
                3);                /* Divide it into 3 sections */
    glPopMatrix();                 // 回到M7

    /*---------------------腳掌----------------------*/
    glPushMatrix(); // 存下M7

    glRotatef(feet_ang, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -2.0, 0.0);
    // 腳掌圓
    glPushMatrix(); // 存下M8

    glScalef(1.0, 0.6, 1.0);
    // glColor3fv(nose_color);
    set_material(polished_silver);
    gluSphere(sphere, 1.0, /* radius=2.0 */
              12,          /* composing of 12 slices*/
              12);         /* composing of 12 stacks */

    glPopMatrix(); // 回到M8
    glPopMatrix(); // 回到M7
    glPopMatrix(); // 回到M6
    glPopMatrix(); // 回到M1
}

void draw_body()
{

    glPushMatrix(); // 存M1
    glScalef(0.9, 1.0, 1.0);

    set_material(polished_silver);
    do_texture(1);
    gluSphere(sphere, 3.77, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */

    // glDisable(GL_TEXTURE_2D);
    no_texture();

    set_material(obsidian);
    gluSphere(sphere, 3.77, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */

    glPopMatrix(); // 回到M1

    // glDisable(GL_ALPHA_TEST);
    // glDisable(GL_BLEND);

    draw_hand(LEFT, left_arm_ang, left_forearm_ang, left_hand_ang);
    draw_hand(RIGHT, right_arm_ang, right_forearm_ang, right_hand_ang);

    draw_feet(LEFT, left_thigh_ang, left_calf_ang, left_feet_ang);
    draw_feet(RIGHT, right_thigh_ang, right_calf_ang, right_feet_ang);
}

void draw_carpet()
{
    glScalef(40.0, 0.2, 20.0);
    do_texture(2);
    for (int i = 0; i < 6; i++)
    { /* draw the six faces one by one */
        glNormal3fv(normal[i]);
        glBegin(GL_POLYGON); /* Draw the face */
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[face[i][0]]);
        glTexCoord2f(2.0, 0.0);
        glVertex3fv(points[face[i][1]]);
        glTexCoord2f(2.0, 2.0);
        glVertex3fv(points[face[i][2]]);
        glTexCoord2f(0.0, 2.0);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
    no_texture();
}

void draw_table()
{
    glPushMatrix();
    glTranslatef(table_pos[0], table_pos[1] + table_thickness + table_height - 0.5, table_pos[2]);
    glPushMatrix();
    glTranslatef(-8, -table_thickness / 2 - table_height - 0.2, -3);
    draw_carpet();
    glPopMatrix();
    glPushMatrix();
    // glColor3f(1.00, 0.8, 0.5);
    set_material(white_plastic);
    glTranslatef(0, -table_thickness / 2, 0);
    // glScalef(table_length, table_thickness, table_width);
    // draw_cube();
    draw_wall(table_length, table_width, 1, 3);
    glPopMatrix();

    /*-------------------畫咖啡-------------------*/
    if (table_coffee == TRUE)
    {
        glPushMatrix();
        glTranslatef(coffee_pos[0], coffee_pos[1], coffee_pos[2]);
        glRotatef(-90, 1.0, 0.0, 0.0);
        // 咖啡
        // glColor4f(0.58, 0.32, 0.16, 1.0);
        set_material(polished_bronze);
        glTranslatef(0.0, 0.0, 0.2);
        gluCylinder(cylind, 0.42, 0.80, /* radius of top and bottom circle */
                    1.8,                /* height of the cylinder */
                    12,                 /* use 12-side polygon approximating circle*/
                    3);                 /* Divide it into 3 sections */
        glTranslatef(0.0, 0.0, 1.8);
        glColor4f(0.58, 0.32, 0.16, 1.0);
        gluDisk(mycircle,
                0.0, /* inner radius=0.0 */
                0.8, /* outer radius=10.0 */
                16,  /* 16-side polygon */
                3);  /* Divide it into 3 sections */
        // 杯子
        glTranslatef(0.0, 0.0, -2.0);
        // glColor4f(1.0, 1.0, 1.0, 0.35);
        glDisable(GL_CULL_FACE);
        set_material(glass);
        gluCylinder(cylind, 0.6, 1.0, /* radius of top and bottom circle */
                    2.7,              /* height of the cylinder */
                    12,               /* use 12-side polygon approximating circle*/
                    3);               /* Divide it into 3 sections */
        glEnable(GL_CULL_FACE);
        glPopMatrix();
    }

    /*----------------畫桌腳----------------------*/
    // glColor3f(1.00, 0.8, 0.5);
    set_material(white_plastic);
    for (int t = 0; t < 4; t++)
    {
        glPushMatrix();
        glTranslatef(table_feet_trans[t][0], -table_thickness / 2 - table_height, table_feet_trans[t][2]);
        // glScalef(table_feet_width, table_height, table_feet_width);
        // draw_cube();
        draw_wall(1, table_height, 1, 2);
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_view_volume()
{
    // glColor4f(1.0, 0.0, 0.0, 0.5); /* Set color */
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_POLYGON); // near clipping
    glVertex3f(-1.0 * zoom_fac, -1.0 * zoom_fac * (float)height / width, -1.0);
    glVertex3f(-1.0 * zoom_fac, 1.0 * zoom_fac * (float)height / width, -1.0);
    glVertex3f(1.0 * zoom_fac, 1.0 * zoom_fac * (float)height / width, -1.0);
    glVertex3f(1.0 * zoom_fac, -1.0 * zoom_fac * (float)height / width, -1.0);
    glEnd();

    glScalef(65, 65, 65);
    glBegin(GL_POLYGON); //(0,(l,b,f),(l,t,f))
    glVertex3f(0, 0, 0);
    glVertex3f(-1.0 * zoom_fac, -1.0 * zoom_fac * (float)height / width, -1.0);
    glVertex3f(-1.0 * zoom_fac, 1.0 * zoom_fac * (float)height / width, -1.0);
    glEnd();
    glBegin(GL_POLYGON); //(0,(l,t,f),(r,t,f))
    glVertex3f(0, 0, 0);
    glVertex3f(-1.0 * zoom_fac, 1.0 * zoom_fac * (float)height / width, -1.0);
    glVertex3f(1.0 * zoom_fac, 1.0 * zoom_fac * (float)height / width, -1.0);
    glEnd();
    glBegin(GL_POLYGON); //(0,(r,b,f),(r,t,f))
    glVertex3f(0, 0, 0);
    glVertex3f(1.0 * zoom_fac, -1.0 * zoom_fac * (float)height / width, -1.0);
    glVertex3f(1.0 * zoom_fac, 1.0 * zoom_fac * (float)height / width, -1.0);
    glEnd();
    glBegin(GL_POLYGON); //(0,(r,b,f),(l,b,f))
    glVertex3f(0, 0, 0);
    glVertex3f(1.0 * zoom_fac, -1.0 * zoom_fac * (float)height / width, -1.0);
    glVertex3f(-1.0 * zoom_fac, -1.0 * zoom_fac * (float)height / width, -1.0);
    glEnd();
}

void draw_scene()
{
    draw_coord_sys();

    glDisable(GL_CULL_FACE);
    glPushMatrix();
    set_material(billboard);
    glColor3f(1.0,0.0,0.0);
    glTranslatef(-110,-50,-110);
    glScalef(220,220,220);
    draw_skybox();
    glPopMatrix();
    glEnable(GL_CULL_FACE);

    do_texture(4);
    draw_billboard(bird_pos_x, bird_pos_y, 51, 16, 24);
    no_texture();

    do_texture(5);
    draw_billboard(12, 0, 75, 40, 60);
    draw_billboard(25, 0, 45, 20, 30);
    draw_billboard(-30, 0, 40, 26, 39);
    no_texture();

    do_texture(6);
    draw_billboard(-18, 0, 60, 32, 48);
    draw_billboard(0, 0, 40, 16, 24);
    draw_billboard(40, 0, 48, 20, 30);
    no_texture();

    // glDisable(GL_LIGHTING);
    // 室內電燈
    glPushMatrix();
    glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
    if (light1_on)
    {
        glMaterialfv(GL_FRONT, GL_EMISSION, lit1_emission);
        gluSphere(sphere, 2, /* radius=0.5 */
                  12,        /* composing of 12 slices*/
                  12);       /* composing of 12 stacks */
    }

    glPopMatrix();
    // glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT1, GL_POSITION, lit1_position);

    glMaterialfv(GL_FRONT, GL_EMISSION, lit1_close_emission);

    // 眼睛
    glPushMatrix();
    glTranslatef(-32.5, 0, -32.5);
    glTranslatef(robot_pos[0], robot_pos[1] + 7.4, robot_pos[2] + 0.0); // M1
    glRotatef(rot_ang[1], 0.0, 1.0, 0.0);
    glRotatef(rot_ang[2], 0.0, 0.0, 1.0); // 轉圈
    glPushMatrix();
    glTranslatef(0.0, 4.2, 0.0);
    glRotatef(head_ang, 0.0, 0.0, 1.0); // 頭旋轉
    glTranslatef(eyes_pos[0], eyes_pos[1], eyes_pos[2]);
    glPushMatrix(); // 存下M4
    glScalef(0.3, 1.0, 1.0);
    // glColor3fv(eyes_color);
    if (light2_on)
        glMaterialfv(GL_FRONT, GL_EMISSION, lit2_emission);
    else
        glMaterialfv(GL_FRONT, GL_EMISSION, lit1_close_emission);
    set_material(brass);
    gluSphere(sphere, 0.87, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();          // 回到M4

    glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lit2_direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    glTranslatef(0, 0, -2 * eyes_pos[2]);
    glPushMatrix(); // 存下M4
    glScalef(0.3, 1.0, 1.0);
    // glColor3fv(eyes_color);
    if (light3_on)
        glMaterialfv(GL_FRONT, GL_EMISSION, lit2_emission);
    else
        glMaterialfv(GL_FRONT, GL_EMISSION, lit1_close_emission);
    set_material(brass);
    gluSphere(sphere, 0.87, /* radius=2.0 */
              12,           /* composing of 12 slices*/
              12);          /* composing of 12 stacks */
    glPopMatrix();          // 回到M4
    glLightfv(GL_LIGHT3, GL_POSITION, lit2_position);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lit2_direction);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, lit2_cutoff);
    glMaterialfv(GL_FRONT, GL_EMISSION, lit1_close_emission);
    glPopMatrix();
    glPopMatrix();
    // glDisable(GL_LIGHTING);

    // 燈罩
    glDisable(GL_CULL_FACE);
    if (!light1_on)
    {
        glPushMatrix();
        glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
        glMaterialfv(GL_FRONT, GL_EMISSION, lit1_close_emission);
        set_material(perl);
        gluSphere(sphere, 2, /* radius=0.5 */
                  12,        /* composing of 12 slices*/
                  12);       /* composing of 12 stacks */
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(lit1_position[0], lit1_position[1] + 3.2, lit1_position[2]);
    glRotatef(90, 1.0, 0, 0);
    set_material(perl);
    gluCylinder(cylind, 1, 6, /* radius of top and bottom circle */
                4.8,          /* height of the cylinder */
                12,           /* use 12-side polygon approximating circle*/
                4);           /* Divide it into 3 sections */
    glPopMatrix();
    glPushMatrix();
    glTranslatef(lit1_position[0], 40, lit1_position[2]);
    glRotatef(90, 1.0, 0, 0);
    set_material(perl);
    gluCylinder(cylind, 1, 1, /* radius of top and bottom circle */
                1.8,          /* height of the cylinder */
                12,           /* use 12-side polygon approximating circle*/
                4);           /* Divide it into 3 sections */
    glPopMatrix();
    glEnable(GL_CULL_FACE);

    if (sun_time > 18 || sun_time < 5)
    {

        for (int i = 0; i < 7; i++)
        {
            int scf = rand() % 3 + 1;
            float sz = 0.2 * scf;
            int pos_x = rand() % 65;
            int pos_y = rand() % 10 + 45;
            int intensity = rand() % 5 + 5;
            float temp_emission[4];
            for (int j = 0; j < 3; j++)
                temp_emission[i] = star_emission[i] * intensity / 10.0;
            glPushMatrix();
            glTranslatef(star_pos[i][0], star_pos[i][1], star_pos[i][2]);
            glMaterialfv(GL_FRONT, GL_EMISSION, star_emission);
            gluSphere(sphere, sz + 0.25, /* radius=0.5 */
                      12,                /* composing of 12 slices*/
                      12);               /* composing of 12 stacks */
            glMaterialfv(GL_FRONT, GL_EMISSION, lit1_close_emission);
            glPopMatrix();
        }
    }

    // 開始
    glPushMatrix();
    glColor3f(1.0, 0, 0);
    glTranslatef(-32.5, 0, -32.5);
    glPushMatrix();
    glColor3f(0.8, 0.2, 0.2);
    glTranslatef(robot_pos[0], robot_pos[1] + 7.4, robot_pos[2] + 0.0); // M1
    glRotatef(rot_ang[1], 0.0, 1.0, 0.0);                               // 轉圈
    glPushMatrix();
    glRotatef(rot_ang[2], 0.0, 0.0, 1.0); // 轉圈
    draw_body();
    draw_head();
    glPopMatrix();
    draw_table();
    draw_floor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glPushMatrix();
    GLdouble t_matrix[16] = {0.0};
    t_matrix[0] = u[0][0];
    t_matrix[1] = u[0][1];
    t_matrix[2] = u[0][2];
    t_matrix[4] = u[1][0];
    t_matrix[5] = u[1][1];
    t_matrix[6] = u[1][2];
    t_matrix[8] = u[2][0];
    t_matrix[9] = u[2][1];
    t_matrix[10] = u[2][2];
    t_matrix[15] = 1.0;
    glMultMatrixd(t_matrix);
    if (view_volume)
    {
        glDisable(GL_LIGHTING);
        glColor4f(0.0, 0.0, 0.0, 0.5); /* Set color */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        draw_view_volume();
        glColor4f(0.6, 0.6, 0.6, 0.3); /* Set color */
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        draw_view_volume();
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();
    glPopMatrix();
}

void make_view(int x)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // glPushMatrix();
    // glLoadIdentity();
    // /*----position light1 (fixed) in eye coord sys ---*/
    // glLightfv(GL_LIGHT1, GL_POSITION, lit1_position); /*fixed position in eye space---*/
    // glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
    // glDisable(GL_LIGHTING);
    // glDisable(GL_CULL_FACE);
    // glColor3f(0.80, 0.80, 0.0);
    // gluSphere(sphere, 3, /* radius=0.5 */
    //           12,          /* composing of 12 slices*/
    //           12);         /* composing of 12 stacks */
    // glPopMatrix();
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_LIGHTING);
    switch (x)
    {
    case 4: /* Perspective */

        /* In this sample program, eye position and Xe, Ye, Ze are computed
           by ourselves. Therefore, use them directly; no trabsform is
           applied upon eye coordinate system
           */
        gluLookAt(eye[0], eye[1], eye[2],
                  eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
                  u[1][0], u[1][1], u[1][2]);
        break;

    case 1: /* X direction parallel viewing */
        gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    case 2: /* Y direction parallel viewing */
        gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        break;
    case 3:
        gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, vmtx);
    double len;
    len = sqrt(vmtx[2] * vmtx[2] + vmtx[10] * vmtx[10]);
    if (len == 0)
    {
        billboard_a[0] = 0;
        billboard_a[1] = 0;
        billboard_a[2] = 0;
    }
    else
    {
        billboard_a[0] = vmtx[10] / len;
        billboard_a[1] = 0;
        billboard_a[2] = -vmtx[2] / len;
    }
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 4)
    {
        // gluPerspective(90.0, (double)width / (double)height, 1.5, 50.0);
        if (width > height)
            glFrustum(-1.0 * zoom_fac, 1.0 * zoom_fac, -1.0 * zoom_fac * (float)height / width, 1.0 * zoom_fac * (float)height / width,
                      1.0, 200.0);
        else
            glFrustum(-1.0 * zoom_fac * (float)width / height, 1.0 * zoom_fac * (float)width / height, -1.0 * zoom_fac, 1.0 * zoom_fac,
                      1.0, 200.0);
    }
    else
    {
        if (x == 1 || x == 3)
        {
            if (width > height)
                glOrtho(-40.0 * zoom_fac, 40.0 * zoom_fac, -20.0 * zoom_fac * (float)height / (float)width,
                        60.0 * zoom_fac * (float)height / (float)width,
                        -0.0, 200.0);
            else
                glOrtho(-40 * zoom_fac * (float)width / (float)height,
                        40 * zoom_fac * (float)width / (float)height, -20 * zoom_fac, 60 * zoom_fac,
                        -0.0, 200.0);
        }
        else
        {
            if (width > height)
                glOrtho(zoom_fac * (-40.0), zoom_fac * 40.0, -40.0 * zoom_fac * (float)height / (float)width,
                        40.0 * zoom_fac * (float)height / (float)width,
                        -0.0, 200.0);
            else
                glOrtho(-40 * zoom_fac * (float)width / (float)height,
                        40 * zoom_fac * (float)width / (float)height, -40 * zoom_fac, 40 * zoom_fac,
                        -0.0, 200.0);
        }
    }
    glMatrixMode(GL_MODELVIEW);
}

/*---------------------------------------------------------
 * Procedure to draw view volume, eye position, focus ,...
 * for perspective projection
 */
void draw_view()
{
    int i;

    glMatrixMode(GL_MODELVIEW);

    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    glPopMatrix();

    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 1.0, 0.0); /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();

    glColor3f(1.0, 0.0, 1.0); /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 1.0, 1.0); /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);
    glEnd();
}

void display()
{
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    switch (view_mode)
    {
    case 0:
        make_view(4);
        make_projection(4);
        glViewport(width / 2, 0, width / 2, height / 2);
        draw_scene();

        make_view(1);
        make_projection(1);
        glViewport(0, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(1);
        draw_view();

        make_view(2);
        make_projection(2);
        glViewport(width / 2, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(2);
        draw_view();

        make_view(3);
        make_projection(3);
        glViewport(0, 0, width / 2, height / 2);
        draw_scene();
        make_view(3);
        draw_view();
        break;
    case 1:
        glViewport(0, 0, width, height);
        make_view(1);
        make_projection(1);
        draw_scene();
        make_view(1);
        draw_view();
        break;
    case 2:
        glViewport(0, 0, width, height);
        make_view(2);
        make_projection(2);
        draw_scene();
        make_view(2);
        draw_view();
        break;
    case 3:
        glViewport(0, 0, width, height);
        make_view(3);
        make_projection(3);
        draw_scene();
        make_view(3);
        draw_view();
        break;

    case 4:
        glViewport(0, 0, width, height);
        make_view(4);
        make_projection(4);
        draw_scene();
        make_view(4);
        draw_view();
        break;
    }
    // draw_scene();
    /*-------Swap the back buffer to the front --------*/
    glutSwapBuffers();
    return;
}

int check(float x, float z)
{
    float cos_ = cos(rot_ang[1] * PI / 180);
    float sin_ = -sin(rot_ang[1] * PI / 180);
    float l_x = x + (-robot_range), r_x = x + robot_range;
    float front_z = z + (-robot_range), back_z = z + robot_range;
    float check_step = FALSE;
    if (step_on_coffee == FALSE)
    {
        // 左前
        if (coffee_onfloor_info[3] - 3 >= l_x && coffee_onfloor_info[3] - 3 <= r_x && coffee_onfloor_info[5] - 3 >= front_z && coffee_onfloor_info[5] - 3 <= back_z)
            check_step = TRUE;
        // 右前
        if (coffee_onfloor_info[3] + 3 >= l_x && coffee_onfloor_info[3] + 3 <= r_x && coffee_onfloor_info[5] - 3 >= front_z && coffee_onfloor_info[5] - 3 <= back_z)
            check_step = TRUE;
        // 左後
        if (coffee_onfloor_info[3] - 3 >= l_x && coffee_onfloor_info[3] - 3 <= r_x && coffee_onfloor_info[5] + 3 >= front_z && coffee_onfloor_info[5] + 3 <= back_z)
            check_step = TRUE;
        // 右後
        if (coffee_onfloor_info[3] + 3 >= l_x && coffee_onfloor_info[3] + 3 <= r_x && coffee_onfloor_info[5] + 3 >= front_z && coffee_onfloor_info[5] + 3 <= back_z)
            check_step = TRUE;
    }
    else
    {
        step_cnt = (step_cnt + 1) % TOTAL_STEP_MOD;
        if (step_cnt % STEP_MOD == 0)
        {
            idx_cnt++;
            step_on_coffee_info[idx_cnt % 6][0] = cos_;
            step_on_coffee_info[idx_cnt % 6][1] = sin_;
            step_on_coffee_info[idx_cnt % 6][2] = rot_ang[1];
            step_on_coffee_info[idx_cnt % 6][3] = robot_pos[0] - 2.5 * cos_;
            step_on_coffee_info[idx_cnt % 6][4] = 0.1;
            if (idx_cnt % 6 % 2 == 1)
                step_on_coffee_info[idx_cnt % 6][5] = robot_pos[2] - 2.5 * sin_ + 2 * cos_;
            else
                step_on_coffee_info[idx_cnt % 6][5] = robot_pos[2] - 2.5 * sin_ - 2 * cos_;
        }
    }
    if (check_step == TRUE)
        step_on_coffee = TRUE;

    // 左前
    if (l_x >= table_pos[0] && l_x <= table_pos[0] + table_length && front_z >= table_pos[2] && front_z <= table_pos[2] + table_width)
        return 0;
    // 右前
    if (r_x >= table_pos[0] && r_x <= table_pos[0] + table_length && front_z >= table_pos[2] && front_z <= table_pos[2] + table_width)
        return 0;
    // 左後
    if (l_x >= table_pos[0] && l_x <= table_pos[0] + table_length && back_z >= table_pos[2] && back_z <= table_pos[2] + table_width)
        return 0;
    // 右後
    if (r_x >= table_pos[0] && r_x <= table_pos[0] + table_length && back_z >= table_pos[2] && back_z <= table_pos[2] + table_width)
        return 0;
    if (l_x <= 0 || r_x > 65 + 1 || front_z <= 0 || back_z > 65 + 1)
        return 0;

    return 1;
}

void walk()
{
    walk_cnt += 1.0;
    float nxt_x = robot_pos[0] + (15.0 / 48) * cos(rot_ang[1] * PI / 180);
    float nxt_z = robot_pos[2] + (15.0 / 48) * (-sin(rot_ang[1] * PI / 180));
    if (check(nxt_x, nxt_z))
    {
        robot_pos[0] = nxt_x;
        robot_pos[2] = nxt_z;
    }
    for (int i = 1; i < 7; i++)
    {
        if (walk_cnt <= walk_state_time[i + 1])
        {
            left_thigh_ang = walk_state_ang[i][0] + (walk_state_ang[i + 1][0] - walk_state_ang[i][0]) * (walk_cnt - walk_state_time[i]) / (walk_state_time[i + 1] - walk_state_time[i]);
            left_calf_ang = walk_state_ang[i][1] + (walk_state_ang[i + 1][1] - walk_state_ang[i][1]) * (walk_cnt - walk_state_time[i]) / (walk_state_time[i + 1] - walk_state_time[i]);
            right_thigh_ang = walk_state_ang[i][2] + (walk_state_ang[i + 1][2] - walk_state_ang[i][2]) * (walk_cnt - walk_state_time[i]) / (walk_state_time[i + 1] - walk_state_time[i]);
            right_calf_ang = walk_state_ang[i][3] + (walk_state_ang[i + 1][3] - walk_state_ang[i][3]) * (walk_cnt - walk_state_time[i]) / (walk_state_time[i + 1] - walk_state_time[i]);
            if (i + 1 <= 3)
            {
                left_arm_ang = walk_hand_state_ang[0][0] + (walk_hand_state_ang[1][0] - walk_hand_state_ang[0][0]) * (walk_cnt - walk_state_time[1]) / (walk_state_time[3] - walk_state_time[1]);
                left_forearm_ang = walk_hand_state_ang[0][1] + (walk_hand_state_ang[1][1] - walk_hand_state_ang[0][1]) * (walk_cnt - walk_state_time[1]) / (walk_state_time[3] - walk_state_time[1]);
                right_arm_ang = walk_hand_state_ang[0][2] + (walk_hand_state_ang[1][2] - walk_hand_state_ang[0][2]) * (walk_cnt - walk_state_time[1]) / (walk_state_time[3] - walk_state_time[1]);
                right_forearm_ang = walk_hand_state_ang[0][3] + (walk_hand_state_ang[1][3] - walk_hand_state_ang[0][3]) * (walk_cnt - walk_state_time[1]) / (walk_state_time[3] - walk_state_time[1]);
            }
            else if (i + 1 <= 4)
            {
                left_arm_ang = walk_hand_state_ang[1][0] + (walk_hand_state_ang[2][0] - walk_hand_state_ang[1][0]) * (walk_cnt - walk_state_time[3]) / (walk_state_time[4] - walk_state_time[3]);
                left_forearm_ang = walk_hand_state_ang[1][1] + (walk_hand_state_ang[2][1] - walk_hand_state_ang[1][1]) * (walk_cnt - walk_state_time[3]) / (walk_state_time[4] - walk_state_time[3]);
                right_arm_ang = walk_hand_state_ang[1][2] + (walk_hand_state_ang[2][2] - walk_hand_state_ang[1][2]) * (walk_cnt - walk_state_time[3]) / (walk_state_time[4] - walk_state_time[3]);
                right_forearm_ang = walk_hand_state_ang[1][3] + (walk_hand_state_ang[2][3] - walk_hand_state_ang[1][3]) * (walk_cnt - walk_state_time[3]) / (walk_state_time[4] - walk_state_time[3]);
            }
            else if (i + 1 <= 6)
            {
                left_arm_ang = walk_hand_state_ang[2][0] + (walk_hand_state_ang[3][0] - walk_hand_state_ang[2][0]) * (walk_cnt - walk_state_time[4]) / (walk_state_time[6] - walk_state_time[4]);
                left_forearm_ang = walk_hand_state_ang[2][1] + (walk_hand_state_ang[3][1] - walk_hand_state_ang[2][1]) * (walk_cnt - walk_state_time[4]) / (walk_state_time[6] - walk_state_time[4]);
                right_arm_ang = walk_hand_state_ang[2][2] + (walk_hand_state_ang[3][2] - walk_hand_state_ang[2][2]) * (walk_cnt - walk_state_time[4]) / (walk_state_time[6] - walk_state_time[4]);
                right_forearm_ang = walk_hand_state_ang[2][3] + (walk_hand_state_ang[3][3] - walk_hand_state_ang[2][3]) * (walk_cnt - walk_state_time[4]) / (walk_state_time[6] - walk_state_time[4]);
            }
            else
            {
                left_arm_ang = walk_hand_state_ang[3][0] + (walk_hand_state_ang[0][0] - walk_hand_state_ang[3][0]) * (walk_cnt - walk_state_time[6]) / (walk_state_time[7] - walk_state_time[6]);
                left_forearm_ang = walk_hand_state_ang[3][1] + (walk_hand_state_ang[0][1] - walk_hand_state_ang[3][1]) * (walk_cnt - walk_state_time[6]) / (walk_state_time[7] - walk_state_time[6]);
                right_arm_ang = walk_hand_state_ang[3][2] + (walk_hand_state_ang[0][2] - walk_hand_state_ang[3][2]) * (walk_cnt - walk_state_time[6]) / (walk_state_time[7] - walk_state_time[6]);
                right_forearm_ang = walk_hand_state_ang[3][3] + (walk_hand_state_ang[0][3] - walk_hand_state_ang[3][3]) * (walk_cnt - walk_state_time[6]) / (walk_state_time[7] - walk_state_time[6]);
            }
            if (take_coffee == TRUE)
            {
                left_arm_ang = 90;
                left_forearm_ang = 0;
            }
            return;
        }
    }
    walk_cnt = 3;
    left_thigh_ang = walk_state_ang[1][0];
    left_calf_ang = walk_state_ang[1][1];
    right_thigh_ang = walk_state_ang[1][2];
    right_calf_ang = walk_state_ang[1][3];
}

void walk_end()
{
    float dif_foot[4] = {0 - left_thigh_ang, 0 - left_calf_ang, 0 - right_thigh_ang, 0 - right_calf_ang};
    float dif_hand[4] = {0 - left_arm_ang, 0 - left_forearm_ang, 0 - right_arm_ang, 0 - right_forearm_ang};
    int div = 1;
    for (int i = 0; i < 4; i++)
    {
        dif_foot[i] /= div;
        dif_hand[i] /= div;
    }
    for (int i = 0; i < div; i++)
    {
        left_thigh_ang += dif_foot[0];
        left_calf_ang += dif_foot[1];
        right_thigh_ang += dif_foot[2];
        right_calf_ang += dif_foot[3];
        if (take_coffee == TRUE)
        {
            left_arm_ang = 90;
            left_forearm_ang = 0;
        }
        else
        {
            left_arm_ang += dif_hand[0];
            left_forearm_ang += dif_hand[1];
        }
        right_arm_ang += dif_hand[2];
        right_forearm_ang += dif_hand[3];

        display();
    }
    walk_cnt = 3;
    turn_cnt = 0;
}

void turn_around(float dif_ang)
{
    turn_cnt += 1.0;
    rot_ang[1] += dif_ang;
    for (int i = 0; i < 4; i++)
    {
        if (turn_cnt <= turn_feet_state_time[i + 1])
        {
            left_thigh_ang = turn_feet_state_ang[i][0] + (turn_feet_state_ang[i + 1][0] - turn_feet_state_ang[i][0]) * (turn_cnt - turn_feet_state_time[i]) / (turn_feet_state_time[i + 1] - turn_feet_state_time[i]);
            left_calf_ang = turn_feet_state_ang[i][1] + (turn_feet_state_ang[i + 1][1] - turn_feet_state_ang[i][1]) * (turn_cnt - turn_feet_state_time[i]) / (turn_feet_state_time[i + 1] - turn_feet_state_time[i]);
            right_thigh_ang = turn_feet_state_ang[i][2] + (turn_feet_state_ang[i + 1][2] - turn_feet_state_ang[i][2]) * (turn_cnt - turn_feet_state_time[i]) / (turn_feet_state_time[i + 1] - turn_feet_state_time[i]);
            right_calf_ang = turn_feet_state_ang[i][3] + (turn_feet_state_ang[i + 1][3] - turn_feet_state_ang[i][3]) * (turn_cnt - turn_feet_state_time[i]) / (turn_feet_state_time[i + 1] - turn_feet_state_time[i]);
            return;
        }
    }
    turn_cnt = 0.0;
    left_thigh_ang = turn_feet_state_ang[0][0];
    left_calf_ang = turn_feet_state_ang[0][1];
    right_thigh_ang = turn_feet_state_ang[0][2];
    right_calf_ang = turn_feet_state_ang[0][3];
}

void jump()
{
    for (int i = 0; i < 7; i++)
    {
        if (jump_cnt <= jump_feet_state_time[i + 1])
        {
            rot_ang[2] = jump_feet_state_ang[i][0] + (jump_feet_state_ang[i + 1][0] - jump_feet_state_ang[i][0]) * (jump_cnt - jump_feet_state_time[i]) / (jump_feet_state_time[i + 1] - jump_feet_state_time[i]);
            if (i == 2)
            {
                float t = jump_time_slice * (jump_cnt - jump_feet_state_time[i]);
                robot_pos[1] = 14.7 * t - 0.5 * 9.8 * t * t;
            }
            else if (i == 3)
            {
                float t = jump_time_slice * (jump_cnt - jump_feet_state_time[i]);
                robot_pos[1] = 11.025 - 0.5 * 9.8 * t * t;
            }
            else
                robot_pos[1] = jump_feet_state_ang[i][1] + (jump_feet_state_ang[i + 1][1] - jump_feet_state_ang[i][1]) * (jump_cnt - jump_feet_state_time[i]) / (jump_feet_state_time[i + 1] - jump_feet_state_time[i]);
            left_thigh_ang = jump_feet_state_ang[i][2] + (jump_feet_state_ang[i + 1][2] - jump_feet_state_ang[i][2]) * (jump_cnt - jump_feet_state_time[i]) / (jump_feet_state_time[i + 1] - jump_feet_state_time[i]);
            right_thigh_ang = left_thigh_ang;
            left_calf_ang = jump_feet_state_ang[i][3] + (jump_feet_state_ang[i + 1][3] - jump_feet_state_ang[i][3]) * (jump_cnt - jump_feet_state_time[i]) / (jump_feet_state_time[i + 1] - jump_feet_state_time[i]);
            right_calf_ang = left_calf_ang;
            left_feet_ang = jump_feet_state_ang[i][4] + (jump_feet_state_ang[i + 1][4] - jump_feet_state_ang[i][4]) * (jump_cnt - jump_feet_state_time[i]) / (jump_feet_state_time[i + 1] - jump_feet_state_time[i]);
            right_feet_ang = left_feet_ang;
            jump_cnt += 1.0;
            return;
        }
    }
    jump_cnt = 0.0;
    rot_ang[2] = jump_feet_state_ang[0][0];
    robot_pos[1] = jump_feet_state_ang[0][1];
    left_thigh_ang = jump_feet_state_ang[0][2];
    right_thigh_ang = left_thigh_ang;
    left_calf_ang = jump_feet_state_ang[0][3];
    right_calf_ang = left_calf_ang;
    left_feet_ang = jump_feet_state_ang[0][4];
    right_feet_ang = left_feet_ang;
}

void say_hi()
{
    for (int i = 0; i < 7; i++)
    {
        if (sayhi_cnt <= sayhi_state_time[i + 1])
        {
            right_arm_ang = sayhi_state_ang[i][0] + (sayhi_state_ang[i + 1][0] - sayhi_state_ang[i][0]) * (sayhi_cnt - sayhi_state_time[i]) / (sayhi_state_time[i + 1] - sayhi_state_time[i]);
            right_forearm_ang = sayhi_state_ang[i][1] + (sayhi_state_ang[i + 1][1] - sayhi_state_ang[i][1]) * (sayhi_cnt - sayhi_state_time[i]) / (sayhi_state_time[i + 1] - sayhi_state_time[i]);
            tpang = sayhi_state_ang[i][2] + (sayhi_state_ang[i + 1][2] - sayhi_state_ang[i][2]) * (sayhi_cnt - sayhi_state_time[i]) / (sayhi_state_time[i + 1] - sayhi_state_time[i]);
            ;
            sayhi_cnt += 1.0;
            return;
        }
    }
    sayhi_cnt = 0.0;
    right_arm_ang = 0;
    right_forearm_ang = 0;
    tpang = 0;
}

void my_reshape(int w, int h)
{
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (double)w / (double)h, 1.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void check_coffee()
{
    float dis = sqrt((coffee_pos[0] + table_pos[0] - robot_pos[0]) * (coffee_pos[0] + table_pos[0] - robot_pos[0]) + (coffee_pos[2] + table_pos[2] - robot_pos[2]) * (coffee_pos[2] + table_pos[2] - robot_pos[2]));
    if (dis <= 12)
    {
        table_coffee = FALSE;
        take_coffee = TRUE;
        left_arm_ang = 90;
    }
}

void my_keyboard(unsigned char key, int x_pos, int y_pos)
{
    float x[3], y[3], z[3];
    int i;
    if (key == 'r')
    {
        wallpaper_mode = !wallpaper_mode;
    }
    if(key=='m')
    {
        if(fog_density<1.0)
            fog_density+=0.01;
        if(fog_dis<150)
            fog_dis+=2;
        glFogf(GL_FOG_DENSITY, fog_density);
        glFogf(GL_FOG_END, fog_dis);
    }
    if(key == 'M')
    {
        if(fog_density>0.0)
            fog_density-=0.01;
        if(fog_dis>0)
            fog_dis-=2;
        glFogf(GL_FOG_DENSITY, fog_density);
        glFogf(GL_FOG_END, fog_dis);
    }
    if(key == ',')
    {
        fog_color=(fog_color + 1) % 7;
        glFogfv(GL_FOG_COLOR, light_color[fog_color]);
    }
    if(key == '.')
    {
        fog_mode=(fog_mode+1)%3;
        if(fog_mode==0)
            glFogi(GL_FOG_MODE, GL_LINEAR);
        else if(fog_mode == 1)
            glFogi(GL_FOG_MODE, GL_EXP);
        else
            glFogi(GL_FOG_MODE, GL_EXP2);
    }
    if(key == '/')
    {
        fog_on=!fog_on;
        if(fog_on)
            glEnable(GL_FOG);
        else
            glDisable(GL_FOG);
    }
    if (key == ' ')
    {
        sun_time += 1;
        if (sun_time == 24)
            sun_time = 0;
        if (sun_time >= 18)
            glClearColor(background_color[1][0], background_color[1][1], background_color[1][2], 1.0);
        else
            glClearColor(0.05 + background_color[0][0] / 6 * (sun_time <= 12 ? sun_time - 6 : 18 - sun_time), 0.05 + background_color[0][1] / 6 * (sun_time <= 12 ? sun_time - 6 : 18 - sun_time), 0.05 + background_color[0][2] / 6 * (sun_time <= 12 ? sun_time - 6 : 18 - sun_time), 1.0);
        if (sun_time == 6)
            sun_ang = 0;
        else if (sun_time > 6 && sun_time < 18)
            sun_ang += 15;
        lit_direction[1] = 100 * sin(sun_ang * PI / 180.0);
        lit_direction[2] = 100 * cos(sun_ang * PI / 180.0);
        if (sun_time >= 18 || sun_time < 6)
            glDisable(GL_LIGHT0);
        else
        {
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_POSITION, lit_direction);
        }
    }
    if (key == 'q')
        head_ang += (head_ang < 45 ? 1.0 : 0.0);
    if (key == 'Q')
        head_ang -= (head_ang > -30 ? 1.0 : 0);
    if (key == 'e')
        lit2_cutoff += (lit2_cutoff < 90 ? 5.0 : 0.0);
    if (key == 'E')
        lit2_cutoff += (lit2_cutoff > 15 ? -5.0 : 0.0);
    // lit顏色
    if (key == 'z')
    {
        lit_color = (lit_color + 1) % 7;
        for (int i = 0; i < 3; i++)
            lit_diffuse[i] = light_color[lit_color][i] * lit_intensity;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    }
    // lit的強度
    if (key == 'x')
    {
        lit_intensity += (lit_intensity < 1.0 ? 0.1 : 0.0);
        for (int i = 0; i < 3; i++)
            lit_diffuse[i] = light_color[lit_color][i] * lit_intensity;
        for (int i = 0; i < 3; i++)
            lit_temp_specular[i] = lit_specular[i] * lit_intensity;
        glLightfv(GL_LIGHT0, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    }
    if (key == 'X')
    {
        lit_intensity += (lit_intensity > 0 ? -0.1 : 0.0);
        for (int i = 0; i < 3; i++)
            lit_diffuse[i] = light_color[lit_color][i] * lit_intensity;
        for (int i = 0; i < 3; i++)
            lit_temp_specular[i] = lit_specular[i] * lit_intensity;
        glLightfv(GL_LIGHT0, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    }
    // lit1的顏色
    if (key == 'c')
    {
        lit1_color = (lit1_color + 1) % 7;
        for (int i = 0; i < 3; i++)
            lit1_diffuse[i] = light_color[lit1_color][i] * lit1_intensity;
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    }
    // lit1的強度
    if (key == 'v')
    {
        lit1_intensity += (lit1_intensity < 1.0 ? 0.1 : 0.0);
        for (int i = 0; i < 3; i++)
            lit1_diffuse[i] = light_color[lit1_color][i] * lit1_intensity;
        for (int i = 0; i < 3; i++)
            lit_temp_specular[i] = lit1_specular[i] * lit1_intensity;
        glLightfv(GL_LIGHT1, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    }
    if (key == 'V')
    {
        lit1_intensity += (lit1_intensity > 0 ? -0.1 : 0.0);
        for (int i = 0; i < 3; i++)
            lit1_diffuse[i] = light_color[lit1_color][i] * lit1_intensity;
        for (int i = 0; i < 3; i++)
            lit_temp_specular[i] = lit1_specular[i] * lit1_intensity;
        glLightfv(GL_LIGHT1, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    }
    // lit2的顏色
    if (key == 'b')
    {
        lit2_color = (lit2_color + 1) % 7;
        for (int i = 0; i < 3; i++)
            lit2_diffuse[i] = light_color[lit2_color][i] * lit2_intensity;
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lit2_diffuse);
    }
    // lit2的強度
    if (key == 'n')
    {
        lit2_intensity += (lit2_intensity < 1.0 ? 0.1 : 0.0);
        for (int i = 0; i < 3; i++)
            lit2_diffuse[i] = light_color[lit2_color][i] * lit2_intensity;
        for (int i = 0; i < 3; i++)
            lit_temp_specular[i] = lit2_specular[i] * lit2_intensity;
        glLightfv(GL_LIGHT2, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT3, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lit2_diffuse);
    }
    if (key == 'N')
    {
        lit2_intensity += (lit2_intensity > 0 ? -0.1 : 0.0);
        for (int i = 0; i < 3; i++)
            lit2_diffuse[i] = light_color[lit2_color][i] * lit2_intensity;
        for (int i = 0; i < 3; i++)
            lit_temp_specular[i] = lit2_specular[i] * lit2_intensity;
        glLightfv(GL_LIGHT2, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT3, GL_SPECULAR, lit_temp_specular);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lit2_diffuse);
    }
    // lit開關
    if (key == '5')
    {
        light0_on = !light0_on;
        if (light0_on && !(sun_time >= 18 || sun_time < 6))
            glEnable(GL_LIGHT0);
        else
            glDisable(GL_LIGHT0);
    }
    if (key == '6')
    {
        if (light1_on)
            glDisable(GL_LIGHT1);
        else
            glEnable(GL_LIGHT1);
        light1_on = !light1_on;
    }
    if (key == '7')
    {
        if (light2_on)
            glDisable(GL_LIGHT2);
        else
            glEnable(GL_LIGHT2);
        light2_on = !light2_on;
        if (light3_on)
            glDisable(GL_LIGHT3);
        else
            glEnable(GL_LIGHT3);
        light3_on = !light3_on;
    }

    if (key == '1')
        view_mode = 1;
    if (key == '2')
        view_mode = 2;
    if (key == '3')
        view_mode = 3;
    if (key == '4')
        view_mode = 4;
    if (key == '0')
        view_mode = 0;
    if (key == '\\')
        view_volume = !view_volume;
    if (key == 'I')
    {
        eyeDx += -0.5; /* move left */
        for (i = 0; i < 3; i++)
            eye[i] += 0.5 * u[0][i];
    }
    if (key == 'i')
    {
        eyeDx += 0.5; /* move right */
        for (i = 0; i < 3; i++)
            eye[i] -= 0.5 * u[0][i];
    }
    if (key == 'O')
    {
        eyeDy += 0.5; /* move up */
        for (i = 0; i < 3; i++)
            eye[i] -= 0.5 * u[1][i];
    }
    if (key == 'o')
    {
        eyeDy += -0.5; /* move down */
        for (i = 0; i < 3; i++)
            eye[i] += 0.5 * u[1][i];
    }

    if (key == 'P')
    {
        eyeDz += 0.5; /* move forward */
        for (i = 0; i < 3; i++)
            eye[i] -= 0.5 * u[2][i];
    }
    if (key == 'p')
    {
        eyeDz += -0.5; /* move backward */
        for (i = 0; i < 3; i++)
            eye[i] += 0.5 * u[2][i];
    }
    if (key == '[') /* zoom in */
    {
        zoom_fac += -0.01;
        zoom_fac = max(0.1, zoom_fac);
    }
    if (key == ']') /* zoom out */
    {
        zoom_fac += 0.01;
        zoom_fac = min(1.5, zoom_fac);
    }
    if (key == 't')
    { /* pitching */
        eyeAngx += 5.0;
        if (eyeAngx > 360.0)
            eyeAngx -= 360.0;
        y[0] = u[1][0] * cv - u[2][0] * sv;
        y[1] = u[1][1] * cv - u[2][1] * sv;
        y[2] = u[1][2] * cv - u[2][2] * sv;

        z[0] = u[2][0] * cv + u[1][0] * sv;
        z[1] = u[2][1] * cv + u[1][1] * sv;
        z[2] = u[2][2] * cv + u[1][2] * sv;

        for (i = 0; i < 3; i++)
        {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    if (key == 'T')
    {
        eyeAngx += -5.0;
        if (eyeAngx < 0.0)
            eyeAngx += 360.0;
        y[0] = u[1][0] * cv + u[2][0] * sv;
        y[1] = u[1][1] * cv + u[2][1] * sv;
        y[2] = u[1][2] * cv + u[2][2] * sv;

        z[0] = u[2][0] * cv - u[1][0] * sv;
        z[1] = u[2][1] * cv - u[1][1] * sv;
        z[2] = u[2][2] * cv - u[1][2] * sv;

        for (i = 0; i < 3; i++)
        {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    if (key == 'y')
    { /* heading */
        eyeAngy += 5.0;
        if (eyeAngy > 360.0)
            eyeAngy -= 360.0;
        for (i = 0; i < 3; i++)
        {
            x[i] = cv * u[0][i] - sv * u[2][i];
            z[i] = sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0; i < 3; i++)
        {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    if (key == 'Y')
    {
        eyeAngy += -5.0;
        if (eyeAngy < 0.0)
            eyeAngy += 360.0;
        for (i = 0; i < 3; i++)
        {
            x[i] = cv * u[0][i] + sv * u[2][i];
            z[i] = -sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0; i < 3; i++)
        {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    if (key == 'u')
    { /* rolling */
        eyeAngz += 5.0;
        if (eyeAngz > 360.0)
            eyeAngz -= 360.0;
        for (i = 0; i < 3; i++)
        {
            x[i] = cv * u[0][i] - sv * u[1][i];
            y[i] = sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0; i < 3; i++)
        {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }
    if (key == 'U')
    {
        eyeAngz += -5.0;
        if (eyeAngz < 0.0)
            eyeAngz += 360.0;
        for (i = 0; i < 3; i++)
        {
            x[i] = cv * u[0][i] + sv * u[1][i];
            y[i] = -sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0; i < 3; i++)
        {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }

    // 走路
    if (key == 'w' || key == 'W')
        walk();
    // 轉圈
    if (key == 'a' || key == 'A')
        turn_around(3);
    if (key == 'd' || key == 'D')
        turn_around(-3);
    // 拿咖啡
    if (key == 's' || key == 'S')
    {
        if (table_coffee == TRUE)
            check_coffee();
        else if (take_coffee == TRUE)
        {
            coffee_overthrow = TRUE;
            walk_end();
        }
    }

    // 跳
    if (key == 'j' || key == 'J')
    {
        jump_cnt += 1.0;
        while (jump_cnt != 0)
        {
            jump();
            display();
        }
    }
    // 招手打招呼
    if (key == 'h' || key == 'H')
    {
        sayhi_cnt += 1.0;
        sayhi = TRUE;
        while (sayhi_cnt != 0)
        {
            say_hi();
            display();
        }
        sayhi = FALSE;
    }

    display();
}

void set_bird_texture()
{
    char image_name[20] = "bird/aa.bmp";
    glGenTextures(13, birdID);
    for (int i = 1; i <= 12; i++)
    {
        image_name[5] = i / 10 + '0';
        image_name[6] = i % 10 + '0';
        image_data = stbi_load(image_name, &image_width, &image_height, &nrChannels, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glBindTexture(GL_TEXTURE_2D, birdID[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glDisable(GL_TEXTURE_2D);
    }
}

void set_tree1_texture()
{
    char image_name[20] = "tree1/00aa.bmp";
    glGenTextures(15, tree1ID);
    for (int i = 1; i <= 14; i++)
    {
        image_name[8] = i / 10 + '0';
        image_name[9] = i % 10 + '0';
        image_data = stbi_load(image_name, &image_width, &image_height, &nrChannels, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glBindTexture(GL_TEXTURE_2D, tree1ID[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glDisable(GL_TEXTURE_2D);
    }
}

void set_tree2_texture()
{
    char image_name[20] = "tree2/0aaa.bmp";
    glGenTextures(119, tree2ID);
    for (int i = 1; i <= 118; i++)
    {
        image_name[7] = i / 100 + '0';
        image_name[8] = (i / 10) % 10 + '0';
        image_name[9] = i % 10 + '0';
        image_data = stbi_load(image_name, &image_width, &image_height, &nrChannels, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glBindTexture(GL_TEXTURE_2D, tree2ID[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glDisable(GL_TEXTURE_2D);
    }
}

void set_skybox_texture()
{
    char image_name[20] = "skybox/skyboxa.bmp";
    glGenTextures(7, skyboxID);
    for (int i = 1; i <= 6; i++)
    {
        image_name[13] = i +'0';
        image_data = stbi_load(image_name, &image_width, &image_height, &nrChannels, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glBindTexture(GL_TEXTURE_2D, skyboxID[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glDisable(GL_TEXTURE_2D);
    }
}

void myinit()
{
    if (sphere == NULL)
    {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
        gluQuadricTexture(sphere, GL_TRUE);
    }
    if (mycircle == NULL)
    {
        mycircle = gluNewQuadric();
        gluQuadricDrawStyle(mycircle, GLU_FILL);
        gluQuadricNormals(mycircle, GLU_SMOOTH);
        gluQuadricTexture(mycircle, GL_TRUE);
    }
    if (cylind == NULL)
    {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
        gluQuadricTexture(cylind, GL_TRUE);
    }
    /*--- Draw a cylinder ---*/
    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);
    /*---- Copy eye position ---*/
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];

    glClearColor(background_color[0][0], background_color[0][1], background_color[0][2], 1.0); /*set the background color BLACK */
    /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST); /*Enable depth buffer for shading computing */
    glEnable(GL_NORMALIZE);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(4, TextureID);

    image_data = stbi_load("wallpaper5.bmp", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    image_data = stbi_load("cat4.bmp", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glDisable(GL_TEXTURE_2D);

    image_data = stbi_load("carpet.bmp", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glDisable(GL_TEXTURE_2D);

    image_data = stbi_load("skybox/skybox1.bmp", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glDisable(GL_TEXTURE_2D);

    set_bird_texture();
    set_tree1_texture();
    set_tree2_texture();
    set_skybox_texture();

    glEnable(GL_FOG);               /*enable fog fade */
    glFogi(GL_FOG_MODE, GL_LINEAR); /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    glFogf(GL_FOG_DENSITY, fog_density);   /*fog opacity(density)= 0.25*/
    glFogf(GL_FOG_START, 1.0);      /*Setup two ends for GL_LINEAR*/
    glFogf(GL_FOG_END, fog_dis);
    glFogfv(GL_FOG_COLOR, light_color[fog_color]); /*set the fog color */
    // if (nrChannels == 4)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    // }
    // else
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    // }

    /*-----Define some global lighting status -----*/
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);    /* local viewer */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

    /*-----Enable face culling -----*/
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING); /*Enable lighting effects */
    glEnable(GL_LIGHT0);   /*Turn on light0 */
    glEnable(GL_LIGHT1);   /*Turn on light1 */
    glEnable(GL_LIGHT2);   /*Turn on light2 */
    glEnable(GL_LIGHT3);   /*Turn on light3 */
    /*-----Define light0 ---------*/
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lit_direction);
    /*-----Define light1 ---------*/
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);
    /*-----Define light2 ---------*/
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    /*-----Define light3 ---------*/
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, lit2_cutoff);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lit2_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, lit2_specular);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, lit2_exponent);
    glFlush(); /*Enforce window system display the results*/
}
void keyUp(unsigned char key, int x, int y)
{
    if (key == 'w' || key == 'W' || key == 'a' || key == 'A' || key == 'd' || key == 'D')
    {
        walk_end();
    }
}
void timerProc(int id)
{
    wallpaper_ang += 10;
    if (wallpaper_ang > 360)
        wallpaper_ang -= 360;
    wallpaper_dis += 0.05;
    if (wallpaper_dis > 1.0)
        wallpaper_dis -= 1.0;

    bird_idx += 1;
    if (bird_idx > 12)
        bird_idx = 1;
    bird_pos_x += 1;
    if (bird_pos_x > 50)
        bird_pos_x = -50;

    tree1_idx += 1;
    if (tree1_idx > 14)
        tree1_idx = 1;

    tree2_idx += 1;
    if (tree2_idx > 118)
        tree2_idx = 1;

    display();
    glutTimerFunc(50, timerProc, 1);
}
void main(int argc, char **argv)
{
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("robot");

    myinit(); /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);
    // glutIdleFunc(idle_func);    /* Add idle event */
    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(my_keyboard);
    glutKeyboardUpFunc(keyUp);

    glutTimerFunc(50, timerProc, 1);

    glutMainLoop();
}
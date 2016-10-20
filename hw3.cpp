#include <string>
#include <iostream>
#include <math.h>
#include "GLee.h"
#include <GL/gl.h>   
#include <GL/glut.h>
#include "glinit.h"
#include "utils.h"

#include "vec3.h"

#include <pthread.h>
void junk()
{
  int i;
  i=pthread_getconcurrency();
};

using namespace std;

GLuint gProgramShader;              // program handler for our shader
GLuint gTextureMap;					// texture handler for the texture map.

GLuint gVertices, gTexcoords;		// Vertex Buffer Objects for the terrain vertices and texture coordinates.

GLfloat gHeightFactor = 20.0;		// displacement level for the height map. Can be passed to the shader source

int width,height;    // texture width and height
int numVertices, numTextures;

// camera parameters
// such as camera direction, pitch, yaw, camera position, camera's current speed
// should be declared as global variables here
vec3 cam, camRef, camSpeed, camTarget, camUp, camPr, camAngle;

void reshape(int w, int h) 
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(45, double(w)/h, 0.1f, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
		
float toRadian(float angle)
{
    return angle * (M_PI/ 180.0);
}

void keyboard(unsigned char key, int x, int y)
{
    float angle;
    switch(key) 
    {
        case 27:	// Escape
            exit(0); 
            break; 
		case 'o':	// increase height factor of the height map
			gHeightFactor += 1.0f;
			break;
		case 'l':	// decrease height factor of the height map
			gHeightFactor -= 1.0f;
			break;
        case 'w':	// increase pitch
            angle = -1;

            camTarget = camTarget * cos(toRadian(angle)) - camUp * sin(toRadian(angle));
            camTarget.normalize();

            camUp = camTarget.cross(camPr) * -1;
            camUp.normalize();
            camAngle.y -= 1;
            break;
        case 's':	// decrease pitch
            angle = +1;

            camTarget = camTarget * cos(toRadian(angle)) - camUp * sin(toRadian(angle));
            camTarget.normalize();

            camUp = camTarget.cross(camPr) * -1;
            camUp.normalize();
            break;
        case 'a':	// turn left
            angle = +1;

            camTarget = camTarget * cos(toRadian(angle)) - camPr * sin(toRadian(angle));
            camTarget.normalize();

            camPr = camTarget.cross(camUp);
            camPr.normalize();
            break;
        case 'd':	// turn right
            angle = -1;

            camTarget = camTarget * cos(toRadian(angle)) - camPr * sin(toRadian(angle));
            camTarget.normalize();

            camPr = camTarget.cross(camUp);
            camPr.normalize();
            break;
        case 'u':	// increase speed
            camSpeed = camSpeed + vec3(0.05,0.05,0.05);
            break;
        case 'j':	// decrease speed 
            camSpeed = camSpeed - vec3(0.05,0.05,0.05);
            break;
        default:
            break;
    }
}


void init(int *argc, char** argv) 
{
    int i,j;
    float *vertices;
    float *texcoords;
   
    // uncomment the following line if you want to see wireframe terrain
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
 
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (*argc < 2)
    {
		cout << "Missing argument(s)." << endl 
			 << "Usage: ./hw3 <texture file name>"  << endl;
			 
		exit(0);	
	}

    initTexture(argv[1],&width,&height);    
    initShaders();
  
    numVertices = 18*width*height; numTextures = 12*width*height;

    // write code below 
    // to initial camera parameters 
    // camera is positioned at (x,y,z) = (width/2,width/10,width/4)
    // camera speed is 0
    // camera gaze direction is (0,0,-1)

    cam = vec3(width/2, width/10, width/4);

    camRef = cam + camTarget;

    camTarget = vec3(0, 0, -1);

    camUp = vec3(0, 1, 0);

    camPr = camTarget.cross(camUp);
    camPr.normalize();
    
    // initialize the terrain
    glGenBuffers(1, &gVertices);
    glGenBuffers(1, &gTexcoords);

    float hg = height, wd = width;

    float *ptr = vertices = (float *) malloc(numVertices * sizeof(float));
    for (float y = 0; y < hg; ++y)
    {
    	for (float x = 0; x < wd; ++x)
        {
    		*ptr++      = x+1 ; *(++ptr)++ -= y   ;

    		*ptr++      = x   ; *(++ptr)++ -= y   ;

    		*ptr++      = x   ; *(++ptr)++ -= y+1 ;

    		*ptr++      = x   ; *(++ptr)++ -= y+1 ;

    		*ptr++      = x+1 ; *(++ptr)++ -= y+1 ;

    		*ptr++      = x+1 ; *(++ptr)++ -= y   ;
    	}
    }

    glBindBuffer(GL_ARRAY_BUFFER, gVertices);
    glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(float), vertices, GL_STATIC_DRAW);

    ptr = texcoords = (float *) malloc(numVertices * sizeof(float));
    for (float y = 0; y <hg; ++y)
    {
    	for (float x = 0; x < wd; ++x)
        {
    		*ptr++  = (x+1) / wd ; *ptr++ -= y     / hg ;

    		*ptr++  = x     / wd ; *ptr++ -= y     / hg ;

    		*ptr++  = x     / wd ; *ptr++ -= (y+1) / hg ;

    		*ptr++  = x     / wd ; *ptr++ -= (y+1) / hg ;

    		*ptr++  = (x+1) / wd ; *ptr++ -= (y+1) / hg ;

    		*ptr++  = (x+1) / wd ; *ptr++ -= y     / hg ;
    	}
    }

	glBindBuffer(GL_ARRAY_BUFFER, gTexcoords);
    glBufferData(GL_ARRAY_BUFFER, numTextures*sizeof(float), texcoords, GL_STATIC_DRAW);
}

// OpenGL display function called continuously in an infinite loop 
void display()
{	
    long startTime = getCurrentTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // compute new camera parameters at the begining of the frame and set camera location and gaze 
    cam = cam + camTarget * camSpeed;

    camRef = cam + camTarget;

    gluLookAt(
            cam.x,    cam.y    , cam.z    ,
            camRef.x, camRef.y , camRef.z ,
            camUp.x,  camUp.y  , camUp.z
    );

    // Display the Terrain using the written shaders
    glUseProgram(gProgramShader);

    // the light position is set to the following coordinates
    glUniform4f(glGetUniformLocation(gProgramShader, "lightPosition")   , width/2.0,width+height,height/2.0,1);

    // texture related function calls
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,gTextureMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(gProgramShader, "texture"), 0);
    glUniform1i(glGetUniformLocation(gProgramShader, "width"), width);
    glUniform1i(glGetUniformLocation(gProgramShader, "height"), height);
    glUniform1f(glGetUniformLocation(gProgramShader,"heightFactor"), gHeightFactor);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVertices);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gTexcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    // Draw the terrain as triangles
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    
    glutSwapBuffers();
	
    // Set FPS to 100
    long diff = getCurrentTime() - startTime;
    if (diff < 10000)
    usleep(10000 - diff);
}

int main(int argc, char** argv)   
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("20151 - CEng477 - HW3");

    init(&argc, argv);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(glutPostRedisplay);

    glutMainLoop();

    return 0;
}


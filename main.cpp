
#ifdef __APPLE__
    #include <GL/glew.h>
    #include <GLUT/glut.h>


#endif

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

GLuint v,f,f2,p;
float lpos[4] = {-0.3,0.0,0.2,0}; //Positioned light
float light_specular[4] = {1, 0.6, 1, 0};
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 0.0 }; //ambient light intensity
GLfloat a;
GLfloat mat_emission[] = {0.3, 0.5, 0.7, 0.0};
GLfloat mat_specular[] = { 1.0, 0.5, 0.0, 0.0 };
GLfloat low_shininess[] = { 0.5 };


const int WINDOW_W = 640;
const int WINDOW_H = 480;

struct Camera {
    float camX, camY, camZ;
    float forwardX, forwardY, forwardZ; 
    float upX, upY, upZ; 

    float theta, phi;

    Camera() {
        camX = 0, camY = 0, camZ = 4.5;
        forwardX = 0, forwardY = 0, forwardZ = -1;
        upX = 0, upY = 1, upZ = 0;

        theta = 0; phi = M_PI;
    }

};

Camera camera;


void draw(void) {


    // Black background

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(camera.camX,camera.camY,camera.camZ, //Camera position
              camera.camX+camera.forwardX,camera.camY+camera.forwardY,camera.camZ+camera.forwardZ, //Position of the object to look at
              camera.upX,camera.upY,camera.upZ); //Camera up direction

    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    //glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    glRotatef(a,0,1,1);

    
    glutSolidSphere(1, 10, 10);
    a+=0.1;

    glutSwapBuffers();


}

char* readFromFile(const char* filename){


    std::ifstream t;
    int length;
    t.open(filename);   
    t.seekg(0, std::ios::end);    
    length = t.tellg();           
    t.seekg(0, std::ios::beg);    
    char* buffer = new char[length];   
    t.read(buffer, length);      
    t.close();

    return buffer;

}

void setShaders() {

    char *vs = NULL,*fs = NULL;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    //f2 = glCreateShader(GL_FRAGMENT_SHADER);

    vs = readFromFile("minimal.vert");
    fs = readFromFile("minimal.frag");

    const char * vv = vs;
    const char * ff = fs;

    glShaderSource(v, 1, &vv,NULL);
    //glShaderSource(f, 1, &ff,NULL);

    free(vs);free(fs);

    glCompileShader(v);
    glCompileShader(f);

    //printShaderInfoLog(v);
    //printShaderInfoLog(f);
    //printShaderInfoLog(f2);

    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);

    glLinkProgram(p);
    //printProgramInfoLog(p);

    glUseProgram(p);

}

void initOpenGL(){

    glewInit();
    if (glewIsSupported("GL_VERSION_2_0"))
        printf("Ready for OpenGL 2.0\n");
    else {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glClearColor(0.7f,0.7f,0.7f,0.7f);

    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);


}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    float ratio = 1.0* w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45,ratio,1,1000);
    glMatrixMode(GL_MODELVIEW);
    
}

void cross(float x1, float y1, float z1, float x2, float y2, float z2,float& rightX, float& rightY, float& rightZ){
    rightX = y1*z2 - z1*y2;
    rightY = x1*z2 - x1*z2;
    rightZ = x1*y2 - y1*x1;

}
void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27) 
        exit(0);

    float vel = 0.1;
    float rightX, rightY, rightZ;
    printf("forwardX: %f forwardY: %f forwardZ: %f\n",camera.forwardX,camera.forwardY,camera.forwardZ);
    printf("upX: %f upY: %f upZ: %f\n", camera.upX,camera.upY, camera.upZ);
    cross(camera.forwardX,camera.forwardY,camera.forwardZ, camera.upX,camera.upY, camera.upZ, rightX, rightY, rightZ);
    float sizeRight = sqrtf(rightX*rightX + rightY*rightY + rightZ*rightZ);
    rightX /= sizeRight; rightY /= sizeRight; rightZ /= sizeRight;
    printf("rightX: %f rightY: %f rightZ: %f\n",rightX, rightY, rightZ);
    if( key == 'w' )
    {
        camera.camX += camera.forwardX*vel;
        camera.camY += camera.forwardY*vel;
        camera.camZ += camera.forwardZ*vel;
    }
    if( key == 's' )
    {
        camera.camX -= camera.forwardX*vel;
        camera.camY -= camera.forwardY*vel;
        camera.camZ -= camera.forwardZ*vel;
    }
    if( key == 'a' )
    {

        camera.camX -= rightX*vel;
        camera.camY -= rightY*vel;
        camera.camZ -= rightZ*vel;
    }
    if( key == 'd' )
    {
        camera.camX += rightX*vel;
        camera.camY += rightY*vel;
        camera.camZ += rightZ*vel;
    }



}

void mouseCallback(int x, int y){


        camera.phi = M_PI+ (0.5 - (float(x)/WINDOW_W))*M_PI;
        camera.theta = (0.5 - (float(y)/WINDOW_H))*M_PI; 


        //printf("phi: %f theta: %f x: %d y: %d\n",camera.phi, camera.theta, x, y);

        float rightX, rightY, rightZ;
        rightX = sinf(camera.phi - M_PI/2.0f);
        rightY = 0;
        rightZ = cosf(camera.phi - M_PI/2.0f);
        float sizeRight = sqrtf(rightX*rightX + rightY*rightY + rightZ*rightZ);
        rightX /= sizeRight; rightY /= sizeRight; rightZ /= sizeRight;


        camera.forwardX = cosf(camera.theta)*sinf(camera.phi);
        camera.forwardY = sinf(camera.theta);
        camera.forwardZ = cosf(camera.theta)*cosf(camera.phi);
        float sizeForward = sqrtf(camera.forwardX*camera.forwardX + camera.forwardY*camera.forwardY + camera.forwardZ*camera.forwardZ);
        camera.forwardX /= sizeForward; camera.forwardY /= sizeForward; camera.forwardZ /= sizeForward;



        //printf("%f %f %f\n",camera.forwardX, camera.forwardY, camera.forwardZ);

       

        float newUpX, newUpY, newUpZ;

        cross(rightX,rightY,rightZ, camera.forwardX,camera.forwardY, camera.forwardZ, newUpX, newUpY, newUpZ);
        float sizeUp = sqrtf(newUpX*newUpX + newUpY*newUpY + newUpZ*newUpZ);
        camera.upX = newUpX/sizeUp; camera.upY = newUpY/sizeUp; camera.upZ = newUpZ/sizeUp;
        


}

//Main program

int main(int argc, char **argv) {

    glutInit(&argc, argv);

    /*Setting up  The Display
    /    -RGB color model + Alpha Channel = GLUT_RGBA
    */
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH | GLUT_DOUBLE );

    //Configure Window Postion
    glutInitWindowPosition(50, 25);

    //Configure Window Size
    glutInitWindowSize(WINDOW_W,WINDOW_H);

    //Create Window
    glutCreateWindow("Hello OpenGL Shaders");


    //Call to the drawing function
    glutDisplayFunc(draw);
    glutIdleFunc(draw);

    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processNormalKeys);
    glutMotionFunc(mouseCallback);

    initOpenGL();
    setShaders();

    // Loop require by OpenGL
    glutMainLoop();
    return 0;
}
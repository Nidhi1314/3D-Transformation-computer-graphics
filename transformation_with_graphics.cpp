#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace std;

struct Point {
    float x, y, z, w;
};

vector<Point> object = {
    {-0.5, -0.5, -0.5, 1}, {0.5, -0.5, -0.5, 1},
    {0.5,  0.5, -0.5, 1}, {-0.5,  0.5, -0.5, 1},
    {-0.5, -0.5,  0.5, 1}, {0.5, -0.5,  0.5, 1},
    {0.5,  0.5,  0.5, 1}, {-0.5,  0.5,  0.5, 1}
};

int edges[12][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};

float Hybrid[4][4]; // Hybrid matrix

int winWidth = 800, winHeight = 600; // Track window size for correct aspect

void initHybrid() {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            Hybrid[i][j] = (i==j)?1:0;
}

// Matrix multiplication C = A*B
void matMul(float A[4][4], float B[4][4], float C[4][4]) {
    float temp[4][4] = {0};
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            for(int k=0;k<4;k++)
                temp[i][j] += A[i][k]*B[k][j];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            C[i][j] = temp[i][j];
}

// Apply Hybrid matrix to a point
Point apply(Point p) {
    Point res;
    res.x = p.x*Hybrid[0][0] + p.y*Hybrid[0][1] + p.z*Hybrid[0][2] + p.w*Hybrid[0][3];
    res.y = p.x*Hybrid[1][0] + p.y*Hybrid[1][1] + p.z*Hybrid[1][2] + p.w*Hybrid[1][3];
    res.z = p.x*Hybrid[2][0] + p.y*Hybrid[2][1] + p.z*Hybrid[2][2] + p.w*Hybrid[2][3];
    res.w = p.x*Hybrid[3][0] + p.y*Hybrid[3][1] + p.z*Hybrid[3][2] + p.w*Hybrid[3][3];
    return res;
}

/*
   One Hybrid Transformation Function
   type:
   1=Translate, 2=Scale, 3=RotateX, 4=RotateY, 5=RotateZ,
   6=ReflectX, 7=ReflectY, 8=ReflectZ,
   9=ShearXY, 10=ShearXZ, 11=ShearYZ
*/
void applyTransformation(int type, vector<float> params) {
    float M[4][4] = {0};
    // start as identity
    for(int i=0;i<4;i++) M[i][i]=1;

    if(type==1) { // Translation Tx Ty Tz
        M[0][3]=params[0]; M[1][3]=params[1]; M[2][3]=params[2];
    }
    else if(type==2) { // Scaling Sx Sy Sz
        M[0][0]=params[0]; M[1][1]=params[1]; M[2][2]=params[2];
    }
    else if(type==3) { // Rotate X
        float rad=params[0]*M_PI/180;
        M[1][1]=cos(rad); M[1][2]=-sin(rad);
        M[2][1]=sin(rad); M[2][2]=cos(rad);
    }
    else if(type==4) { // Rotate Y
        float rad=params[0]*M_PI/180;
        M[0][0]=cos(rad); M[0][2]=sin(rad);
        M[2][0]=-sin(rad); M[2][2]=cos(rad);
    }
    else if(type==5) { // Rotate Z
        float rad=params[0]*M_PI/180;
        M[0][0]=cos(rad); M[0][1]=-sin(rad);
        M[1][0]=sin(rad); M[1][1]=cos(rad);
    }
    else if(type==6) { // Reflect X-axis
        M[1][1]=-1;
    }
    else if(type==7) { // Reflect Y-axis
        M[0][0]=-1;
    }
    else if(type==8) { // Reflect Z-axis
        M[2][2]=-1;
    }
    else if(type==9) { // Shear XY (shx, shy)
        M[0][2]=params[0]; // x depends on z
        M[1][2]=params[1]; // y depends on z
    }
    else if(type==10) { // Shear XZ (shx, shz)
        M[0][1]=params[0]; // x depends on y
        M[2][1]=params[1]; // z depends on y
    }
    else if(type==11) { // Shear YZ (shy, shz)
        M[1][0]=params[0]; // y depends on x
        M[2][0]=params[1]; // z depends on x
    }

    matMul(M,Hybrid,Hybrid); // Update Hybrid
}

// Draw cube
void drawObject(vector<Point> verts) {
    glBegin(GL_LINES);
    for(int i=0;i<12;i++) {
        Point p1 = verts[edges[i][0]];
        Point p2 = verts[edges[i][1]];
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the camera (view) so cube is seen in 3D (diagonal view)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Eye at (2.5,2,4), looking at origin, up along +Y
    gluLookAt(2.5, 2.0, 4.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);

    glLineWidth(2.0f);

    // Original (red)
    glColor3f(1,0,0);
    drawObject(object);

    // Transformed (green)
    glColor3f(0,1,0);
    vector<Point> transformed;
    transformed.reserve(object.size());
    for(auto p: object) transformed.push_back(apply(p));
    drawObject(transformed);

    glutSwapBuffers();
}

// Handle window resize to maintain correct perspective
void reshape(int w, int h) {
    if(h==0) h = 1; // avoid divide by zero
    winWidth = w; winHeight = h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Wider FOV and proper aspect ratio
    gluPerspective(60.0, (double)w/(double)h, 0.1, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    initHybrid();
    int n;
    cout<<"Enter number of transformations: ";
    cin>>n;
    for(int i=0;i<n;i++) {
        cout<<"Choose transformation:\n"
              "1=Translate,2=Scale,3=RotateX,4=RotateY,5=RotateZ,\n"
              "6=ReflectX,7=ReflectY,8=ReflectZ,\n"
              "9=ShearXY,10=ShearXZ,11=ShearYZ : ";
        int type; cin>>type;
        vector<float> params;
        if(type==1){float tx,ty,tz;cin>>tx>>ty>>tz;params={tx,ty,tz};}
        else if(type==2){float sx,sy,sz;cin>>sx>>sy>>sz;params={sx,sy,sz};}
        else if(type>=3 && type<=5){float ang;cin>>ang;params={ang};}
        else if(type>=9 && type<=11){float a,b;cin>>a>>b;params={a,b};}
        applyTransformation(type,params);
    }
    cout << "\nTransformation complete. \n Red color - Original\n Green color - Transformed\n Output window displayed. Close window to exit.\n";


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("3D Hybrid Transformation");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f,0.05f,0.08f,1.f); // subtle dark background for contrast

    // Initial projection setup (will also be set on first reshape)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)winWidth/(double)winHeight, 0.1, 50.0);
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

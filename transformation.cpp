#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Point {
    double x, y, z, w;
};

// Multiply point with transformation matrix
Point multiply(const vector<vector<double>> &mat, const Point &p) {
    Point result = {0, 0, 0, 0};
    double arr[4] = {p.x, p.y, p.z, p.w};
    double res[4] = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i] += mat[i][j] * arr[j];
        }
    }

    result.x = res[0];
    result.y = res[1];
    result.z = res[2];
    result.w = res[3];
    return result;
}

// Apply chosen transformation
vector<vector<double>> transformationMatrix(int choice) {
    vector<vector<double>> mat(4, vector<double>(4, 0));

    switch(choice) {
        case 1: { // Translation
            double tx, ty, tz;
            cout << "Enter tx ty tz: ";
            cin >> tx >> ty >> tz;
            mat = {{1,0,0,tx}, {0,1,0,ty}, {0,0,1,tz}, {0,0,0,1}};
            break;
        }
        case 2: { // Scaling
            double sx, sy, sz;
            cout << "Enter sx sy sz: ";
            cin >> sx >> sy >> sz;
            mat = {{sx,0,0,0}, {0,sy,0,0}, {0,0,sz,0}, {0,0,0,1}};
            break;
        }
        case 3: { // Rotation X
            double theta; cout << "Enter angle in degrees: "; cin >> theta;
            double rad = theta * M_PI / 180;
            mat = {{1,0,0,0}, {0,cos(rad),-sin(rad),0}, {0,sin(rad),cos(rad),0}, {0,0,0,1}};
            break;
        }
        case 4: { // Rotation Y
            double theta; cout << "Enter angle in degrees: "; cin >> theta;
            double rad = theta * M_PI / 180;
            mat = {{cos(rad),0,sin(rad),0}, {0,1,0,0}, {-sin(rad),0,cos(rad),0}, {0,0,0,1}};
            break;
        }
        case 5: { // Rotation Z
            double theta; cout << "Enter angle in degrees: "; cin >> theta;
            double rad = theta * M_PI / 180;
            mat = {{cos(rad),-sin(rad),0,0}, {sin(rad),cos(rad),0,0}, {0,0,1,0}, {0,0,0,1}};
            break;
        }
        case 6: { // Reflection XY
            mat = {{1,0,0,0}, {0,1,0,0}, {0,0,-1,0}, {0,0,0,1}};
            break;
        }
        case 7: { // Reflection XZ
            mat = {{1,0,0,0}, {0,-1,0,0}, {0,0,1,0}, {0,0,0,1}};
            break;
        }
        case 8: { // Reflection YZ
            mat = {{-1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
            break;
        }
        case 9: { // Shear XY
            double shx, shy;
            cout << "Enter shx shy: ";
            cin >> shx >> shy;
            mat = {{1,shx,0,0}, {shy,1,0,0}, {0,0,1,0}, {0,0,0,1}};
            break;
        }
        case 10: { // Shear XZ
            double shx, shz;
            cout << "Enter shx shz: ";
            cin >> shx >> shz;
            mat = {{1,0,shx,0}, {0,1,0,0}, {shz,0,1,0}, {0,0,0,1}};
            break;
        }
        case 11: { // Shear YZ
            double shy, shz;
            cout << "Enter shy shz: ";
            cin >> shy >> shz;
            mat = {{1,0,0,0}, {0,1,shy,0}, {0,shz,1,0}, {0,0,0,1}};
            break;
        }
        default:
            cout << "Invalid choice" << endl;
    }

    return mat;
}

int main() {
    // Define a cube (8 vertices)
    vector<Point> cube = {
        {-0.5,-0.5,-0.5,1}, {0.5,-0.5,-0.5,1},
        {0.5,0.5,-0.5,1},  {-0.5,0.5,-0.5,1},
        {-0.5,-0.5,0.5,1}, {0.5,-0.5,0.5,1},
        {0.5,0.5,0.5,1},   {-0.5,0.5,0.5,1}
    };

    int n;
    cout << "Enter number of transformations: ";
    cin >> n;

    for(int t=0; t<n; t++) {
        cout << "\\nChoose transformation:\\n"
             << "1=Translate, 2=Scale, 3=RotateX, 4=RotateY, 5=RotateZ,\\n"
             << "6=ReflectXY, 7=ReflectXZ, 8=ReflectYZ,\\n"
             << "9=ShearXY, 10=ShearXZ, 11=ShearYZ : ";
        int choice;
        cin >> choice;

        vector<vector<double>> mat = transformationMatrix(choice);

        // Apply to all cube points
        for(auto &p : cube) {
            p = multiply(mat, p);
        }
    }

    cout << "\\nFinal Cube Coordinates:\\n";
    for(auto &p : cube) {
        cout << "(" << p.x << ", " << p.y << ", " << p.z << ")\\n";
    }

    return 0;
}

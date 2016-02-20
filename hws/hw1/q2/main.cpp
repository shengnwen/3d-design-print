#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;
bool input(float points[][3], float v[3], char*  filename);
void calAvg(float * avg, float points[][3]);
void print(string discription, float * p1);
void calInters(vector<float> &inters, float points[][3], float* v, float d);
int main(int argc, char * argv[]) {
    // Ax + By + Cz + d = 0;
    if (argc == 1) {
        cout << "You should input filename for points!" << endl;
        return 0;
    } else if (argc > 2) {
        cout << "You should input only one file name" << endl;
        return 0;
    }
    float points[5][3], v[3], avg[3], d;
    vector<float> intersects;
    if (!input(points, v, argv[1])) {
        cout << "Error!" << endl;
        return 0;
    } else {
        cout << "Input is valid." << endl;
    }
    // caculate the avarage point of five points
    calAvg(avg, points);
    print("avg", avg);
    // calculate d: Ax + By + Cz + d = 0;
    d = - avg[0] * v[0] - avg[1] * v[1] - avg[2] * v[2];
    // calculate potential intersect points: p1p2, p1p3, p2p3, p2p4, p3p4, p3p5, p4p5;
    calInters(intersects, points, v, d);
    cout << intersects.size() / 3 << " points intersects:" << endl;
    for (int i = 0; i < intersects.size() / 3; i++) {
        cout << "(" << intersects.at(i * 3) << ", " << intersects.at(i * 3 + 1) << ", " << intersects.at(i * 3 + 2) <<")"<< endl;
    }
    return 0;
}

float vec_len(float v[3]) {
    float len = 0;
    for (int i = 0; i < 3; i++) {
        len += v[i] * v[i];
    }
    return sqrtf(len);
}
void normalize(float vec[3]){
    float len;
    len = vec_len(vec);
    for (int i = 0; i < 3; i++) {
        vec[i] /= len;
    }
}
void triNormal(float* p1, float* p2, float* p3, float* normal) {
    normal[0] = (p2[1] - p1[1])*(p3[2] - p1[2]) - (p3[1] - p1[1])*(p2[2] - p1[2]);
    normal[1] = (p2[2] - p1[2])*(p3[0] - p1[0]) - (p2[0] - p1[0])*(p3[2] - p1[2]);
    normal[2] = (p2[0] - p1[0])*(p3[1] - p1[1]) - (p3[0] - p1[0])*(p2[1] - p1[1]);
    normalize(normal);
}
bool vecsParellel(float* v1, float *v2) {
    if (fabsf(fabsf(v1[0]) - fabsf(v2[0])) <= 0.01 && fabsf(fabsf(v1[1]) - fabsf(v2[1])) <= 0.01
        && fabsf(fabsf(v2[2]) - fabsf(v2[2])) <= 0.01) {
        return true;
    }
    return false;
}
bool input(float points[][3], float v[3], char * filename){
    float normals[3][3];
    ifstream f;
    f.open(filename, ios_base::in);
    cout << filename <<endl;
    if (!f) {
        cout << "Cannot open Data!" << endl;
        return false;
    }
    for (int i = 0; i < 5; i ++) {
        for (int idx = 0; idx < 3; idx ++) {
            f >> points[i][idx];
            cout << points[i][idx];
        }
    }
    f >> v[0] >> v[1] >> v[2];
//    cout << "Please input x, y, z of 5 points and 1 normal."  << endl;
//    for(int i = 0; i < 5; i++) {
//        for (int idx= 0; idx < 3; idx++) {
//            cin >> points[i][idx];
//        }
//    }
//    cin >> v[0] >> v[1] >> v[2];
    for (int tri = 0; tri < 3; tri++) {
        float *p1, *p2, *p3;
        p1 = points[tri];
        p2 = points[tri + 1];
        p3 = points[tri + 2];
        triNormal(p1, p2, p3, normals[tri]);
    }
    normalize(v);
    int tri = 0;
    for (; tri < 3; tri++) {
        if (!vecsParellel(normals[tri], v)){
            break;
        }
    }
    return tri != 3;
}
void calAvg(float * avg, float points[][3]) {
    for (int idx = 0; idx < 3; idx ++) {
        avg[idx] = 0;
        for (int i = 0; i < 5; i++) {
            avg[idx] += points[i][idx];
        }
        avg[idx] /= 5.0;
    }
}
float pointMultiply(float * p1, float* p2) {
    float res = 0;
    for (int i = 0; i < 3; i++) {
        res += p1[i] * p2[i];
    }
    return res;
}
void print(string discription, float * p1){
    cout << discription + ":";
    cout << "(" << p1[0] << ", " << p1[1] << ", " << p1[2] << ")"<< endl;
}
void calIntersect(vector<float> &inters, float* p1, float* p2, float* v, float d) {
    float t, k[3], v_p1, v_k;
    // px = p1 + k * t;
    // A(x1 + (x2 - x1)t) + B(y1 + (y2 - y1)t) + C(z1 + (z2 - z1)t) + d = 0;
    // (Ax1 + By1 + Cz1) + d + (Ak1 + Bk2 + Ck3)t = 0;
    // V * P1 + d + (V * K)t = 0
    t = 0;
//    print("p1:", p1);
//    print("p2:", p2);
    for (int i = 0; i < 3; i++) {
        k[i] = p2[i] - p1[i];
    }
    v_k = pointMultiply(k, v);
    if (v_k == 0) {
        // line and plane is parallel
//        cout << "parallel" << endl;
        return;
    }
    v_p1 = pointMultiply(v, p1);
    t = -(v_p1 + d) / v_k;
//    cout << "t:" << t << endl;
    if (0 <= t && t <= 1) {
//        print("K:", k);
        for (int i = 0; i < 3; i++) {
            inters.push_back(k[i] * t + p1[i]);
        }
    }

}
void calInters(vector<float> &inters, float points[][3], float* v, float d) {
    calIntersect(inters, points[0], points[1], v, d);
    calIntersect(inters, points[0], points[2], v, d);
    calIntersect(inters, points[1], points[2], v, d);
    calIntersect(inters, points[1], points[3], v, d);
    calIntersect(inters, points[2], points[3], v, d);
    calIntersect(inters, points[2], points[4], v, d);
    calIntersect(inters, points[3], points[4], v, d);
}
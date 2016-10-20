#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

class vec3 {
    public:
        vec3(){};
        vec3(float _x, float _y, float _z){x=_x;y=_y;z=_z;};

        vec3 operator+(vec3 rhs){ return vec3(x + rhs.x, y + rhs.y, z + rhs.z); };
        vec3 operator-(vec3 rhs){ return vec3(x - rhs.x, y - rhs.y, z - rhs.z); };
        vec3 operator*(vec3 rhs){ return vec3(x * rhs.x, y * rhs.y, z * rhs.z); };
        vec3 operator/(vec3 rhs){ return vec3(x / rhs.x, y / rhs.y, z / rhs.z); };

        vec3 operator+(float n){ return vec3(x + n, y + n, z + n); };
        vec3 operator-(float n){ return vec3(x - n, y - n, z - n); };
        vec3 operator*(float n){ return vec3(x * n, y * n, z * n); };
        vec3 operator/(float n){ return vec3(x / n, y / n, z / n); };

        vec3 cross(vec3 rhs){ 
            return vec3(y * rhs.z - z * rhs.y,
                        z * rhs.x - x * rhs.z,
                        x * rhs.y - y * rhs.x);
        }

        float length(){ 
            return sqrtf(powf(x,2) + powf(y,2) + powf(z,2));
        }
        void normalize(){ 
            *this= *this / length();
        }

        float x, y, z;
};
#endif //VECTOR3_H

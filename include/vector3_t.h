#ifndef __VECTOR3_T_H__
#define __VECTOR3_T_H__


#include <math.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


template <class T>
class Vector3
{
public:

    union
    {
        T v[3];
        struct { T x, y, z; };
    };

    inline Vector3(): x(0), y(0), z(0) { };
    Vector3(T x, T y, T z): x(x), y(y), z(z) { }
    Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) { }

    const bool operator == (const Vector3& v) const;
    const bool operator != (const Vector3& v) const;

    const Vector3  operator +  (const Vector3& v) const;
    const Vector3& operator += (const Vector3& v);

    const Vector3  operator -  (const Vector3& v) const;
    const Vector3  operator -  () const;
    const Vector3& operator -= (const Vector3& v);

    // cross product
    const Vector3  operator *  (const Vector3& v) const;
    const Vector3& operator *= (const Vector3& v);
    const Vector3  operator *  (const T d) const;
    const Vector3& operator *= (const T d);

    const Vector3  operator /  (const T d) const;
    const Vector3& operator /= (const T d);

    // TODO: does the 'normal' have to be unit-length?
    void rotate(const Vector3& normal, const T angle);
    void normalize();

    T len  () const;
    T len2 () const;
    T angle(const Vector3& v) const;
    T dist (const Vector3& v) const;
    T dist2(const Vector3& v) const;

    inline const Vector3 cross(const Vector3& v) const;
    inline T             dot(const Vector3& v) const;
    const Vector3        reflection(const Vector3& plane_normal) const;

    bool is_unit() const;
};


// --------------------------------------------------------------------------
// ---- OPERATORS -----------------------------------------------------------
// --------------------------------------------------------------------------


template <class T>
const Vector3<T> operator * (const T d, const Vector3<T>& v)
{
    return Vector3<T>(v.x * d, v.y * d, v.z * d);
}

template <class T>
const Vector3<T> operator / (const T d, const Vector3<T>& v)
{
    const T dd = 1.0 / d;
    return Vector3<T>(v.x * dd, v.y * dd, v.z * dd);
}

// ----

template <class T>
const bool Vector3<T>::operator == (const Vector3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

template <class T>
const bool Vector3<T>::operator != (const Vector3<T>& v) const
{
    return x != v.x || y != v.y || z != v.z;
}

template <class T>
const Vector3<T> Vector3<T>::operator + (const Vector3<T>& v) const
{
    return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template <class T>
const Vector3<T>& Vector3<T>::operator += (const Vector3<T>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <class T>
const Vector3<T> Vector3<T>::operator - (const Vector3<T>& v) const
{
    return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template <class T>
const Vector3<T> Vector3<T>::operator - () const
{
    return Vector3<T>(-x, -y, -z);
}

template <class T>
const Vector3<T>& Vector3<T>::operator -= (const Vector3<T>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template <class T>
const Vector3<T> Vector3<T>::operator * (const Vector3<T>& v) const
{
    return Vector3<T>(z * v.y - y * v.z,
                    x * v.z - z * v.x,
                    y * v.x - x * v.y);
}

template <class T>
const Vector3<T> Vector3<T>::operator * (const T d) const
{
    return Vector3<T>(x * d, y * d, z * d);
}

// cross product
template <class T>
const Vector3<T>& Vector3<T>::operator *= (const Vector3<T>& v)
{
    T xx = x;
    T yy = y;
    x = z  * v.y  -  y  * v.z;
    y = xx * v.z  -  z  * v.x;
    z = yy * v.x  -  xx * v.y;
    return *this;
}

template <class T>
const Vector3<T>& Vector3<T>::operator *= (const T d)
{
    x *= d; y *= d; z *= d;
    return *this;
}

template <class T>
const Vector3<T> Vector3<T>::operator / (const T d) const
{
    const T dd = 1.0 / d;
    return Vector3<T>(x * dd, y * dd, z * dd);
}

template <class T>
const Vector3<T>& Vector3<T>::operator /= (const T d)
{
    const T dd = 1.0 / d;
    x *= dd; y *= dd; z *= dd;
    return *this;
}


// --------------------------------------------------------------------------
// ---- METHODS -------------------------------------------------------------
// --------------------------------------------------------------------------


// rotate angle degrees around normal
template <class T>
void Vector3<T>::rotate(const Vector3<T>& normal, const T angle)
{
    const T c = cos(-angle * M_PI / 180.0);
    const T s = sin(-angle * M_PI / 180.0);

    //assert(normal.isUnitLength());

    //return *this * c + (dot(normal) * (1.0f - c)) * normal + cross(normal) * s;

    // A - normal
    // V - vector or point that's being rotated

    // this is THE equation
    // c * V + (1 - c) * (A dot V) * A + s * (A cross V);

    Vector3<T> vect;
    T dotp;

    dotp = dot(normal);
    vect = cross(normal);

    x = c * x + (1.0 - c) * dotp * normal.x + s * vect.x;
    y = c * y + (1.0 - c) * dotp * normal.y + s * vect.y;
    z = c * z + (1.0 - c) * dotp * normal.z + s * vect.z;
}

template <class T>
void Vector3<T>::normalize()
{
    T l = len();

    // raise ZeroDivisionError, "can't normalize a zero-length vector"
    //assert(l > 0.000001f);

    *this *= 1.0 / l;
}

template <class T>
T Vector3<T>::len() const
{
    return sqrt(x*x + y*y + z*z);
}

template <class T>
T Vector3<T>::len2() const
{
    return x*x + y*y + z*z;
}

template <class T>
T Vector3<T>::angle(const Vector3<T>& v) const
{
    //T d = dot(v) / (len() * v.len());
    //if (d > 1.) d = 1.;
    //else if (d < -1.) d = -1.;
    return acos(dot(v) / (len() * v.len())) * 180.0 / M_PI;
}


// this is here just in case the return value optimization doesn't work.
//template <class T>
//void Vector3<T>::cross(const Vector3<T>& v, Vector3<T>* result) const
//{
//    x = z  * v.y  -  y  * v.z;
//    y = xx * v.z  -  z  * v.x;
//    z = yy * v.x  -  xx * v.y;
// the first one is right
//    result->x = y * v.z - z * v.y;
//    result->y = z * v.x - x * v.z;
//    result->z = x * v.y - y * v.x;
//}


// reflect in normal vector
template <class T>
const Vector3<T> Vector3<T>::reflection(const Vector3<T>& plane_normal) const
{
    //assert(plane_normal.isUnitLength());

    return (*this - plane_normal * 2.0 * (dot(plane_normal))) * len();
}


template <class T>
T Vector3<T>::dist(const Vector3<T>& v) const
{
    T dx = v.x - x;
    T dy = v.y - y;
    T dz = v.z - z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

template <class T>
T Vector3<T>::dist2(const Vector3<T>& v) const
{
    T dx = v.x - x;
    T dy = v.y - y;
    T dz = v.z - z;
    return dx * dx + dy * dy + dz * dz;
}

template <class T>
inline const Vector3<T> Vector3<T>::cross(const Vector3<T>& v) const
{
    return Vector3<T>(z * v.y - y * v.z,
                      x * v.z - z * v.x,
                      y * v.x - x * v.y);
}

template <class T>
inline T Vector3<T>::dot(const Vector3<T>& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

template <class T>
bool Vector3<T>::is_unit() const
{
    // well... this value is from sponge... may need some adjusting...
    static const T some_small_value = 0.0001;

    T l = len();

    if (l < 1.0 - some_small_value || l > 1.0 + some_small_value)
        return false;
    else return true;
}


#endif // __VECTOR3_T_H__


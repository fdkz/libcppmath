#ifndef __AXIAL_FRAME_T_H__
#define __AXIAL_FRAME_T_H__


#include "vector3_t.h"

//
// x - right
// y - up
// z - front
//
// clockwise motion around axis is considered positive.
// rotate():
//
//     turn down  (x-axis) - negative
//     turn right (y-axis) - negative
//     tilt right (z-axis) - positive
//

// Axial frame has axis of unit-lengths.
// T should be float or double
template <class T>
class AxialFrame
{
public:

    Vector3<T> x_axis;
    Vector3<T> y_axis;
    Vector3<T> z_axis;

    AxialFrame(): x_axis(1, 0, 0), y_axis(0, 1, 0), z_axis(0, 0, 1) { }
    AxialFrame(const Vector3<T>& xa, const Vector3<T>& ya, const Vector3<T>& za):
            x_axis(xa), y_axis(ya), z_axis(za) { }
    AxialFrame(const AxialFrame& af):
            x_axis(af.x_axis), y_axis(af.y_axis), z_axis(af.z_axis) { }


    void reset();

    // project v out of the axial frame. to world-space
    inline Vector3<T> projv_out(const Vector3<T>& v) const;
    // project v into the axial frame. to object-space
    inline Vector3<T> projv_in(const Vector3<T>& v) const;

    // project af out of *this. to world-space
    AxialFrame proj_out(const AxialFrame& af) const;
    // project af into *this. to object-space
    AxialFrame proj_in(const AxialFrame& af) const;

    // rotate *this around v. screw-tightening-direction is positive.
    void rotate(const Vector3<T>& v, const T angle);

    // turn axial_frame to look in the given direction.
    // orientation will be the closest to what already was.
    void look_direction(const Vector3<T>& dir);
    // turn axial-frame to look in direction_vector, up points to up_hint.
    void look_direction2(const Vector3<T>& dir, const Vector3<T>& up_hint);
    // generate an axial frame from the given vector. z-axis points in
    // the given direction. x-axis and y-axis are random, but guaranteed
    // to be orthonormal to every other axis.
    void look_direction_random(const Vector3<T>& dir);

    void remove_xtilt(const Vector3<T>& up_vector);

    void normalize();

    // x-axis will not change
    // Sometimes errors accumulate and the axial frame becomes more and more
    // unorthogonal. You might want to call one of these after a couple of
    // hundred rotations made with axial frame.
    void orthogonalize_x();
    void orthogonalize_y();
    void orthogonalize_z();

    // isEqual(); // maybe with some equality percent or something...

private:
};


//////////////////////////////////////////////////////////////////////////////


template <class T>
void AxialFrame<T>::reset()
{
    x_axis = Vector3<T>(1., 0., 0.);
    y_axis = Vector3<T>(0., 1., 0.);
    z_axis = Vector3<T>(0., 0., 1.);
}

// project v out of the axial frame. to world-space
template <class T>
inline Vector3<T> AxialFrame<T>::projv_out(const Vector3<T>& v) const
{
    // x_axis * v.x + y_axis * v.y + z_axis * v.z
    // x_axis.x * v.x, y_axis.x * v.y + z_axis.x * v.z, ... , ...
    return x_axis * v.x + y_axis * v.y + z_axis * v.z;
}

// project v into the axial frame. to object-space
template <class T>
inline Vector3<T> AxialFrame<T>::projv_in(const Vector3<T>& v) const
{
    return Vector3<T>(x_axis.dot(v), y_axis.dot(v), z_axis.dot(v));
}


// project af out of *this. to world-space
template <class T>
AxialFrame<T> AxialFrame<T>::proj_out(const AxialFrame<T>& af) const
{
    return AxialFrame<T>(projv_out(af.x_axis), projv_out(af.y_axis), projv_out(af.z_axis));
}

// project af into *this. to object-space
template <class T>
AxialFrame<T> AxialFrame<T>::proj_in(const AxialFrame<T>& af) const
{
    return AxialFrame<T>(projv_in(af.x_axis), projv_in(af.y_axis), projv_in(af.z_axis));
}

template <class T>
void AxialFrame<T>::rotate(const Vector3<T>& v, const T angle)
{
    x_axis.rotate(v, angle);
    y_axis.rotate(v, angle);
    z_axis.rotate(v, angle);

    orthogonalize_z();
}

// turn axial_frame to look in the given direction.
// orientation will be the closest to what already was.
template <class T>
void AxialFrame<T>::look_direction(const Vector3<T>& dir)
{
    z_axis = dir;
    z_axis.normalize();

    // y & z axis too parallel?
    if (z_axis.cross(y_axis).len2() < 0.001)
    {
        // can't compute x-axis from z & y. have to compute y-axis first
        y_axis = x_axis.cross(z_axis);
        x_axis = z_axis.cross(y_axis);
        x_axis.normalize();
        y_axis.normalize();
    }
    else
    {
        x_axis = z_axis.cross(y_axis);
        y_axis = x_axis.cross(z_axis);
        x_axis.normalize();
        y_axis.normalize();
    }
}

// turn axial-frame to look in direction_vector, up points to up_hint.
template <class T>
void AxialFrame<T>::look_direction2(const Vector3<T>& dir, const Vector3<T>& up_hint)
{
    z_axis = dir;
    z_axis.normalize();

    if (z_axis.cross(up_hint).len2() < 0.001)
    {
        // our view-direction (z-axis) is the same as the up_hint_vector.
        // so we have two choices:
        //    1. generate a random down-looking aframe
        //    2. try to preserve orientation from the previous frame
        if (z_axis.cross(y_axis).len2() < 0.001)
        {
            // choice 2 failed. generate a random aframe
            look_direction_random(z_axis);
            return;
        }
    }
    else
        y_axis = up_hint;

    x_axis = z_axis * y_axis;
    y_axis = x_axis * z_axis;
    x_axis.normalize();
    y_axis.normalize();
}


// generate an axial frame from the given vector. z-axis points in
// the given direction. x-axis and y-axis are random, but guaranteed
// to be orthonormal to every other axis.
template <class T>
void AxialFrame<T>::look_direction_random(const Vector3<T>& dir)
{
    z_axis = dir;
    z_axis.normalize();

    // guard for collinearity

    //if x*x + y*y < x*x + z*z: a.x_axis.set((0., 1., 0.))
    if (z_axis.y * z_axis.y < z_axis.z * z_axis.z)
        // near the z-axis. have to use y-axis
        x_axis = Vector3<T>(0., 1., 0.);
    else
        // near the y-axis. have to use z-axis
        x_axis = Vector3<T>(0., 0., 1.);

    // make the axial frame orthogonal.

    y_axis = x_axis * z_axis;
    x_axis = z_axis * y_axis;
    x_axis.normalize();
    y_axis.normalize();
}

template <class T>
void AxialFrame<T>::remove_xtilt(const Vector3<T>& up_vector)
{
    if (z_axis.cross(up_vector).len2() > 0.001)
    {
        Vector3<T> new_x_axis(z_axis.cross(up_vector));
        new_x_axis.normalize();

        if (x_axis.dot(new_x_axis) < 0.)
            x_axis = -new_x_axis;
        else
            x_axis = new_x_axis;

        y_axis = x_axis.cross(z_axis);
        y_axis.normalize();
    }
    else
    {
        // if smaller than 0.001, then we already ARE almost looking down.. but it never
        // hurts to try fix things that are already working.
        Vector3<T> new_x_axis(y_axis.cross(up_vector));

        if (x_axis.dist2(new_x_axis) > x_axis.len2())
            x_axis = -new_x_axis;
        else
            x_axis = new_x_axis;

        z_axis = y_axis.cross(x_axis);
        z_axis.normalize();
    }
}

template <class T>
void AxialFrame<T>::normalize()
{
    x_axis.normalize();
    y_axis.normalize();
    z_axis.normalize();
}

template <class T>
void AxialFrame<T>::orthogonalize_x()
{
    z_axis = y_axis.cross(x_axis);
    y_axis = x_axis.cross(z_axis);
    normalize();
}

template <class T>
void AxialFrame<T>::orthogonalize_y()
{
    x_axis = z_axis.cross(y_axis);
    z_axis = y_axis.cross(x_axis);
    normalize();
}

template <class T>
void AxialFrame<T>::orthogonalize_z()
{
    y_axis = x_axis.cross(z_axis);
    x_axis = z_axis.cross(y_axis);
    normalize();
}


#endif // __AXIAL_FRAME_T_H__


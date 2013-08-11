#ifndef __COORDINATE_SYSTEM_T_H__
#define __COORDINATE_SYSTEM_T_H__


#include "vector3_t.h"
#include "axial_frame_t.h"


// orthogonal coordinate system
template <class T>
class CoordinateSystem
{
public:

    AxialFrame<T> a_frame;
    Vector3<T>    pos;

    inline CoordinateSystem() {}
    CoordinateSystem(const AxialFrame<T>& af, const Vector3<T>& pos_):
            a_frame(af), pos(pos_) { }
    CoordinateSystem(const CoordinateSystem& ocs):
            a_frame(ocs.a_frame), pos(ocs.pos) { }

    // project v out of the coordinate-system. to world-space
    inline Vector3<T> projv_out(const Vector3<T>& v) const;
    // project v into the coordinate-system. to object-space
    inline Vector3<T> projv_in(const Vector3<T>& v) const;

    // project coordinate-system out of *this. to world-space
    CoordinateSystem proj_out(const CoordinateSystem& ocs) const;
    // project coordinate-system into *this. to object-space
    CoordinateSystem proj_in(const CoordinateSystem& ocs) const;

    void reset();
    void rotate(const Vector3<T>& point, const Vector3<T>& axis, T angle);

    //def get_opengl_matrix(self):
    //def get_opengl_matrix2(self):

private:
};


//////////////////////////////////////////////////////////////////////////////


template <class T>
inline Vector3<T> CoordinateSystem<T>::projv_out(const Vector3<T>& v) const
{
    return pos + a_frame.projv_out(v);
}

template <class T>
inline Vector3<T> CoordinateSystem<T>::projv_in(const Vector3<T>& v) const
{
    return a_frame.projv_in(v - pos);
}

template <class T>
CoordinateSystem<T> CoordinateSystem<T>::proj_out(const CoordinateSystem<T>& ocs) const
{
    return CoordinateSystem<T>(a_frame.proj_out(ocs.a_frame), projv_out(ocs.pos));
}

template <class T>
CoordinateSystem<T> CoordinateSystem<T>::proj_in(const CoordinateSystem<T>& ocs) const
{
    return CoordinateSystem<T>(a_frame.proj_in(ocs.a_frame), projv_in(ocs.pos));
}

template <class T>
void CoordinateSystem<T>::reset()
{
    a_frame.reset();
    pos = Vector3<T>(0., 0., 0.);
}

template <class T>
void CoordinateSystem<T>::rotate(const Vector3<T>& point, const Vector3<T>& axis, T angle)
{
    a_frame.rotate(axis, angle);
    Vector3<T> newpos = pos - point;
    newpos.rotate(axis, angle);
    pos = newpos + point;
}

/*
def get_opengl_matrix(self):
    """
    multiplication projects things OUT OF this coordinate system.
    """

    m = self.a_frame.get_opengl_matrix()
    v = self.pos

    # this method first rotates, then translates..
    # TODO: negative is correct?
    m[0+4+4+4] = -v[0]
    m[1+4+4+4] = -v[1]
    m[2+4+4+4] = -v[2]

    return m


def get_opengl_matrix2(self):
    """
    multiplication projects things INTO this coordinate system.
    """

    m = self.a_frame.get_opengl_matrix2()
    v = self.pos

    m[0+4+4+4] = v[0]
    m[1+4+4+4] = v[1]
    m[2+4+4+4] = v[2]

    return m
*/

#endif // __COORDINATE_SYSTEM_T_H__


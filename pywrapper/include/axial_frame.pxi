include "vector3.pxi"


cdef extern from "axial_frame_t.h":
    cdef cppclass AxialFrame[T]:
        Vector3[T] x_axis
        Vector3[T] y_axis
        Vector3[T] z_axis

        AxialFrame()
        AxialFrame(Vector3[T]& xa, Vector3[T]& ya, Vector3[T]& za)
        AxialFrame(AxialFrame& af)

        void reset()

        Vector3[T] projv_out(Vector3[T]& v)
        Vector3[T] projv_in(Vector3[T]& v)

        AxialFrame proj_out(AxialFrame& af)
        AxialFrame proj_in(AxialFrame& af)

        void rotate(Vector3[T]& v, T angle)

        void look_direction(Vector3[T]& dir)
        void look_direction2(Vector3[T]& dir, Vector3[T]& up_hint)
        void look_direction_random(Vector3[T]& dir)

        void remove_xtilt(Vector3[T]& up_vector)

        void normalize()

        void orthogonalize_x()
        void orthogonalize_y()
        void orthogonalize_z()


ctypedef AxialFrame[double] AxialFrame3_test

def test_axial_frame():
    print "axialframe cython test:"

    # this should work, but cython has a bug:
    #    axial_frame.pxi:56:42: Cannot assign type 'Vector3d' to 'Vector3<double>'
    #
    #ctypedef Vector3[double] Vector3d
    #cdef Vector3d v1 = Vector3d(1., 2., 3.)
    #cdef Vector3d v2 = Vector3d(4., 5., 6.)
    #cdef Vector3d v3 = Vector3d(7., 8., 9.)
    #cdef AxialFrame3d a = AxialFrame3d(v1, v2, v3)

    # this works. but it's annoying, because the Vector3d typedef can't be used:
    #
    #cdef Vector3[double] v1 = Vector3d(1., 2., 3.)
    #cdef Vector3[double] v2 = Vector3d(4., 5., 6.)
    #cdef Vector3[double] v3 = Vector3d(7., 8., 9.)
    #cdef AxialFrame3d a = AxialFrame3d(v1, v2, v3)

    # another tip: for pointer dererefencing use ptr[0] instead of *ptr:
    #
    #cdef Vector3[double] v1 = new Vector3d(1., 2., 3.)
    #cdef Vector3[double] v2 = new Vector3d(4., 5., 6.)
    #cdef Vector3[double] v3 = new Vector3d(7., 8., 9.)
    #cdef AxialFrame3d *a = new AxialFrame3d(v1[0], v2[0], v3[0])
    #del v3
    #del v2
    #del v1
    #del a

    cdef Vector3[double] v1 = Vector3[double](1., 2., 3.)
    cdef Vector3[double] v2 = Vector3[double](4., 5., 6.)
    cdef Vector3[double] v3 = Vector3[double](7., 8., 9.)
    cdef AxialFrame3_test a  = AxialFrame3_test(v1, v2, v3)
    cdef AxialFrame3_test a1 = AxialFrame3_test(a)
    a.orthogonalize_z()

    print "a values:", a.x_axis.x, a.x_axis.y, a.x_axis.z


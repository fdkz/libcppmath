include "axial_frame.pxi"


cdef extern from "coordinate_system_t.h":
    cdef cppclass CoordinateSystem[T]:
        AxialFrame[T] a_frame
        Vector3[T]    pos

        CoordinateSystem()
        CoordinateSystem(AxialFrame& af, Vector3& pos_)
        CoordinateSystem(CoordinateSystem& ocs)

        Vector3[T] projv_out(Vector3[T]& v)
        Vector3[T] projv_in(Vector3[T]& v)

        CoordinateSystem proj_out(CoordinateSystem& ocs)
        CoordinateSystem proj_in(CoordinateSystem& ocs)

        void reset()
        void rotate(Vector3[T]& point, Vector3[T]& axis, T angle)


ctypedef CoordinateSystem[double] CoordinateSystem_test

def test_coordinate_system():
    print "coordinatesystem cython test:"

    cdef Vector3[double] v1 = Vector3[double](1., 2., 3.)
    cdef Vector3[double] v2 = Vector3[double](4., 5., 6.)
    cdef Vector3[double] v3 = Vector3[double](7., 8., 9.)
    cdef CoordinateSystem_test c = CoordinateSystem_test()
    c.reset()

    print "c values:", c.pos.x, c.pos.y, c.pos.z


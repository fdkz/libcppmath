from libcpp cimport bool as cbool


cdef extern from "vector3_t.h":
    cdef cppclass Vector3[T]:
        T x
        T y
        T z
        T v[]

        Vector3()
        Vector3(T x, T y, T z)
        Vector3(Vector3& v)

        cbool operator == (Vector3& v)
        cbool operator != (Vector3& v)

        Vector3  operator +  (Vector3& v)
        # Overloading operator '+=' not yet supported.
        #Vector3& operator += (Vector3& v)

        Vector3  operator -  (Vector3& v)
        Vector3  operator -  ()
        # Overloading operator '-=' not yet supported.
        #Vector3& operator -= (Vector3& v)

        Vector3  operator *  (Vector3& v)
        # Overloading operator '*=' not yet supported.
        #Vector3& operator *= (Vector3& v)
        Vector3  operator *  (T d)
        # Overloading operator '*=' not yet supported.
        #Vector3& operator *= (T d)

        Vector3  operator /  (T d)
        # Overloading operator '/=' not yet supported.
        #Vector3& operator /= (T d)

        void rotate(Vector3& normal, T angle)
        void normalize()

        T len  ()
        T len2 ()
        T angle(Vector3& v)
        T dist (Vector3& v)
        T dist2(Vector3& v)

        Vector3 cross(Vector3& v)
        T       dot(Vector3& v)
        Vector3 reflection(Vector3& plane_normal)

        cbool is_unit()


ctypedef Vector3[double] Vector3_test

def test_vector3():

    print "vector3 test"
    cdef Vector3_test *v = new Vector3_test(1., 2., 3.)

    print "v values:", v.x, v.y, v.z
    del v


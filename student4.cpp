#include "Student.h"

#include <osg/Plane>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

osg::Vec3d projectOnPlane(const osg::Vec3d &vector, const osg::Vec3d &planeNormal)
{
    auto pp = vector - planeNormal * (vector * planeNormal);
    // get rid of round-off error
    pp[0] = std::abs(pp[0]) < 1e-6 ? 0 : pp[0];
    pp[1] = std::abs(pp[1]) < 1e-6 ? 0 : pp[1];
    pp[2] = std::abs(pp[2]) < 1e-6 ? 0 : pp[2];

    return pp;
}

double dot(const osg::Vec3d &u, const osg::Vec3d &v)
{
    auto d = u * v;

    // handle rounding errors
    d = std::clamp(d, -1.0, 1.0);

    return d;
}

osg::Vec3d cross(const osg::Vec3d &u, const osg::Vec3d &v)
{
    return u ^ v;
}

void normalize(osg::Vec3d &v)
{
    v.normalize();
}

osg::Vec3d normalized(const osg::Vec3d &v)
{
    osg::Vec3d u(v);
    u.normalize();

    return u;
}

osg::Matrix inverse(const osg::Matrix &m)
{
    return osg::Matrix::inverse(m);
}

#if USE_STUDENT_SOLUTION
osg::Matrix getScaleMatrix(const osg::Vec3d &scale)
{
    // Task 1 - fill in the scale matrix

    osg::Matrix m;

    m(0, 0) = scale[0];
    m(1, 1) = scale[1];
    m(2, 2) = scale[2];

    return m;
}

osg::Matrix getTranslationMatrix(const osg::Vec3d &translation)
{
    // Task 2 - fill in the translation matrix

    osg::Matrix m;

    m(3, 0) = translation[0];
    m(3, 1) = translation[1];
    m(3, 2) = translation[2];

    return m;
}

osg::Matrix rotateAroundX(double angle)
{

    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    // Task 3 - fill in the rotation matrix around X axis

    osg::Matrix m;

    m(1, 1) = cosa;
    m(1, 2) = -sina;
    m(2, 1) = sina;
    m(2, 2) = cosa;

    return m;
}

osg::Matrix rotateAroundY(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    // Task 4 - fill in the rotation matrix around Y axis

    osg::Matrix m;

    m(0, 0) = cosa;
    m(0, 2) = sina;
    m(2, 0) = -sina;
    m(2, 2) = cosa;

    return m;
}

osg::Matrix rotateAroundZ(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    // Task 5 - fill in the rotation matrix around Z axis

    osg::Matrix m;

    m(0, 0) = cosa;
    m(0, 1) = -sina;
    m(1, 0) = sina;
    m(1, 1) = cosa;

    return m;
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    if (u.length2() == 0 || v.length2() == 0)
    {
        return 0.0;
    }

    // Task 6
    // Normalize both vectors - method normalize()
    // Compute cosine of the angle between the vectors using the dot product - function dot
    // Return the angle using arcus cosine - function std::acos()

    normalize(u);
    normalize(v);

    double cosAngle = dot(u, v);
    cosAngle = std::clamp(cosAngle, -1.0, 1.0);

    return std::acos(cosAngle);
}

osg::Matrix getRotationMatrix(const osg::Vec3d &fromVector, const osg::Vec3d &toVector)
{
    const osg::Vec3d xAxis(1, 0, 0);
    const osg::Vec3d yAxis(0, 1, 0);
    const osg::Vec3d zAxis(0, 0, 1);

    // Task 7 - compute the rotation matrix around arbitrary axis

    // Compute the angle between input vectors - function angleBetweenVectors

    double angle = angleBetweenVectors(fromVector, toVector);

    // 7a - compute the rotation axis using the cross product - function cross

    if (angle == 0.0)
    {
        return osg::Matrix::identity();
    }

    osg::Vec3d axis = cross(fromVector, toVector);
    normalize(axis);

    // 7b - project rotation axis into XY plane

    osg::Vec3d projAxis = projectOnPlane(axis, zAxis);

    // 7c - compute the angle between projected rotation axis and X axis

    double angleXY = angleBetweenVectors(projAxis, xAxis);

    // 7d - compute rotation around Z axis

    osg::Matrix rotZ;
    rotZ.makeRotate(angle, axis);

    // Compute projection of rotation axis onto XZ plane

    osg::Vec3d projAxisXZ = projectOnPlane(axis, yAxis);

    // 7e - compute angle between (projection of) rotation axis in XZ and X axis and compute rotation matrix around Y corrently, hint - for Pi use M_PI

    double angleXZ = angleBetweenVectors(projAxisXZ, xAxis);

    osg::Matrix rotY;
    if (projAxisXZ.y() >= 0)
    {
        rotY.makeRotate(angleXZ, yAxis);
    }
    else
    {
        rotY.makeRotate(-angleXZ, yAxis);
    }

    // 7f - compute the desired rotation around X axis

    osg::Matrix rotX;
    rotX.makeRotate(angleXY, xAxis);

    // 7g - compute and return the final rotation - for matrix inverse use function inverse
    osg::Matrix result = rotZ * rotY * rotX;
    result.invert(result);

    return result;
}
#else

osg::Matrix getScaleMatrix(const osg::Vec3d &scale)
{
    return osg::Matrix::scale(scale);
}

osg::Matrix getTranslationMatrix(const osg::Vec3d &translation)
{
    return osg::Matrix::translate(translation);
}

osg::Matrix getRotationMatrix(const osg::Vec3d &fromVector, const osg::Vec3d &toVector)
{
    osg::Quat q;
    q.makeRotate(fromVector, toVector);

    return osg::Matrix(q);
}

osg::Matrix rotateAroundX(double angle)
{
    return {};
}

osg::Matrix rotateAroundY(double angle)
{
    return {};
}

osg::Matrix rotateAroundZ(double angle)
{
    return {};
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    return 0.0;
}
#endif
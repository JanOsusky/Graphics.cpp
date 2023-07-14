# Five Graphics Exercises in C++

This repository contains a collection of five graphics exercises implemented in C++. Each exercise focuses on different aspects of graphics programming and includes various functions and algorithms for handling vectors, matrices, transformations, and trajectory calculations.

## Exercise 1: Point2d Operations

The first exercise provides operations for manipulating 2D points using the `Point2d` structure. The available functions include:

- `addPoint2d(const Point2d* a, const Point2d* b, Point2d* result)`: Adds two `Point2d` objects and stores the result in the `result` object.
- `mullPoint2d(double val, const Point2d* p, Point2d* result)`: Multiplies a `Point2d` object by a scalar value and stores the result in the `result` object.

## Exercise 2: Control Points Initialization

The second exercise focuses on initializing control points for upper and lower trajectories. The functions provided are:

- `initControlPointsUp(S_Vector** points, int offset_x, int offset_y)`: Initializes control points for the upper trajectory in the drawing window.
- `initControlPointsDown(S_Vector** points, int offset_x, int offset_y)`: Initializes control points for the lower trajectory in the drawing window.

## Exercise 3: Bezier Cubic Calculation

Exercise 3 introduces the calculation of Bezier cubic curves. The functions available are:

- `bezierCubic(const Point2d* P1, const Point2d* P2, const Point2d* P3, const Point2d* P4, int precision, S_Vector* trajectory_points)`: Calculates a Bezier cubic curve using four control points.
- `bezierCubicsTrajectory(int precision, const S_Vector* control_points, S_Vector* trajectory_points)`: Calculates a trajectory composed of multiple Bezier cubic curves.

## Exercise 4: Transformation Matrices

The fourth exercise focuses on transformation matrices. It includes functions for creating scale, translation, and rotation matrices around different axes.

- `getScaleMatrix(const osg::Vec3d& scale)`: Returns a scale matrix based on the provided scale vector.
- `getTranslationMatrix(const osg::Vec3d& translation)`: Returns a translation matrix based on the provided translation vector.
- `rotateAroundX(double angle)`: Returns a rotation matrix around the X-axis based on the given angle.
- `rotateAroundY(double angle)`: Returns a rotation matrix around the Y-axis based on the given angle.
- `rotateAroundZ(double angle)`: Returns a rotation matrix around the Z-axis based on the given angle.
- `getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector)`: Returns a rotation matrix that rotates from one vector to another.

## Exercise 5: Vector Operations

The final exercise provides operations for vector manipulation. It includes functions for vector projection, dot product, cross product, vector normalization, and angle calculation between vectors.

- `projectOnPlane(const osg::Vec3d& vector, const osg::Vec3d& planeNormal)`: Projects a vector onto a plane defined by its normal vector.
- `dot(const osg::Vec3d& u, const osg::Vec3d& v)`: Calculates the dot product of two vectors.
- `cross(const osg::Vec3d& u, const osg::Vec3d& v)`: Calculates the cross product of two vectors.
- `normalize(osg::Vec3d& v)`: Normalizes a vector in place.
- `normalized(const osg::Vec3d& v)`: Returns a normalized version of a vector.
- `angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)`: Calculates the angle between two vectors.

These exercises cover a range of graphics programming concepts and provide practical implementations for each topic. Refer to the documentation and guidelines provided within the code for more details on how to use these functions.


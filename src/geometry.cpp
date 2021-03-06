#include "geometry.h"

Vector2D::Vector2D(float x, float y)
    :   x(x), y(y)
{
}

Vector2D::Vector2D()
{
    x = 0;
    y = 0;
}

Vector2D Vector2D::operator+(const Vector2D v) const {
    return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D v) const {
    return Vector2D(x - v.x, y - v.y);
}

Vector2D Vector2D::operator-() const {
    return Vector2D(-x, -y);
}

Vector2D Vector2D::operator*(const Vector2D v) const {
    return Vector2D(x*v.x, y*v.y);
}

Vector2D Vector2D::operator*(const float s) const {
    return Vector2D(s * x, s * y);
}

Vector2D Vector2D::operator/(const float d) const {
    return Vector2D(x/d, y/d);
}

std::ostream & operator<<(std::ostream & Str, const Vector2D& v) {
    return Str << "(" << v.x << ", " << v.y << ")\n";
}

QDebug operator<<(QDebug d, const Vector2D &v) {
    return d << "(" << v.x << ", " << v.y << ")\n";
}

Vector3D::Vector3D(float x, float y, float z)
    : x(x), y(y), z(z)
{
}

Vector3D::Vector3D(Vector2D v)
    :   x(v.x), y(v.y)
{
}

Vector3D::Vector3D()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3D Vector3D::operator+(const Vector3D v) const {
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::operator-(const Vector3D v) const {
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator-() const {
    return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator*(const Vector3D v) const {
    return Vector3D(x*v.x, y*v.y, z*v.z);
}

Vector3D Vector3D::operator*(const float s) const {
    return Vector3D(s * x, s * y, s * z);
}

Vector3D Vector3D::operator/(const float d) const {
    return Vector3D(x/d, y/d, z/d);
}

//commutativity
Vector2D operator*(const float s, const Vector2D v) {
    return v * s;
}

//commutativity
Vector3D operator*(const float s, const Vector3D v) {
    return v * s;
}

//commutativity
Vector4D operator*(const float s, const Vector4D v) {
    return v * s;
}

float Vector3D::norm()
{
    return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalized() {
    return Vector3D(x / norm(), y / norm(), z / norm());
}

float Vector3D::dot_prod(Vector3D a, Vector3D b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3D Vector3D::cross_prod(Vector3D a, Vector3D b)
{
    float x = a.y * b.z - b.y * a.z;
    float y = -(a.x * b.z - b.x * a.z);
    float z = a.x * b.y - b.x * a.y;
    return Vector3D(x, y, z);
}

Vector3D Vector3D::reflect(Vector3D vec, Vector3D normal) {
    return vec - 2*Vector3D::dot_prod (vec, normal)*normal;
}

Vector3D Vector3D::interpolate(Vector3D a, Vector3D b) {
    return 0.5* a + 0.5 * b;
}

Vector3D Vector3D::zero() {
    return Vector3D(0,0,0);
}

Vector3D Vector3D::up() {
    return Vector3D(0,1,0);
}

Vector3D Vector3D::right() {
    return Vector3D(1,0,0);
}

Vector3D Vector3D::forward() {
    return Vector3D(0,0,1);
}

Vector2D Vector3D::asVector2D () {
    return Vector2D(x, y);
}

std::ostream & operator<<(std::ostream & Str, const Vector3D& v) {
    return Str << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

QDebug operator<<(QDebug d, const Vector3D &v) {
    return d << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

Vector4D::Vector4D(float x, float y, float z, float w)
    :   x(x), y(y), z(z), w(w)
{
}

Vector2D Vector4D::asVector2D () {
    return Vector2D(x, y);
}

Vector3D Vector4D::asVector3D () {
    return Vector3D(x, y, z);
}

Vector4D::Vector4D()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4D::Vector4D(Vector2D v)
    :   Vector4D(Vector3D(v))
{
}

Vector4D::Vector4D(Vector3D v)
    :   x(v.x), y(v.y), z(v.z), w(0)
{
}

Vector4D Vector4D::operator+(const Vector4D v) const {
    return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4D Vector4D::operator-(const Vector4D v) const {
    return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4D Vector4D::operator-() const {
    return Vector4D(-x, -y, -z, -w);
}

Vector4D Vector4D::operator*(const Vector4D v) const {
    return Vector4D(x*v.x, y*v.y, z*v.z, w*v.w);
}

Vector4D Vector4D::operator*(const float s) const {
    return Vector4D(s * x, s * y, s * z, s * w);
}

Vector4D Vector4D::operator/(const float d) const {
    return Vector4D(x/d, y/d, z/d, w/d);
}

std::ostream & operator<<(std::ostream & Str, const Vector4D& v) {
    return Str << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
}

QDebug operator<<(QDebug d, const Vector4D &v) {
    return d << "(" << v.x << ", " << v.y << ", " << v.z << ", " <<  v.w << ")\n";
}

//Vector interpolation
void interpolate(const Vector2D a, const Vector2D b, std::vector<Vector2D> &result) {
    int N = result.size();
    Vector2D step = (b - a) / fmax(N - 1, 1);
    Vector2D current = a;
    for( int i=0; i<N; ++i ) {
        result[i] = current;
        current = current + step;
    }
}


Matrix4x4::Matrix4x4() {

}

Matrix4x4 Matrix4x4::identity() {
    Matrix4x4 res;
    for(int i = 0; i < 4; i++) {
        res(i,i) = 1;
    }
    return res;
}

Matrix4x4 Matrix4x4::translation(const Vector3D v) {
    Matrix4x4 t = Matrix4x4::identity ();
    t(0, 3) = v.x;
    t(1, 3) = v.y;
    t(2, 3) = v.z;

    return t;
}

Matrix4x4 Matrix4x4::rotation(const Vector3D v) {
    Matrix4x4 tx = Matrix4x4::identity ();
    tx(1, 1) = cosf(v.x);
    tx(1, 2) = -sinf(v.x);
    tx(2, 1) = sinf(v.x);
    tx(2, 2) = cosf(v.x);

    Matrix4x4 ty = Matrix4x4::identity();
    ty(0, 0) = cosf(v.y);
    ty(0, 2) = sinf(v.y);
    ty(2, 0) = -sinf(v.y);
    ty(2, 2) = cosf(v.y);

    Matrix4x4 tz = Matrix4x4::identity();
    tz(0, 0) = cosf(v.z);
    tz(0, 1) = -sinf(v.z);
    tz(1, 0) = sinf(v.z);
    tz(1, 1) = cosf(v.z);

    return tx * ty * tz;
}

bool Matrix4x4::inverse(Matrix4x4 mat, Matrix4x4 &out) {
    Matrix4x4 inv;
    float m[16];
    //    std::copy(mat.elements, m);
    std::copy(std::begin(mat.elements), std::end(mat.elements), std::begin(m));
    double det = 0.0;
    int i;

    inv(0,0) = m[5]  * m[10] * m[15] -
            m[5]  * m[11] * m[14] -
            m[9]  * m[6]  * m[15] +
            m[9]  * m[7]  * m[14] +
            m[13] * m[6]  * m[11] -
            m[13] * m[7]  * m[10];

    inv(1,0) = -m[4]  * m[10] * m[15] +
            m[4]  * m[11] * m[14] +
            m[8]  * m[6]  * m[15] -
            m[8]  * m[7]  * m[14] -
            m[12] * m[6]  * m[11] +
            m[12] * m[7]  * m[10];

    inv(2,0) = m[4]  * m[9] * m[15] -
            m[4]  * m[11] * m[13] -
            m[8]  * m[5] * m[15] +
            m[8]  * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

    inv(3,0) = -m[4]  * m[9] * m[14] +
            m[4]  * m[10] * m[13] +
            m[8]  * m[5] * m[14] -
            m[8]  * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

    inv(0,1) = -m[1]  * m[10] * m[15] +
            m[1]  * m[11] * m[14] +
            m[9]  * m[2] * m[15] -
            m[9]  * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

    inv(1,1) = m[0]  * m[10] * m[15] -
            m[0]  * m[11] * m[14] -
            m[8]  * m[2] * m[15] +
            m[8]  * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

    inv(2,1) = -m[0]  * m[9] * m[15] +
            m[0]  * m[11] * m[13] +
            m[8]  * m[1] * m[15] -
            m[8]  * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

    inv(3,1) = m[0]  * m[9] * m[14] -
            m[0]  * m[10] * m[13] -
            m[8]  * m[1] * m[14] +
            m[8]  * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

    inv(0,2) = m[1]  * m[6] * m[15] -
            m[1]  * m[7] * m[14] -
            m[5]  * m[2] * m[15] +
            m[5]  * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

    inv(1,2) = -m[0]  * m[6] * m[15] +
            m[0]  * m[7] * m[14] +
            m[4]  * m[2] * m[15] -
            m[4]  * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

    inv(2,2) = m[0]  * m[5] * m[15] -
            m[0]  * m[7] * m[13] -
            m[4]  * m[1] * m[15] +
            m[4]  * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

    inv(3,2) = -m[0]  * m[5] * m[14] +
            m[0]  * m[6] * m[13] +
            m[4]  * m[1] * m[14] -
            m[4]  * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

    inv(0,3) = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

    inv(1,3) = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

    inv(2,3) = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

    inv(3,3) = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

    det = m[0] * inv(0,0) + m[1] * inv(1,0) + m[2] * inv(2,0) + m[3] * inv(3,0);

    if (det == 0) {
        return false;
    }

    det = 1.0 / det;

    for (i = 0; i < 4 ; i++) {
        for (int j = 0; j < 4; j++) {
            out(i, j) = inv(i, j) * det;
        }
    }
    return true;
}

bool Matrix4x4::transpose(Matrix4x4 mat, Matrix4x4 &out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out(j, i) = mat(i, j);
        }
    }
    return true;
}

float& Matrix4x4::operator()(const int i, const int j) {
    return elements[i * 4 + j];
}

Vector3D Matrix4x4::operator*(const Vector3D v) {
    Vector3D out;
    out.x   = v.x * (*this)(0,0) + v.y * (*this)(0,1) + v.z * (*this)(0,2) + /* v.z = 1 */ (*this)(0,3);
    out.y   = v.x * (*this)(1,0) + v.y * (*this)(1,1) + v.z * (*this)(1,2) + /* v.z = 1 */ (*this)(1,3);
    out.z   = v.x * (*this)(2,0) + v.y * (*this)(2,1) + v.z * (*this)(2,2) + /* v.z = 1 */ (*this)(2,3);
    float w = v.x * (*this)(3,0) + v.y * (*this)(3,1) + v.z * (*this)(3,2) + /* v.z = 1 */ (*this)(3,3);

    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1.0) {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
    return out;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 m) {
    Matrix4x4 res;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            float sum = 0;
            for(int k = 0; k < 4; k++) {
                float t1 = (*this)(i, k);
                float t2 = m(k, j);
                sum += t1 * t2;
            }
            res(i, j) = sum;
        }
    }
    return res;
}

std::ostream& operator<<(std::ostream& Str, Matrix4x4& m) {
    for(int i = 0; i < 4; i++) {
        Str << "\n(";
        for(int j = 0; j < 4; j++) {
            Str << m(i, j) << " ";
        }
    }
    Str << ")\n";
    return Str;
}

QDebug operator<<(QDebug d, Matrix4x4 &m) {

    for(int i = 0; i < 4; i++) {
        d << "\n(";
        for(int j = 0; j < 4; j++) {
            d << m(i, j) << " ";
        }
    }
    d << ")\n";
    return d;
}

Ray::Ray()
{
    origin = Vector3D(0,0,0);
    direction = Vector3D(0,1,0);
}

Ray::Ray(Vector3D origin, Vector3D direction)
    :   origin(origin), direction(direction)
{
    Vector3D inv_direction = Vector3D(1/direction.x, 1/direction.y, 1/direction.z);
    sign.x = (inv_direction.x < 0)? -1 : 1;
    sign.y = (inv_direction.y < 0)? -1 : 1;
    sign.z = (inv_direction.z < 0)? -1 : 1;

}

Vector3D Ray::pointOnRay(float dist) {
    return origin + dist * direction;
}

#include "DEpch.h"
#include "Quaternion.h"
#include <cmath>

namespace DeltaEngine
{
    const float pi = 3.14159265358979323846f;

    Quaternion::Quaternion()
        : x{ 0 }, y{ 0 }, z{ 0 }, w{ 1 } {}
    Quaternion::Quaternion(float q0, float q1, float q2, float q3)
        : x{ q0 }, y{ q1 }, z{ q2 }, w{ q3 } {}

    Quaternion Quaternion::identity{ 0, 0, 0, 1 };

    Quaternion Quaternion::AngleAxis(const float angle, const Vector3 axis)
    {
        Quaternion q;
        Vector3 v = axis;
        v.Normalize();
        v *= sinf(angle / 360 * pi);
        //printf("%f, %f, %f, %f\n", q.x, q.y, q.z, q.w);

        return Quaternion{ v.x, v.y, v.z, cosf(angle / 360 * pi) };
    }

    //assumes rotation sequencing of yaw, pitch, then roll, or Body 3-2-1
    Quaternion Quaternion::EulerAnglesToQuaternion(Vector3 eulerAngles)
    {
        float cos_y = cosf(eulerAngles.z * 0.5f);
        float sin_y = sinf(eulerAngles.z * 0.5f);
        float cos_p = cosf(eulerAngles.y * 0.5f);
        float sin_p = sinf(eulerAngles.y * 0.5f);
        float cos_r = cosf(eulerAngles.x * 0.5f);
        float sin_r = sinf(eulerAngles.x * 0.5f);

        return Quaternion
        {
            sin_r * cos_p * cos_y - cos_r * sin_p * sin_y,
            cos_r * sin_p * cos_y + sin_r * cos_p * sin_y,
            cos_r * cos_p * sin_y - sin_r * sin_p * cos_y,
            cos_r * cos_p * cos_y + sin_r * sin_p * sin_y
        };
    }

    Quaternion Quaternion::EulerAnglesToQuaternion(float x, float y, float z)
    {
        return EulerAnglesToQuaternion(Vector3(x, y, z));
    }

    Vector3 Quaternion::QuaternionToEulerAngles(Quaternion q)
    {
        Vector3 eulerAngles;

        // roll (x-axis rotation)
        float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
        float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
        eulerAngles.x = std::atan2f(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        float sinp = 2 * (q.w * q.y - q.z * q.x);
        if (std::abs(sinp) >= 1)
            eulerAngles.y = std::copysign(pi / 2, sinp); // use 90 degrees if out of range
        else
            eulerAngles.y = std::asinf(sinp);

        // yaw (z-axis rotation)
        float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
        float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
        eulerAngles.z = std::atan2f(siny_cosp, cosy_cosp);

        return eulerAngles;
    }

    Quaternion Quaternion::operator*=(Quaternion rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    Quaternion operator*(Quaternion lhs, Quaternion rhs)
    {
        return lhs *= rhs;
    }
}

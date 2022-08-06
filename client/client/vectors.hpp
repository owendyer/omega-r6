#pragma once

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>

#define M_PI 3.14159265358979323846264338327950288419716939937510

class vector2
{
public:
	vector2() : x(0.f), y(0.f)
	{

	}

	vector2(float _x, float _y) : x(_x), y(_y)
	{

	}
	~vector2()
	{

	}

	float x;
	float y;

	bool operator==(const vector2& vector) const;
	bool operator!=(const vector2& vector) const;

	bool equals(vector2 other) {
		return (x == other.x && y == other.y);
	}

};

inline bool vector2::operator==(const vector2& vector) const
{
	return x == vector.x && y == vector.y;
}

inline bool vector2::operator!=(const vector2& vector) const
{
	return x != vector.x || y != vector.y;
}

class vector3
{
public:
	float x;
	float y;
	float z;

	vector3();
	vector3(float x, float y, float z);

	vector3 operator+(const vector3& vector) const;
	vector3 operator-(const vector3& vector) const;
	vector3 operator-() const;
	vector3 operator*(float number) const;
	vector3 operator/(float number) const;

	vector3& operator+=(const vector3& vector);
	vector3& operator-=(const vector3& vector);
	vector3& operator*=(float number);
	vector3& operator/=(float number);

	bool operator==(const vector3& vector) const;
	bool operator!=(const vector3& vector) const;

	inline float dot(const vector3& vector)
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	inline float distance(const vector3& vector)
	{
		float xCoord = vector.x - x;
		float yCoord = vector.y - y;
		float zCoord = vector.z - z;
		return sqrtf((xCoord * xCoord) + (yCoord * yCoord) + (zCoord * zCoord));
	}

	inline float length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	inline void rotate2D(float angle)
	{
		// use local variables to find transformed components
		float Vx1 = cosf(angle) * x - sinf(angle) * y;
		float Vy1 = sinf(angle) * x + cosf(angle) * y;
		// store results thru the pointer
		x = Vx1;
		y = Vy1;

		return;
	}

	inline void clamp()
	{
		if (x > 75.f) x = 75.f;
		else if (x < -75.f) x = -75.f;
		if (z < -180) z += 360.0f;
		else if (z > 180) z -= 360.0f;

		y = 0.f;
	}
};

inline bool vector3::operator==(const vector3& vector) const
{
	return x == vector.x && y == vector.y && z == vector.z;
}

inline bool vector3::operator!=(const vector3& vector) const
{
	return x != vector.x || y != vector.y || z != vector.z;
}

inline vector3 vector3::operator+(const vector3& vector) const
{
	return vector3(x + vector.x, y + vector.y, z + vector.z);
}

inline vector3 vector3::operator-(const vector3& vector) const
{
	return vector3(x - vector.x, y - vector.y, z - vector.z);
}

inline vector3 vector3::operator-() const
{
	return vector3(-x, -y, -z);
}

inline vector3 vector3::operator*(float number) const
{
	return vector3(x * number, y * number, z * number);
}

inline vector3 vector3::operator/(float number) const
{
	return vector3(x / number, y / number, z / number);
}

inline vector3::vector3()
{
}

inline vector3::vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

inline vector3& vector3::operator+=(const vector3& vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return *this;
}

inline vector3& vector3::operator-=(const vector3& vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return *this;
}

inline vector3& vector3::operator*=(float number)
{
	x *= number;
	y *= number;
	z *= number;

	return *this;
}

inline vector3& vector3::operator/=(float number)
{
	x /= number;
	y /= number;
	z /= number;

	return *this;
}

//Quickly needed for distance calculations
static float distance_vec_2(vector2 src, vector2 dst)
{
	float distance;
	distance = sqrtf(powf(src.x - dst.x, 2) + powf(src.y - dst.y, 2));
	return distance;
}

static float distance_vec_3(vector3 src, vector3 dst)
{
	float distance;
	distance = sqrtf(powf(src.x - dst.x, 2) + powf(src.y - dst.y, 2) + powf(src.z - dst.z, 2));
	return distance;
}

class vector4
{
public:
	vector4() : x(0.f), y(0.f), z(0.f), w(0.f)
	{

	}

	vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{

	}
	~vector4()
	{

	}

	float x;
	float y;
	float z;
	float w;
};

#endif
#pragma once

#include <cmath>

class Vector {
public:
	float x, y, z;

	inline Vector() noexcept : x(.0f), y(.0f), z(.0f) {}
	inline Vector(const float _x, const float _y, const float _z) noexcept : x(_x), y(_y), z(_z) {}

	inline Vector operator+(const Vector& vec) const noexcept { return Vector(this->x + vec.x, this->y + vec.y, this->z + vec.z); }
	inline Vector operator+(const float value) const noexcept { return Vector(this->x + value, this->y + value, this->z + value); }
	inline Vector operator*(const float value) const noexcept { return Vector(this->x * value, this->y * value, this->z * value); }
	inline Vector operator*(const Vector& vec) const noexcept { return Vector(this->x * vec.x, this->y * vec.y, this->z * vec.z); }
	inline Vector operator-(const float value) const noexcept { return Vector(this->x - value, this->y - value, this->z - value); }
	inline Vector operator-(const Vector& vec) const noexcept { return Vector(this->x - vec.x, this->y - vec.y, this->z - vec.z); }
	inline Vector operator/(const float value) const noexcept { return Vector(this->x / value, this->y / value, this->z / value); }
	inline Vector operator/(const Vector& vec) const noexcept { return Vector(this->x / vec.x, this->y / vec.y, this->z / vec.z); }

	inline float dot(const Vector& vec) const noexcept { return (this->x * vec.x + this->y * vec.y + this->z * vec.z); }
	inline float lengthSquared() const noexcept { return (this->x * this->x + this->y * this->y + this->z * this->z); }
	inline float length() const noexcept { return std::sqrt(this->lengthSquared()); }
	inline Vector perpendicularTo() const noexcept { return Vector(this->z, this->y, -this->x); }

	inline Vector Normalize() const noexcept
	{
		Vector vec{ Vector(this->x, this->y, this->z) };
		if (const float length{ this->length() }; length != 0) {
			const float inv{ 1.0f / length };
			vec.x *= inv;
			vec.y *= inv;
			vec.z *= inv;
		}
		return vec;
	}

	inline float distTo(const Vector& vec) const noexcept
	{
		const float out{ std::sqrtf(std::powf(this->x - vec.x, 2) + std::powf(this->y - vec.y, 2) + std::powf(this->z - vec.z, 2)) };
		return (out < 0) ? out * -1 : out;
	}
};

#include "pch.h"
#include "vec3.h"


namespace AnorEngine {
	namespace Math{
		vec3& vec3::add(const vec3 &other) {
			
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}
		vec3& vec3::subtract(const vec3& other) {
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}
		vec3& vec3::multiply(const vec3& other) {
;
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;
		}
		vec3& vec3::divide(const vec3& other) {
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;
		}
		vec3::vec3()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		vec3::vec3(const float &x, const float &y, const float& z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		std::ostream& operator << (std::ostream& out, vec3& other) {
			out << " vec3: <" << other.x << "," << other.y << "," << other.z <<">" << std::endl;
			return out;
		}
		vec3 operator + (vec3 left, const vec3& right) {			
			return left.add(right);
		}
		vec3 operator - (vec3 left, const vec3& right)
		{
			return left.subtract(right);
		}
		vec3 operator * (vec3 left, const vec3& right)
		{
			return left.multiply(right);
		}
		vec3 operator / (vec3 left, const vec3& right)
		{
			return left.divide(right);
		}
		vec3& vec3::operator += (const vec3& other)
		{	
			this->add(other);
			return *this;
		}
		vec3& vec3::operator -= (const vec3& other)
		{
			this->subtract(other);
			return *this;
		}
		vec3& vec3::operator *= (const vec3& other)
		{
			this->multiply(other);
			return *this;
		}
		vec3& vec3::operator /= (const vec3& other)
		{
			this->divide(other);
			return *this;
		}
		bool operator == (const vec3& left, const vec3& right)
		{
			return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
		}
		bool operator != (const vec3& left, const vec3& right)
		{
			return !(left.x == right.x) && (left.y == right.y) && (left.z == right.z);
		}
	}
}




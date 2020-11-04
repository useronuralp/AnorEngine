#include "vec4.h"


namespace GameEngineTest {
	namespace Math{
		vec4& vec4::add(const vec4 &other) {
			
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
			return *this;
		}
		vec4& vec4::subtract(const vec4& other) {
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
			return *this;
		}
		vec4& vec4::multiply(const vec4& other) {
;
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			this->w *= other.w;
			return *this;
		}
		vec4& vec4::divide(const vec4& other) {
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			this->w /= other.w;
			return *this;
		}
		vec4::vec4()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}
		vec4::vec4(const float &x, const float &y, const float& z, const float& w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		std::ostream& operator << (std::ostream& out, vec4& other) {
			out << " vec4: <" << other.x << "," << other.y << "," << other.z << "," << other.w <<">" << std::endl;
			return out;
		}
		vec4 operator + (vec4 left, const vec4& right) {			
			return left.add(right);
		}
		vec4 operator - (vec4 left, const vec4& right)
		{
			return left.subtract(right);
		}
		vec4 operator * (vec4 left, const vec4& right)
		{
			return left.multiply(right);
		}
		vec4 operator / (vec4 left, const vec4& right)
		{
			return left.divide(right);
		}
		vec4& vec4::operator += (const vec4& other)
		{
			this->add(other);
			return *this;
		}
		vec4& vec4::operator -= (const vec4& other)
		{
			this->subtract(other);
			return *this;
		}
		vec4& vec4::operator *= (const vec4& other)
		{
			this->multiply(other);
			return *this;
		}
		vec4& vec4::operator /= (const vec4& other)
		{
			this->divide(other);
			return *this;
		}
		bool operator == (const vec4& left, const vec4& right)
		{
			return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
		}
		bool operator != (const vec4& left, const vec4& right)
		{
			return !(left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
		}
	}
}




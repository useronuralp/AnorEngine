#pragma once
namespace GameEngineTest {
	namespace Math {

		struct vec2 {
			float x, y;
			vec2();
			vec2(const float& x, const float& y);
			vec2& add(const vec2& other);
			vec2& subtract(const vec2& other);
			vec2& multiply(const vec2& other);
			vec2& divide(const vec2& other);

			friend vec2 operator + (vec2 left , const vec2& right);
			friend vec2 operator - (vec2 left , const vec2& right);
			friend vec2 operator * (vec2 left , const vec2& right);
			friend vec2 operator / (vec2 left , const vec2& right);

			vec2& operator += (const vec2& other);
			vec2& operator -= (const vec2& other);
			vec2& operator *= (const vec2& other);
			vec2& operator /= (const vec2& other);
			
			friend bool operator == (const vec2& left, const vec2& right);
			friend bool operator != (const vec2& left, const vec2& right);
			friend std::ostream& operator << (std::ostream& out, vec2& other);

		};
	}
}
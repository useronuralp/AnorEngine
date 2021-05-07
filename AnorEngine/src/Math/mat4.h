#pragma once
#include "maths.h"

	 namespace AnorEngine {
		namespace Math {

			struct ANOR_API mat4 {

				union {
					float elements[4 * 4];
					vec4 columns[4]; // this is a very cool trick to reduce the allocated memory when we want to grab columns from an array. 
					//An array of 4 vec4's will have a memory size of 64 bytes which is exaclty the
					//same as an array of 16 floats (elements array). By setting the memory addresses of these two variables the same (unionizing them), we can grab columns as a whole using our vec4
					//custom struct.
				};

				mat4();
				mat4(float diagonal);


				mat4& multiply(const mat4& other);
				friend mat4 operator * (mat4 left, const mat4& right);
				mat4& operator *= (const mat4& other);
				




				friend vec3 operator * (mat4 &left, const vec3& right);
				vec3 multiply(const vec3& other) const;

				friend vec4 operator * (mat4& left, const vec4& right);
				vec4 multiply(const vec4& other) const;






				static mat4 identity();

				static mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
				static mat4 perspective(float fov, float aspectRatio, float near, float far);

				static mat4 translation(const vec3& translation);
				static mat4 rotation(float angle, const vec3& axis);
				static mat4 scale(const vec3& scale);

			};
	}
}

#pragma once
#include "pch.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
# define M_PI           3.14159265358979323846  /* pi */
namespace AnorEngine {
	namespace Math {
		inline float toRadians(float degrees) {
			return (float) (degrees * (M_PI / 180.0f));
		}
	}
}

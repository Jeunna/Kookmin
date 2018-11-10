#ifndef KMUCS_GRAPHICS_TRANSFORM_HPP
#define KMUCS_GRAPHICS_TRANSFORM_HPP

#include <cmath>
#include "vec.hpp"
#include "mat.hpp"
#include "operator.hpp"

namespace kmuvcl
{
    namespace math
    {
#ifndef M_PI
        const float M_PI = 3.14159265358979323846;
#endif

        template <typename T>
        mat<4, 4, T> translate(T dx, T dy, T dz)
        {
            mat<4, 4, T> translateMat;

            // TODO: Fill up this function properly
		for (int i = 0; i < 4; i++) {
			translateMat(i, i) = 1;
		}

		translateMat(0, 3) = dx;
		translateMat(1, 3) = dy;
		translateMat(2, 3) = dz;
 

            return translateMat;
        }

        template <typename T>
        mat<4, 4, T> rotate(T angle, T x, T y, T z)
        {
            mat<4, 4, T> rotateMat;

            // TODO: Fill up this function properly
		T M_radian = angle * M_PI / 180;
		T M_vector = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

		T ux = x / M_vector;
		T uy = y / M_vector;
		T uz = z / M_vector;

		rotateMat(0, 0) = cos(M_radian) + pow(ux, 2)*(1 - cos(M_radian));
		rotateMat(1, 0) = uy*ux*(1 - cos(M_radian)) + uz*sin(M_radian);
		rotateMat(2, 0) = uz*ux*(1 - cos(M_radian)) - uy*sin(M_radian);

		rotateMat(0, 1) = ux*uy*(1 - cos(M_radian)) - uz*sin(M_radian);
		rotateMat(1, 1) = cos(M_radian) + pow(uy, 2)*(1 - cos(M_radian));
		rotateMat(2, 1) = uz*uy*(1 - cos(M_radian)) + ux*sin(M_radian);

		rotateMat(0, 2) = ux*uz*(1 - cos(M_radian)) + uy*sin(M_radian);
		rotateMat(1, 2) = uy*uz*(1 - cos(M_radian)) - ux*sin(M_radian);
		rotateMat(2, 2) = cos(M_radian) + pow(uz, 2)*(1 - cos(M_radian));

		rotateMat(3, 3) = 1;
 

            return rotateMat;
        }

        template<typename T>
        mat<4, 4, T> scale(T sx, T sy, T sz)
        {
            mat<4, 4, T> scaleMat;

            // TODO: Fill up this function properly 
		scaleMat(0, 0) = sx;
		scaleMat(1, 1) = sy;
		scaleMat(2, 2) = sz;
		scaleMat(3, 3) = 1;

            return scaleMat;
        }

        template<typename T>
        mat<4, 4, T> lookAt(T eyeX, T eyeY, T eyeZ, T centerX, T centerY, T centerZ, T upX, T upY, T upZ)
        {
            mat<4, 4, T> viewMat;

            // TODO: Fill up this function properly
		//cam-pos
		mat<4, 4, T> cam_pos;

		for (int i = 0; i < 4; i++) {
			cam_pos(i, i) = 1;
		}
		cam_pos(0, 3) = -eyeX;
		cam_pos(1, 3) = -eyeY;
		cam_pos(2, 3) = -eyeZ;

		//cam-axis
		mat<4, 4, T> cam_axis;
		cam_axis(3, 3) = 1;

		//cam-z-axis
		T subX = eyeX - centerX;
		T subY = eyeY - centerY;
		T subZ = eyeZ - centerZ;

		T z_vector = sqrt(pow(subX, 2) + pow(subY, 2) + pow(subZ, 2));
		T z_axisX = subX / z_vector;
		T z_axisY = subY / z_vector;
		T z_axisZ = subZ / z_vector;

		cam_axis(2, 0) = z_axisX;
		cam_axis(2, 1) = z_axisY;
		cam_axis(2, 2) = z_axisZ;

		//cam-x-axis
		vec<3, T> up_vector(upX, upY, upZ);
		vec<3, T> z_axis(z_axisX, z_axisY, z_axisZ);

		vec<3, T> cross_vecX = cross(up_vector, z_axis);
		T normal_vecX = sqrt(pow(cross_vecX(0), 2) + pow(cross_vecX(1), 2) + pow(cross_vecX(2), 2));
		T x_axisX = cross_vecX(0) / normal_vecX;
		T x_axisY = cross_vecX(1) / normal_vecX;
		T x_axisZ = cross_vecX(2) / normal_vecX;

		cam_axis(0, 0) = x_axisX;
		cam_axis(0, 1) = x_axisY;
		cam_axis(0, 2) = x_axisZ;

		//cam-y-axis
		vec<3, T> x_axis(x_axisX, x_axisY, x_axisZ);

		vec<3, T> cross_vecY = cross(z_axis, x_axis);

		T normal_vecY = sqrt(pow(cross_vecY(0), 2) + pow(cross_vecY(1), 2) + pow(cross_vecY(2), 2));
		cam_axis(1, 0) = cross_vecY(0) / normal_vecY;
		cam_axis(1, 1) = cross_vecY(1) / normal_vecY;
		cam_axis(1, 2) = cross_vecY(2) / normal_vecY;

		//multiplication
		viewMat = cam_axis * cam_pos;
 

            return viewMat;
        }

        template<typename T>
        mat<4, 4, T> ortho(T left, T right, T bottom, T top, T nearVal, T farVal)
        {
            mat<4, 4, T> orthoMat;
            
            // TODO: Fill up this function properly 
		orthoMat(0, 0) = 2 / (right - left);
		orthoMat(1, 1) = 2 / (top - bottom);
		orthoMat(2, 2) = -2 / (farVal - nearVal);
		orthoMat(3, 3) = 1;

		orthoMat(3, 0) = -(right + left) / (right - left);
		orthoMat(3, 1) = -(top + bottom) / (top - bottom);
		orthoMat(3, 2) = -(farVal + nearVal) / (farVal - nearVal);


            return orthoMat;
        }

        template<typename T>
        mat<4, 4, T> frustum(T left, T right, T bottom, T top, T nearVal, T farVal)
        {
           mat<4, 4, T> frustumMat;

           // TODO: Fill up this function properly 
		frustumMat(0, 0) = 2 * nearVal / (right - left);
		frustumMat(0, 2) = (right + left) / (right - left);

		frustumMat(1, 1) = 2 * nearVal / (top - bottom);
		frustumMat(1, 2) = (top + bottom) / (top - bottom);

		frustumMat(2, 2) = -(farVal + nearVal) / (farVal - nearVal);
		frustumMat(2, 3) = -(2 * farVal*nearVal) / (farVal - nearVal);

		frustumMat(3, 2) = -1;

           return frustumMat;
        }

        template<typename T>
        mat<4, 4, T> perspective(T fovy, T aspect, T zNear, T zFar)
        {
          T  right, top;

          // TODO: Fill up this function properly 
		/*267p
		left = -right / bottom = -top / top = near*tan(fovy) / right = top*aspect*/
		fovy = fovy / 2 * M_PI / 180;
		top = zNear*tan(fovy);
		right = top*aspect;

          return frustum(-right, right, -top, top, zNear, zFar);
        }
    }
}
#endif

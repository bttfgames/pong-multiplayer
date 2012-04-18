/******************************************************************************
*
* COPYRIGHT Vinícius G. Mendonça ALL RIGHTS RESERVED.
*
* This software cannot be copied, stored, distributed without  
* Vinícius G.Mendonça prior authorization.
*
* This file was made available on http://www.pontov.com.br and it is free 
* to be restributed or used under Creative Commons license 2.5 br: 
* http://creativecommons.org/licenses/by-sa/2.5/br/
*
*******************************************************************************
* Este software nao pode ser copiado, armazenado, distribuido sem autorização 
* a priori de Vinícius G. Mendonça
*
* Este arquivo foi disponibilizado no site http://www.pontov.com.br e esta 
* livre para distribuição seguindo a licença Creative Commons 2.5 br: 
* http://creativecommons.org/licenses/by-sa/2.5/br/
*
******************************************************************************/

/* Vinícius Godoy de Mendonça */
#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include "MathUtil.h"

namespace math
{
    /**
        This class represents a 2 coordinate space mathematical vector.
        All operations expected for a vector, like sum, subtraction, product,
        cross product, dot product and normalization are provided.
    */
    class Vector2D
    {
        public:
            float x;
            float y;

            /** Builds a NULL vector */
            Vector2D();
            /** Builds a vector based in it's x and y components */
            explicit Vector2D(float aX, float anY);

            /** Builds a vector based in a two dimensional array */
            explicit Vector2D(float xy[2]);

            /** Builds a vector based in it's size (magnitude) and angle */
            static Vector2D createBySizeAngle(float mag, float angle);

            /** Changes the x and y values */
            Vector2D& set(float x, float y);

            /** Changes the x and y values */
            Vector2D& set(const Vector2D& other);

            /** Changes the x and y values */
            Vector2D& set(const float xy[2]);

            /** Retrieves the size of this vector squared */
            float getSizeSqr() const;

            /** Retrieves this vector magnitude */
            float getSize() const;

            /** Changes this vector magnitude. This operation will not interfere
            * in the vector angulation.
            * Return this vector itself, after changing the magnitude. */
            Vector2D& setSize(float _size);

			/** Changes the vector magnitude to size, only if it's greater 
			* than size. This operation will not interfere in the vector 
			* angulation.
			* Return this vector itself, after changing the magnitude. */
			Vector2D& truncate(float size);

            /** Returns the angle of the vector in relation to the x
            * axis, in a counter-clockwise rotation. That is, 90 degrees is
            * a vector pointing up and 0 degrees is a vector pointing right. */
            float getAngle() const;

            /** Rotates the vector counter-clockwise.
            * Returns the vector itself, after the rotation. */
            Vector2D& rotate(float angle);

            /** Normalizes this vector.
            * Returns the vector itself, after the normalization. */
            Vector2D& normalize();

            /** Adds this vector to another vector */
            Vector2D& operator += (const Vector2D& other);

            /** Subtracts this vector from another vector */
            Vector2D& operator -= (const Vector2D& other);

            /** Multiplies the magnitude of this vector to a constant */
            Vector2D& operator *= (float c);

            /** Divides this vector magnitude by a constant*/
            Vector2D& operator /= (float c);

            /** Negates this vector.
            * Same as rotating the vector in 180º but possibly much faster. */
            Vector2D operator -(void) const;
            Vector2D operator +(const Vector2D& other) const;
            Vector2D operator -(const Vector2D& other) const;

            Vector2D operator *(float c) const;
            Vector2D operator /(float c) const;

            float operator[] (long index) const;
            float& operator[] (long index);

            bool operator ==(const Vector2D& other) const;

            float dot(const Vector2D& other) const;
            float angleBetween(const Vector2D other) const;
            float angleSign(const Vector2D& v) const;
    };

	Vector2D operator*(float scalar, const Vector2D& vector);
}


#endif


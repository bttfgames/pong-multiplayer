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

#include "Vector2D.h"
#include <stdexcept>
#include <sstream>

using namespace math;

Vector2D::Vector2D() : x(0), y(0)
{
}

Vector2D::Vector2D(float _x, float _y) : x(_x), y(_y)
{
}

Vector2D::Vector2D(float xy[2]) : x(xy[0]), y(xy[1])
{
}

Vector2D Vector2D::createBySizeAngle(float size, float angle)
{
    return Vector2D(cos(angle) * size,
                  sin(angle) * size);
}

Vector2D& Vector2D::set(float _x, float _y)
{
	x = _x;
	y = _y;
	return *this;
}

Vector2D& Vector2D::set(const float xy[2])
{
    x = xy[0];
    y = xy[1];
    return *this;
}

Vector2D& Vector2D::set(const Vector2D& other)
{
    if (&other == this)
        return *this;

    x = other.x;
    y = other.y;
    return *this;
}

float Vector2D::getSizeSqr() const
{
	return x * x + y * y;
}

float Vector2D::getSize() const
{
	return sqrtf(getSizeSqr());
}

Vector2D& Vector2D::setSize(float _size)
{
	//Avoid the size calculation for the zero vector
	if (_size == 0)
		return set(0,0);

	//Calculate the new size.
	return (*this *= (_size / getSize()));
}

Vector2D& Vector2D::truncate(float size)
{
	if (getSizeSqr() > size * size)
		setSize(size);
	return *this;
}

float Vector2D::getAngle() const
{
	return atan2f(y,x);
}

Vector2D& Vector2D::rotate(float angle)
{
	float s = sin(angle);
    float c = cos(angle);

    float newX = x * c - y * s;
    float newY = x * s + y * c;

    x = newX;
    y = newY;

    return *this;
}
Vector2D& Vector2D::normalize()
{
	return (*this /= getSize());
}

float Vector2D::operator[] (long index) const
{
    if (index == 0)
        return x;
    if (index == 1)
        return y;

    std::stringstream ss;
    ss << "Index out of bounds: " << index << " size: 2";
    throw std::out_of_range(ss.str());
}

float& Vector2D::operator[] (long index)
{
    if (index == 0)
        return x;
    if (index == 1)
        return y;

    std::stringstream ss;
    ss << "Index out of bounds: " << index << " size: 2";
    throw std::out_of_range(ss.str());
}

Vector2D& Vector2D::operator += (const Vector2D& other)
{
	x += other.x;
    y += other.y;
    return *this;
}

Vector2D& Vector2D::operator -= (const Vector2D& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2D& Vector2D::operator *= (float c)
{
	x *= c;
	y *= c;
	return *this;
}

Vector2D& Vector2D::operator /= (float c)
{
	x /= c;
    y /= c;
    return *this;
}

Vector2D Vector2D::operator -(void) const
{
	return Vector2D(-x, -y);
}

Vector2D Vector2D::operator +(const Vector2D& other) const
{
	return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator -(const Vector2D& other) const
{
	return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator *(float c) const
{
	return Vector2D(x*c, y*c);
}

Vector2D math::operator*(float scalar, const Vector2D& vector)
{
	return vector * scalar;
}

Vector2D Vector2D::operator /(float c) const
{
	return Vector2D(x/c, y/c);
}

bool Vector2D::operator ==(const Vector2D& other) const
{
	return x == other.x && y == other.y;
}

float Vector2D::dot(const Vector2D& other) const
{
    return x * other.x + y * other.y;
}

/**
 * Return the smallest angle between 2 vectors.
 * The signal is used to indicate the angle rotation direction.
 * Positive for counter-clockwise and negative for clockwise direction
 *
 * This method uses difference between two vectors.
 */
float Vector2D::angleBetween(const Vector2D other) const
{
    float angle = other.getAngle() - getAngle();

    if (fabs(angle) < PI)
        return angle;

    return static_cast<float>(angle + (angle < 0 ? 2*PI : -2*PI));
}

/**
 * Return the smallest angle between 2 vectors.
 * The signal is used to indicate the angle rotation direction.
 * Positive for counter-clockwise and negative for clockwise direction
 *
 * This method uses the dot product.
 */
float Vector2D::angleSign(const Vector2D& v2)const
{
    /** Must guarantee that v2 is a normalized vector */
    Vector2D v(v2);
    v.normalize();
    float dp, angPi ;


    dp = dot( v ); //dot product
    if(dp >= 1.0) dp = 1.0f;
    if(dp <=-1.0) dp =-1.0f;

    angPi = (float)acos( dp );

    //side test
    if (y*v.x > x*v.y)
        return -angPi;
    else
        return angPi;
}

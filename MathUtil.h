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

#ifndef MATHUTIL_H_INCLUDED
#define MATHUTIL_H_INCLUDED

#include <cmath>

namespace math
{
    static const double PI = 3.14159265358979323846;
    static const double ONE_DEGREE = 180.0 / PI;
    static const double E = 2.7182818284590452354;

    inline double toDegrees(double angrad)
    {
        return angrad * ONE_DEGREE;
    }

    inline float toDegrees(float angrad)
    {
        return static_cast<float>(angrad * ONE_DEGREE);
    }

    inline float toDegrees(int angrad)
    {
        return toDegrees(static_cast<float>(angrad));
    }

    inline double toRadians(double angdeg)
    {
        return angdeg / ONE_DEGREE;
    }

    inline float toRadians(float angdeg)
    {
        return static_cast<float>(angdeg / ONE_DEGREE);
    }

    inline float toRadians(int angdeg)
    {
        return toRadians(static_cast<float>(angdeg));
    }

    void padValue(float &value, float min, float max);
}

#endif // MATHUTIL_H_INCLUDED

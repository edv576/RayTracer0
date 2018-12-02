#pragma once

#ifndef _MATERIAL_H
#define _MATERIAL_H


#include "Vect.h"
#include "Color.h"
#include "Ray.h"

class Material
{
	//materialType values can be from 1 to 3
	//1: Specular
	//2: Reflective
	//3: Reflective with reflection
	int materialType;
	double reflectionValue;
	double refractionValue;
	Color color;
public:
	Material();
	Material(int, double, double, Color);


	Color getColor() { return color; }
	double getReflectionValue() { return reflectionValue; }
	double getRefractionValue() { return refractionValue; }



private:

};

Material::Material()
{
	materialType = 1;
	reflectionValue = 0;
	refractionValue = 0;
	color = Color(0.5, 0.5, 0.5, 0);
}

Material::Material(int mt, double reflecValue, double refracValue, Color colorValue)
{
	materialType = mt;
	reflectionValue = reflecValue;
	refractionValue = refracValue;
	color = colorValue;
}

#endif // !_Material_H


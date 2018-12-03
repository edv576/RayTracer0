#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Ray.h"
#include "Vect.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Source.h"
#include "Triangle.h"


#pragma warning(disable : 4996)

using namespace std;

struct RGBType
{
	double r;
	double g;
	double b;
};

void savebmp(const char *filename, int w, int h, int dpi, RGBType *data)
{
	FILE *f;
	int k = w * h;
	int s = 4 * k;
	

	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi * m;

	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0,  0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = {40, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1, 0, 24, 0};

	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);

	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	f = fopen(filename, "wb");

	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	for (int i = 0; i < k; i++) {

		RGBType rgb = data[i];
		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;

		unsigned char color[3] = { (int)floor(blue), (int)floor(green), (int)floor(red) };

		fwrite(color, 1, 3, f);

	}

	fclose(f);
}

int thisone;

int winningObjectIndex(vector<double> object_intersections) {
	//return index of the winning intersection
	int index_of_minimum_value;


	if (object_intersections.size() == 0)
	{
		return -1;

	}
	else if(object_intersections.size() == 1)
	{
		if (object_intersections.at(0) > 0) {
			return 0;

		}
		else
		{
			//otherwise the only value is negative
			return -1;
		}

	}
	else
	{
		//otherwise there is more than one intersection
		//find the maximum one
		double max = 0;

		for (int i = 0; i < object_intersections.size(); i++) {
			if (max < object_intersections.at(i))
			{
				max = object_intersections.at(i);
			}

		}
		
		if (max > 0) {
			for (int index = 0; index < object_intersections.size(); index++) {
				if (object_intersections.at(index)>0 && object_intersections.at(index)<=max)
				{
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}

			return index_of_minimum_value;

		}
		else
		{
			//all the intersections were negative
			return -1;
		}

	}

}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight) {

	Material winning_object_material = scene_objects.at(index_of_winning_object)->getMaterial();
	Color winning_object_color = scene_objects.at(index_of_winning_object)->getMaterial().getColor();
	Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

	if (winning_object_color.getColorSpecial() == 2) {
		// checkered/tile floor pattern

		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());

		if ((square % 2) == 0) {
			// black tile
			winning_object_color.setColorRed(0);
			winning_object_color.setColorGreen(0);
			winning_object_color.setColorBlue(0);
		}
		else {
			// white tile
			winning_object_color.setColorRed(1);
			winning_object_color.setColorGreen(1);
			winning_object_color.setColorRed(1);
		}
	}

	Color final_color = winning_object_color.colorScalar(ambientlight);

	if (winning_object_material.getMaterialType() == 2)
	{
		if (winning_object_material.getReflectionValue() > 0 && winning_object_material.getReflectionValue() <= 1) {
			// reflection from objects with specular intensity
			double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
			Vect scalar1 = winning_object_normal.vectMult(dot1);
			Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
			Vect scalar2 = add1.vectMult(2);
			Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
			Vect reflection_direction = add2.normalize();

			Ray reflection_ray(intersection_position, reflection_direction);

			// determine what the ray intersects with first
			vector<double> reflection_intersections;

			for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
				reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
			}

			int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

			if (index_of_winning_object_with_reflection != -1) {
				if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
					// determine the position and direction at the point of intersection with the reflection ray
					// the ray only affects the color if it reflected off something

					Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
					Vect reflection_intersection_ray_direction = reflection_direction;

					Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);

					final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_material.getReflectionValue()));
				}
			}
		}
	}

	

	for (int light_index = 0; light_index < light_sources.size(); light_index++) {
		Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

		float cosine_angle = winning_object_normal.dotProduct(light_direction);

		if (cosine_angle > 0) {
			// test for shadows
			bool shadowed = false;

			Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
			float distance_to_light_magnitude = distance_to_light.magnitude();

			Ray shadow_ray(intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());

			vector<double> secondary_intersections;

			for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
			}

			for (int c = 0; c < secondary_intersections.size(); c++) {
				if (secondary_intersections.at(c) > accuracy) {
					if (secondary_intersections.at(c) <= distance_to_light_magnitude) {
						shadowed = true;
					}
					break;
				}

			}

			if (shadowed == false) {
				final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
					double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
					Vect scalar1 = winning_object_normal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
					Vect scalar2 = add1.vectMult(2);
					Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();

					double specular = reflection_direction.dotProduct(light_direction);
					if (specular > 0) {
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
					}
				}

			}

		}
	}

	return final_color.clip();
}

int main(int argc, char *argv[]) {

	cout << "rendering ..." << endl;

	clock_t t1, t2;
	t1 = clock();

	int dpi = 72;
	int width = 640;
	int height = 480;	
	int n = width*height;

	const int aadepth = 1;
	double aathreshold = 0.1;
	double aspectratio = (double)width / (double)height;
	double ambientlight = 0.2;
	double accuracy = 0.00000001;

	RGBType *pixels = new RGBType[n];

	Vect O(0, 0, 0);
	Vect X(1, 0, 0);
	Vect Y(0, 1, 0);
	Vect Z(0, 0, 1);
	Vect new_sphere_position(1.75, -0.25, 0);
	Vect new_sphere_position2(-1, -0.25, -2);
	Vect new_sphere_position3(0, 2, 0);

	Vect campos(3, 1.5, -4);

	Vect look_at(0, 0, 0);
	Vect diff_btw(campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());

	Vect camdir = diff_btw.negative().normalize();
	Vect camright = Y.crossProduct(camdir).normalize();
	Vect camdown = camright.crossProduct(camdir).normalize();

	Camera scene_cam(campos, camdir, camright, camdown);

	Color white_light(1.0, 1.0, 1.0, 1);
	Color very_green(0.5, 1.0, 0.5, 0.3);
	Color maroon(0.5, 0.25, 0.25, 0);
	Color grey(0.5, 0.5, 0.5, 0);
	Color black(0.0, 0.0, 0.0, 0);
	Color orange(0.94, 0.75, 0.31, 0);
	Color orange_reflective(0.94, 0.75, 0.31, 0.2);
	Color check_floor(1, 1, 1, 2);

	Vect light_position(-7, 10, -10);
	Light scene_light(light_position, white_light);

	vector<Source*> light_sources;
	light_sources.push_back(dynamic_cast<Source*>(&scene_light));

	//Objects

	Sphere scene_sphere(O, 1, very_green, Material(2,0.3,0,very_green));
	Plane scene_plane(Y, -1, check_floor, Material(1,0,0,check_floor));
	Sphere scene_sphere2(new_sphere_position, 0.5, grey, Material(1,0,0,grey));
	Sphere scene_sphere3(new_sphere_position2, 0.5, orange_reflective, Material(2,0.2,0,orange_reflective));
	Sphere scene_sphere4(new_sphere_position3, 0.5, grey, Material(1, 0.2, 0, grey));
	Triangle scene_triangle(Vect(3, 0, 0), Vect(0, 3, 0), Vect(0, 0, 3), orange, Material(1,0,0,orange));

	vector<Object*> scene_objects;
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere3));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere4));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));

	double xamnt, yamnt;
	int thisone, aa_index;

	double tempRed[aadepth*aadepth];
	double tempGreen[aadepth*aadepth];
	double tempBlue[aadepth*aadepth];

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//return color
			thisone = y * width + x;

			for (int aax = 0; aax < aadepth; aax++) {
				for (int aay = 0; aay < aadepth; aay++) {

					aa_index = aay*aadepth + aax;

					srand(time(0));

					if (aadepth == 1)
					{
						// begin with no anti aliasing

						if (width > height)
						{
							xamnt = ((x + 0.5) / width)*aspectratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + 0.5) / height;

						}
						else if (height > width)
						{
							xamnt = (x + 0.5) / width;
							yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else
						{
							xamnt = (x + 0.5) / width;
							yamnt = ((height - y) + 0.5) / height;
						}

					}
					else
					{
						// anti-aliasing
						if (width > height) {
							// the image is wider than it is tall
							xamnt = ((x + (double)aax / ((double)aadepth - 1)) / width)*aspectratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + (double)aax / ((double)aadepth - 1)) / height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamnt = (x + (double)aax / ((double)aadepth - 1)) / width;
							yamnt = (((height - y) + (double)aax / ((double)aadepth - 1)) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else {
							// the image is square
							xamnt = (x + (double)aax / ((double)aadepth - 1)) / width;
							yamnt = ((height - y) + (double)aax / ((double)aadepth - 1)) / height;

						}
					}




						Vect cam_ray_origin = scene_cam.getCameraPosition();
						Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();

						Ray cam_ray = Ray(cam_ray_origin, cam_ray_direction);

						vector<double> intersections;

						for (int index = 0; index < scene_objects.size(); index++) {
							intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));

						}

						int index_of_winning_object = winningObjectIndex(intersections);

						//cout << index_of_winning_object;

						if (index_of_winning_object == -1) {
							tempRed[aa_index] = 0;
							tempGreen[aa_index] = 0;
							tempBlue[aa_index] = 0;

						}
						else
						{
							if (intersections.at(index_of_winning_object) > accuracy) {

								Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
								Vect intersecting_ray_direction = cam_ray_direction;

								Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight);
								tempRed[aa_index] = intersection_color.getColorRed();
								tempGreen[aa_index] = intersection_color.getColorGreen();
								tempBlue[aa_index] = intersection_color.getColorBlue();
							}



						}


					}
				}


			// average the pixel color
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
				totalRed = totalRed + tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
				totalGreen = totalGreen + tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
				totalBlue = totalBlue + tempBlue[iBlue];
			}

			double avgRed = totalRed / (aadepth*aadepth);
			double avgGreen = totalGreen / (aadepth*aadepth);
			double avgBlue = totalBlue / (aadepth*aadepth);

			pixels[thisone].r = avgRed;
			pixels[thisone].g = avgGreen;
			pixels[thisone].b = avgBlue;
			

		}
	}

	savebmp("scene.bmp", width, height, dpi, pixels);

	delete pixels, tempRed, tempGreen, tempBlue;

	t2 = clock();
	float diff = ((float)t2 - (float)t1) / 1000;

	cout << diff << " seconds" << endl;

	return 0;

}
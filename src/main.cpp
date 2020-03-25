#include <iostream>
#include <cstdlib>
#include <list>
#include <limits>
#include <c3ga/Mvec.hpp>
#include <omp.h>
#include "c3gaTools.hpp"
#include "Geogebra_c3ga.hpp"

#include "Entry.hpp"

const double epsilon = 1e-6;

bool display_geogebra = true;

double correct_intersection(c3ga::Mvec<double> line, c3ga::Mvec<double> cercle, Viewer_c3ga& viewer) {
	c3ga::Mvec<double> sphere_dual = (!line) | (cercle);
	viewer.push(sphere_dual, 200, 0, 0);

	double radius = (sphere_dual) | (sphere_dual);
	return radius;
}

c3ga::Mvec<double> creer_cercle_x(double x_move) {

	return c3ga::point<double>(0 + x_move, 1, -1.0)
		^ c3ga::point<double>(0 + x_move, -1, -1.0)
		^ c3ga::point<double>(0 + x_move, 1, 2.0);
}

c3ga::Mvec<double> creer_cercle(double x_move) {

	return c3ga::point<double>(0 + x_move - 1, 0, -1.0)
		^ c3ga::point<double>(0 + x_move + 1, 0, -1.0)
		^ c3ga::point<double>(0 + x_move - 1, 0, 2.0);
}

#ifdef _WIN32
	#define M_PI 3.141592
#endif // _WIN32




/*
c3ga::point<double>(0 + x_move, -1, -1.0)
^ c3ga::point<double>(0 + x_move, 1, -1.0)
^ c3ga::point<double>(0 + x_move, -1, 2.0)
^ c3ga::point<double>(0 + x_move, 1, 1.0);
*/
c3ga::Mvec<double> creer_sphere(double x_move, double y_move, double z_move) {
	/*
	return c3ga::point<double>(x_move, -1, 0.0)
		 ^ c3ga::point<double>(x_move, 0, 1.0)
		 ^ c3ga::point<double>(x_move, 1, 0.0)

		 ^ c3ga::point<double>(x_move, 0, -1.0)
		;*/


	return c3ga::point<double>(x_move - 1, y_move, z_move)
		^ c3ga::point<double>(1 + x_move, 0 + y_move, z_move)
		^ c3ga::point<double>(0 + x_move, 1 + y_move, z_move)
		^ c3ga::point<double>(0 + x_move, 1 + y_move, 2.0 + z_move)
		//^ c3ga::ei<double>()
		;
}
/*
c3ga::Mvec<double> creer_sphere(double x_move, double y_move, double z_move, double radius) {

		std::cout << "Put: " << x_move << ", " << y_move << ", " << z_move << std::endl;
    //x_move -= radius * 0.5;
    //y_move -= radius * 0.5;

	return
	      c3ga::point<double>(x_move,     y_move,     z_move - 1)
        ^ c3ga::point<double>(x_move + 1, y_move - 1, z_move)
		^ c3ga::point<double>(x_move - 1, y_move - 1, z_move)
		^ c3ga::point<double>(x_move,     y_move,     z_move + 1)
		;
}*/


c3ga::Mvec<double> creer_sphere(double x_move) {
	/*
	return c3ga::point<double>(x_move, -1, 0.0)
		 ^ c3ga::point<double>(x_move, 0, 1.0)
		 ^ c3ga::point<double>(x_move, 1, 0.0)

		 ^ c3ga::point<double>(x_move, 0, -1.0)
		;*/
	return c3ga::point<double>(x_move, 0, 2.0)
		^ c3ga::point<double>(2 + x_move, 0, 2.0)
		^ c3ga::point<double>(1 + x_move, 1, 2.0)
		^ c3ga::point<double>(1 + x_move, 0, 3.0);
}



#include <vector>
#include <fstream>



c3ga::Mvec<double> intersection(c3ga::Mvec<double> a, c3ga::Mvec<double> b) {
	return !(!a ^ !b);
}

c3ga::Mvec<double> creer_plan(double x_move, double y_move, double z_move) {

	return c3ga::point<double>(x_move, 0, 0.0)
		^ c3ga::point<double>(x_move + 1, 1, 0.0)
		^ c3ga::point<double>(x_move, 2, 0.5)
		^ c3ga::ei<double>()
		;
}


c3ga::Mvec<double> creer_rayon(double x, double y);

class ColorContainer {
public:

	ColorContainer() : ColorContainer(0, 0, 0) {}

	ColorContainer(int r, int g, int b) : r(r), g(g), b(b) {}

	ColorContainer operator*(double val) {
		if (val < 0)
			val = 0;
		ColorContainer out(r * val, g * val, b * val);
		//std::cout << val * 100.0 << "%" << std::endl;
		return out;
	}

	friend std::ostream& operator<<(std::ostream& os, const ColorContainer &color)
	{
		os << "Color: " << color.r << ", " << color.g << ", " << color.b << std::endl;
		return os;
	}


	int r;
	int g;
	int b;
};



class Obj {

public:
	ColorContainer color;
	c3ga::Mvec<double> obj;
};
/*
class Plan : public Obj {
public:
	Plan() : Plan(0, 0, 0) {

	}

	Plan(double x, double y, double z) {
		obj = creer_plan(x, y, z);
	}
};*/

class Sphere : public Obj {
public:
	Sphere() : Sphere(0, 0, 0) {

	}

	Sphere(double x, double y, double z) {
		obj = creer_sphere(x, y, z);
	}
};

class Camera {
public:
	Camera(int res_x, int res_y, double max_z, double pixel_res) : res_x(res_x), res_y(res_y), pixel_res(pixel_res) {
        max_distance = max_z;
		renderedImage = (ColorContainer**)malloc(sizeof(ColorContainer*) * res_y);
        if(renderedImage == nullptr){
            std::cerr << "Malloc error" << std::endl;
            exit(EXIT_FAILURE);
        }
		for (int y = 0; y < res_y; y++) {
			renderedImage[y] = (ColorContainer*)malloc(sizeof(ColorContainer) * res_x);
			if(renderedImage[y] == nullptr){
                std::cerr << "Malloc secondary error" << std::endl;
                exit(EXIT_FAILURE);
            }
		}
	}

	int res_x, res_y;
	double pixel_res;
	double max_distance;

	ColorContainer** renderedImage;


	void render(Viewer_c3ga& viewer, std::vector< Obj > objList) {

		if (display_geogebra) {

			viewer.push(c3ga::point<double>(1, 0, 0.0), 1, 0, 0);
			viewer.push(c3ga::point<double>(0, 1, 0.0), 0, 1, 0);
			viewer.push(c3ga::point<double>(0, 0, 1.0), 0, 0, 1);
		}


		omp_set_num_threads(8);
		#pragma omp parallel for
		for (int y = 0; y < res_y; y++) {
			//int num = omp_get_thread_num();
			//std::cout << "y[" << y << "] : " << num << std::endl;
			for (int x = 0; x < res_x; x++) {
				auto pos = c3ga::point<double>(x * pixel_res, y * pixel_res, 0.0);
				if(display_geogebra)
					viewer.push(pos, 1, 1, 1);
				c3ga::Mvec<double> ligne = creer_rayon(x * pixel_res, y * pixel_res);

				if (display_geogebra)
					viewer.push(ligne, 0, 1, 0);

				//std::cout << "Line: " << ligne << std::endl;
				bool hit = false;
				double shortestDistance = std::numeric_limits<double>::max();
				ColorContainer colorOfShortest = ColorContainer(0, 0, 0);
				for (auto& obj : objList) {
					auto& plan = obj.obj;
					auto intersect = intersection(ligne, plan);
					//std::cout << "Intersect result type is: " << c3ga::whoAmI(intersect) << std::endl;

					auto test = intersect | intersect;
					//std::cout << "Test: " << test << std::endl;
					if (test >= 0) {
						hit = true;

					}
					else
						continue;

					if (display_geogebra)
						viewer.push(intersect, 1, 0, 0);

					const auto pp = intersect;
					c3ga::Mvec<double> intersectPoint;
					auto p1 = (pp - sqrt(pp * pp)) / (-c3ga::ei<double>() | pp);
					//std::cout << "Plan: " << c3ga::whoAmI(plan) << std::endl;
					//std::cout << "intersectPoint: " << c3ga::whoAmI(intersectPoint) << std::endl;
					auto p2 = (pp + sqrt(pp * pp)) / (-c3ga::ei<double>() | pp);
					//std::cout << "grade:" << p1.grade() << std::endl;
					//c3ga::Mvec<double> normal = plan | p1;
					double dist;
					/*std::cout << "The intersect point is: " << c3ga::whoAmI(pp) << std::endl;
					std::cout << pp << std::endl;

					std::cout << "p1 is: " << c3ga::whoAmI(p1) << std::endl;
					std::cout << p1 << std::endl;

					std::cout << "p2 is: " << c3ga::whoAmI(p2) << std::endl;
					std::cout << p2 << std::endl;*/

					if (p1.grade() == 0) {
						//intersectPoint = p1;
						//normal = plan | pp;

						intersectPoint = intersection(pp, c3ga::ei<double>());
						std::cout << "         GRADE 0" << std::endl;
						std::cout << "Intersect point type is: " << c3ga::whoAmI(intersectPoint) << std::endl;
						std::cout << intersectPoint << std::endl;
						auto pos_flat = pos ^ c3ga::ei<double>();
						std::cout << "pos type is: " << c3ga::whoAmI(pos) << std::endl;
						std::cout << pos << std::endl;

						std::cout << "FLAT pos type is: " << c3ga::whoAmI(pos_flat) << std::endl;
						std::cout << pos_flat << std::endl;

						auto F = ligne;
						auto test =-( (c3ga::e0i<double>() * (c3ga::e0<double>() ^ F)) / (c3ga::e0i<double>() * F));

						viewer.push(test, 1, 0, 1);
						std::cout << "test: " << c3ga::whoAmI(test) << std::endl;
						std::cout << test << std::endl;
						dist = (pos_flat - pp).norm();
						//pp.display();
						//Flat point
					}
					else {
						//Point (dual tangent trivector)
						if ((pos - p1).norm() < (pos - p2).norm())
							intersectPoint = p1;
						else
							intersectPoint = p2;
						dist = (pos - intersectPoint).norm();
					}
					//std::cout << "          out" << std::endl;
					if (display_geogebra)
						viewer.push(intersectPoint, 0, 0, 0);
					//std::cout << "Dist p1: " << (pos - p1).norm() << std::endl;
					//std::cout << "Dist p2: " << (pos - p2).norm() << std::endl;
					//std::cout << "Dist: " << dist << std::endl;
					//std::cout << "Pos type is: " << c3ga::whoAmI(pos) << std::endl;
					//std::cout << "Intersect point type is: " << c3ga::whoAmI(intersectPoint) << std::endl;
					//std::cout << "Choosen dist: " << (pos - intersectPoint).norm() << std::endl;



					if (dist < shortestDistance) {
						shortestDistance = dist;
						colorOfShortest = obj.color;
					}
					//std::cout << "normal: " << c3ga::whoAmI(normal) << std::endl;
					//if (display_geogebra)
					//normal.display();
					//auto ref = normal * ligne * normal;
					//auto experiment = normal * ligne * normal.inv();
					//std::cout << "intersect: " << std::endl;
					//if (display_geogebra)
					//    pp.display();
					/*viewer.push(p1, 1, 1, 0);
					viewer.push(normal, 0, 0, 0);
					viewer.push(intersection(normal, p2), 1, 0, 1);*/

					//std::cout << "intersect: " << intersect << std::endl;
					//intersect.display();

					//std::string mvType = c3ga::whoAmI(intersect);
					//std::cout << "Intersection type = " << mvType << std::endl;


					//mvType = c3ga::whoAmI(test);
					//std::cout << "Intersection type = " << mvType << ":" << test << std::endl;
					//double testval = intersect.dual() / (intersect.dual() * c3ga::ei<double>());
					//auto testval = (intersect + sqrt(pow(intersect, 2.0)))/ ((-c3ga::ei<double>()) | intersect);
					//std::cout << "testval: " << testval << std::endl;


					//auto test = (obj | ligne) * obj;
					//double val = correct_intersection(ligne, !test, viewer);
					//viewer.push(test, 200, 0, 1);
					//std::cout << "Intersect: " << val << std::endl;
					//if(val < 0)

				}


				if (hit) {
					//std::cout << "Distance : " << shortestDistance << std::endl;
					//viewer.push(ligne, 0, 1, 0);
					renderedImage[y][x] = colorOfShortest * (((double)(max_distance - shortestDistance)) / max_distance);
				}
				else
					renderedImage[y][x] = ColorContainer(0, 0, 0);
				//viewer.push(ligne, 1, 0, 0);
			}
		}
	};

	void output_image() {

		std::ofstream myfile;
		myfile.open("output.ppm");
		myfile << "P3" << std::endl << res_x << " " << res_y << std::endl << 255 << std::endl;
		for (int y = 0; y < res_y; y++) {
			for (int x = 0; x < res_x; x++) {
				ColorContainer& color = renderedImage[y][x];

				myfile << color.r << " " << color.g << " " << color.b << " ";


			}
			myfile << std::endl;
		}
	}

};

c3ga::Mvec<double> creer_rayon(double x, double y) {

	return c3ga::point<double>(x, y, 0)
		^ c3ga::point<double>(x, y, 1)
		^ c3ga::ei<double>()
		;
}


c3ga::Mvec<double> creer_ligne(double x_move) {

	return c3ga::point<double>(x_move, 0, 0.0)
		^ c3ga::point<double>(x_move + 1.0, 0.0, 0)
		^ c3ga::ei<double>()
		;
}






void render_random(int nb_spheres, int positionAreaMax, double max_z, int res) {
	Viewer_c3ga viewer;
	// sphere

	//Cercle 0
	std::vector< Obj > objList;


	for (int i = 0; i < nb_spheres; i++) {
		Obj sphere = Sphere(rand() % positionAreaMax, rand() % positionAreaMax, rand() % ((int) max_z));//creer_plan(3.f);

		sphere.color = ColorContainer(rand() % 255, rand() % 255, rand() % 255);

		//if (i == 87) {
			objList.push_back(sphere);
			//std::cout << sphere.color;
			if (display_geogebra)
				viewer.push(sphere.obj, 0, 0, 1);
		//}
	}

    double pixel_res = ((double) positionAreaMax) / ((double) res);
	Camera cam = Camera(res, res, max_z, pixel_res);
	cam.render(viewer, objList);
	cam.output_image();


	//c3ga::Mvec<double> intersect = (plan.dual() ^ ligne.dual()).dual();

	//viewer.push(intersect, 0, 20, 20);

	//viewer.display();
	if (display_geogebra)
		viewer.render("display_.html");
}



///////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

    int nb_spheres = 10;
    int maxPositionAreaSize = 30;
    double maxZposition = 1000;
    int renderRes = 3000;

    if(argc == 5){
        nb_spheres = atoi(argv[1]);
        maxPositionAreaSize = atoi(argv[2]);;
        maxZposition = atoi(argv[3]);
        renderRes=  atoi(argv[4]);


    }

    std::cout << "Config: " << nb_spheres << " spheres, ["
         << maxPositionAreaSize << "," << maxPositionAreaSize << "] area, max depth: " << maxZposition
         << ", render resolution: " << renderRes << " x " << renderRes << std::endl;

    display_geogebra = false;
	render_random(nb_spheres, maxPositionAreaSize, maxZposition, renderRes);
	return 0;
}

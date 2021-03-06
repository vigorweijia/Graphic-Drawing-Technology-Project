#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "utils.h"
#include "vec3.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"
#include "Pdf.h"

using namespace std;

void vec3_test();

vec3 color(const Ray& r, Hitable *world, int depth)
{
    HitRecord record;
    if(world->HitObject(r, 0.001, 1000, record))
    {
		Ray scattered;
		vec3 albedo;
		vec3 emitted = record.matPtr->emitted(r, record, record.u, record.v, record.p);
		float pdfVal;
		if (depth < 50 && record.matPtr->scatter(r, record, albedo, scattered, pdfVal))
		{
			/*vec3 onLight = vec3(213 + randomUniform()*(343-213), 554, 227+randomUniform()*(332-227));
			vec3 toLight = onLight - record.p;
			float distanceSquared = toLight.length() * toLight.length();
			toLight.make_unit();
			if (dot(toLight, record.normal) < 0)
				return emitted;
			float lightArea = (343 - 213)*(332 - 227);
			float lightCosine = fabs(toLight.y());
			if (lightCosine < 0.000001)
				return emitted;
			pdf = distanceSquared / (lightCosine * lightArea);
			scattered = Ray(record.p, toLight);*/
			/*CosinePdf p(record.normal);
			vec3 direction;
			do {
				vec3 direction = p.generate();
				scattered = Ray(record.p, unit_vector(direction));
				pdfVal = p.value(scattered.direction());
				//pdfVal = dot(p.uvw.w(), scattered.direction()) / M_PI; // seem as the same as above
			} while (fabs(pdfVal) < 0.00001);*/
			/*Hitable* lightShape = new XzRect(213, 343, 227, 332, 554, 0);
			HitablePdf p(lightShape, record.p);
			vec3 direction;
			do {
				vec3 direction = p.generate();
				scattered = Ray(record.p, unit_vector(direction));
				pdfVal = p.value(scattered.direction());
			} while (fabs(pdfVal) < 0.00001);*/

			if (record.matPtr->isSpecular)
			{
				Hitable* lightShape = new XzRect(213, 343, 227, 332, 554, 0);
				//Hitable* lightShape = new XzCircle(vec3(278, 554, 279), 60, 0);
				//Hitable* lightShape = new XzTriangle(vec3(213, 554, 227), vec3(343, 554, 227), vec3(278, 554, 332), 0);
				HitablePdf p0(lightShape, record.p);
				CosinePdf p1(record.normal);
				MixturePdf p(&p0, &p1);
				vec3 direction;
				do {
					vec3 direction = p.generate();
					scattered = Ray(record.p, unit_vector(direction));
					pdfVal = p.value(scattered.direction());
				} while (fabs(pdfVal) < 0.00001);

				//return emitted + attenuation * color(scattered, world, depth + 1);
				return emitted + albedo * record.matPtr->scatterPdf(r, record, scattered) * color(scattered, world, depth + 1) / pdfVal;
			}
			else
			{
				return emitted + albedo * color(scattered, world, depth + 1);
			}
		}
		else
		{
			return emitted;
		}
    } else{
        /*vec3 unitDirection = r.direction().unit();
        float t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);*/
		return vec3(0, 0, 0); //change background color to black
    }
}

Hitable* GenerateRandomScene()
{
	int n = 500;
	Hitable **list = new Hitable*[n + 1];
	//list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
	list[0] = new Plane(vec3(0, 0, 0), vec3(0, 1, 0), new Lambertian(new ConstantTexture(vec3(0.8, 0.7, 0.9))));
	int i = 1;
	//list[1] = new Plane(vec3(0, 0, -2), vec3(0, 0, 1), new Lambertian(vec3(0.5, 0.5, 0.5)));
	list[i++] = new Cylinder(vec3(0, 0.5, -0.5), 0.8, 1.0, Cylinder::yAxis, new Dielectric(1.5));
	list[i++] = new Cylinder(vec3(-4, 0.8, -0.5), 0.8, 1.0, Cylinder::xAxis, new Lambertian(new ConstantTexture(vec3(0.4,0.2,0.1))));
	list[i++] = new Cylinder(vec3(4, 0.8, -0.5), 0.8, 1.0, Cylinder::zAxis, new Metal(vec3(0.7, 0.6, 0.5), 0.0));
	/*for (int a = -3; a < 3; a++)
	{
		for (int b = -3; b < 3; b++)
		{
			float randomMaterial = randomUniform();
			vec3 randomCenter(a+0.9*randomUniform(), 0.2, b+randomUniform());
			if ((randomCenter - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if(randomMaterial < 0.8)
				{ 
					list[i++] = new Sphere(randomCenter, 0.2, 
						new Lambertian(vec3(randomUniform()*randomUniform(), 
											randomUniform()*randomUniform(), 
											randomUniform()*randomUniform())));
				}
				else if(randomMaterial < 0.95)
				{ 
					list[i++] = new Sphere(randomCenter, 0.2, 
						new Metal(vec3(0.5*(1+randomUniform()),
										0.5*(1+randomUniform()),
										0.5*(1+randomUniform())),
									0.5*randomUniform()));
				}
				else
				{
					list[i++] = new Sphere(randomCenter, 0.2, new Dielectric(1.5));
				}
			}
		}
	}*/
	//list[i++] = new Cylinder(vec3(0, 0.2, 4.2), 0.3, 0.4, Cylinder::yAxis, new Dielectric(1.5));
	//list[i++] = new Cylinder(vec3(-3.5, 0.3, 3.8), 0.3, 0.4, Cylinder::xAxis, new Metal(vec3(0.4, 0.2, 0.1), 0.0));
	//list[i++] = new Cylinder(vec3(3.5, 0.3, 3.4), 0.3, 0.4, Cylinder::zAxis, new Lambertian(vec3(0.7, 0.6, 0.5)));
	//list[i++] = new Sphere(vec3(0, 0.8, 2.5), 0.8, new Dielectric(1.5));
	//list[i++] = new Sphere(vec3(-4, 0.8, 2.2), 0.8, new Lambertian(vec3(0.4, 0.2, 0.1)));
	//list[i++] = new Sphere(vec3(4, 0.8, 2), 0.8, new Metal(vec3(0.7, 0.6, 0.5), 0.0));
	list[i++] = new Sphere(vec3(0,0.8,2.5), 0.8, new Dielectric(1.5));
	list[i++] = new Sphere(vec3(-4,0.8,2.2), 0.8, new Lambertian(new ConstantTexture(vec3(0.4,0.2,0.1))));
	list[i++] = new Sphere(vec3(4,0.8,2), 0.8, new Metal(vec3(0.7,0.6,0.5), 0.0));
	return new HitableList(list, i);
}

Hitable* CornellBox()
{
	Hitable **list = new Hitable*[8];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.12)));
	Material *blue = new Lambertian(new ConstantTexture(vec3(0.1, 0.1, 0.65)));
	Material *light = new DiffuseLight(new ConstantTexture(vec3(15, 15, 15)));
	Material *checker = new Lambertian(new CheckerTexture(new ConstantTexture(vec3(0.1, 0.1, 0.65)), new ConstantTexture(vec3(0.73, 0.73, 0.73))));
	Material *noise = new Lambertian(new NoiseTexture());
	Material *roughBlue = new Microfacet();
	Material *metal = new Metal(vec3(0.65, 0.45, 0.1), 0.01);
	list[i++] = new FlipNormals(new YzRect(0, 555, 0, 555, 555, green));
	list[i++] = new YzRect(0, 555, 0, 555, 0, red);
	list[i++] = new FlipNormals(new XzRect(213, 343, 227, 332, 554, light));
	//list[i++] = new FlipNormals(new XzCircle(vec3(278, 554, 279), 60, light));
	//list[i++] = new FlipNormals(new XzTriangle(vec3(200, 554, 227), vec3(420, 554, 227), vec3(310, 554, 380), light));
	list[i++] = new FlipNormals(new XzRect(0, 555, 0, 555, 555, white));
	list[i++] = new XzRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals(new XyRect(0, 555, 0, 555, 555, white));
	/*list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), 
		vec3(130, 0, 65));*/
	list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(265, 0, 295));
	list[i++] = new Sphere(vec3(190, 90, 190), 90, roughBlue);
	return new HitableList(list, i);
}

int main() {
	srand(time(0));
    vec3_test();
    int nx = 300;
    int ny = 300;
	int ns = 50;

    ofstream outImg("test.ppm", ios_base::out);
    outImg << "P3\n" << nx << " " << ny << "\n255\n";
    /*Hitable *list[5];
    list[0] = new Sphere(vec3(0,0,-1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new Sphere(vec3(0,-100.5,-1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.1));
	list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	list[4] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
	*/
	Hitable *world = CornellBox();

	/*vec3 lookFrom(13, 3, 3);
	vec3 lookAt(0, 0, 0);
	float dist2focus = 10.0;
	float aperture = 0.0;
	Camera camera(lookFrom, lookAt, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist2focus);
    */
	vec3 lookFrom(278, 278, -800);
	vec3 lookAt(278, 278, 0);
	float dist2focus = 10.0;
	float aperture = 0.0;
	float vofv = 40.0;
	Camera camera(lookFrom, lookAt, vec3(0, 1, 0), vofv, float(nx) / float(ny), aperture, dist2focus);
	
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0.0, 0.0, 0.0);
			for (int k = 0; k < ns; k++)
			{
				float u = float(i + float(k/ns)) / float(nx);
				float v = float(j + HammersleyPhi(k)) / float(ny);
				Ray r = camera.getRay(u, v);
				col += color(r, world, 0);
			}

			// vec3 p = r.loc_at_param(2.0);
			col /= ns;
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outImg << ir << " " << ig << " " << ib << endl;
			
		}
		cout << j << endl;
	}
}

void vec3_test()
{
    vec3 a(1, 1, 1);
    vec3 b(-1, 1, -1);
    std::cout << a << " " << 2.0 * b << std::endl;
}
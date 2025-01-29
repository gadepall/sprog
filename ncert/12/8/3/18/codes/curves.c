#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "libs/matfun.h"
#include "libs/geofun.h"

typedef struct 
{
    double* x_values;
    double* y_values;
    int num_points;
    double a; 
    double x_min; 
    double x_max;
    double **V;
    double **u;
    double f; 
} Parabola;

typedef struct 
{
    double* x_values;
    double* y_values;
    int num_points;
    double r; 
    double **V;
    double **u;
    double f;
} Circle;

Parabola* create_parabola(int num_points, double a, double x_min, double x_max) 
{
    Parabola* parabola = (Parabola*)malloc(sizeof(Parabola));
    parabola->x_values = (double*)malloc(num_points * sizeof(double));
    parabola->y_values = (double*)malloc(num_points * sizeof(double));
    parabola->num_points = num_points;
    parabola->a = a;
    parabola->x_min = x_min;
    parabola->x_max = x_max;
    parabola->V = createMat(2, 2);
    parabola->u = createMat(2, 1);
    return parabola;
}

Circle* create_circle(int num_points, double r) 
{
    Circle* circle = (Circle*)malloc(sizeof(Circle));
    circle->x_values = (double*)malloc(num_points * sizeof(double));
    circle->y_values = (double*)malloc(num_points * sizeof(double));
    circle->num_points = num_points;
    circle->r = r;
    circle->V = createMat(2, 2);
    circle->u = createMat(2, 1);
    return circle;
}

void free_parabola(Parabola* parabola) 
{
    if (parabola) 
    {
        free(parabola->x_values);
        free(parabola->y_values);
        free(parabola->V);
        free(parabola->u);
        free(parabola);
    }
}

void free_circle(Circle* circle) 
{
    if (circle) 
    {
        free(circle->x_values);
        free(circle->y_values);
        free(circle->V);
        free(circle->u);
        free(circle);
    }
}

// Generating points of parabola y^2 = 4ax
void gen_parab(Parabola* parabola) 
{
    double step = (parabola->x_max - parabola->x_min) / (parabola->num_points - 1);
    for (int i = 0; i < parabola->num_points; i++) 
    {
        parabola->x_values[i] = parabola->x_min + i * step;
        parabola->y_values[i] = sqrt(4 * parabola->a * parabola->x_values[i]);
    }
}

// Generating points of circle
void gen_circle(Circle* circle) 
{
    double inclination_step = (2 * M_PI) / circle->num_points;
    for (int i = 0; i < circle->num_points; i++) 
    {
        double inclination = inclination_step * i;
        circle->x_values[i] = circle->r * cos(inclination);
        circle->y_values[i] = circle->r * sin(inclination);
    }
}

// Function to find intersections between the parabola and circle
void find_intersections(double* intersection_points, Parabola* parabola, Circle* circle) 
{
    circle->V[0][0] = pow(circle->r, 2);
    circle->V[1][0] = 0.0;
    circle->V[0][1] = 0.0;
    circle->V[1][1] = pow(circle->r, 2);
    
    parabola->V[0][0] = 0.0;
    parabola->V[1][0] = 0.0;
    parabola->V[0][1] = 0.0;
    parabola->V[1][1] = 1.0;
    
    circle->u[0][0] = 0.0;
    circle->u[1][0] = 0.0;
    
    parabola->u[0][0] = -2 * parabola->a;
    parabola->u[1][0] = 0.0;
    
    circle->f = -pow(circle->r, 4);
    parabola->f = 0.0;
    
    double mu = (double)(-1/pow(circle->r, 2));
    
    double cV1, cV2, cV3, cV4;
    double pV1, pV2, pV3, pV4;
    
    cV1 = circle->V[0][0];
    cV2 = circle->V[1][0];
    cV3 = circle->V[0][1];
    cV4 = circle->V[1][1];
    
    pV1 = parabola->V[0][0];
    pV2 = parabola->V[1][0];
    pV3 = parabola->V[0][1];
    pV4 = parabola->V[1][1];
    
    double cu1, cu2;
    double pu1, pu2;
    
    cu1 = circle->u[0][0];
    cu2 = circle->u[1][0];
    
    pu1 = parabola->u[0][0];
    pu2 = parabola->u[1][0];
    
    double cf, pf;
    
    cf = circle->f;
    pf = parabola->f;
    
    double x_coordinate, y_coordinate;
    
    double A, B, C;
    A = pV1 + pV2 + mu * (cV1 + cV2);
    B = 2 * (pu1 + mu * cu1) + 4 * parabola->a * (pV3 + pV4 + mu * (cV3 + cV4));
    C = pf + mu * (cf);
    
    double discriminant = pow(B, 2) - 4 * A * C;
    if ( discriminant < 0 ) 
    {
	printf("no intersections\n");
    } 
    else if ( discriminant == 0 ) 
    {
	x_coordinate = -B / (2 * A);
	y_coordinate = sqrt(4 * parabola->a * x_coordinate);
    } 
    else 
    {
	double x_coordinate_1 = (-B + sqrt(discriminant)) / (2 * A);
	double x_coordinate_2 = (-B - sqrt(discriminant)) / (2 * A);
	    
	if (x_coordinate_1 > 0) 
	{
	    x_coordinate = x_coordinate_1;
	    y_coordinate = sqrt(4 * parabola->a * x_coordinate);
	} 
	else 
	{
	    x_coordinate = x_coordinate_2;
	    y_coordinate = sqrt(4 * parabola->a * x_coordinate);
	}
    }
    intersection_points[0] = x_coordinate; 
    intersection_points[1] = y_coordinate;
    intersection_points[2] = x_coordinate; 
    intersection_points[3] = -y_coordinate;  
}

// Calculating the area under the parabola using the trapezoidal rule
double area_parab(Parabola* parabola, double x_start, double x_end) 
{
    double step = (x_end - x_start) / (parabola->num_points - 1);
    double area = 0.0;

    for (int i = 0; i < parabola->num_points - 1; i++) 
    {
        double x_left = x_start + i * step;
        double x_right = x_start + (i + 1) * step;

        double y_left = sqrt(4 * parabola->a * x_left);
        double y_right = sqrt(4 * parabola->a * x_right);

        area += (y_left + y_right) * step / 2; 
    }

    return 2 * area; 
}

// Calculating the area under the circle using the trapezoidal rule
double area_circle(Circle* circle, double x_start, double x_end) 
{
    double step = (x_end - x_start) / (circle->num_points - 1);
    double area = 0.0;

    for (int i = 0; i < circle->num_points - 1; i++) 
    {
        double x_left = x_start + i * step;
        double x_right = x_start + (i + 1) * step;

        double y_left = sqrt(pow(circle->r, 2) - pow(x_left, 2));
        double y_right = sqrt(pow(circle->r, 2) - pow(x_right, 2));

        area += (y_left + y_right) * step / 2;
    }

    return 2 * area;
}


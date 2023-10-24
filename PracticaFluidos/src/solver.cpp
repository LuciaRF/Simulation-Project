//Lucía Rodríguez Fernández


#include "solver.h"
#include <stdlib.h>
#include <stdio.h>


void Solver::Init(unsigned N, float dt, float diff, float visc)
{
	this->dt = dt;
	this->diff = diff;//k densidad
	this->visc = visc; //v velocidad
	this->N = N;
}
/*
----------------------------------------------------------------------
free/clear/allocate simulation data
----------------------------------------------------------------------
*/
void Solver::FreeData(void)
{
//TODO: Libera los buffers de memoria.  liberar arrays
	free(u_prev);
	free(v_prev);
	free(dens_prev);
	free(u);
	free(v);
	free(dens);
}

void Solver::ClearData(void)
{
//TODO: Borra todo el contenido de los buffers.  Lo pone a cero

	for (int i = 0; i < (N + 2) * (N + 2); i++) {
		u[i] = 0;
		v[i] = 0;
		dens[i] = 0;

	}
}

bool Solver::AllocateData(void)
{
	bool resul;
//TODO:
//Reservamos memoria, en caso de fallo devlvemos false.
//Antes de devolver true, hay que limpiar la memoria reservada con un ClearData().

	u = (float*)malloc(((N+2)*(N+2)) * sizeof(float));
	v = (float*)malloc(((N+2)*(N+2)) * sizeof(float));
	dens = (float*)malloc(((N+2)*(N+2)) * sizeof(float));
	u_prev = (float*)malloc(((N+2)*(N+2)) * sizeof(float));
	v_prev = (float*)malloc(((N+2)*(N+2)) * sizeof(float));
	dens_prev = (float*)malloc(((N+2)*(N+2)) * sizeof(float));

	if (u == nullptr || v  == nullptr || dens == nullptr|| u_prev == nullptr || v_prev == nullptr || dens_prev == nullptr) {
		resul = false;
		FreeData();
	}
	else {
		ClearData();
		ClearPrevData();
		resul = true;
	}
	
	return resul;
}

void Solver::ClearPrevData() 
{
//TODO: Borra el contenido de los buffers _prev SOLO
	for (int i = 0; i < (N + 2) * (N + 2); i++) {
		u_prev[i] = 0;
		v_prev[i] = 0;
		dens_prev[i] = 0;

	}
}

void Solver::AddDensity(unsigned x, unsigned y, float source)
{
//TODO: Añade el valor de source al array de densidades. Sería interesante usar la macro: XY_TO_ARRAY

	FOR_EACH_CELL
		dens_prev[XY_TO_ARRAY(x, y)] = source;
	END_FOR
}

void Solver::AddVelocity(unsigned x, unsigned y, float forceX, float forceY)
{
//TODO: Añade el valor de fuerza a sus respectivos arrays. Sería interesante usar la macro: XY_TO_ARRAY
	FOR_EACH_CELL
		u_prev[XY_TO_ARRAY(x, y)] = forceX;
		v_prev[XY_TO_ARRAY(x, y)] = forceY;
	END_FOR
}

void Solver::Solve()
{
	VelStep();
	DensStep();
}

void Solver::DensStep()
{
	AddSource(dens, dens_prev);			//Adding input density (dens_prev) to final density (dens).
	SWAP(dens_prev, dens)				//Swapping matrixes, because we want save the next result in dens, not in dens_prev.
	Diffuse(0, dens, dens_prev);		//Writing result in dens because we made the swap before. bi = dens_prev. The initial trash in dens matrix, doesnt matter, because it converges anyways.
	SWAP(dens_prev, dens)				//Swapping matrixes, because we want save the next result in dens, not in dens_prev.
	Advect(0, dens, dens_prev, u, v);	//Advect phase, result in dens.
}

void Solver::VelStep()
{
	AddSource(u, u_prev);
	AddSource(v, v_prev);
	SWAP (u_prev,u)			
	SWAP (v_prev, v)
	Diffuse(1, u, u_prev);  
	Diffuse(2, v, v_prev); 
	Project(u, v, u_prev, v_prev);		//Mass conserving.
	SWAP (u_prev,u)			
	SWAP (v_prev,v)
	Advect(1, u, u_prev, u_prev, v_prev);
	Advect(2, v, v_prev, u_prev, v_prev);
	Project(u, v, u_prev, v_prev);		//Mass conserving.
}

void Solver::AddSource(float * base, float * source)
{
//TODO: Teniendo en cuenta dt (Delta Time), incrementar el array base con nuestro source. Esto sirve tanto para añadir las nuevas densidades como las nuevas fuerzas.


	for (int i = 0; i < (N + 2) * (N + 2); i++) {
		base[i] += dt * source[i];
	}

}


void Solver::SetBounds(int b, float * x)
{
	int j = 0;
	int i = 0;

	//indices del cubo dentro de la escena
	int A = 32;
	int B = 60;

/*TODO:
Input b: 0, 1 or 2.
	0: borders = same value than the inner value.
	1: x axis borders inverted, y axis equal.
	2: y axis borders inverted, x axis equal.
	Corner values allways are mean value between associated edges.
*/


	switch (b)
	{
	case 0:

		for (i = 1; i <= N; i++) {
			x[XY_TO_ARRAY(i, 0)] = x[XY_TO_ARRAY(i, 1)];
			x[XY_TO_ARRAY(i, N)] = x[XY_TO_ARRAY(i, N - 1)];
		}

		for (j = 1; j <= N; j++) {
			x[XY_TO_ARRAY(0, j)] = x[XY_TO_ARRAY(1, j)];
			x[XY_TO_ARRAY(N, j)] = x[XY_TO_ARRAY(N - 1, j)];
		}


		//cuadrado

		for (i = A; i <= B; i++) {
			x[XY_TO_ARRAY(i, A)] = x[XY_TO_ARRAY(i, A + 1)];
			x[XY_TO_ARRAY(i, B)] = x[XY_TO_ARRAY(i, B - 1)];
		}

		for (j = A; j <= B; j++) {
			x[XY_TO_ARRAY(A, j)] = x[XY_TO_ARRAY(A + 1, j)];
			x[XY_TO_ARRAY(B, j)] = x[XY_TO_ARRAY(B - 1, j)];
		}

		for (i = A + 1; i < B; i++) {
			for (j = A + 1; j < B; j++) {
				x[XY_TO_ARRAY(i, j)] = 0;
			}
		}



		x[XY_TO_ARRAY(0, 0)] = (x[XY_TO_ARRAY(1, 0)] + x[XY_TO_ARRAY(0, 1)]) / 2;
		x[XY_TO_ARRAY(N+1, 0)] = (x[XY_TO_ARRAY(N, 0)] + x[XY_TO_ARRAY(N+1, 1)]) / 2;
		x[XY_TO_ARRAY(0, N+1)] = (x[XY_TO_ARRAY(0, N)] + x[XY_TO_ARRAY(1, N+1)]) / 2;
		x[XY_TO_ARRAY(N+1,N+1)] = (x[XY_TO_ARRAY(N, N)] + x[XY_TO_ARRAY(N, N+1)]) / 2;

		break;

	case 1:

		for (i = 1; i <= N; i++) {
			x[XY_TO_ARRAY(i, 0)] = x[XY_TO_ARRAY(i, 1)];
			x[XY_TO_ARRAY(i, N)] = x[XY_TO_ARRAY(i, N - 1)];
		}

		for (j = 1; j <= N; j++) {
			x[XY_TO_ARRAY(0, j)] = -x[XY_TO_ARRAY(1, j)];
			x[XY_TO_ARRAY(N, j)] = -x[XY_TO_ARRAY(N - 1, j)];
		}
		//cuadrado

		for (i = A; i <= B; i++) {
			x[XY_TO_ARRAY(i, A)] = x[XY_TO_ARRAY(i, A + 1)];
			x[XY_TO_ARRAY(i, B)] = x[XY_TO_ARRAY(i, B - 1)];
		}

		for (j = A; j <= B; j++) {
			x[XY_TO_ARRAY(A, j)] = -x[XY_TO_ARRAY(A + 1, j)];
			x[XY_TO_ARRAY(B, j)] = -x[XY_TO_ARRAY(B - 1, j)];
		}

		for (i = A + 1; i < B; i++) {
			for (j = A + 1; j < B; j++) {
				x[XY_TO_ARRAY(i, j)] = 0;
			}
		}


		x[XY_TO_ARRAY(0, 0)] = (x[XY_TO_ARRAY(1, 0)] + x[XY_TO_ARRAY(0, 1)]) / 2;
		x[XY_TO_ARRAY(N + 1, 0)] = (x[XY_TO_ARRAY(N, 0)] + x[XY_TO_ARRAY(N + 1, 1)]) / 2;
		x[XY_TO_ARRAY(0, N + 1)] = (x[XY_TO_ARRAY(0, N)] + x[XY_TO_ARRAY(1, N + 1)]) / 2;
		x[XY_TO_ARRAY(N + 1, N + 1)] = (x[XY_TO_ARRAY(N, N)] + x[XY_TO_ARRAY(N, N + 1)]) / 2;


		break;

	case 2:

		for (i = 1; i <= N; i++) {
			x[XY_TO_ARRAY(i, 0)] = -x[XY_TO_ARRAY(i, 1)];
			x[XY_TO_ARRAY(i, N)] = -x[XY_TO_ARRAY(i, N - 1)];
		}

		for (j = 1; j <= N; j++) {
			x[XY_TO_ARRAY(0, j)] = x[XY_TO_ARRAY(1, j)];
			x[XY_TO_ARRAY(N, j)] = x[XY_TO_ARRAY(N - 1, j)];
		}

		//cuadrado

		for (i = A; i <= B; i++) {
			x[XY_TO_ARRAY(i, A)] = -x[XY_TO_ARRAY(i, A + 1)];
			x[XY_TO_ARRAY(i, B)] = -x[XY_TO_ARRAY(i, B - 1)];
		}

		for (j = A; j <= B; j++) {
			x[XY_TO_ARRAY(A, j)] = x[XY_TO_ARRAY(A + 1, j)];
			x[XY_TO_ARRAY(B, j)] = x[XY_TO_ARRAY(B - 1, j)];
		}

		for (i = A + 1; i < B; i++) {
			for (j = A + 1; j < B; j++) {
				x[XY_TO_ARRAY(i, j)] = 0;
			}
		}


		x[XY_TO_ARRAY(0, 0)] = (x[XY_TO_ARRAY(1, 0)] + x[XY_TO_ARRAY(0, 1)]) / 2;
		x[XY_TO_ARRAY(N + 1, 0)] = (x[XY_TO_ARRAY(N, 0)] + x[XY_TO_ARRAY(N + 1, 1)]) / 2;
		x[XY_TO_ARRAY(0, N + 1)] = (x[XY_TO_ARRAY(0, N)] + x[XY_TO_ARRAY(1, N + 1)]) / 2;
		x[XY_TO_ARRAY(N + 1, N + 1)] = (x[XY_TO_ARRAY(N, N)] + x[XY_TO_ARRAY(N, N + 1)]) / 2;

		break;
	}

}

/*
https://www.youtube.com/watch?v=62_RUX_hrT4
https://es.wikipedia.org/wiki/M%C3%A9todo_de_Gauss-Seidel <- Solución de valores independientes.
Despreciando posibles valores de x no contiguos, se simplifica mucho. Mirar diapositivas y la solución de Gauss Seidel de términos independientes.
Gauss Seidel -> Matrix x and x0
*/
void Solver::LinSolve(int b, float * x, float * x0, float aij, float aii)
{
//TODO: Se recomienda usar FOR_EACH_CELL, END_FOR y XY_TO_ARRAY.

	int iter = 20;

	for (int k = 0; k < iter; k++) {

		FOR_EACH_CELL
			x[XY_TO_ARRAY(i, j)] = (x0[XY_TO_ARRAY(i, j)] + aij * (x[XY_TO_ARRAY(i + 1, j)] + x[XY_TO_ARRAY(i - 1, j)] + x[XY_TO_ARRAY(i, j + 1)] + x[XY_TO_ARRAY(i, j - 1)]))/aii;
		END_FOR

	}

	SetBounds(b, x);

}

void Solver::SOR(int b, float* x, float* x0, float aij, float aii, float w)
{
	//TODO: Se recomienda usar FOR_EACH_CELL, END_FOR y XY_TO_ARRAY.

	int iter = 20;

	for (int k = 0; k < iter; k++) {

		//método GS necesario para SOR + los cambios necesarios para SOR
		FOR_EACH_CELL
			x[XY_TO_ARRAY(i, j)] = (1-w)* x0[XY_TO_ARRAY(i, j)] + (w * (x0[XY_TO_ARRAY(i, j)] + aij * (x[XY_TO_ARRAY(i + 1, j)] + x[XY_TO_ARRAY(i - 1, j)] + x[XY_TO_ARRAY(i, j + 1)] + x[XY_TO_ARRAY(i, j - 1)])) / aii);
		END_FOR

	}

	//Jcobi
	

		//FOR_EACH_CELL
		//	for (int k = 0; k < iter; k++) {
		//	x0[XY_TO_ARRAY(i, j)] = (x0[XY_TO_ARRAY(i, j)] + aij * (x0[XY_TO_ARRAY(i + 1, j)] + x0[XY_TO_ARRAY(i - 1, j)] + x0[XY_TO_ARRAY(i, j + 1)] + x0[XY_TO_ARRAY(i, j - 1)])) / aii;
		//	}
		//x[XY_TO_ARRAY(i, j)] = x0[XY_TO_ARRAY(i, j)];
		//END_FOR

	

	SetBounds(b, x);

}

/*
Nuestra función de difusión solo debe resolver el sistema de ecuaciones simplificado a las celdas contiguas de la casilla que queremos resolver,
por lo que solo con la entrada de dos valores, debemos poder obtener el resultado.
*/
void Solver::Diffuse(int b, float * x, float * x0)
{
//TODO: Solo necesitaremos pasar dos parámetros a nuestro resolutor de sistemas de ecuaciones de Gauss Seidel. Calculamos dichos valores y llamamos a la resolución del sistema.
	float aij = 0.0f;
	float aii = 0.0f;
	float h = 1.0f / N;
	
	switch (b)
	{
	case 0:
		aij = (diff * dt) / (h*h);
		aii = 4 * aij + 1;
		break;
	case 1:
		aij = (visc * dt) / (h * h);

		aii = 4 * aij + 1;
		break;
	case 2:
		aij = (visc * dt) / (h * h);
		aii = 4 * aij + 1;
		break;
	}

	LinSolve(b, x, x0, aij, aii);

}

/*
d is overwrited with the initial d0 data and affected by the u & v vectorfield.
Hay que tener en cuenta que el centro de las casillas representa la posición entera dentro de la casilla, por lo que los bordes estan
en las posiciones x,5.
*/
void Solver::Advect(int b, float * d, float * d0, float * u, float * v)
{
//TODO: Se aplica el campo vectorial realizando una interploación lineal entre las 4 casillas más cercanas donde caiga el nuevo valor.
	float x, y = 0.0f;
	int id0, id1, jd0, jd1 = 0;
	float dx, dy,d00,d10,d01,d11 = 0.0f;



	for (int i = 0; i < N+1; i++) { 
		for (int j = 0; j < N+1; j++) {

			x = i - u[XY_TO_ARRAY(i, j)] * dt * N;
			y = j - v[XY_TO_ARRAY(i, j)] * dt * N;
			
			//sacamos los indices de x

			if (x <= 0.5f) {
				x = 0.5f;
			}
			else if(x>N+0.5f) {
				x = N+0.5f;
			}

			if (y <= 0.5f) {
				y = 0.5f;
			}
			else if (y > N + 0.5f) {
				y = N + 0.5f;
			}

			id0 = (int)x;
			id1 = id0 + 1;

			jd0 = (int)y;
			jd1 = jd0 + 1;

			dx = x - id0;
			dy = y - jd0;
			d00 = d0[XY_TO_ARRAY(id0, jd0)];
			d01 = d0[XY_TO_ARRAY(id0, jd1)];
			d10 = d0[XY_TO_ARRAY(id1, jd0)];
			d11 = d0[XY_TO_ARRAY(id1, jd1)];

			d[XY_TO_ARRAY(i, j)] = (1 - dx) * (1 - dy) * d00 + dx * (1 - dy) * d10 + (1 - dx) * dy * d01 + dx * dy * d11;
			
		}
	}

	SetBounds(b, d);
}
	


/*
Se encarga de estabilizar el fluido y hacerlo conservativo de masa. Se usa solo en las matrices de velocidades.
No necesaria implementación por su complejidad.
*/
void Solver::Project(float * u, float * v, float * p, float * div)
{
	int i, j;
	float h = 1.0f / N;
	float w = 0.01f;

	FOR_EACH_CELL
		div[XY_TO_ARRAY(i, j)] = -0.5f*(u[XY_TO_ARRAY(i + 1, j)] - u[XY_TO_ARRAY(i - 1, j)] + v[XY_TO_ARRAY(i, j + 1)] - v[XY_TO_ARRAY(i, j - 1)]) / N;
		p[XY_TO_ARRAY(i, j)] = 0;
	END_FOR
	SetBounds(0, div);
	SetBounds(0, p);

	//LinSolve(0, p, div, 1, 4); //llamar otro metodo en lugar de LinSolve
	SOR(0, p, div, 1, 4,w);

	//Aproximamos: Laplaciano de q a su gradiente.
	FOR_EACH_CELL
		u[XY_TO_ARRAY(i, j)] -= 0.5f*N*(p[XY_TO_ARRAY(i + 1, j)] - p[XY_TO_ARRAY(i - 1, j)]);
		v[XY_TO_ARRAY(i, j)] -= 0.5f*N*(p[XY_TO_ARRAY(i, j + 1)] - p[XY_TO_ARRAY(i, j - 1)]);
	END_FOR
	SetBounds(1, u);
	SetBounds(2, v);
}
#include "helper.h"
#include "visual.h"
#include "init.h"
#include "uvp.h"
#include "boundary_val.h"
#include "sor.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

/**
 * The main operation reads the configuration file, initializes the scenario and
 * contains the main loop. So here are the individual steps of the algorithm:
 *
 * - read the program configuration file using read_parameters()
 * - set up the matrices (arrays) needed using the matrix() command
 * - create the initial setup init_uvp(), init_flag(), output_uvp()
 * - perform the main loop
 * - trailer: destroy memory allocated and do some statistics
 *
 * The layout of the grid is decribed by the first figure below, the enumeration
 * of the whole grid is given by the second figure. All the unknowns corresond
 * to a two dimensional degree of freedom layout, so they are not stored in
 * arrays, but in a matrix.
 *
 * @image html grid.jpg
 *
 * @image html whole-grid.jpg
 *
 * Within the main loop the following big steps are done (for some of the 
 * operations a definition is defined already within uvp.h):
 *
 * - calculate_dt() Determine the maximal time step size.
 * - boundaryvalues() Set the boundary values for the next time step.
 * - calculate_fg() Determine the values of F and G (diffusion and confection).
 *   This is the right hand side of the pressure equation and used later on for
 *   the time step transition.
 * - calculate_rs()
 * - Iterate the pressure poisson equation until the residual becomes smaller
 *   than eps or the maximal number of iterations is performed. Within the
 *   iteration loop the operation sor() is used.
 * - calculate_uv() Calculate the velocity at the next time step.
 */
int main(int argn, char** args){


    // check the correctness of the input parameters
    //
    // stop the execution if:
    //
    // 1. the number of input parameters is not equal to two
    //    ( including both the program name and the name of the input data file )
    //
    // 2. there is no such file at the project folder
    //
    if ( argn == 2 ) {

        // check if it's possible to open the file
        FILE* file;
        file = fopen( args[1], "r" );

        // Abort execution if it's not possible to open the file
        if ( file == 0 ) {
            printf( "\nERROR: The file comtaining the input data doesn't exsist\n\n" );
            printf( "HINT:   Check the file spelling or its existence and try again\n\n");

            return -1;
        }

    }
    else {

        // Abort execution if the number of input parameters is wrong
        printf( "\nERROR: The number of input parameters is wrong\n\n" );

        printf( "HINT:   You've probably forgot to pass the input file" );
        printf( " as a parameter\n" );
        printf( "\tor you've passed more than one parameter\n\n" );

        printf( "Example: ./sim cavity100.dat\n\n" );
        return -1;
    }



	// declare the matrices
	double **U, **V, **P;
	
	double **RS, **F, **G;

	// Declare variables for the parameters that will be read from the input file
	double Re,          // Reynalds number
	       UI,          // initial values of the U velocity component
	       VI,          // initial values of the V velocity component
	       PI,          // initial values of the pressure
	       GX,          // acceleration due to gravity in x-direction
	       GY,          // acceleration due to gravity in y-direction
	       t_end,       // final of simulation time
	       xlength,     // dimension of the problem in x-direction
	       ylength,     // dimension of the problem in y-direction
	       dt,          // time step
	       dx,          //Grid size in x-direction
	       dy,         //Grid size in y-direction
	       alpha,
	       omg,         // SOR-relaxation factor
	       tau,         //safety factor
	       eps,         // numerical precision
	       dt_value;    // time for output
	
	int  imax, //number of cells x-direction
	     jmax, //number of cells y-direction
	     itermax;




    // Reading parameters from input file
    const char* INPUT_FILE_NAME = args[1];
	read_parameters( INPUT_FILE_NAME,
	                 &Re,
	                 &UI,
	                 &VI,
	                 &PI,
	                 &GX,
	                 &GY,
	                 &t_end,
	                 &xlength,
	                 &ylength,
	                 &dt,
	                 &dx,
	                 &dy,
	                 &imax,
	                 &jmax,
	                 &alpha,
	                 &omg,
	                 &tau,
	                 &itermax,
	                 &eps,
	                 &dt_value );


	// Allocating memory for matrices: U, V and P
	U = matrix(0, imax + 1, 0, jmax + 1);
	V = matrix(0, imax + 1, 0, jmax + 1);
	P = matrix(0, imax + 1, 0, jmax + 1);

	// iniitialize U, V and P matrices
	init_uvp(UI, VI, PI, imax, jmax, U, V, P);


    // Allocating memory for matrices: U, V and P
	RS = matrix(0, imax + 1, 0, jmax + 1);
	F = matrix(0, imax + 1, 0, jmax + 1);
	G = matrix(0, imax + 1, 0, jmax + 1 );


    // iniitialize RS, F and G matrixes
	double InitialMatrixValues = 0.0;
	init_matrix(RS, 0, imax + 1, 0, jmax + 1, InitialMatrixValues);
    init_matrix(F, 0, imax + 1, 0, jmax + 1, InitialMatrixValues);
    init_matrix(G, 0, imax + 1, 0, jmax + 1, InitialMatrixValues);



	//Some additional variables
	double t = 0.0; // Time of simulation
	const double InitialResidualValue = 1.0;
	double Residual = InitialResidualValue;
	int TimeStepNumber = 0;
	int SolverIterationNumber = 0;
	const char* OUPUT_FILE_NAME = "Cavity100";
	double FrameCounter = 0.0;


    // Initializing timer
    clock_t Begin = clock();

	//The main while loop that iterates over time
	while ( t < t_end ) {

		//Calcaulte the adaptive time step
		if (tau >= 0.0)
		    calculate_dt(Re, tau, &dt, dx, dy, imax, jmax, U, V);


		// Set the boundary for U and V
		boundaryvalues(imax, jmax, U, V);

		// Calcualte F and G
		calculate_fg(Re, GX, GY, alpha, dt, dx, dy, imax, jmax, U, V, F, G);


		// Calculate the RHS of Pressure Poisson Equation
		calculate_rs(dt, dx, dy, imax, jmax, F, G, RS);

		SolverIterationNumber = 0;
		Residual = InitialResidualValue;

		//Perform SOR
		while ( ( SolverIterationNumber < itermax ) && ( Residual > eps )) {

		    //Perform one SOR iteration
			sor (omg, dx, dy, imax, jmax, P, RS, &Residual);
			SolverIterationNumber++;
		}


		if ( SolverIterationNumber == itermax ) {
			printf("\nSOR not converged ");
			printf("at the time: %f\n", t);
			printf("time iteration: %d", TimeStepNumber);

            // increas the current simulation time
            // to abort while loop
            //
            // IDEA: if we can't achieve convergence at the current
            // time step we stop proceeding the simulation since
            // the futher steps will be meaningless in terms of
            // the physics
            t = t_end;
        }


		// Update the velocities U and V
		calculate_uv(dt, dx, dy, imax, jmax, U, V, F, G, P);


        // write result to a vtk file each ${dt_value} seconds
        if ( floor( FrameCounter / dt_value ) ) {

                write_vtkFile(  OUPUT_FILE_NAME,
                                TimeStepNumber,
                                xlength,
                                ylength,
                                imax,
                                jmax,
                                dx,
                                dy,
                                U,
                                V,
                                P );

                FrameCounter = 0.0;
	}


		//Update the loop variables
		t += dt;
		++TimeStepNumber;
		FrameCounter += dt;
	}
	

	// display the output information
    clock_t End = clock();
    double ConsumedTime = (double)( End - Begin ) / CLOCKS_PER_SEC;

    printf("\n\nComputational time: %f sec\n", ConsumedTime);
    printf("Mesh size [ imax x jmax ]: %d x %d \n", imax, jmax ); 
    printf("Relaxation factor [ omega ]: %4.4f \n\n", omg );



    // write the last step to a vtk file
    write_vtkFile(  OUPUT_FILE_NAME,
                    TimeStepNumber,
                    xlength,
                    ylength,
                    imax,
                    jmax,
                    dx,
                    dy,
                    U,
                    V,
                    P );


	//Free the memory held by matrix
	free_matrix(U, 0, imax + 1, 0, jmax + 1);
	free_matrix(V, 0, imax + 1, 0, jmax + 1);
	free_matrix(P, 0, imax + 1, 0, jmax + 1);

	free_matrix(RS, 0, imax + 1, 0, jmax + 1);
	free_matrix(F, 0, imax + 1, 0, jmax + 1);
	free_matrix(G, 0, imax + 1, 0, jmax + 1);

	return 0;
}







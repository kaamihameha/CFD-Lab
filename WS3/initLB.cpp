#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <iostream>


void initialiseFields( double *collideField,
					   double *streamField,
					   int *flagField,
					   int *xlength ) {


	//Variable declaration
	int Current_Filed_Cell = 0;
	int Current_Flag_Cell = 0;


   //Initialization of collideField
	for( int z = 0 ; z <= xlength[2] + 1; ++z )  {
		for( int y = 0 ; y <= xlength[2] + 1 ; ++y )  {
			for( int x = 0 ; x <= xlength[0] + 1 ; ++x ) {

				Current_Filed_Cell = computeFieldIndex( x, y, z, xlength );

		        for( int i = 0 ; i < Vel_DOF ; ++i ) {
					//Initialization of collideField
		          	collideField [ Current_Filed_Cell + i ] = LATTICEWEIGHTS[ i ];

					//Initialization of streamField
				  	streamField [ Current_Filed_Cell + i ] = LATTICEWEIGHTS[ i ];
		        }


				//Initialization of flagField
				Current_Flag_Cell = computeFlagIndex( x, y, z, xlength );

				if( z == ( xlength[0] + 1 ) ) {
					flagField [ Current_Flag_Cell ] = MOVING_WALL;
				}
				else if ( ( x == 0 ) || ( y == 0 ) || ( z == 0 )
					   || ( x == ( xlength[0] + 1 ) ) || y == ( ( xlength[0] + 1 ) ) ) {

					flagField [ Current_Flag_Cell ] = WALL;
				}
				else {
					flagField [ Current_Flag_Cell ] = FLUID ;
				}
			}
		}
	}
}
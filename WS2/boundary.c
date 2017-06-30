#include "boundary.h"
#include "LBDefinitions.h"
<<<<<<< HEAD
#include "computeCellValues.h"
#include <math.h>

double dotProduct(double * A , double *B) {
	return (A[0]*B[0]) + (A[1]*B[1]) + (A[2]*B[2]) ;
}



/*
This is stupid implementation
void mixedBoundary( double *collideField,
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength,
					int Square_xlength,
					double Inverse_Square_CS,
					int X_Coordinate,
					int Y_Coordinate,
					int Z_Coordinate,
					int Current_Cell){

	double Density = 1.0 ;
	double Velocity[3] = {0.0};
	int Target_Cell = 0 ;
	computeDensity( (collideField+Current_Cell) , Density ) ;

	for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
		Target_Cell = Vel_DOF * ( ( ( Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
					+ ( ( Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength )
					+ ( X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );
		if( flagField[Target_Cell] == 1 ) {
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + 2 * ( 9 -Vel_Component )] ;
		}
		if( flagField[Target_Cell] == 2 ) {
			Velocity[ 0 ] = LATTICEVELOCITIES[ Vel_Component ][ 0 ];
			Velocity[ 1 ] = LATTICEVELOCITIES[ Vel_Component ][ 1 ];
			Velocity[ 2 ] = LATTICEVELOCITIES[ Vel_Component ][ 2 ];
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + abs( 18-Vel_Component)]
													   + ( 2 * LATTICEWEIGHTS[Vel_Component] * Density
													   * dotProduct(wallVelocity,Velocity) * Inverse_Square_CS );
		}
	}
}

void movingBoundary( double *collideField,
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength,
					int Square_xlength,
					double Inverse_Square_CS,
					int X_Coordinate,
					int Y_Coordinate,
					int Z_Coordinate,
					int Current_Cell){

	double Density = 1.0 ;
	double Velocity[3] = {0.0};
	int Target_Cell = 0 ;
	computeDensity( (collideField+Current_Cell) , Density ) ;


	for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
		Target_Cell = Vel_DOF * ( ( ( Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
					+ ( ( Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength )
					+ ( X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );
		if( flagField[Target_Cell] == 2 ) {
			Velocity[ 0 ] = LATTICEVELOCITIES[ Vel_Component ][ 0 ];
			Velocity[ 1 ] = LATTICEVELOCITIES[ Vel_Component ][ 1 ];
			Velocity[ 2 ] = LATTICEVELOCITIES[ Vel_Component ][ 2 ];
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + 2 * ( 9 -Vel_Component )]
													   + ( 2 * LATTICEWEIGHTS[Vel_Component] * Density
													   * dotProduct(wallVelocity,Velocity) * Inverse_Square_CS );
		}
	}
}

void noSlipBoundary( double *collideField,
                    int* flagField,
                    int xlength,
					int Square_xlength,
					int X_Coordinate,
					int Y_Coordinate,
					int Z_Coordinate,
					int Current_Cell){

	int Target_Cell = 0 ;

	for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
		Target_Cell = Vel_DOF * ( ( ( Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
					+ ( ( Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength )
					+ ( X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );
		if( flagField[Target_Cell] == 1 ) {
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + 2 * ( 9 -Vel_Component )] ;
		}
	}
}


void treatBoundary( double *collideField,
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength ){

	int X_Coordinate = 0, Y_Coordinate = 0, Z_Coordinate = 1;
	int Current_Cell = 0, Target_Cell = 0;
	int Square_xlength = xlength * xlength;
	double Inverse_Square_CS = 1.0 / ( C_S * C_S );
=======
#include <stdlib.h>

#include "DataStructure.h"
#include <list>
#include <iostream>
#include <stdio.h>
#include "helper.h"

void scanBoundary( std::list<Fluid*>& ObstacleList,
                    int* flagField,
                    unsigned xlength,
                    double* wallVelocity ) {

    unsigned Current_Cell_Flag = 0;
    unsigned Neighbour_Cell_Flag = 0;
	unsigned Current_Cell_Field = 0;
    unsigned Neighbour_Cell_Field = 0;
	double Dot_Product = 0.0;
    unsigned TotalLength = xlength + 2;

    for( unsigned z = 1 ; z <= xlength; ++z )  {
        for( unsigned y = 1 ; y <= xlength; ++y )  {
            for( unsigned x = 1 ; x <= xlength; ++x ) {

                // Compute the current cell
                Current_Cell_Flag = computeFlagIndex( x, y, z, TotalLength );
				Current_Cell_Field = Vel_DOF * computeFlagIndex( x, y, z, TotalLength );

                // Allocate a new fluid cell
                Fluid* aFluidCell = new Fluid( Current_Cell_Flag );

                // scan neighbours
                for ( unsigned i = 0; i < Vel_DOF; ++i ) {


                    Neighbour_Cell_Flag = computeFlagIndex(  x + LATTICEVELOCITIES[ i ][ 0 ],
                                                             y + LATTICEVELOCITIES[ i ][ 1 ],
                                                             z + LATTICEVELOCITIES[ i ][ 2 ],
                                                             TotalLength );


                    // add neighbours cell ( which is wall or moving wall to the list )
                    if ( flagField[ Neighbour_Cell_Flag ] == WALL ) {

						Dot_Product = 0.0;
						Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;

                        Obstacle* Wall = new StationaryWall( Neighbour_Cell_Field, //Here,Neighbour means wall
                                                             Current_Cell_Field,   //Here,Current means fluid
                                                             i,
                                                             Dot_Product );

                        aFluidCell->addObstacle( Wall );

                        }


                    if ( flagField[ Neighbour_Cell_Flag ] == MOVING_WALL ) {

						Dot_Product = ( wallVelocity[0] * LATTICEVELOCITIES[ 18 - i ][0] )
									+ ( wallVelocity[1] * LATTICEVELOCITIES[ 18 - i ][1] )
									+ ( wallVelocity[2] * LATTICEVELOCITIES[ 18 - i ][2] );
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343

	// XY plane with Z = 1
    for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
        }
    }

	//XY plane with Z = xlength excluding edges
	Z_Coordinate = xlength - 1 ;
	for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			movingBoundary( collideField, flagField, wallVelocity, xlength, Square_xlength,
							Inverse_Square_CS, X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
        }
    }

<<<<<<< HEAD
	//XY plane with Z = xlength only edges
	for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; X_Coordinate+=xlength-1 ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			mixedBoundary( collideField, flagField, wallVelocity, xlength, Square_xlength,
							Inverse_Square_CS, X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
=======
						Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;

                        Obstacle* Wall = new MovingWall( Neighbour_Cell_Field,//Here,Neighbour means wall
                                                         Current_Cell_Field,  //Here,Current means fluid
                                                         i,
                                                         Dot_Product );
                        aFluidCell->addObstacle( Wall );
                    }

                }

                // Delete a fluid cell if there was no obstacle cells
                if ( aFluidCell->isEmpty() == true ) {
                    delete aFluidCell;
                }
                else {
                    ObstacleList.push_back( aFluidCell );
                }

            }
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343
        }
    }
	for ( Y_Coordinate = 1; Y_Coordinate < xlength; Y_Coordinate+=xlength-1 ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			mixedBoundary( collideField, flagField, wallVelocity, xlength, Square_xlength,
							Inverse_Square_CS, X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
        }
    }

	//YZ plane
	for ( Z_Coordinate = 2; Z_Coordinate < xlength - 2; ++Z_Coordinate ) {
		//YZ plane with X_Coordinate = 1
		X_Coordinate = 1;
		for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}

		//YZ plane with X_Coordinate = xlength-1
		X_Coordinate = xlength-1;
		for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}
	}

	//XZ plane
	for ( Z_Coordinate = 2; Z_Coordinate < xlength - 2; ++Z_Coordinate ) {
		//XZ plane with Y_Coordinate = 1
		Y_Coordinate = 1;
		for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}

		//YZ plane with X_Coordinate = xlength-1
		Y_Coordinate = xlength-1;
		for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}
	}
}
*/

void teachBoundary( int* flagField,
										int xlength,
										int **NoSlip,
										int **MovingWall,
										double *MovingWallDotProduct,
										double * wallVelocity) {

	int X_Coordinate = 0, Y_Coordinate = 0, Z_Coordinate = 0;
	int Vel_Component = 0, Current_Cell = 0, Target_Cell = 0;
	int Square_xlength = xlength * xlength;
	int X_Target = 0, Y_Target = 0, Z_Target = 0;
	int CountNoSlip = 0, CountMovingWall = 0;
	double Inverse_Square_CS = 1.0 / ( C_S * C_S );
	double Velocity[3] = {0.0};

	//Looping through individual element
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength + 1 ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength + 1 ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength + 1 ; ++X_Coordinate ) {

				Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
				if(flagField[Current_Cell/Vel_DOF] == WALL) {
					//Cell wise streaming considering the velocity component

					for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
						NoSlip[CountNoSlip][0] = Current_Cell + Vel_Component;
						X_Target = X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ];
						Y_Target = Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ];
						Z_Target = Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ];
						if( X_Target < 0 || Y_Target < 0 || Z_Target < 0
						  ||X_Target > xlength+1 || Y_Target > xlength+1 || Z_Target > xlength+1 ) {
							Target_Cell = Vel_DOF * ( (  ( Z_Coordinate - LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
											+ ( ( Z_Coordinate - LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength ) + ( X_Coordinate - LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );

							NoSlip[CountNoSlip][1] = Target_Cell + (2 * ( 9 -Vel_Component ));
						}
						else {
							NoSlip[CountNoSlip][1] = Current_Cell + Vel_Component;
						}
						++CountNoSlip;
					}

				}
				if(flagField[Current_Cell/Vel_DOF] == MOVING_WALL) {
					//Cell wise streaming considering the velocity component
					for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
						MovingWall[CountMovingWall][0] = Current_Cell + Vel_Component;
						X_Target = X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ];
						Y_Target = Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ];
						Z_Target = Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ];
						if( X_Target < 0 || Y_Target < 0 || Z_Target < 0
						  ||X_Target > xlength+1 || Y_Target > xlength+1 || Z_Target > xlength+1 ) {
							Target_Cell = Vel_DOF * ( (  ( Z_Coordinate - LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
												+ ( ( Z_Coordinate - LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength ) + ( X_Coordinate - LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );

							MovingWall[CountMovingWall][1] = Target_Cell + (2 * ( 9 -Vel_Component ));
							Velocity[ 0 ] = LATTICEVELOCITIES[ Vel_Component ][ 0 ];
							Velocity[ 1 ] = LATTICEVELOCITIES[ Vel_Component ][ 1 ];
							Velocity[ 2 ] = LATTICEVELOCITIES[ Vel_Component ][ 2 ];
							MovingWallDotProduct[CountMovingWall] = 2 * LATTICEWEIGHTS[ Vel_Component ]
																	* dotProduct(wallVelocity, Velocity) * Inverse_Square_CS;
						}
						else {
							MovingWall[CountMovingWall][1] = Current_Cell + Vel_Component;
							MovingWallDotProduct[CountMovingWall] = 0.0;
						}
						++CountMovingWall;
					}

				}

			}
		}
	}

}

void treatBoundary( double *collideField,
                    int* flagField,
                    double * wallVelocity,
                    int xlength,
										int **NoSlip,
										int **MovingWall,
										double *MovingWallDotProduct ){

	double Density = 0.0;
	int Density_Index = 0;

	int NumberOfNoSlip = sizeof( NoSlip ) / sizeof( NoSlip[0] );
	//assumption: there are atleast some Moving walls
	int NumberOfMovingWall = sizeof( MovingWall ) / sizeof( MovingWall[0] );

	for( int i = 0; i < NumberOfNoSlip; ++i ) {
		collideField[NoSlip[i][1]] += collideField[NoSlip[i][0]];
	}

	for( int i = 0; i < NumberOfMovingWall; ++i ) {
		Density_Index = Vel_DOF * ((int)MovingWall[i][1]/Vel_DOF) ;
		computeDensity( collideField+Density_Index, &Density );
		collideField[MovingWall[i][1]] += collideField[MovingWall[i][0]]
																	 + Density * MovingWallDotProduct[i];

	}

}



void treatBoundary( double *collideField,
                    std::list<Fluid*>& BoundaryLayerList,
                    const double * const wallVelocity,
                    unsigned xlength ) {


    // iterate through out all boundary layer cells
    for ( std::list<Fluid*>::iterator FluidCell = BoundaryLayerList.begin();
          FluidCell != BoundaryLayerList.end();
          ++FluidCell ) {

              (*FluidCell)->processBoundary( collideField );

    }
}

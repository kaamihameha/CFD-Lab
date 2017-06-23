#include "DataStructure.h"
#include "helper.h"
#include "computeCellValues.h"
#include "LBDefinitions.h"

#include <list>
#include <iostream>
#include <stdlib.h>



//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------

/********************StationaryWall************************************/
inline void StationaryWall::treatBoundary( double * Field ) {


	int Reflected_Vel_Component = 18 - m_VelocityComponent;



	Field[ m_SelfIndex + Reflected_Vel_Component ]
                                = Field[ m_SourceIndex + m_VelocityComponent ];
}

/********************MovingWall************************************/
inline void MovingWall::treatBoundary( double * Field ) {

	double Density = 0.0;
	int Reflected_Vel_Component = 18 - m_VelocityComponent;


	computeDensity( Field + m_SourceIndex, &Density );

	Field[ m_SelfIndex + Reflected_Vel_Component ]
			          = Field[ m_SourceIndex + m_VelocityComponent ]
					  + ( 2.0 * LATTICEWEIGHTS[ m_VelocityComponent ] * Density
				          * m_DotProduct * InverseCS_Square );


}

/********************FreeSlip************************************/
inline void FreeSlip::treatBoundary( double * Field ) {

	unsigned  Reflected_Vel_Component[5] = { 0 };
	unsigned Source_Vel_Component[5] = { 0 };

	if(m_VelocityComponent == 2) {

		Reflected_Vel_Component[ 0 ] = 16;
		Reflected_Vel_Component[ 1 ] = 18;
		Reflected_Vel_Component[ 2 ] = 14;
		Reflected_Vel_Component[ 3 ] = 17;
		Reflected_Vel_Component[ 4 ] = 15;


		Source_Vel_Component[ 0 ] = 2;
		Source_Vel_Component[ 1 ] = 4;
		Source_Vel_Component[ 2 ] = 0;
		Source_Vel_Component[ 3 ] = 3;
		Source_Vel_Component[ 4 ] = 1;

	}

	else if(m_VelocityComponent == 6) {

		Reflected_Vel_Component[ 0 ] = 12;
		Reflected_Vel_Component[ 1 ] = 18;
		Reflected_Vel_Component[ 2 ] = 4;
		Reflected_Vel_Component[ 3 ] = 13;
		Reflected_Vel_Component[ 4 ] = 11;


		Source_Vel_Component[ 0 ] = 6;
		Source_Vel_Component[ 1 ] = 14;
		Source_Vel_Component[ 2 ] = 0;
		Source_Vel_Component[ 3 ] = 7;
		Source_Vel_Component[ 4 ] = 5;

	}

	else if(m_VelocityComponent == 8) {

		Reflected_Vel_Component[ 0 ] = 10;
		Reflected_Vel_Component[ 1 ] = 13;
		Reflected_Vel_Component[ 2 ] = 7;
		Reflected_Vel_Component[ 3 ] = 17;
		Reflected_Vel_Component[ 4 ] = 3;


		Source_Vel_Component[ 0 ] = 8;
		Source_Vel_Component[ 1 ] = 11;
		Source_Vel_Component[ 2 ] = 5;
		Source_Vel_Component[ 3 ] = 15;
		Source_Vel_Component[ 4 ] = 1;

	}

	else if(m_VelocityComponent == 10) {

		Reflected_Vel_Component[ 0 ] = 8;
		Reflected_Vel_Component[ 1 ] = 11;
		Reflected_Vel_Component[ 2 ] = 5;
		Reflected_Vel_Component[ 3 ] = 15;
		Reflected_Vel_Component[ 4 ] = 1;


		Source_Vel_Component[ 0 ] = 10;
		Source_Vel_Component[ 1 ] = 13;
		Source_Vel_Component[ 2 ] = 7;
		Source_Vel_Component[ 3 ] = 17;
		Source_Vel_Component[ 4 ] = 3;

	}

	else if(m_VelocityComponent == 12) {

		Reflected_Vel_Component[ 0 ] = 6;
		Reflected_Vel_Component[ 1 ] = 0;
		Reflected_Vel_Component[ 2 ] = 14;
		Reflected_Vel_Component[ 3 ] = 7;
		Reflected_Vel_Component[ 4 ] = 5;


		Source_Vel_Component[ 0 ] = 12;
		Source_Vel_Component[ 1 ] = 4;
		Source_Vel_Component[ 2 ] = 18;
		Source_Vel_Component[ 3 ] = 13;
		Source_Vel_Component[ 4 ] = 11;

	}

	else {

		Reflected_Vel_Component[ 0 ] = 2;
		Reflected_Vel_Component[ 1 ] = 4;
		Reflected_Vel_Component[ 2 ] = 0;
		Reflected_Vel_Component[ 3 ] = 3;
		Reflected_Vel_Component[ 4 ] = 1;


		Source_Vel_Component[ 0 ] = 16;
		Source_Vel_Component[ 1 ] = 18;
		Source_Vel_Component[ 2 ] = 14;
		Source_Vel_Component[ 3 ] = 17;
		Source_Vel_Component[ 4 ] = 15;
	}

	for(int i=0; i < 5; ++i) {
		Field[ m_SelfIndex + Reflected_Vel_Component[i] ]
						= Field[ m_SourceIndex + Source_Vel_Component[i] ];
	}

}

/********************Inflow************************************/
inline void Inflow::treatBoundary( double * Field ) {

	double Density = 1.0;
 	double TempF = 0.0;
 	computeDensity( Field + m_SourceIndex, &Density );
 	computeSingleFeq( &Density, m_InletVelocity, &TempF, m_VelocityComponent );
 	Field[m_SelfIndex + m_VelocityComponent] = TempF;


}

/********************PressureIn************************************/
inline void PressureIn::treatBoundary( double * Field ) {

	double Density = Density_Reference + m_DeltaDensity;

	double VelocityNeighbourFluid[ Dimensions ] = {0.0};
	double FeqI = 0.0;
	double FeqInvI = 0.0;
	double TempDensity = 0.0;

	computeDensity( Field + m_SourceIndex, &TempDensity );
	computeVelocity( Field+m_SourceIndex, &TempDensity, VelocityNeighbourFluid );
	computeSingleFeq( &Density, VelocityNeighbourFluid, &FeqI, m_VelocityComponent );
	computeSingleFeq( &Density, VelocityNeighbourFluid, &FeqInvI, 18 - m_VelocityComponent );

	Field[m_SelfIndex + m_VelocityComponent] = FeqI + FeqInvI
											 - Field[m_SourceIndex + 18 - m_VelocityComponent];

}

/********************Outflow************************************/
inline void Outflow::treatBoundary( double * Field ) {

	double Density = 0.0;
	double VelocityNeighbourFluid[ Dimensions ] = {0.0};
	double FeqI = 0.0;
	double FeqInvI = 0.0;

	computeDensity( Field + m_SourceIndex, &Density );
	computeVelocity( Field + m_SourceIndex, &Density, VelocityNeighbourFluid );
	computeSingleFeq( &Density_Reference, VelocityNeighbourFluid, &FeqI, m_VelocityComponent );
	computeSingleFeq( &Density_Reference, VelocityNeighbourFluid, &FeqInvI, 18 - m_VelocityComponent );

	Field[m_SelfIndex + m_VelocityComponent] = FeqI + FeqInvI
											 - Field[m_SourceIndex + 18 - m_VelocityComponent];

}


//------------------------------------------------------------------------------
//                            Boundary Fluid cell
//------------------------------------------------------------------------------
void BoundaryFluid::addObstacle( Obstacle* Obj ) {
    ObstacleList.push_back( Obj );
}


void BoundaryFluid::processBoundary( double * Field ) {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {
              ( *Iterator)->treatBoundary( Field );
    }
}


void BoundaryFluid::deleteObstacles() {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {

        delete ( *Iterator );
    }
}

//------------------------------------------------------------------------------
//                            Boundary Buffer
//------------------------------------------------------------------------------
BoundaryBuffer::BoundaryBuffer() : m_Protocol( 0 ),
								   m_Field( 0 ),
								   m_Index( -1 ),
 								   m_BufferSize( 0 ),
 								   m_isProtocolReady( false ) {
	for ( int i = 0; i < Dimensions; ++i ) {
		m_Length[ i ] = 0;
	}
}

BoundaryBuffer::~BoundaryBuffer() {
	if ( m_Protocol != 0 ) {
		delete [] m_Protocol;
	}
}

// inline DEBUGGING
void BoundaryBuffer::addBufferElement( unsigned Index ) {
	BufferElements.push_back( Index );
	++m_BufferSize;
}


// inline DEBUGGING
void BoundaryBuffer::setDomainLength( unsigned* Length ) {
	for ( int i = 0; i < Dimensions; ++i ) {
		m_Length[ i ] = Length[ i ];
	}
}


int BoundaryBuffer::generateProtocol() {

// ............................ GUARDS: BEGINING ...............................

	if ( m_Index == -1 ) {
		std::cout << "\tERROR: you've tried to generate the protocol" << std::endl;
		std::cout << "\twithout initializing the buffer index" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> generateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif
	}


	bool isLengthInitialized = true;
	for ( int i = 0; i < Dimensions; ++i )
		isLengthInitialized *= m_Length[ i ];
	if ( isLengthInitialized == false ) {
		std::cout << "\tERROR: you've tried to generate the protocol" << std::endl;
		std::cout << "\twithout initializing geometry parameters" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> generateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif

	}


	if ( m_Field == 0 ) {
		std::cout << "\tERROR: you've tried to generate the protocol" << std::endl;
		std::cout << "\twithout initializing the field" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> generateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif
	}


// .............................. GUARDS: END .................................

  	// Boundary Buffer Scheme:
  	// index 0: +x direction
  	// index 1: -x direction
  	// index 2: +y direction
  	// index 3: -y direction
  	// index 4: +z direction
  	// index 5: -z direction

	int Shift = 0;
	int TransferVelComponent[ 5 ] = { 0 };
	unsigned X = m_Length[ 0 ];
	unsigned Y = m_Length[ 1 ];
	unsigned Z = m_Length[ 2 ];

	switch ( m_Index ) {

		case 0: Shift = Vel_DOF * X;
						TransferVelComponent[ 0 ] = 10;
						TransferVelComponent[ 1 ] = 13;
						TransferVelComponent[ 2 ] = 7;
						TransferVelComponent[ 3 ] = 17;
						TransferVelComponent[ 4 ] = 3;
				break;

		case 1: Shift = ( -1 ) *  Vel_DOF * X;
						TransferVelComponent[ 0 ] = 8;
						TransferVelComponent[ 1 ] = 11;
						TransferVelComponent[ 2 ] = 5;
						TransferVelComponent[ 3 ] = 15;
						TransferVelComponent[ 4 ] = 1;
				break;

		case 2: Shift = Vel_DOF * (X + 2) * Y;
						TransferVelComponent[ 0 ] = 12;
						TransferVelComponent[ 1 ] = 4;
						TransferVelComponent[ 2 ] = 18;
						TransferVelComponent[ 3 ] = 13;
						TransferVelComponent[ 4 ] = 11;
				break;

		case 3: Shift = ( -1 ) * Vel_DOF * (X + 2) * Y;
						TransferVelComponent[ 0 ] = 6;
						TransferVelComponent[ 1 ] = 0;
						TransferVelComponent[ 2 ] = 14;
						TransferVelComponent[ 3 ] = 7;
						TransferVelComponent[ 4 ] = 5;
				break;

		case 4: Shift = Vel_DOF * (X + 2) * (Y + 2) * Z;
						TransferVelComponent[ 0 ] = 16;
						TransferVelComponent[ 1 ] = 18;
						TransferVelComponent[ 2 ] = 14;
						TransferVelComponent[ 3 ] = 17;
						TransferVelComponent[ 4 ] = 15;
				break;

		case 5: Shift = ( -1 ) * Vel_DOF * (X + 2) * (Y + 2) * Z;
						TransferVelComponent[ 0 ] = 2;
						TransferVelComponent[ 1 ] = 4;
						TransferVelComponent[ 2 ] = 0;
						TransferVelComponent[ 3 ] = 3;
						TransferVelComponent[ 4 ] = 1;
				break;

		default: std::cout << "ERROR: Buffer index is wrong, namely:"
 						   << m_Index
						   << std::endl
						   << "ERROR SOURCE: DataStructure.cpp -> generateProtocol"
 						   << std::endl;
				 exit( EXIT_FAILURE );
				 break;
	}


	if ( this->getBufferSize() != 0 ) {
		m_Protocol = new double[ this->getProtocolSize() ];
	}


	unsigned Counter = 0;
	for ( std::list<unsigned>::iterator Iterator = BufferElements.begin();
 		  Iterator != BufferElements.end();
		  ++Iterator, Counter += 10 ) {
				for(int i = 0; i < 5; ++i) {
					m_Protocol[ Counter ] = (double)( ( *Iterator )
 										  + Shift
										  + TransferVelComponent[ i ]);

					m_Protocol[ Counter + 1 ] = m_Field[ ( *Iterator )
											  + TransferVelComponent[ i ] ];
				}
	}

	m_isProtocolReady = true;

	return 1;
}



int  BoundaryBuffer::updateProtocol() {

	if ( m_isProtocolReady == false ) {
		std::cout << "\tERROR: you've tried to update the protocol" << std::endl;
		std::cout << "\twhich was not be generated" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> updateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif
	}


	unsigned Counter = 0;
	for ( std::list<unsigned>::iterator Iterator = BufferElements.begin();
 		  Iterator != BufferElements.end();
		  ++Iterator, Counter += 2 ) {

		m_Protocol[ Counter + 1 ] = m_Field[ ( *Iterator ) ];
	}

	return 0;
}



void decodeProtocol( double* Protocol,
					 unsigned ProtocolSize,
					 double* Field ) {

	unsigned Index = 0;
	for ( unsigned i = 0; i < ProtocolSize; i += 2 ) {
		Index = (unsigned)Protocol[ i ];
		Field[ Index ] = Protocol[ i + 1 ];
	}
}

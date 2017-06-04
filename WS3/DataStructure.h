#include <list>


#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
class Obstacle {
    public:
        Obstacle() : m_SelfIndex( 0 ), m_SourceIndex( 0 ),
					 m_VelocityComponent( 0 ), m_DotProduct( 0.0 ) {}
        Obstacle( int SelfIndex,
                  int getSourceIndex,
                  int Component,
				  double DotProduct) : m_SelfIndex( SelfIndex ),
                                    m_SourceIndex( getSourceIndex ),
                                    m_VelocityComponent( Component ),
									m_DotProduct( DotProduct ) {}


        virtual ~Obstacle() {}
        virtual void treatBoundary( double * Field, double * Dummy ) = 0;


        int getSelfIndex() { return m_SelfIndex; }
        int getSourceIndex() { return m_SourceIndex; }
		int getVelocityComponent() { return m_VelocityComponent; }
		double getDotProduct() { return m_DotProduct; }

    protected:
        int m_SelfIndex;
        int m_SourceIndex;
        int m_VelocityComponent;
		double m_DotProduct;
};


class StationaryWall : public Obstacle {
    public:
        StationaryWall( int SelfIndex,
                        int getSourceIndex,
                        int Component,
						double DotProduct) : Obstacle( SelfIndex,
                                                    getSourceIndex,
                                                    Component,
													DotProduct) {}

        virtual void treatBoundary( double * Field, double * Dummy );
};


class MovingWall : public Obstacle {
    public:
        MovingWall( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
};

class FreeSlip : public Obstacle {
    public:
        FreeSlip( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
};

class Inflow : public Obstacle {
    public:
        Inflow( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct,
					double InletX,
					double InletY,
					double InletZ) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {
						InletVelocity[0] = InletX;
						InletVelocity[1] = InletY;
						InletVelocity[2] = InletZ;						
					}

        virtual void treatBoundary( double * Field );
	private:
		double InletVelocity[ Dimensions ]
};

class Outflow : public Obstacle {
    public:
        Outflow( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
};

class PressureIn : public Obstacle {
    public:
		double * DeltaDensity = NULL;
        PressureIn( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
};



//------------------------------------------------------------------------------
//                            Fluid cell
//------------------------------------------------------------------------------
class BoundaryFluid {
    public:
        BoundaryFluid() : m_Coordinate( 0 ) {}
        BoundaryFluid( int Coordinate ) : m_Coordinate( Coordinate ) {}

        void addObstacle( Obstacle* Obj );
        void processBoundary( double * Field );
        void deleteObstacles();
        bool isEmpty() { return ObstacleList.empty(); }
        int getCoodinate() { return m_Coordinate; }

    protected:
        int m_Coordinate;
        std::list<Obstacle*> ObstacleList;

};
class Fluid {
	public:
		Fluid(){
			for(int i=0; i < Vel_DOF; ++i) {
				m_Index[i] = 0;
			}
		}
		Fluid( int * Index ) {
			for(int i=0; i < Vel_DOF; ++i) {
				m_Index[i] = Index[i];
			}
		}
		int getIndex(int Index) {
			return m_Index[Index];
		}
	
	private:
		int m_Index[Vel_DOF];
		
};

#endif
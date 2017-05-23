#include "visualLB.h"

//Writes Density and Velocity from the collision field
void writeVtkOutput(const double * const collideField, const int * const flagField, const char * filename, unsigned int t, int xlength) {
    int x, y, z;
    char szFilename[80];
    FILE *fp = NULL;
    sprintf( szFilename, "%s.%i.vtk", filename, t);
    fp = fopen( szFilename, "w");

    if(fp==NULL){
        char szBuff[80];
        sprintf(szBuff, "Failed to open %s", filename, t);
        ERROR(szBuff);
        return;
    }

    write_vtkHeader(fp, xlength);
    write_vtkPointCoordinates(fp, xlength);

    fprintf(fp, "POINT_DATA %i \n", xlength*xlength*xlength);

    int Dimension = xlength + 2;
    int Squared_Dimension = Dimension * Dimension;
    double density = 0;
    double velocity[3] = {0};

    //Computing Density
    fprintf(fp, "SCALARS density float 1 \n");
    fprintf(fp, "LOOKUP_TABLE default \n");
    for (z = 1; z <= xlength; ++z) {
        for (y = 1; y <= xlength; ++y) {
            for (x = 1; x <= xlength; ++x){
                computeDensity (collideField + Vel_DOF * (x + Dimension * y + Squared_Dimension * z), &density);
                fprintf(fp, "%f\n", density);
            }
        }
    }

    //Computing Velocity
    fprintf(fp, "\nVECTORS velocity float \n");
    for (z = 1; z <= xlength; ++z) {
        for(y = 1; y <= xlength; ++y) {
            for (x = 1; x <= xlength; ++x){
                const double * const idx = collideField + Vel_DOF * (x + Dimension * y + Squared_Dimension * z);
                computeDensity (idx, &density);
                computeVelocity (idx, &density, velocity);
                fprintf(fp, "%f %f %f\n", velocity [0], velocity [1], velocity [2]);
            }
        }
    }

    fclose(fp);

}

void write_vtkHeader( FILE *fp, int imax, int jmax,
                      double dx, double dy) {
    if( fp == NULL )
    {
        char szBuff[80];
        sprintf( szBuff, "Null pointer in write_vtkHeader" );
        ERROR( szBuff );
        return;
    }

    fprintf(fp,"# vtk DataFile Version 2.0\n");
    fprintf(fp,"generated by CFD-lab course output (written by Tobias Neckel) \n");
    fprintf(fp,"ASCII\n");
    fprintf(fp,"\n");
    fprintf(fp,"DATASET STRUCTURED_GRID\n");
    fprintf(fp,"DIMENSIONS  %i %i 1 \n", xlength, xlength, xlength);
    fprintf(fp,"POINTS %i float\n", xlength * xlength * xlength );
    fprintf(fp,"\n");
}

void write_vtkPointCoordinates( FILE *fp, int xlength ){
    double originX = 0.0;
    double originY = 0.0;
    double originZ = 0.0;

    int x, y, z;

    //Since we are working with cubes of unit dimensions, dx = dy = dz = 1 / (xlength - 1)
    double dx = 1.0 / (xlength - 1);
    double dy = 1.0 / (xlength - 1);
    double dz = 1.0 / (xlength - 1);

    //TODO: I think I can be wrong here. Are the co-ordinates right here?
    for( z = 0; z <= xlength; z+=dz )
        for( y = 0; y <= xlength; y+=dy ) {
            for( x = 0; x <= xlength; x+=dx ) {
            fprintf(fp, "%f %f %f\n", x,y,z );
        }
    }
}
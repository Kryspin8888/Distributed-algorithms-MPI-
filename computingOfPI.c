#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main (int argc, char **argv){
int iterations = 1000000000;
int myrank, proccount;
double pi, pi_final;
double radius = 2.0;

MPI_Init (&argc,&argv);

MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
MPI_Comm_size (MPI_COMM_WORLD, &proccount);

if(iterations < proccount)
{
    printf ("Precision smaller than number of processes - try again.\n");
    MPI_Finalize ();
    return -1;
}

pi = 0;
double radiusToSquare = pow(radius,2);
double dx = (double)(radius/iterations);
int i;

for( i = myrank + 1; i <= iterations ; i += proccount)
{
    pi += sqrt(radiusToSquare - (pow(i*dx,2))) * dx;
}

MPI_Reduce (&pi,&pi_final,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

if(!myrank)
{
    printf ("pi=%f\n", pi_final);
}

MPI_Finalize ();

return 0;
}
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
MPI_Init (&argc,&argv);
int size, rank;
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
if(rank==0)
cout<<"Hello world"<<endl;
cout<<"To ja prosecor "<<rank<<" z puli "<<size<<endl;
MPI_Finalize();
return 0;
}
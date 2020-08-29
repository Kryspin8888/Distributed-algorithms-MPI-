#include <mpi.h>
#include <iostream>
#include "timer.h"

using namespace std;

int main(int argc, char** argv){
MPI_Init (&argc, &argv);
int world_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
int world_size;
MPI_Comm_size(MPI_COMM_WORLD,&world_size);

const int mysize = 10000000;
const int k = mysize/4;
const int n=50;
int* buffor = new int [k];
for(int i=0 ; i<k; i++)
{
buffor[i] = 5*i;
}
unsigned int start_time = timer();
for(int j=0; j<n; j++)
{
if (world_rank == 0){
MPI_Send(buffor,k,MPI_INT,1,0,MPI_COMM_WORLD);

MPI_Recv(buffor, k, MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

}

else if(world_rank == 1)
{

MPI_Recv(buffor,k,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
MPI_Send(buffor, k , MPI_INT,0,0,MPI_COMM_WORLD);

}
}
unsigned int stop_time = timer();
if(world_rank == 0)
{
cout<<"Czas całkowity "<<stop_time - start_time<<"\n";
cout<<"Czas średni "<<double(stop_time - start_time)/n<<"\n";
cout<<"Rozmiar int: "<<sizeof(int)<<"\n";

}
delete [] buffor;
MPI_Finalize();
}

#include <mpi.h>
#include <iostream>
#include<fstream>
#include <cstdlib>
#include<vector>

using namespace std;

void vectorsend(const string &values, int dest );
string vectorrecv (int source);

int main(int argc, char** argv){
MPI_Init (&argc, &argv);
int world_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
int world_size;
MPI_Comm_size(MPI_COMM_WORLD,&world_size);

if(world_rank == 0)
    {	
	string tekst = "La La Land";
	
	cout<<"Procesor 0 - wysylam\n";
	vectorsend(tekst,1);
    }	

if (world_rank == 1)
    {
    cout<<"Procesor 1 - odbieram\n"<<vectorrecv(0)<<"\n";
	    
    }

MPI_Finalize();
}

void vectorsend(const string &values, int dest )
{
    int rozm1 = values.size();
    MPI_Send(&rozm1,1,MPI_INT,dest,0,MPI_COMM_WORLD);
    MPI_Send(values.data(),rozm1,MPI_CHAR,dest,0,MPI_COMM_WORLD);
    
}

string vectorrecv (int source)
{	
    int rozm;
    MPI_Recv(&rozm,1,MPI_INT,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    char * buff = new char[rozm-2];
    MPI_Recv(buff,rozm,MPI_INT,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    buff[rozm] = '\0';
    string result(buff);
    
    delete[] buff;
    return result;
}
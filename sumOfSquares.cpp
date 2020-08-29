#include <mpi.h>
#include <iostream>
#include<fstream>
#include <cstdlib>
#include "timer.h"

using namespace std;

int main(int argc, char** argv){
MPI_Init (&argc, &argv);
int world_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
int world_size;
MPI_Comm_size(MPI_COMM_WORLD,&world_size);
int size = 0;
int partitionsize;

if(world_rank == 0)
{
    int dana;    
    ifstream we1("wejscie");
    for(;;)
    {
    
	we1>>dana;
    if(!we1)
	{ 
	    if(we1.eof()) break;
	    else
		{
		cerr<<endl<<"Blad podczas odczytu z pliku wejsciowego\n"; 
		MPI_Abort(MPI_COMM_WORLD,0);
		}
	}
    else
	{
	size++;
	}
    }
    
    partitionsize = size/world_size;
}

MPI_Bcast(&partitionsize,1,MPI_INT,0,MPI_COMM_WORLD);
int *partition = new int[partitionsize];
int sumofsquares = 0;
int *tab;

if(world_rank == 0)
{
    tab = new int[size];
    ifstream we("wejscie");
    if(!we)
    {
	cerr<<endl<<"Blad podczas odczytu z pliku wejsciowego\n";
	delete [] tab; 
	MPI_Abort(MPI_COMM_WORLD,0);
    }
    else cout<<"[proc 0] Odczytano poprawnie dane\trozmiar: "<<size<<"\n";

    unsigned int start_time = timer();
    for(int i=0; i<size; ++i) we>>tab[i];
    unsigned int stop_time = timer();    
    cout<<"[proc 0] Odczyt: "<<stop_time - start_time<<" ms\n";

}
unsigned int start_time = timer();
MPI_Scatter(&tab,partitionsize,MPI_INT,&partition,partitionsize,MPI_INT,0,MPI_COMM_WORLD);
cout<<"[proc "<<world_rank<<"] komunikacja: "<<timer()-start_time<<" ms\n";

start_time = timer();
for(int i=0; i<partitionsize; i++)
{
    sumofsquares += partition[i]*partition[i];
}
cout<<"[proc "<<world_rank<<"] sumowanie: "<<timer()-start_time<<" ms\n";
cout<<"[proc "<<world_rank<<"] wynik: "<<sumofsquares<<"\n";

int reduceresult;

MPI_Barrier(MPI_COMM_WORLD);
start_time = timer();
MPI_Reduce(&sumofsquares,&reduceresult,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
cout<<"[proc "<<world_rank<<"] redukcja: "<<timer()-start_time<<" ms\n";
if(world_rank == 0)
{
cout<<"Wynik po redukcji: "<<reduceresult<<"\n";
}
MPI_Finalize();
}

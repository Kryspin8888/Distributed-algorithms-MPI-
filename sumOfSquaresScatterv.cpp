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
    
}
MPI_Bcast(&size,1,MPI_INT,0,MPI_COMM_WORLD);
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

int rem = (size)%world_size;
int sum = 0;
int *sendcounts = new int[world_size];
int *displs = new int[world_size];

for(int i=0; i<world_size; i++)
{
    sendcounts[i] = (size)/world_size;
    if(rem>0){
	sendcounts[i]++;
	rem--;
    }
    displs[i] = sum;
    sum += sendcounts[i];
}

if(world_rank == 0)
{
    cout<<"  [proc 0] Dane: \n";
    for (int i=0; i<world_size; i++)
	cout<<"  sendcounts[ "<<i<<"]  "<<sendcounts[i]<<" displs["<<i<<"]  "<<displs[i]<<"\n";
}

int *recv_buf = new int [sendcounts[world_rank]];

unsigned int start_time = timer();
MPI_Scatterv(tab,sendcounts,displs,MPI_INT,recv_buf,sendcounts[world_rank],MPI_INT,0,MPI_COMM_WORLD);
cout<<"[proc "<<world_rank<<"] komunikacja: "<<timer()-start_time<<" ms\n";

start_time = timer();
for(int i=0; i<sendcounts[world_rank]; i++)
{
    sumofsquares += recv_buf[i]*recv_buf[i];
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
delete[] sendcounts;
delete[] displs;
MPI_Finalize();
}

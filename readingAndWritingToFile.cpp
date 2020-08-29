#include <mpi.h>
#include <iostream>
#include<fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv){
MPI_Init (&argc, &argv);
int world_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
int world_size;
MPI_Comm_size(MPI_COMM_WORLD,&world_size);
int size = 0;
int dana;
if(world_rank == 0)
{
    if(argc<2)
    {
    cerr<<endl<<"Nie podales argumentu \n"<<endl;    
    MPI_Abort(MPI_COMM_WORLD,0);
    }
    
    ifstream we(argv[1]);
    for(;;)
    {
    
	we>>dana;
    if(!we)
	{ 
	    if(we.eof()) break;
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

int *tab = new int[size];

if (world_rank == 0)
{
    ifstream we(argv[1]);
    if(!we)
	{
	cerr<<endl<<"Blad podczas odczytu z pliku wejsciowego\n";
	delete [] tab; 
	MPI_Abort(MPI_COMM_WORLD,0);
	}
    else cout<<"\nOdczytano poprawnie dane z "<<argv[1]<<"\n";

    for(int i=0; i<size; ++i) we>>tab[i];
    cout<<"Procesor 0 - wysyłam\n"<<endl;
    MPI_Send(tab,size,MPI_INT,world_size-1,0,MPI_COMM_WORLD);

    MPI_Recv(tab,size, MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    cout<<"Procesor 0 - odebralem\n";
    ofstream wy("out.txt");
    if(!wy)
	{
	cerr<<endl<<"Blad podczas otwierania out.txt\n";
	delete [] tab;
	MPI_Abort(MPI_COMM_WORLD,0);
	}
    for(int i=0; i<size; ++i) wy<<tab[i]<<"\n";
    wy.close();
    if(!wy)
	{
	cerr<<"Zapis do out.txt mogl sie nie powiesc\n";
	delete [] tab;
	MPI_Abort(MPI_COMM_WORLD,0);
	}
    else cout<<"Zapisano poprawnie do out.txt\n\n";
}

else
{
    if(world_rank == world_size-1)
    {
    MPI_Recv(tab,size,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    else
    {
    MPI_Recv(tab,size,MPI_INT,world_rank+1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    
    for(int i=0; i<size; i++) tab[i]+=world_rank;
    MPI_Send(tab,size,MPI_INT,world_rank-1,0,MPI_COMM_WORLD);
}
delete [] tab;
MPI_Finalize();
}

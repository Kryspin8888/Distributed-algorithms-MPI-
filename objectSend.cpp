#include <mpi.h>
#include <iostream>
#include<fstream>
#include <cstdlib>
#include<vector>
#include<string>

using namespace std;

class Rekord{

private: vector <int> wektor;
	 string lancuch;

public: Rekord(){
    lancuch="Ala ma kota";
    for(int i=0; i<100; i++){
        wektor.push_back(i);
    }
};
void show(){
    cout<<lancuch<<"\n";
    int siz = wektor.size();
    for(int i=0; i<siz; i++)
        cout<<wektor[i]<<" ";
    cout<<"\n";
};
vector<int> get_wektor()const{
    return wektor;
};
string get_lancuch()const{
    return lancuch;
};
void set_wektor(const vector<int> &aa){
    wektor = aa;
};
void set_lancuch(const string &bb){
    lancuch = bb;
};
void send(int dest){
    int rozm1 = wektor.size();
    MPI_Send(&rozm1,1,MPI_INT,dest,0,MPI_COMM_WORLD);
    MPI_Send(&(wektor[0]),rozm1,MPI_INT,dest,0,MPI_COMM_WORLD);
    int rozm2 = lancuch.size();
    MPI_Send(&rozm2,1,MPI_INT,dest,0,MPI_COMM_WORLD);
    MPI_Send(lancuch.data(),rozm2,MPI_CHAR,dest,0,MPI_COMM_WORLD);
};
void recv(int source){
    int rozm1;
    MPI_Recv(&rozm1,1,MPI_INT,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    wektor.resize(rozm1);
    MPI_Recv(&(wektor[0]),rozm1,MPI_INT,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    int rozm2; 
    MPI_Recv(&rozm2,1,MPI_INT,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    char * buff = new char [rozm2+1];
    MPI_Recv(buff,rozm2,MPI_CHAR,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    buff[rozm2]='\0';
    string result(buff);
    lancuch = result;
    delete[] buff;
};
void broadcast(int source){
    int rozm1 = wektor.size();
    MPI_Bcast(&rozm1,1,MPI_INT,source,MPI_COMM_WORLD);
    wektor.resize(rozm1);
    MPI_Bcast(&(wektor[0]),rozm1,MPI_INT,source,MPI_COMM_WORLD);
    int rozm2 = lancuch.size();
    MPI_Bcast(&rozm2,1,MPI_INT,source,MPI_COMM_WORLD);
    char * buff = new char [rozm2+1];
    strcpy(buff,lancuch.data());
    MPI_Bcast(buff,rozm2+1,MPI_CHAR,source,MPI_COMM_WORLD);
    buff[rozm2] = '\0';
    string result(buff);
    lancuch = result;
    delete[] buff;
}
};

int main(int argc, char** argv){
MPI_Init (&argc, &argv);
int world_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
int world_size;
MPI_Comm_size(MPI_COMM_WORLD,&world_size);

if(world_rank == 0)
    {	
	cout<<"Procesor 0 - pracuje\n";
	Rekord pierwszy;
	vector<int> w;
	w.push_back(5);
	w.push_back(10);
	w.push_back(15);
	pierwszy.set_wektor(w);
	pierwszy.set_lancuch("Lancuch testowy");
	pierwszy.show();
	pierwszy.broadcast(0);
    }	

if (world_rank == 1)
    {	
    	cout<<"Procesor 1 - pracuje\n";
	Rekord drugi;
	drugi.broadcast(0);
	drugi.show();
    }

MPI_Finalize();
}
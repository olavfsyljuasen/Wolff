#include<iostream>
#include<vector>
#include<cstdlib>
#include<math.h>
#include<fstream>
#include<string>
#include<complex>
#include<iomanip>
using namespace std;

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

const double PI=M_PI;

const bool TRACE=true;

const int NSTATICOBS=10;
const int  MAXRUNS = 20; // the maximum number of runs in the read.in file  
const int  MAXPAR = 100;  // the maximum allowed input parameters

const string RESFILENAME ="res.dat";
const string RES1FILENAME="res1.dat";
const string RES2FILENAME="res2.dat";
const string RES4FILENAME="res4.dat";
const string RECORRFILENAME="recorr.dat"; 
const string IMCORRFILENAME="imcorr.dat"; 

const string READIN   = "read.in";

const int q=POTTSQ;          // q-state Potts model
const int L=LINEARSIZE;      // linear system size

const int NPARAMS=6;

enum params{LINEID,BETA,NBINS,MSTEPS,EQFLAG,EQSTEPS};


ofstream logfile("log.txt",ios::app);

#include "rnddef.h" // the random number generator RAN                                                                               
#include "globalheader.h"
#include "RunParameter.h"


const int NCLUSTERS=q; //    # of cluster builds in one MC step.

// lattice handling:

#ifdef SQUARELATTICE
const int N=L*L;    // total number of spins
const int d=2;
enum dirs{RIGHT,LEFT,UP,DOWN};
int indx(int x,int y){return y*L+x;} // make an indx on every site
int xpos(int i){return i%L;}
int ypos(int i){return i/L;}

int Nbr(int i,int dir)
{
  int x=xpos(i);  int y=ypos(i);
  switch(dir)
    {
    case RIGHT: return indx((x+1)%L,y);
    case LEFT:  return indx((x-1+L)%L,y);
    case UP:    return indx(x,(y+1)%L);
    case DOWN:  return indx(x,(y-1+L)%L);
    }
}
#elif defined CHAINLATTICE
const int N=L;    // total number of spins
const int d=1;
enum dirs{RIGHT,LEFT};
int indx(int x){return x;} // make an indx on every site
int xpos(int i){return i%L;}

int Nbr(int i,int dir)
{
  int x=xpos(i); 
  switch(dir)
    {
    case RIGHT: return indx((x+1)%L);
    case LEFT:  return indx((x-1+L)%L);
    }
}
#endif


// state variables
vector<int> S(N);  // the spin array
vector<int> M(q);  // number of spins in different states.
vector<complex<double> > W(q); // order parameter 
// correlations
vector<complex<double> > C(N);  // 1d correlations




void FlipandBuildFrom(int s,double pconnect)
{
  int oldstate=S[s];
  int newstate=(S[s]+1)%q;  
  
  S[s]=newstate; // flip spinn

  M[oldstate]--;  // update spin counts
  M[newstate]++;


  for(int dir=0; dir<2*d; dir++) // go thru neighbors
    {
      int j=Nbr(s,dir);
      if(S[j] == oldstate) 
	if( RAN() <pconnect){FlipandBuildFrom(j,pconnect);}
    }
}

int main()
{
  RunParameters rp;
  logfile << "parameters: " << rp;

  double* par = rp.GetPars(1);

  const double beta=par[BETA];
  const double pconnect=1.-exp(-beta); // connection probability

  logfile << "Using beta= "<< beta << " pconnect= " << pconnect << endl;

  
  const int NEQSTEPS =par[EQSTEPS]; // # of equilibrium MC step.
  const int NMESTEPS =par[MSTEPS]; // # of measurement MC step.
  const int NMBINS   =par[NBINS] ; //   #of measurement bins


  // initialize order parameter weights
  for(int s=0; s<q; s++)
    W[s]=complex<double>(cos(2*PI*s/q),sin(2*PI*s/q));

  
  // initialize to all spins
  for(int i=0; i<N; i++) S[i]=0;   // initialize to the spin=0 state 

  for(int s=0; s<q; s++) M[s]=0;
  M[0]=N;

  
  // equilibriate
  for(int t=0; t<NEQSTEPS; t++)
    for(int c=0; c<NCLUSTERS; c++)
      {
	FlipandBuildFrom(RAN()*N,pconnect);

	if(TRACE)
	  { cout << "Spin config: " << endl;
	    for(int i=0; i<N; i++) cout << S[i] << " ";
	    cout << endl;
	  }
      }

  // measure
  for(int n=0; n<NMBINS; n++)
    {
      complex<double> m(0.,0.);
      double m1=0, m2=0, m4=0; // measurement results
#ifdef CORRELATIONS
      for(int r=0; r<N; r++)
	{
	  C[r] = complex<double>(0.,0.);
	}
#endif	  	  
      
      for(int t=0; t<NMESTEPS; t++)
	{
	  for(int c=0; c<NCLUSTERS; c++) FlipandBuildFrom(RAN()*N,pconnect);

	  complex<double> tm(0.,0.);
	  for(int s=0; s<q; s++)
	    {
	      tm+= W[s]*double(M[s]);
	    }
	  tm/=N; 
	  double tm1= abs(tm);
	  double tm2=tm1*tm1;
	  m+=tm; m1+=tm1; m2+=tm2;  m4+=tm2*tm2;

#ifdef CORRELATIONS
	  for(int r=0; r<N; r++)
	    {
	      C[r] += conj(W[S[0]])*W[S[r]]; 
	    }
#endif	  	  
	}
      m/=NMESTEPS; m1/=NMESTEPS;  m2/=NMESTEPS;  m4/=NMESTEPS;

      ofstream resfile(RESFILENAME.c_str(),ios::app);
      resfile << "1 " << setprecision(10) << real(m) << " " << imag(m) << endl;
      resfile.close();
      
      ofstream resfile1(RES1FILENAME.c_str(),ios::app);
      resfile1 << "1 " << setprecision(10) << m1 << endl;
      resfile1.close();
      
      ofstream resfile2(RES2FILENAME.c_str(),ios::app);
      resfile2 << "1 " << setprecision(10) << m2 << endl;
      resfile2.close();
      
      ofstream resfile4(RES4FILENAME.c_str(),ios::app);
      resfile4 << "1 " << setprecision(10) << m4 << endl;
      resfile4.close();

      
#ifdef CORRELATIONS
      ofstream recorrfile(RECORRFILENAME.c_str(),ios::app);
      ofstream imcorrfile(IMCORRFILENAME.c_str(),ios::app);

      recorrfile << "1 " << setprecision(10);
      imcorrfile << "1 " << setprecision(10);

      for(int r=0; r<N; r++)
	{
	  C[r] /= NMESTEPS;
	  recorrfile << real(C[r]) << " "; 
	  imcorrfile << imag(C[r]) << " ";
	}

      recorrfile << endl;
      imcorrfile << endl;

      recorrfile.close();
      imcorrfile.close();
#endif	  	  
      
    }
}

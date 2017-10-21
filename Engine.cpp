#include <math.h>
#include "engine.h"
#include <windows.h>

//int alphafit(int minchan,int &p,int maxchan,double &a,double &sig,double B,int itermax,double q0, long int spec[])
//{
//	p *= 2;
//	a *= 2;
//	sig *= 2;
//	return 1;
//}


void gaussJordan(double x[][10],double y[][1],double E[][1],const int n);// is defined in engine2.cpp

// alpha peak fitting with two part function (gaussian + exponential) //Marquardt method
//long int spec[],an array containing data
//other parameters entered by user
//function gaussjordan is also needed for solving linear equations

static int LastJ;
static double Landa;
static double Chi2[25];

void GetParameters(int &j, double &landa, double chi2[])
{
	j = LastJ;
	landa = Landa;
	for (int i = 0; i < 25; ++i)
		chi2[i] = Chi2[i];
}

int alphafit(int minchan,double &p,int maxchan,double &a,double &sig,double &B,int itermax,double q0, long int spec[])
{
 int start = p;
 int count1 = maxchan-start+1;
 bool flag1=false;
 bool flag2=false;
 double f[1000][1];
 double ft[1][1000];
 double e[1][1];
 double y[1000][1];
 double A[10][10]={0};
 double b[1][1]={0};
 double chi2[25]={0};
 double chi22[25]={0};
 int j;
 for (int i=start ; i<= maxchan ; ++i )
    if(spec[i]!=0)
    chi2[0]+=(pow(a*exp(-1*(i-p)*(i-p)/(2*pow(sig,2)))-spec[i],2)/spec[i]);
    double landa=0.001;
 for (j=1;(j<=itermax && flag1==false);++j)
     {
      
  /*for ( int h=start ; h<=maxchan ;++h)
	{
	f[h-start][0]=exp(-1*(h-p)*(h-p)/(2*pow(sig,2)));
	ft[0][h-start]=exp(-1*(h-p)*(h-p)/(2*pow(sig,2)));
	}			
	for (int h=start ; h<=maxchan ;++h)
	{
	f[h-start][1]=a*(h-p)*exp(-1*(h-p)*(h-p)/(2*pow(sig,2)))/pow(sig,2);
	ft[1][h-start]=a*(h-p)*exp(-1*(h-p)*(h-p)/(2*pow(sig,2)))/pow(sig,2);
	}*/
	for (int h=start ; h<=maxchan ;++h)
	{
	f[h-start][0]=a*(h-p)*(h-p)*exp(-1*(h-p)*(h-p)/(2*pow(sig,2)))/pow(sig,3);
	ft[0][h-start]=a*(h-p)*(h-p)*exp(-1*(h-p)*(h-p)/(2*pow(sig,2)))/pow(sig,3);
    }

  for (int h=start ; h<=maxchan ;++h)
	y[h-start][0]=spec[h]-(a*exp(-1*(h-p)*(h-p)/(2*pow(sig,2))));
				
	for(int r=0; r<1 ; ++r)
	    for(int col=0; col<1 ; ++col)
 	      for( int s=0 ; s< count1 ; ++s)
		A[r][col]+=ft[r][s]*f[s][col];
                A[0][0]*=(1+ landa);
                /*A[1][1]*=(1+ landa);
                A[2][2]*=(1+ landa);*/
						
	for (int d=0;d<1 ;++d)
	    for(int g=0; g< count1;++g)
		b[d][0]+=ft[d][g]*y[g][0];
        
        gaussJordan(A,b,e,1);    //a function for solving linear equations 
        
		for (int v=0; v<10 ; v++)
			for (int f=0 ; f<10 ;f++ )
				A[v][f]=0;

		b[0][0]=0; /*b[1][0]=0; b[2][0]=0;*/

        for (int i=start ; i<= maxchan ; ++i )
         if(spec[i]!=0)
         chi2[j]+=(pow(a*exp(-1*pow(i-p,2)/(2*pow((sig+e[0][0]),2)))-spec[i],2)/spec[i]);

       if( chi2[j] >chi2 [j-1] )
         {
          landa*=10;
		  chi2[j]=0;
		    j--;
          continue;
         }
        else
         {
          landa=landa/10;
          /*a+=e[0][0];  p+=e[1][0];*/ sig+=e[0][0];
		  e[0][0]=0;
         }
    
         if( (fabs(chi2[j]-chi2[j-1])/count1-3) < q0 )
         flag1=true;
      }

	LastJ = j;
	Landa = landa;
	for (int i = 0; i < 25; ++i)
		Chi2[i] = chi2[i];
	
	int count2 = start-minchan;
	for (int i=minchan ; i<start ; ++i )
    if(spec[i]!=0)
    chi22[0]+=(pow(a*exp(B*(i-p))-spec[i],2)/spec[i]);
    double beta=0.001;
 for (j=1;(j<=itermax && flag2==false);++j)
     {
      
  /*for ( int h=start ; h<=maxchan ;++h)
	{
	f[h-start][0]=exp(-1*(h-p)*(h-p)/(2*pow(sig,2)));
	ft[0][h-start]=exp(-1*(h-p)*(h-p)/(2*pow(sig,2)));
	}			
	for (int h=start ; h<=maxchan ;++h)
	{
	f[h-start][1]=a*(h-p)*exp(-1*(h-p)*(h-p)/(2*pow(sig,2)))/pow(sig,2);
	ft[1][h-start]=a*(h-p)*exp(-1*(h-p)*(h-p)/(2*pow(sig,2)))/pow(sig,2);
	}*/
	for (int h=minchan ; h<start ;++h)
	{
	f[h-minchan][0]=a*(h-p)*exp(B*(h-p));
	ft[0][h-minchan]=a*(h-p)*exp(B*(h-p));
    }

  for (int h=minchan ; h<start ;++h)
	y[h-minchan][0]=spec[h]-a*exp(B*(h-p));
				
	for(int r=0; r<1 ; ++r)
	    for(int col=0; col<1 ; ++col)
 	      for( int s=0 ; s< count2 ; ++s)
		A[r][col]+=ft[r][s]*f[s][col];
                A[0][0]*=(1+ beta);
                /*A[1][1]*=(1+ landa);
                A[2][2]*=(1+ landa);*/
						
	for (int d=0;d<1 ;++d)
	    for(int g=0; g< count2;++g)
		b[d][0]+=ft[d][g]*y[g][0];
        
        gaussJordan(A,b,e,1);    //a function for solving linear equations 
        
		for (int v=0; v<10 ; v++)
			for (int f=0 ; f<10 ;f++ )
				A[v][f]=0;

		b[0][0]=0; /*b[1][0]=0; b[2][0]=0;*/

        for (int i=minchan ; i<start ; ++i )
         if(spec[i]!=0)
         chi22[j]+=(pow(a*exp((B+e[0][0])*(i-p))-spec[i],2)/spec[i]);

       if( chi22[j] >chi22 [j-1] )
         {
          beta*=10;
		  chi22[j]=0;
		    j--;
          continue;
         }
        else
         {
          beta=beta/10;
          /*a+=e[0][0];  p+=e[1][0];*/ B+=e[0][0];
		  e[0][0]=0;
         }
    
         if( (fabs(chi22[j]-chi22[j-1])/count2-3) < q0 )
         flag2=true;
      }



       if((flag1&&flag2)==true)
       {
       /*cout<<" fitting routine converged.\n";
       cout<<" number of iteration:"<< j <<"\nlanda=" <<landa <<endl;
       cout<<" a="<<a<<"\np="<<p<<"\nsig="<<sig<<endl;*/
       return 1;
       }
       else
       {
      /* cout<<" fitting routin is not converged.";*/
       return 0;
       }
}


void coefficient(int z0,int w0,int co[])
	{
		int n;
		n=(w0-1)/2;
		int co_1[200]={0};
        

		for (int i=0; i< z0; ++i){
			for (int j=30; j<=170 ; ++j){
				for( int r=j-n; r<=j+n; ++r)
					co_1[j] += co[r];
			}
			for (int y=0; y<200 ;++y)
				co[y]=co_1[y];
			for (int t=0 ; t<200 ; ++t)
				co_1[t]=0;
		}
	}

void peakSearch(long int a[],long int s[],long int f[],int peak[],int peak3[],int peak5[])
		{
			int n1,n2,n3,flag,l=0,min=0;
			int set[6]={0};						//an array for the set candidate for peaks

			n1=static_cast<int>(floor(1.22*5+0.5));
			for (int i=3 ; i<=20000 ; ++i){
				if ( s[i] > f[i])
					flag=1;
				else if ( s[i] > 0)
					flag=2;
				else
					flag=3;
				if ( s[i-1] > f[i-1] ){
					if ( flag==3 ){
						set[3] = i;
						set[2] = i-1;
					}
					if ( flag == 2)
						set[2] = i-1;
				}
				else if( s[i-1] > 0 ) {
					if (flag ==3 )
						set[3] = i;
					if ( flag ==1 )
						set[1] = i;
				}
				else 
				{
					if ( (flag==1) || (flag==2) )
					{
						set[5]= i-1;
						if ((set[1] != 0) && (set[2] != 0) && (set[3] != 0) && (set [5] !=0)){
							for ( int j=set[3] ; j <= set[5] ; ++j)
								if ( s[j] < min ){
										min=s[j];
										set[4]=j;
										}
								min=0;

						}
						if (s[set[4]] != 0){
						n2=static_cast<int>(floor((f[set[4]]/fabs((double)s[set[4]]))*0.5*(n1+2)+0.5));
						n3=static_cast<int>(floor((1-2*(f[set[4]]/(s[set[4]])))*(n1-2)+0.5));
						}

	if (fabs((double)s[set[4]]) > (4*f[set[4]]))  /*&& (fabs(set[5]-set[3]+1-n1) <= 2) && ((set[2]-set[1]+1) >= n3+2) ) 
		if(((n2==0) && (set[3]-set[2]-1 <= 1)) || ((n2 >= 1) && (set[3]-set[2]-1 <= n2-3)))*/{
	
			peak3[l]=set[3];
			peak[l]=set[4];
			peak5[l]=set[5];
	++l;
	
	set[1]=0;set[2]=0;set[3]=0;set[4]=0;set[5]=0; min=0;}

					}
				}
		
			}
		/*cout<<"\n\nThe list of the peaks:\n";
		int v=0;
		while (peak[v]!=0)
		{
			cout<<setw(7)<<peak[v];
			if ( v%10==0)
				cout<<endl;
			++v;
		}	*/


		}

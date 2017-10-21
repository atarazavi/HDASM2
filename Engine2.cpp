#include <math.h>
#include "Engine.h"
#include <windows.h>

void gaussJordan(double x[][100],double y[][1],double E[][1],const int n)
		{
		
			double temp1,div;
			int h;
			double temp[101]={0};
			double z[101][101]={0};
			for(int r=0; r<n ; ++r)
				for(int col=0; col<n ; ++col)
					z[r][col]=x[r][col];
				for(int i=0; i<n;++i)
					z[i][n]=y[i][0];
				

				for(int j=0; j<n ; ++j)
				{
					bool flag=true;
					if(z[j][j]==0)
					{
						for( h=j+1;((h<n)&&(flag==true));++h)
						{
							if(z[h][j] != 0)
							{
						
							
							for(int k=0;k<n;++k)
							{
								temp[k]=z[h][k];
								z[h][k]=z[j][k];
								z[j][k]=temp[k];
							}
							flag=false;
							}
						}
						
					}

							div=z[j][j];
							for (int f=0;f<(n+1);++f)
								z[j][f]/=div;
							
							for(int v=0 ;v<j ;++v)
							{
								temp1=z[v][j];
								for(int q=0; q<(n+1) ;++q)
									z[v][q]-=(temp1*z[j][q]);}

							for(int v=j+1 ;v<n ;++v)
							{
								temp1=z[v][j];
								for(int q=0; q<(n+1) ;++q)
									z[v][q]-=(temp1*z[j][q]);}
					
				}
				for(int g=0 ;g<n ;++g)
					E[g][0]=z[g][n];
		}

void gaussJordan(double x[][10],double y[][1],double E[][1],const int n)
		{
		
			double temp1,div;
			int h;
			double temp[11]={0};
			double z[11][11]={0};
			for(int r=0; r<n ; ++r)
				for(int col=0; col<n ; ++col)
					z[r][col]=x[r][col];
				for(int i=0; i<n;++i)
					z[i][n]=y[i][0];
				

				for(int j=0; j<n ; ++j)
				{
					bool flag=true;
					if(z[j][j]==0)
					{
						for( h=j+1;((h<n)&&(flag==true));++h)
						{
							if(z[h][j] != 0)
							{
						
							
							for(int k=0;k<n;++k)
							{
								temp[k]=z[h][k];
								z[h][k]=z[j][k];
								z[j][k]=temp[k];
							}
							flag=false;
							}
						}
						
					}

							div=z[j][j];
							for (int f=0;f<(n+1);++f)
								z[j][f]/=div;
							
							for(int v=0 ;v<j ;++v)
							{
								temp1=z[v][j];
								for(int q=0; q<(n+1) ;++q)
									z[v][q]-=(temp1*z[j][q]);}

							for(int v=j+1 ;v<n ;++v)
							{
								temp1=z[v][j];
								for(int q=0; q<(n+1) ;++q)
									z[v][q]-=(temp1*z[j][q]);}
					
				}
				for(int g=0 ;g<n ;++g)
					E[g][0]=z[g][n];
		}


double fact(int x)
{
	if (x==0)
		return 1;
	else
		return x*fact(x-1);
}

int sign(double x)
{
    if(x==0)
		return 0;
    if(x>0)
		return 1;
    if(x<0)
		return -1;
}


double erfc(double z)
{
	return 1 - sign(z) * sqrt(1 - exp(-pow(z, 2) * (1.27324 + 0.140012 * pow(z, 2)) / (1 + 0.140012 *pow(z, 2))));
}
double peakshape(const int n, int u ,int Amp[],int mio[],double sigma[],double taw[],double eta[])
{
	double sum=0;
	for(int i=1;i<=n;i++)
	{
		double er = erfc((1/sqrt((double)2))*((u-mio[i])/sigma[i]+sigma[i]*taw[i]));
		if (er!=0)
		{
			double ex = exp((u-mio[i])*taw[i]+0.5*sigma[i]*sigma[i]*taw[i]*taw[i]);
			sum+=1.2533*Amp[i]*sigma[i]*taw[i]*eta[i]*ex*er;
		}
	}
	return sum;
}

void matrixInvertor(double A[][100],double Ainverse[100][100],int n)
{
	double temp1,div;
			int h;
			double temp[200]={0};
			double z[200][200]={0};
			for(int r=0; r<n ; ++r)
				for(int col=0; col<n ; ++col)
					z[r][col]=A[r][col];
				
			for(int r=0; r<n ; ++r)
					z[r][r+n]=1;
				

				for(int j=0; j<n ; ++j)
				{
					bool flag=true;
					if(z[j][j]==0)
					{
						for( h=j+1;((h<n)&&(flag==true));++h)
						{
							if(z[h][j] != 0)
							{
						
							
							for(int k=0;k<2*n;++k)
							{
								temp[k]=z[h][k];
								z[h][k]=z[j][k];
								z[j][k]=temp[k];
							}
							flag=false;
							}
						}
						
					}

							div=z[j][j];
							for (int f=0;f<(2*n);++f)
								z[j][f]/=div;
							
							for(int v=0 ;v<j ;++v)
							{
								temp1=z[v][j];
								for(int q=0; q<(2*n) ;++q)
									z[v][q]-=(temp1*z[j][q]);}

							for(int v=j+1 ;v<n ;++v)
							{
								temp1=z[v][j];
								for(int q=0; q<(2*n) ;++q)
									z[v][q]-=(temp1*z[j][q]);}
					
				}
				for(int g=0 ;g<n ;++g)
					for(int k=0;k<n;++k)
						Ainverse[g][k]=z[g][k+n];
}
int entirefit(int n,int maxchan,int itermax,double q0, long int spec[],int Amp[],int mio[],double sigma[],double taw[],double eta[]
	,double sigmaError[],double tawError[],double etaError[], int &runIter, double chi2[])
{
 bool flag=false;
 //double f[5000][100]={0};
 //double ft[100][5000]={0};
 double e[100][1]={0};
 double y[5000][1]={0};
 double A[100][100]={0};
 double Ainverse[100][100]={0};
 double b[100][1]={0};
 //double chi2[30]={0};


 double *f = (double*)malloc(maxchan * n * 3 * sizeof(double));
 double *ft = (double*)malloc(maxchan * n * 3 * sizeof(double));

 for (int i = 0; i < 30; ++i)
	 chi2[i] = 0;
 for (int i=1 ; i<= maxchan ; ++i )
 {
    if(spec[i]!=0)
    chi2[0]+=(pow(peakshape(n,i,Amp,mio,sigma,taw,eta)-spec[i],2)/spec[i]);
 }
    double landa=0.001;
 for (int j=1;(j<=itermax && flag==false);++j)
     {
		 for(int num=1; num<=n ;num++)
		 {
		 for (int h=1 ; h<=maxchan ;++h)
		 {
			 int fIdx = (h-1)*3*n + 3*num-3;
	         f[fIdx]=1.2533*Amp[num]*taw[num]*eta[num]*((exp((h-mio[num])*
				 taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num])))+
				 sigma[num]*(taw[num]*taw[num]*sigma[num]*exp((h-mio[num])*taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]))+
				 exp((h-mio[num])*taw[num]+0.5*pow(sigma[num]*taw[num],2))*0.7979*((h-mio[num])/pow(sigma[num],2)+taw[num])*
				 exp(-(pow((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]),2)))));

			 int ftIdx = (3*num-3) * maxchan + h-1;
	         ft[ftIdx]=1.2533*Amp[num]*taw[num]*eta[num]*((exp((h-mio[num])*
				 taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num])))+
				 sigma[num]*(taw[num]*taw[num]*sigma[num]*exp((h-mio[num])*taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]))+
				 exp((h-mio[num])*taw[num]+0.5*pow(sigma[num]*taw[num],2))*0.7979*((h-mio[num])/pow(sigma[num],2)+taw[num])*
				 exp(-(pow((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]),2)))));
         }

		 for (int h=1;h<=maxchan;++h)
		 {
			 int fIdx = (h-1)*3*n + 3*num-2;
			 f[fIdx]=1.2533*Amp[num]*sigma[num]*eta[num]*((exp((h-mio[num])*
				 taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num])))+
				 taw[num]*(((h-mio[num])+sigma[num]*sigma[num]*taw[num])*
				 exp((h-mio[num])* taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]))-
				  exp((h-mio[num])*taw[num]+0.5*pow(sigma[num]*taw[num],2))*0.7979*sigma[num]*
				 exp(-(pow((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]),2)))));

			 int ftIdx = (3*num-2) * maxchan + h-1;
			 ft[ftIdx]=1.2533*Amp[num]*sigma[num]*eta[num]*((exp((h-mio[num])*
				 taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num])))+
				 taw[num]*(((h-mio[num])+sigma[num]*sigma[num]*taw[num])*
				 exp((h-mio[num])* taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]))-
				  exp((h-mio[num])*taw[num]+0.5*pow(sigma[num]*taw[num],2))*0.7979*sigma[num]*
				 exp(-(pow((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]),2)))));
		 }

		 for(int h=1;h<=maxchan;++h)
		 {
			 int fIdx = (h-1)*3*n + 3*num-1;
			 f[fIdx]=1.2533*Amp[num]*sigma[num]*taw[num]*((exp((h-mio[num])*
				 taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]))));
			 int ftIdx = (3*num-1) * maxchan + h-1;
			 ft[ftIdx]=1.2533*Amp[num]*sigma[num]*taw[num]*((exp((h-mio[num])*
				 taw[num]+0.5*pow(sigma[num]*taw[num],2))*
				 erfc((1/sqrt((double)2))*((h-mio[num])/sigma[num]+sigma[num]*taw[num]))));
		 }

		 }

		 for (int h=1 ; h<=maxchan ;++h)
			 y[h-1][0]=spec[h]-peakshape(n,h,Amp,mio,sigma,taw,eta);

		 for (int v=0; v<100 ; v++)
			for (int f=0 ; f<100 ;f++ )
				A[v][f]=0;

		 for(int r=0; r<3*n ; ++r)
	       for(int col=0; col<3*n ; ++col)
 	         for( int s=0 ; s< maxchan ; ++s)
			 {
				 int fIdx = s*3*n + col;
				 int ftIdx = r*maxchan + s;
		         A[r][col]+=ft[ftIdx]*f[fIdx];
			 }

		 for(int i=0; i<3*n; i++)
                 A[i][i]*=(1+ landa);
                 
		 for(int i=0;i<100;i++)
			b[i][0]=0;

		 for (int d=0;d<3*n ;++d)
	          for(int g=0; g< maxchan;++g)
			  {
				  int ftIdx = d*maxchan + g;
		            b[d][0]+=ft[ftIdx]*y[g][0];
			  }

		 gaussJordan(A,b,e,3*n);    //a function for solving linear equations 
        
				
		double sigma2[100]={0};
		double taw2[100]={0};
		double eta2[100]={0};

		for(int i=1;i<=n;i++)
		{
			sigma2[i]=sigma[i]+e[3*i-3][0];
			taw2[i]=taw[i]+e[3*i-2][0];
			eta2[i]=eta[i]+e[3*i-1][0];
		}


		for (int i=1 ; i<= maxchan ; ++i )
		{
         if(spec[i]!=0)
			 chi2[j]+=(pow(peakshape(n,i,Amp,mio,sigma2,taw2,eta2)-spec[i],2)/spec[i]);
		}
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
          for(int i=1;i<=n;i++)
		    {
			   sigma[i]=sigma2[i];
			   taw[i]=taw2[i];
			   eta[i]=eta2[i];
		    }
		  for (int i=0;i<100;i++)
			    e[i][0]=0;
         }
	   if( (fabs(chi2[j]-chi2[j-1])/(maxchan-3*n)) < q0 )
	   {
		   runIter = j;
		   flag=true;
	   }
	   }

	   matrixInvertor(A,Ainverse,3*n);
	   
	   for (int i=0; i<n ;i++)
	   {
		   sigmaError[i+1]=sqrt(Ainverse[3*i][3*i]);
		   tawError[i+1]=sqrt(Ainverse[3*i+1][3*i+1]);
		   etaError[i+1]=sqrt(Ainverse[3*i+2][3*i+2]);

	   }


	   free(f);
	   free(ft);

	   if (flag==true)
		   return 1;
	   else
		   return 0;
}





// this function recieves pairs of channels/Energies (up to 10)
// and holds this number in the "k" variable and performs a
//quadratic fit (y=a*x*x+b*x+c /Energy=a*channel^2+b*channel+c) to data and saves the final results
//of consts(a,b,c) in the c[3] array.as previously stated this function needs 
//the function gaussjordan.

void energyCalibration(int k, double ch[], double energy[], double c[])
	{
                         
			double f[10][3];
			double ft[3][10];
			double e[3][1];
			double y[10][1];
			double A[10][10]={0};
			double b[3][1]={0};

	//cout<<"\n\nFor energy calibration you must enter pairs \n
 //       of channel numbers and corresponding energies:(up to 10 and -1 for channel to exit.)"<<endl;
	//cout<<"\n\nThis Program will perform a quadratic fit for the enterd data:"<<endl;

	//        int k=0;
	//        cout<<"\nplease enter channel number "<<k+1<<":";
	//	cin>>ch[k];
	//	cout<<"please enter energy(Kev): "<<k+1<<":";
	//	cin>>energy[k];

 //       while( ch[k]!= -1 && k<10)
 //         {
 //            k++;
 //            cout<<"\nplease enter channel number "<<k+1<<":";
	//     cin>>ch[k];
	//     cout<<"please enter energy(Kev): "<<k+1<<":";
	//     cin>>energy[k];
 //         }          
	
	for (int i=0;i<k;i++)
	{
		f[i][0]=ch[i]*ch[i];
		ft[0][i]=ch[i]*ch[i];
	}
	for (int i=0;i<k;i++)
	{
		f[i][1]=ch[i];
		ft[1][i]=ch[i];
	}
	for (int i=0;i<k;i++)
	{
		f[i][2]=1;
		ft[2][i]=1;
	}
	for (int i=0;i<k;i++)
		y[i][0]=energy[i];
	
	
	for(int r=0; r<3 ; ++r)
		for(int col=0; col<3 ; ++col)
			for( int s=0 ; s<k; ++s)
			A[r][col]+=ft[r][s]*f[s][col];
						
		for (int d=0;d<3 ;++d)
			for(int g=0;g<k;++g)
			b[d][0]+=ft[d][g]*y[g][0];

			gaussJordan(A,b,e,3);

			c[0]=e[0][0];
			c[1]=e[1][0];
			c[2]=e[2][0];

//a=c[0]  b=c[1] c=c[2]
  
	}

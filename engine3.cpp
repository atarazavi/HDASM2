#include <math.h>
#include "Engine.h"
#include <windows.h>

void gaussJordan(double x[][100],double y[][1],double E[][1],const int n);

//void gaussJordan(double x[][100],double y[][1],double E[][1],const int n)
//		{
//		
//			double temp1,div;
//			int h;
//			double temp[101]={0};
//			double z[101][101]={0};
//			for(int r=0; r<n ; ++r)
//				for(int col=0; col<n ; ++col)
//					z[r][col]=x[r][col];
//				for(int i=0; i<n;++i)
//					z[i][n]=y[i][0];
//				
//
//				for(int j=0; j<n ; ++j)
//				{
//					bool flag=true;
//					if(z[j][j]==0)
//					{
//						for( h=j+1;((h<n)&&(flag==true));++h)
//						{
//							if(z[h][j] != 0)
//							{
//						
//							
//							for(int k=0;k<n;++k)
//							{
//								temp[k]=z[h][k];
//								z[h][k]=z[j][k];
//								z[j][k]=temp[k];
//							}
//							flag=false;
//							}
//						}
//						
//					}
//
//							div=z[j][j];
//							for (int f=0;f<(n+1);++f)
//								z[j][f]/=div;
//							
//							for(int v=0 ;v<j ;++v)
//							{
//								temp1=z[v][j];
//								for(int q=0; q<(n+1) ;++q)
//									z[v][q]-=(temp1*z[j][q]);}
//
//							for(int v=j+1 ;v<n ;++v)
//							{
//								temp1=z[v][j];
//								for(int q=0; q<(n+1) ;++q)
//									z[v][q]-=(temp1*z[j][q]);}
//					
//				}
//				for(int g=0 ;g<n ;++g)
//					E[g][0]=z[g][n];
//		}





//int sign(double x)
//{
//    if(x==0)
//		return 0;
//    if(x>0)
//		return 1;
//    if(x<0)
//		return -1;
//}

//double erfc(double z)
//{
//	return 1 - sign(z) * sqrt(1 - exp(-pow(z, 2) * (1.27324 + 0.140012 * pow(z, 2)) / (1 + 0.140012 *pow(z, 2))));
//}
double specshape(const int n, int u ,double Amp[],double mio[],double sigma,double taw1,double taw2,double eta)
{
	double sum=0;
	for(int i=1;i<=n;i++)
		sum+=1.2533*Amp[i]*sigma*(taw1*eta*exp((u-mio[i])*taw1+0.5*sigma*sigma*taw1*taw1)*erfc(0.707*((u-mio[i])/sigma+sigma*taw1))+
                taw2*(1-eta)*exp((u-mio[i])*taw2+0.5*sigma*sigma*taw2*taw2)*erfc(0.707*((u-mio[i])/sigma+sigma*taw2)));
	return sum;
}


int entirefit2(int n,int maxchan,int itermax,double q0, long int spec[],double Amp[], double mio[],double &sigma,double &taw1,double &taw2,double &eta
	, int &runIter, double chi2[])
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
 int fIdx;
 int ftIdx;

 double *f = (double*)malloc(maxchan * (n * 2+4) * sizeof(double));
 double *ft = (double*)malloc(maxchan * (n * 2+4) * sizeof(double));

  for (int i = 0; i < 30; ++i)
	 chi2[i] = 0;
 for (int i=1 ; i<= maxchan ; ++i )
    if(spec[i]!=0)
    chi2[0]+=(pow(specshape(n,i,Amp,mio,sigma,taw1,taw2,eta)-spec[i],2)/spec[i]);
    double landa=0.001;
 for (int j=1;(j<=itermax && flag==false);++j)
     {
		 for(int num=1; num<=n ;num++)
		 {
		 for (int h=1 ; h<=maxchan ;++h)
		 {
			 fIdx = (h-1)*2*n + 2*num-2;
	         f[fIdx]=1.2533*Amp[num]*sigma*(taw1*eta*(-1*taw1*exp((h-mio[num])*taw1+0.5*sigma*sigma*taw1*taw1)*
			erfc(0.707*(h-mio[num])/sigma+sigma*taw1)+exp((h-mio[num])*taw1+0.5*sigma*sigma*taw1*taw1)*
			(0.798/sigma)*exp(-1*pow(0.707*(h-mio[num])/sigma+sigma*taw1,2)))+(1-eta)*taw2*
			(-1*taw2*exp((h-mio[num])*taw2+0.5*sigma*sigma*taw2*taw2)*
			erfc(0.707*(h-mio[num])/sigma+sigma*taw2)+exp((h-mio[num])*taw2+0.5*sigma*sigma*taw2*taw2)*
			(0.798/sigma)*exp(-1*pow(0.707*(h-mio[num])/sigma+sigma*taw2,2))));

			 ftIdx = (2*num-2) * maxchan + h-1;
	         ft[ftIdx]=1.2533*Amp[num]*sigma*(taw1*eta*(-1*taw1*exp((h-mio[num])*taw1+0.5*sigma*sigma*taw1*taw1)*
			erfc(0.707*(h-mio[num])/sigma+sigma*taw1)+exp((h-mio[num])*taw1+0.5*sigma*sigma*taw1*taw1)*
			(0.798/sigma)*exp(-1*pow(0.707*(h-mio[num])/sigma+sigma*taw1,2)))+(1-eta)*taw2*
			(-1*taw2*exp((h-mio[num])*taw2+0.5*sigma*sigma*taw2*taw2)*
			erfc(0.707*(h-mio[num])/sigma+sigma*taw2)+exp((h-mio[num])*taw2+0.5*sigma*sigma*taw2*taw2)*
			(0.798/sigma)*exp(-1*pow(0.707*(h-mio[num])/sigma+sigma*taw2,2))));
        	 }

		 for (int h=1;h<=maxchan;++h)
		 {
			 fIdx = (h-1)*2*n + 2*num-1;
			 f[fIdx]=1.2533*sigma*(taw1*eta*exp((h-mio[num])*taw1+0.5*sigma*sigma*taw1*taw1)*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1))+taw2*(1-eta)*
 				 exp((h-mio[num])*taw2+0.5*sigma*sigma*taw2*taw2)*erfc(0.707*((h-mio[num])/sigma+sigma*taw2)));

			 ftIdx = (2*num-1) * maxchan + h-1;
			 ft[ftIdx]=1.2533*sigma*(taw1*eta*exp((h-mio[num])*taw1+0.5*sigma*sigma*taw1*taw1)*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1))+taw2*(1-eta)*
 				 exp((h-mio[num])*taw2+0.5*sigma*sigma*taw2*taw2)*erfc(0.707*((h-mio[num])/sigma+sigma*taw2)));

		 }

		 
		 }

		for( int h=1 ; h<=maxchan ;++h)
		{
		fIdx= (h-1)*2*n+(2*n);
		for( int num=1 ; num<=n ; num++)
		f[fIdx]+=1.2533*Amp[num]*taw1*eta*((exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*erfc(0.707*((h-mio[num])/sigma+sigma*taw1)))+
			sigma*(taw1*taw1*sigma*exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*
			erfc(0.707*((h-mio[num])/sigma+sigma*taw1))+
			exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*0.7979*((h-mio[num])/pow(sigma,2)+taw1)*
			exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw1),2)))))+
			1.2533*Amp[num]*taw2*(1-eta)*((exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*erfc(0.707*((h-mio[num])/sigma+sigma*taw2)))+
			sigma*(taw2*taw2*sigma*exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*
			erfc(0.707*((h-mio[num])/sigma+sigma*taw2))+
			exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*0.7979*((h-mio[num])/pow(sigma,2)+taw2)*
			exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw2),2)))));

		ftIdx = 2*n*maxchan + h - 1;
		for( int num=1 ; num<=n ; num++)
		ft[ftIdx]+=1.2533*Amp[num]*taw1*eta*((exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*erfc(0.707*((h-mio[num])/sigma+sigma*taw1)))+
			sigma*(taw1*taw1*sigma*exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*
			erfc(0.707*((h-mio[num])/sigma+sigma*taw1))+
			exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*0.7979*((h-mio[num])/pow(sigma,2)+taw1)*
			exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw1),2)))))+
			1.2533*Amp[num]*taw2*(1-eta)*((exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*erfc(0.707*((h-mio[num])/sigma+sigma*taw2)))+
			sigma*(taw2*taw2*sigma*exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*
			erfc(0.707*((h-mio[num])/sigma+sigma*taw2))+
			exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*0.7979*((h-mio[num])/pow(sigma,2)+taw2)*
			exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw2),2)))));

		

		fIdx= (h-1)*2*n+(2*n+1);
		for( int num=1 ; num<=n ; num++)
		f[fIdx]+=1.2533*Amp[num]*sigma*eta*((exp((h-mio[num])*
				 taw1+0.5*pow(sigma*taw1,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1)))+
				 taw1*(((h-mio[num])+sigma*sigma*taw1)*
				 exp((h-mio[num])* taw1+0.5*pow(sigma*taw1,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1))-
				  exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*0.7979*sigma*
				 exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw1),2)))));

		int ftIdx = (2*n+1)*maxchan + h - 1;
		for( int num=1 ; num<=n ; num++)
		ft[ftIdx]+=1.2533*Amp[num]*sigma*eta*((exp((h-mio[num])*
				 taw1+0.5*pow(sigma*taw1,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1)))+
				 taw1*(((h-mio[num])+sigma*sigma*taw1)*
				 exp((h-mio[num])* taw1+0.5*pow(sigma*taw1,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1))-
				  exp((h-mio[num])*taw1+0.5*pow(sigma*taw1,2))*0.7979*sigma*
				 exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw1),2)))));


		fIdx= (h-1)*2*n+(2*n+2);
		for(int num=1; num<=n ;num++)
		f[fIdx]+=1.2533*Amp[num]*sigma*(1-eta)*((exp((h-mio[num])*
				 taw2+0.5*pow(sigma*taw2,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw2)))+
				 taw2*(((h-mio[num])+sigma*sigma*taw2)*
				 exp((h-mio[num])* taw2+0.5*pow(sigma*taw2,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw2))-
				  exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*0.7979*sigma*
				 exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw2),2)))));

		ftIdx= (2*n+2)*maxchan + h - 1;
		for(int num=1; num<=n ;num++)
		ft[ftIdx]+=1.2533*Amp[num]*sigma*(1-eta)*((exp((h-mio[num])*
				 taw2+0.5*pow(sigma*taw2,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw2)))+
				 taw2*(((h-mio[num])+sigma*sigma*taw2)*
				 exp((h-mio[num])* taw2+0.5*pow(sigma*taw2,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw2))-
				  exp((h-mio[num])*taw2+0.5*pow(sigma*taw2,2))*0.7979*sigma*
				 exp(-(pow(0.707*((h-mio[num])/sigma+sigma*taw2),2)))));

		
		fIdx= (h-1)*2*n+(2*n+3);
		for(int num=1; num<=n ;num++)
		f[fIdx]+=1.2533*Amp[num]*sigma*taw1*((exp((h-mio[num])*
				 taw1+0.5*pow(sigma*taw1,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1))))-
			1.2533*Amp[num]*sigma*taw2*((exp((h-mio[num])*
				 taw2+0.5*pow(sigma*taw2,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw2))));

		ftIdx= (2*n+3)*maxchan + h - 1;
		for(int num=1; num<=n ;num++)
		ft[ftIdx]+=1.2533*Amp[num]*sigma*taw1*((exp((h-mio[num])*
				 taw1+0.5*pow(sigma*taw1,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw1))))-
			1.2533*Amp[num]*sigma*taw2*((exp((h-mio[num])*
				 taw2+0.5*pow(sigma*taw2,2))*
				 erfc(0.707*((h-mio[num])/sigma+sigma*taw2))));
		}

		

		 for (int h=1 ; h<=maxchan ;++h)
			 y[h-1][0]=spec[h]-specshape(n,h,Amp,mio,sigma,taw1,taw2,eta);

		 for (int v=0; v<100 ; v++)
			for (int f=0 ; f<100 ;f++ )
				A[v][f]=0;

		 for(int r=0; r<2*n+4 ; ++r)
	       for(int col=0; col<2*n+4 ; ++col)
 	         for( int s=0 ; s< maxchan ; ++s)
			 {
				 fIdx = s*(2*n+4) + col;
				 int ftIdx = r*maxchan + s;
		         A[r][col]+=ft[ftIdx]*f[fIdx];
			 }

		 for(int i=0; i<(2*n+4); i++)
                 A[i][i]*=(1+ landa);
                 
		 for(int i=0;i<100;i++)
			b[i][0]=0;

		 for (int d=0;d<(2*n+4) ;++d)
	          for(int g=0; g< maxchan;++g)
			  {
				  int ftIdx = d*maxchan + g;
		            b[d][0]+=ft[ftIdx]*y[g][0];
			  }

		 gaussJordan(A,b,e,(2*n+4));    //a function for solving linear equations 
        
				
		double mio2[100]={0};
		double amp2[100]={0};
		double sigma2,taw12,taw22,eta2=0;

		for(int i=1;i<=n;i++)
		{
			mio2[i]=mio[i]+e[2*i-2][0];
			amp2[i]=Amp[i]+e[2*i-1][0];
			
		}

			sigma2=sigma+e[2*n][0];
			taw12=taw1+e[2*n+1][0];
			taw22=taw2+e[2*n+2][0];
			eta2=eta+e[2*n+3][0];

		for (int i=1 ; i<= maxchan ; ++i )
         if(spec[i]!=0)
         chi2[j]+=(pow(specshape(n,i,amp2,mio2,sigma2,taw12,taw22,eta2)-spec[i],2)/spec[i]);

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
			   mio[i]=mio2[i];
			   Amp[i]=amp2[i];
			  
		    }

			sigma=sigma2;
			taw1=taw12;
			taw2=taw22;
			eta=eta2;


		  for (int i=0;i<100;i++)
			    e[i][0]=0;
         }
	   if( (fabs(chi2[j]-chi2[j-1])/(maxchan-(2*n+4))) < q0 )
	   {
		   runIter = j;
           flag=true;
	   }
	   }

	   free(f);
	   free(ft);

	   if (flag==true)
		   return 1;
	   else
		   return 0;
}

#include "math.h"
#include "Engine.h"
#include "global.h"

double SigmaHCalc(long int Spec[],int FirstCh, int LastCh, int A)
{
double SigmaH=0.1;
double Chi2=1e300;

	//for(int i=FirstCh;i<=LastCh;i++)
	//	Chi2+=pow(Spec[i]-A*exp(-0.5*(i-FirstCh)*(i-FirstCh)/(SigmaH*SigmaH)),2);
	//SigmaH+=0.1;
	for (SigmaH = 0.1; SigmaH<200;SigmaH+=0.1)
	{
		double TempChi2=0;
		for(int i=FirstCh;i<=LastCh;i++)
			TempChi2+=pow(Spec[i]-A*exp(-0.5*(i-FirstCh)*(i-FirstCh)/(SigmaH*SigmaH)),2);
		if(TempChi2< Chi2)
		{
			Chi2=TempChi2;
			FinalSigmaH=SigmaH;
		}
	}
	return FinalSigmaH;
}

double SigmaLCalc(long int Spec[],int FirstCh, int LastCh, int A)
{
double SigmaL=0.1;
double Chi2=1e300;

	//for(int i=FirstCh;i<=LastCh;i++)
	//	Chi2+=pow(Spec[i]-A*exp(-0.5*(LastCh-i)/SigmaL),2)/Spec[i];
	//SigmaL+=0.1;
	for(SigmaL=0.1; SigmaL<200; SigmaL+=0.1)
	{
		double TempChi2=0;
		for(int i=FirstCh;i<=LastCh;i++)
			TempChi2+=pow(Spec[i]-A*exp(-0.5*(LastCh-i)/SigmaL),2);
		if(TempChi2< Chi2)
		{
			Chi2=TempChi2;
			FinalSigmaL=SigmaL;
		}
	}
	return FinalSigmaL;
}

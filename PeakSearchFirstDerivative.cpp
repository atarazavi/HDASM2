#include "Engine.h"

void PeakSearchFirstDerivative(long int Spec1[], long int Spec2[],int NumberOfPoints,int SpecLength , int peaks[])
{
	int point5[]={-2,-1,0,1,2};
	int point7[]={-3,-2,-1,0,1,2,3};
	int point9[]={-4,-3,-2,-1,0,1,2,3,4};
	int point11[]={-5,-4,-3,-2,-1,0,1,2,3,4,5};
	int point13[]={-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6};
	int point15[]={-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7};
	int point17[]={-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8};
	int point19[]={-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9};

	for(int i=0; i<SpecLength; i++)
		Spec2[i]=0;

	switch(NumberOfPoints)
	{
	case 5:
		for(int h=2; h<(SpecLength-2) ; h++)
		{
			Spec2[h]=(1.0/10.0)*(Spec1[h]*point5[2]+Spec1[h-1]*point5[1]+Spec1[h-2]*point5[0]+Spec1[h+1]*point5[3]+Spec1[h+2]*point5[4]);
		}
		break;

	case 7:

		for(int h=3; h<(SpecLength-3) ; h++)
		{
			Spec2[h]=(1.0/28.0)*(Spec1[h]*point7[3]+Spec1[h-1]*point7[2]+Spec1[h-2]*point7[1]+Spec1[h-3]*point7[0]
			+Spec1[h+1]*point7[4]+Spec1[h+2]*point7[5]+Spec1[h+3]*point7[6]);
		}
		break;

	case 9:

		for(int h=4; h<(SpecLength-4) ; h++)
		{
			Spec2[h]=(1.0/60.0)*(Spec1[h]*point9[4]+Spec1[h-1]*point9[3]+Spec1[h-2]*point9[2]+Spec1[h-3]*point9[1]
			+Spec1[h-4]*point9[0]+Spec1[h+1]*point9[5]+Spec1[h+2]*point9[6]+Spec1[h+3]*point9[7]+Spec1[h+4]*point9[8]);
		}
		break;

	case 11:

		for(int h=5; h<(SpecLength-5) ; h++)
		{
			Spec2[h]=(1.0/110.0)*(Spec1[h]*point11[5]+Spec1[h-1]*point11[4]+Spec1[h-2]*point11[3]+Spec1[h-3]*point11[2]
			+Spec1[h-4]*point11[1]+Spec1[h-5]*point11[0]+Spec1[h+1]*point11[6]+Spec1[h+2]*point11[7]+Spec1[h+3]*point11[8]
			+Spec1[h+4]*point11[9]+Spec1[h+5]*point11[10]);
		}
		break;

	case 13:

		for(int h=6; h<(SpecLength-6) ; h++)
		{
			Spec2[h]=(1.0/182.0)*(Spec1[h]*point13[6]+Spec1[h-1]*point13[5]+Spec1[h-2]*point13[4]+Spec1[h-3]*point13[3]
			+Spec1[h-4]*point13[2]+Spec1[h-5]*point13[1]+Spec1[h-6]*point13[0]+Spec1[h+1]*point13[7]+Spec1[h+2]*point13[8]
			+Spec1[h+3]*point13[9]+Spec1[h+4]*point13[10]+Spec1[h+5]*point13[11]+Spec1[h+6]*point13[12]);
		}
		break;

	case 15:

		for(int h=7; h<(SpecLength-7) ; h++)
		{
			Spec2[h]=(1.0/280.0)*(Spec1[h]*point15[7]+Spec1[h-1]*point15[6]+Spec1[h-2]*point15[5]+Spec1[h-3]*point15[4]
			+Spec1[h-4]*point15[3]+Spec1[h-5]*point15[2]+Spec1[h-6]*point15[1]+Spec1[h-7]*point15[0]+Spec1[h+1]*point15[8]
			+Spec1[h+2]*point15[9]+Spec1[h+3]*point15[10]+Spec1[h+4]*point15[11]+Spec1[h+5]*point15[12]+Spec1[h+6]*point15[13]
			+Spec1[h+7]*point15[14]);
		}
		break;

	case 17:

		for(int h=8; h<(SpecLength-8) ; h++)
		{
			Spec2[h]=(1.0/408.0)*(Spec1[h]*point17[8]+Spec1[h-1]*point17[7]+Spec1[h-2]*point17[6]+Spec1[h-3]*point17[5]
			+Spec1[h-4]*point17[4]+Spec1[h-5]*point17[3]+Spec1[h-6]*point17[2]+Spec1[h-7]*point17[1]+Spec1[h-8]*point17[0]
			+Spec1[h+1]*point17[9]+Spec1[h+2]*point17[10]+Spec1[h+3]*point17[11]+Spec1[h+4]*point17[12]+Spec1[h+5]*point17[13]
			+Spec1[h+6]*point17[14]+Spec1[h+7]*point17[15]+Spec1[h+8]*point17[16]);
		}
		break;

	case 19:

		for(int h=9; h<(SpecLength-9) ; h++)
		{
			Spec2[h]=(1.0/570.0)*(Spec1[h]*point19[9]+Spec1[h-1]*point19[8]+Spec1[h-2]*point19[7]+Spec1[h-3]*point19[6]
			+Spec1[h-4]*point19[5]+Spec1[h-5]*point19[4]+Spec1[h-6]*point19[3]+Spec1[h-7]*point19[2]+Spec1[h-8]*point19[1]
			+Spec1[h-9]*point19[0]+Spec1[h+1]*point19[10]+Spec1[h+2]*point19[11]+Spec1[h+3]*point19[12]+Spec1[h+4]*point19[13]
			+Spec1[h+5]*point19[14]+Spec1[h+6]*point19[15]+Spec1[h+7]*point19[16]+Spec1[h+8]*point19[17]+Spec1[h+9]*point19[18]);
		}
		break;

	default:
		break;
	}
	int k=0;
	for(int i=5 ; i <(SpecLength-5) ; i++)
	{
		if( Spec2[i]<0 &&  Spec2[i+1]<0 &&  Spec2[i+2]<0 &&  Spec2[i-1]>0 &&  Spec2[i-2]>0 &&  Spec2[i-3]>0)
		{
			int peaktemp=i;
			int max= Spec1[i];
			for(int r=i-5 ; r<= i+5  ; r++)
				if( Spec1[r]>max)
				{
					max=Spec1[r];
					peaktemp=r;
				}
				peaks[k]=peaktemp;
				k++;
		}
	}
}
#include "Engine.h"

int PeaksValleysFinder(long int mainspec[], long int smoothedspec[], int peaks[], int valleys[], int distance1, int distance2,int speclength)
{

long int max,min;
int temp,ttemp;
int p=0;
int peakFlatness = distance1/8;

	for(int i=distance1; i<(speclength-distance1) ; i++)
	{
		bool flag=true;
		int noOfSameHeights=0;
		for(int h=i-distance1; h<=i+distance1; h++)
		{
			if (smoothedspec[h]>smoothedspec[i])
			{
				flag=false;
				break;
			}
			else if (smoothedspec[h]==smoothedspec[i])
				++noOfSameHeights;
		}
		if (noOfSameHeights > peakFlatness)
			flag=false;
		if(flag==true)
		{
			temp=i;
			max=mainspec[i];
			for(int k=i-distance2; k<i+distance2 ; k++)
			{
				if(mainspec[k]>max)
				{
					max=mainspec[k];
					temp=k;
				}
			}
			if (p== 0 || peaks[p - 1] != temp)
			{
				peaks[p] = temp;
				p++;
			}
			i += distance1-1;
		}
	}
	if (p == 0)
		return 0;
	for(int i=1 ; i< peaks[0]; i++)
	{
		if( smoothedspec[i] != 0 )
		{
			valleys[0]=i;
			break;
		}
	}
	for(int  i=speclength-2 ; i> peaks[p-1] ; i--)
	{
		if( smoothedspec[i] != 0 )
		{
			valleys[p]=i;
			break;
		}
	}
	for(int i=1 ;i<p; i++)
	{
		min=smoothedspec[peaks[i-1]];
		ttemp=peaks[i-1];
		for(int j=peaks[i-1]; j< peaks[i] ;j++)
		{
			if( smoothedspec[j]< min)
			{
				min=smoothedspec[j];
				ttemp=j;
			}
		}
		valleys[i]=ttemp;
	}
	return p;
}


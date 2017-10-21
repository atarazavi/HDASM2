#ifndef __ENGINE_H
#define __ENGINE_H


extern "C" __declspec( dllexport ) int alphafit(int minchan,double &p,int maxchan,double &a,double &sig,double &B,int itermax,double q0, long int spec[]);

extern "C" __declspec( dllexport ) void GetParameters(int &j, double &landa, double chi2[]);

extern "C" __declspec( dllexport ) void coefficient(int z0,int w0,int co[]);

extern "C" __declspec( dllexport ) void peakSearch(long int a[],long int s[],long int f[],int peak[],int peak3[],int peak5[]);

//extern "C" __declspec( dllexport ) void gaussJordan(double x[][100],double y[][1],double E[][1],const int n);

extern "C" __declspec( dllexport ) void energyCalibration(int k, double ch[], double energy[], double c[]);

extern "C" __declspec( dllexport ) double erfc(double z);

extern "C" __declspec( dllexport ) double peakshape(const int n, int u ,int Amp[],int mio[],double sigma[],double taw[],double eta[]);

extern "C" __declspec( dllexport ) int entirefit(int n, int maxchan, int itermax, double q0, long int spec[], int Amp[], int mio[]
	, double sigma[], double taw[], double eta[], double sigmaError[], double tawError[], double etaError[], int &runIter, double chi2[]);

extern "C" __declspec( dllexport ) int entirefit2(int n,int maxchan,int itermax,double q0, long int spec[],double Amp[], double mio[]
	, double &sigma, double &taw1, double &taw2, double &eta, int &runIter, double chi2[]);

extern "C" __declspec( dllexport ) void quadraticCubicSmoothing(long int Spec1[], long int Spec2[],int NumberOfPoints,int SpecLength);

extern "C" __declspec( dllexport ) void PeakSearchFirstDerivative(long int Spec1[], long int Spec2[],int NumberOfPoints,int SpecLength , int peaks[]);

extern "C" __declspec( dllexport ) double SigmaHCalc(long int Spec[],int FirstCh, int LastCh, int A);

extern "C" __declspec( dllexport ) double SigmaLCalc(long int Spec[],int FirstCh, int LastCh, int A);

extern "C" __declspec( dllexport ) int PeaksValleysFinder(long int mainspec[], long int smoothedspec[], int peaks[], int valleys[], int distance1, int distance2,int speclength);

extern "C" __declspec( dllexport ) int DerivativePeaksValleysFinder(long int Spec1[], long int smoothedspec[],long int spec2[], int peaks[], int valleys[], int NumberOfPoints,int SpecLength);

#endif

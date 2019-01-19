#ifndef _WAVELET_H_
#define _WAVELET_H_

#include <vector>
using namespace std;

// Wavelet filter
typedef struct tagWaveFilter
{
	vector<double> Lo_D;  // Decomposition low pass
	vector<double> Hi_D;  // Decomposition high pass
	vector<double> Lo_R;  // Reconstruct low pass
	vector<double> Hi_R;  // Reconstruct high pass
	int filterLen;
}WaveFilter;

// 1D decomposition information
typedef struct tagCL1D
{
	vector<int> msgLen;
	int Scale;		// Decomposition scale
	int dbn;		// db wavelet number
	int allSize;	// total length of data
}msgCL1D;

// 2D decomposition information
typedef struct tagCL2D
{
	vector<int> msgHeight;
	vector<int> msgWidth;
	int Scale;		// Decomposition scale
	int dbn;		// db wavelet number
	int allSize;	// total length of data
}msgCL2D;


typedef bool SORH;

class CWavelet
{
public:
	CWavelet(int dbn=3);
	~CWavelet() {}

	int  DWT(
		double *pSrcData,
		int srcLen,
		double *pDstCeof
		);

	void  IDWT(double *pSrcCoef,
		int dstLen,
		double *pDstData
		);
	

	// Wavelet decomposition of 2D data
	void  DWT2(
		double *pSrcImage, // Source image data, realigned as 1D
		int height,		   // image height
		int width,		   // image width
		double *pImagCeof  // decomposition coefficient
		);

	void  IDWT2(
		double *pSrcCeof,
		int height,
		int width,
		double *pDstImage
		);


	bool WaveDec(
		double *pSrcData,
		double *pDstCeof
		);

	bool WaveRec(
		double *pSrcCoef,
		double *pDstData
		);


	bool InitDecInfo(
		const int srcLen,
		const int Scale,
		const int dbn =3
		);

	bool InitDecInfo2D(
		const int height,
		const int width,
		const int Scale,
		const int dbn = 3
		);

	bool thrDenoise(
		double *pSrcNoise,
		double *pDstData,
		bool isHard=true
		);

	void Wthresh(
		double *pDstCoef,
		double thr,
		const int allsize,
		const int gap,
		SORH  ish
		);

	// Multi-level 2D wavelet decompositon
	bool WaveDec2(
		double *pSrcData,
		double *pDstCeof
		);

	// Reconstruction of 2D signal
	bool WaveRec2(
		double *pSrcCoef,  // Source coefficient
		double *pDstData   // Destination signal
		);

	void InitFilter();

	void SetFilter(int dbn);

	// Get threshold
	double getThr(
		double *pDetCoef,
		int detlen,
		bool is2D =false
		);

	bool AdjustData(
		double *pDetCoef, 
		const int height,
		const int width
		);

	// Inverse adjustment
	bool IAdjustData(
		double *pDetCoef,
		const int height,
		const int width
		);

	bool thrDenoise2D(
		double *pNoiseImag,
		double *pDstImag,
		bool isHard = true);

public:
	msgCL1D m_msgCL1D;
	msgCL2D m_msgCL2D;
private:
	bool m_bInitFlag1D;
	bool m_bInitFlag2D;
	WaveFilter m_dbFilter;
};

#endif
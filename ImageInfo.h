#include <iostream>
#include <vector>
#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <python3.10/Python.h>
#include <python3.10/numpy/arrayobject.h>
#include <stdio.h>
#include <queue>
#include <typeinfo>
using namespace std;

class ImageInfo{
	public:
		ImageInfo(int ImageNumber);
		~ImageInfo();
		queue<queue<double>> layersX();
		queue<queue<double>> layerInfoX();
		queue<queue<double>> layerInfoX2();
		queue<queue<double>> layerInfoX3();
	private:
		queue<queue<double>> layers_queue;
		queue<queue<double>> layerInfoX_queue;
		queue<queue<double>> layerInfoX2_queue;
		queue<queue<double>> layerInfoX3_queue;
		
};


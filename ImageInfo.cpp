#include "ImageInfo.h"


ImageInfo::ImageInfo(int ImageNumber){

    // Import the Python module
    PyObject* module_name = PyUnicode_FromString("FowardProp");
    PyObject* module = PyImport_Import(module_name);
    Py_DECREF(module_name);


    // Get the function from the module
    PyObject* func = PyObject_GetAttrString(module, "run_foward_prop_on_image");
    // PyObject* func2 = PyObject_GetAttrString(module, "getLayerInfo");


    // Call the function and get the returned values
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, PyLong_FromLong(ImageNumber));
    PyObject* result = PyObject_CallObject(func, args);
    Py_DECREF(func);

    // Extract the returned values
    PyObject* scalar_obj = PyTuple_GetItem(result, 0);
    PyObject* layers = PyTuple_GetItem(result, 1);
    PyObject* layerInfoX = PyTuple_GetItem(result, 2);
    PyObject* layerInfoX2 = PyTuple_GetItem(result, 3);
    PyObject* layerInfoX3 = PyTuple_GetItem(result, 4);
    // Print the returned values
    double scalar = PyFloat_AsDouble(scalar_obj);
    printf("Scalar: %f\n", scalar);

    // int scalar2 = PyFloat_AsDouble(scalar_obj2);
    // // printf("Scalar: %d\n", scalar2);

    PyArrayObject* layers_array = reinterpret_cast<PyArrayObject*>(layers);
    double* layers_array_data = static_cast<double*>(PyArray_DATA(layers_array));

    PyArrayObject* layerInfoX_array = reinterpret_cast<PyArrayObject*>(layerInfoX);
    double* layerInfoX_array_data = static_cast<double*>(PyArray_DATA(layerInfoX_array));

    PyArrayObject* layerInfoX2_array = reinterpret_cast<PyArrayObject*>(layerInfoX2);
    double* layerInfoX2_array_data = static_cast<double*>(PyArray_DATA(layerInfoX2_array));

    PyArrayObject* layerInfoX3_array = reinterpret_cast<PyArrayObject*>(layerInfoX3);
    double* layerInfoX3_array_data = static_cast<double*>(PyArray_DATA(layerInfoX3_array));


    // 

    // layers_queue;
    // layerInfoX_queue;
    // queue<queue<double>> layerInfoX2_queue;
    // queue<queue<double>> layerInfoX3_queue;
    
    // 
    for (int i = 0; i < PyArray_DIMS(layers_array)[0]; i++) {
        queue<double> temp;
        for (int j = 0; j < PyArray_DIMS(layers_array)[1]; j++) {
            temp.push(layers_array_data[i * PyArray_DIMS(layers_array)[1] + j]);
        }
        layers_queue.push(temp);
    }

    for (int i = 0; i < PyArray_DIMS(layerInfoX_array)[0]; i++) {
        queue<double> temp;
        for (int j = 0; j < PyArray_DIMS(layerInfoX_array)[1]; j++) {
            temp.push(layerInfoX_array_data[i * PyArray_DIMS(layerInfoX_array)[1] + j]);
        }
        layerInfoX_queue.push(temp);
    }

    for (int i = 0; i < PyArray_DIMS(layerInfoX2_array)[0]; i++) {
        queue<double> temp;
        for (int j = 0; j < PyArray_DIMS(layerInfoX2_array)[1]; j++) {
            temp.push(layerInfoX2_array_data[i * PyArray_DIMS(layerInfoX2_array)[1] + j]);
        }
        layerInfoX2_queue.push(temp);
    }

    for (int i = 0; i < PyArray_DIMS(layerInfoX3_array)[0]; i++) {
        queue<double> temp;
        for (int j = 0; j < PyArray_DIMS(layerInfoX3_array)[1]; j++) {
            temp.push(layerInfoX3_array_data[i * PyArray_DIMS(layerInfoX3_array)[1] + j]);
        }
        layerInfoX3_queue.push(temp);
    }

 

    Py_DECREF(result);
    Py_DECREF(module);

    // Clean up

}
ImageInfo::~ImageInfo(){
	cout << " Finished Image" << endl;
}
queue<queue<double>> ImageInfo::layersX(){
	return layers_queue;
}
queue<queue<double>> ImageInfo::layerInfoX(){
	return layerInfoX_queue;
}
queue<queue<double>> ImageInfo::layerInfoX2(){
	return layerInfoX2_queue;
}
queue<queue<double>> ImageInfo::layerInfoX3(){
	return layerInfoX3_queue;
}


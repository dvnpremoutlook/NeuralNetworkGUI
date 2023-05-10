#ifndef TENSOR_H
#define TENSOR_H

#include <iostream>
#include <vector>

class Tensor {
    private:
        std::vector<double> data;
        std::vector<int> shape;

    public:
        Tensor(std::vector<double> data_, std::vector<int> shape_);
        double& operator()(std::vector<int> indices);
        void print();
};

#endif

#include "tensor.h"

Tensor::Tensor(std::vector<double> data_, std::vector<int> shape_) {
    data = data_;
    shape = shape_;
}

double& Tensor::operator()(std::vector<int> indices) {
    int index = 0;
    int stride = 1;
    for (int i = 0; i < indices.size(); i++) {
        index += stride * indices[i];
        stride *= shape[i];
    }
    return data[index];
}

void Tensor::print() {
    std::cout << "Tensor shape: (";
    for (int i = 0; i < shape.size(); i++) {
        std::cout << shape[i];
        if (i != shape.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;

    std::cout << "Tensor data:" << std::endl;
    for (int i = 0; i < shape[0]; i++) {
        if (shape.size() == 1) {
            std::cout << data[i] << " ";
        } else {
            std::vector<int> indices(shape.size() - 1);
            for (int j = 0; j < shape.size() - 1; j++) {
                indices[j] = 0;
            }
            indices.insert(indices.begin(), i);
            Tensor sub_tensor(data, shape);
            sub_tensor.shape.erase(sub_tensor.shape.begin());
            sub_tensor.print();
        }
    }
    std::cout << std::endl;
}

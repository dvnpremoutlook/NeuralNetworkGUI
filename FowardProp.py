from keras.models import load_model
import tensorflow as tf
from pprint import pprint
import numpy as np
import json
minst = tf.keras.datasets.mnist
# (train_images, train_lables),(main_1,main_1lables) = minst.load_data()
# main_1 = main_1.reshape(10000, 2, 1, 1)
# main_1 = main_1 / 255.0
main_1 = np.array([[[0], [0]], [[0], [1]], [[1], [0]], [[1], [1]]], dtype=np.float32).reshape(4, 2, 1, 1) 
main_1lables = np.array([[0], [1], [1], [0]], dtype=np.float32)
main_1 = main_1.reshape(4, 2, 1, 1)

model = load_model('Final (1).h5')

class Tensor:
    def __init__(self, data, shape):
        self.data = data
        self.shape = shape
        self.tensor = []
    def convertData(self):
        if not self.shape:
            print([])
            return 0
        def createtensor(depth, num):
            if depth[1:]:
                return [createtensor(depth[1:], num) for _ in range(depth[0])]
            else:
                return depth[0] * [num]
        self.tensor = createtensor((self.shape), 0)
        def insertdata(array1):
            for q in array1:
                if type(q) == list:
                    insertdata(q)
                else:
                    array1.pop(0)
                    if self.data:
                        array1.append(self.data[0])
                        self.data.pop(0)
                    else:
                        array1.append(0)
            return array1
        return insertdata(self.tensor)

class Denses:
    def __init__(self,x,w,b):
        self.x = x
        self.w = w
        self.b = b
        self.DesenLayer = []
    def forward(self,inputs,activation):
        size = inputs
        temp = []
        all_layer_weights = []
        biases = []
        temp2xodes = []
        def Flatten(array1):
          for q in array1:
            if type(q) == list:
              Flatten(q)
            else:
              temp2xodes.append(q)
          return temp2xodes
        DesenLayer = []
        Underneath_DenseLayer = []
        temp_Dense = []
        def dense(x,w,b):
          def relu(x):
            if x > 0:
              return x
            else:
              return 0
          for a in range(size):
            temp = 0
            ValuesOfaNeuron = []
            for ew in range(len(w)):
              temp = temp + ((x[ew])*w[ew][a])
              ValuesOfaNeuron.append(temp)
            temp = temp + self.b[a]
            if activation == 2:
                temp = relu(temp)
            DesenLayer.append(temp)
            Underneath_DenseLayer.append(ValuesOfaNeuron)
        input = Flatten(self.x)
        weight = self.w
        bias = self.b
        dense(input,weight,bias)
        return DesenLayer,Underneath_DenseLayer




all_weights = []
all_biases = []

for x in range(3):
  r = model.layers[x+1].get_weights()[0]
  u = model.layers[x+1].get_weights()[1]
  all_biases.append(u)
  all_weights.append(r)
weight_shape = [3]
t1 = Tensor(all_weights, weight_shape)
all_weights = t1.convertData()
biases_shape = [3]
t3 = Tensor(all_biases, biases_shape)
all_biases = t3.convertData()



def run_foward_prop_on_image(num):
  z = ((np.array(main_1[num])).reshape(2)).tolist()
  shape0 = [4]
  t2 = Tensor(z, shape0)
  input_iamge = t2.convertData()
  dense0 = Denses(input_iamge,all_weights[0],all_biases[0])
  layer,layerInfo = dense0.forward(10,2)
  dense1 = Denses(layer,all_weights[1],all_biases[1])
  layer2,layerInfo2 = dense1.forward(10,2)
  dense2 = Denses(layer2,all_weights[2],all_biases[2])
  layer3,layerInfo3 = dense2.forward(10,6)
  softmax = np.exp(layer3)/sum(np.exp(layer3))
  softmax = np.array(softmax).argmax()
  layerInfoX = np.array([layerInfo[0],layerInfo[1],layerInfo[2],layerInfo[3],layerInfo[4],layerInfo[5],layerInfo[6],layerInfo[7],layerInfo[8],layerInfo[9]])
  layerInfoX2 = np.array([layerInfo2[0],layerInfo2[1],layerInfo2[2],layerInfo2[3],layerInfo2[4],layerInfo2[5],layerInfo2[6],layerInfo2[7],layerInfo2[8],layerInfo2[9]])
  layerInfoX3 = np.array([layerInfo3[0],layerInfo3[1],layerInfo3[2],layerInfo3[3],layerInfo3[4],layerInfo3[5],layerInfo3[6],layerInfo3[7],layerInfo3[8],layerInfo3[9]])
  if softmax == main_1lables[num]:
    return 1,np.array([layer,layer2,layer3]),layerInfoX,layerInfoX2,layerInfoX3
  else:
    return 0,np.array([layer,layer2,layer3]),layerInfoX,layerInfoX2,layerInfoX3



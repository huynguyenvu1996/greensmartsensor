#https://machinelearningmastery.com/keras-functional-api-deep-learning/
#https://stackoverflow.com/questions/17718271/python-count-for-multidimensional-arrays-list-of-lists
#https://deeplearning.vn/2018/01/10/nhan-dien-chu-viet-bang-keras-phan-1/
#https://stackoverflow.com/questions/44747343/keras-input-explanation-input-shape-units-batch-size-dim-etc

import csv
import numpy as np
import operator
from collections import Counter

list = []
with open('D:\Project\TimeSeriesDataForecast\dump_data\stores_sales\\135.csv', encoding='UTF-8', newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        list.append(row)

unique = []
tmp = []

for item in list:
    tmp.append(item[2])
unique = np.unique(tmp)

print(unique)

for item in unique:
    s = sum(x.count(item) for x in list)
    if(s > 350):
        print(str(item) + ":" + str(s))

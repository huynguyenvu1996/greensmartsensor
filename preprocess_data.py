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

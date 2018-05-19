https://medium.com/@jamsawamsa/running-a-google-cloud-gpu-for-fast-ai-for-free-5f89c707bae6
    http://omg-it.works/how-to-grab-video-frames-directly-from-qcamera/#acceptLicense
        https://stackoverflow.com/questions/43529229/dlib-static-lib-error-in-debug-mode
            http://eitguide.net/su-dung-videocapture-trong-opencv/
                https://stackoverflow.com/questions/37462330/show-webcam-stream-from-opencv-with-qt
import numpy as np
import glob
import os
import csv

WD = r"D:\Project\TimeSeriesDataForecast\dump_data\stores_sales"


class PreProcesssData:
    def __init__(self):
        pass

    def get_list_file_stores(self, wd, ext="*.csv", wf=False, path_file=os.getcwd(), file_name="list_file_stores_tmp"):
        stores = glob.glob(os.path.join(wd, ext))
        if (wf):
            with open(path_file + file_name + ".csv", 'w',
                      encoding='UTF-8', newline='') as outfile:
                wr = csv.writer(outfile, delimiter=',')
                wr.writerows(stores)
        print("==>Done get list file stores")
        return stores

    def get_list_category(self, list_file_stores, wd=os.getcwd(), min_day_numbers=350):
        list_category_all_stores = []
        for file in list_file_stores:
            list_category_sales = []
            list_id_category_tmp = []
            list_category = []
            list_id_category = None
            print("==>Processing on file: \"" + file + "\" ")
            with open(file, encoding='UTF-8',
                      newline='') as csvfile:
                spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
                for row in spamreader:
                    list_category_sales.append(row)

            for item in list_category_sales:
                list_id_category_tmp.append(item[2])

            list_id_category = np.unique(list_id_category_tmp)

            for item in list_id_category:
                s = sum(x.count(item) for x in list_category_sales)
                if (s > min_day_numbers):
                    list_category.append({'category_id': item, 'number_day': s})

            print("-------------------------")
            print('==>Store: ' + str(list_category_sales[0][0]))
            if len(list_category) > 0:
                with open(wd + str(list_category_sales[0][0]) + ".csv", 'w',
                          encoding='UTF-8', newline='') as outfile:
                    wr = csv.writer(outfile, delimiter=',')
                    for item in list_category:
                        wr.writerow([str(item['category_id']), str(item['number_day'])])
                        print('Cat:' + str(item['category_id']) + "- Number day: " + str(item['number_day']))
            print("-------------------------")

        #     list_category_all_stores.append({'store_id': list_category_sales[0][0], 'list_category': list_category})
        #
        # for item in list_category_all_stores:
        #     print("-------------------------")
        #     print('==>Store: ' + str(item['store_id']))
        #     with open(wd + str(item['store_id']) + ".csv", 'w',
        #               encoding='UTF-8', newline='') as outfile:
        #         wr = csv.writer(outfile, delimiter=',')
        #         for cat in item['list_category']:
        #             wr.writerow([str(cat['category_id']), str(cat['number_day'])])
        #             print('Cat:' + str(cat['category_id']) + "- Number day: " + str(cat['number_day']))
        #
        #     print("-------------------------")


prdata = PreProcesssData()
list_file_stores = prdata.get_list_file_stores(WD)
prdata.get_list_category(list_file_stores=list_file_stores,
                         wd=r"D:\\Project\\TimeSeriesDataForecast\\dump_data\\category\\",
                         min_day_numbers=350)

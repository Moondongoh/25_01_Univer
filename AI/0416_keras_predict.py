import FinanceDataReader as fdr
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

samsung = fdr.DataReader("005930", "2016")
print(samsung)

openValues = samsung[["Open"]]

from sklearn.preprocessing import MinMaxScaler

scaler = MinMaxScaler(
    feature_range=(0, 1)
)  # MinMaxScaler를 사용하여 데이터를 0과 1 사이로 변환 정규화하는 작업
scaled = scaler.fit_transform(openValues)

TEST_SIZE = 200
train_data = scaled[:-TEST_SIZE]
test_data = scaled[-TEST_SIZE:]


def make_sample(data, window):
    train = []
    target = []
    for i in range(len(data) - window):
        train.append(data[i : i + window])
        target.append(data[i + window])
    return np.array(train), np.array(target)


X_train, y_train = make_sample(train_data, 30)

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import keras
from zipfile import ZipFile

# ====================
# 1. 데이터 로딩
# ====================
uri = "https://storage.googleapis.com/tensorflow/tf-keras-datasets/jena_climate_2009_2016.csv.zip"
zip_path = keras.utils.get_file(origin=uri, fname="jena_climate_2009_2016.csv.zip")
ZipFile(zip_path).extractall()
df = pd.read_csv("jena_climate_2009_2016.csv")

date_time_key = "Date Time"
feature_keys = [
    "p (mbar)",
    "T (degC)",
    "VPmax (mbar)",
    "VPdef (mbar)",
    "sh (g/kg)",
    "rho (g/m**3)",
    "wv (m/s)",
]
target_key = "T (degC)"

# ====================
# 2. 파라미터 설정
# ====================
split_fraction = 0.715
step = 6
past = 720
future = 72
batch_size = 256
epochs = 10
learning_rate = 0.001

train_split = int(split_fraction * len(df))


# ====================
# 3. 정규화
# ====================
def normalize(data, train_idx):
    mean = data[:train_idx].mean(axis=0)
    std = data[:train_idx].std(axis=0)
    return (data - mean) / std


features = df[feature_keys]
features.index = df[date_time_key]
features = normalize(features.values, train_split)
features = pd.DataFrame(features)

# ====================
# 4. 데이터셋 구성
# ====================
train_data = features.iloc[:train_split]
val_data = features.iloc[train_split:]

sequence_length = int(past / step)
start = past + future
end = start + train_split
x_train = train_data.values
y_train = features.iloc[start:end][[1]].values

x_val = val_data.iloc[: len(val_data) - past - future].values
y_val = features.iloc[train_split + past + future :][[1]].values

dataset_train = keras.preprocessing.timeseries_dataset_from_array(
    x_train,
    y_train,
    sequence_length=sequence_length,
    sampling_rate=step,
    batch_size=batch_size,
)

dataset_val = keras.preprocessing.timeseries_dataset_from_array(
    x_val,
    y_val,
    sequence_length=sequence_length,
    sampling_rate=step,
    batch_size=batch_size,
)

# ====================
# 5. 모델 정의
# ====================
inputs = keras.Input(shape=(sequence_length, len(feature_keys)))
x = keras.layers.LSTM(32)(inputs)
outputs = keras.layers.Dense(1)(x)

model = keras.Model(inputs, outputs)
model.compile(optimizer=keras.optimizers.Adam(learning_rate), loss="mse")
model.summary()

# ====================
# 6. 콜백 설정
# ====================
callbacks = [
    keras.callbacks.EarlyStopping(
        monitor="val_loss", patience=5, restore_best_weights=True
    ),
    keras.callbacks.ModelCheckpoint(
        filepath="model_checkpoint.weights.h5",
        save_best_only=True,
        monitor="val_loss",
        save_weights_only=True,
        verbose=1,
    ),
]

# ====================
# 7. 모델 학습
# ====================
history = model.fit(
    dataset_train, validation_data=dataset_val, epochs=epochs, callbacks=callbacks
)


# ====================
# 8. 학습 시각화
# ====================
def plot_loss(hist, title="Loss"):
    plt.plot(hist.history["loss"], label="Train")
    plt.plot(hist.history["val_loss"], label="Val")
    plt.title(title)
    plt.xlabel("Epoch")
    plt.ylabel("Loss")
    plt.legend()
    plt.grid(True)
    plt.show()


plot_loss(history)


# ====================
# 9. 예측 시각화
# ====================
def show_plot(plot_data, delta, title):
    labels = ["History", "True Future", "Prediction"]
    markers = [".-", "rx", "go"]
    time_steps = list(range(-plot_data[0].shape[0], 0))
    future = delta if delta else 0

    plt.title(title)
    for i, data in enumerate(plot_data):
        if i:
            plt.plot(future, data, markers[i], label=labels[i])
        else:
            plt.plot(time_steps, data.flatten(), markers[i], label=labels[i])
    plt.legend()
    plt.xlim([time_steps[0], future + 5])
    plt.xlabel("Time Step")
    plt.show()


for x, y in dataset_val.take(5):
    prediction = model.predict(x)[0]
    show_plot(
        [x[0][:, 1].numpy(), y[0].numpy(), prediction],
        delta=12,
        title="Single Step Prediction",
    )

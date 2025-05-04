import numpy as np
import matplotlib.pyplot as plt
import keras
from keras import layers, models, callbacks


# 데이터 로드 + 전처리
def load_fordA():
    def read_ucr(url):
        data = np.loadtxt(url, delimiter="\t")
        x, y = data[:, 1:], data[:, 0]
        y = (y == 1).astype(int)  # -1 → 0, 1 → 1
        return x, y

    base_url = "https://raw.githubusercontent.com/hfawaz/cd-diagram/master/FordA/"
    x_train, y_train = read_ucr(base_url + "FordA_TRAIN.tsv")
    x_test, y_test = read_ucr(base_url + "FordA_TEST.tsv")

    # reshape for Conv1D
    x_train = x_train[..., np.newaxis]
    x_test = x_test[..., np.newaxis]

    # shuffle train set
    idx = np.random.permutation(len(x_train))
    return x_train[idx], y_train[idx], x_test, y_test


x_train, y_train, x_test, y_test = load_fordA()


# 모델 정의
def build_model(input_shape):
    inputs = layers.Input(shape=input_shape)
    x = inputs
    for _ in range(3):
        x = layers.Conv1D(64, 3, padding="same")(x)
        x = layers.BatchNormalization()(x)
        x = layers.ReLU()(x)
    x = layers.GlobalAveragePooling1D()(x)
    outputs = layers.Dense(1, activation="sigmoid")(x)  # 이진 분류
    return models.Model(inputs, outputs)


model = build_model(x_train.shape[1:])
model.compile(optimizer="adam", loss="binary_crossentropy", metrics=["accuracy"])

# 콜백 설정
cb = [
    callbacks.ModelCheckpoint(
        "best_model.keras", save_best_only=True, monitor="val_loss"
    ),
    callbacks.ReduceLROnPlateau(
        monitor="val_loss", factor=0.5, patience=10, min_lr=1e-4
    ),
    callbacks.EarlyStopping(
        monitor="val_loss", patience=20, verbose=1, restore_best_weights=True
    ),
]

# 학습
history = model.fit(
    x_train,
    y_train,
    epochs=100,
    batch_size=32,
    validation_split=0.2,
    callbacks=cb,
    verbose=1,
)

# 평가
test_loss, test_acc = model.evaluate(x_test, y_test, verbose=0)
print(f"\n✅ Test Accuracy: {test_acc:.4f}")
print(f"❌ Test Loss: {test_loss:.4f}")

# 학습 시각화 (선택)
plt.plot(history.history["accuracy"], label="Train Acc")
plt.plot(history.history["val_accuracy"], label="Val Acc")
plt.title("Accuracy over epochs")
plt.xlabel("Epoch")
plt.ylabel("Accuracy")
plt.legend()
plt.grid(True)
plt.show()

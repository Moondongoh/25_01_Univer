# import matplotlib.pyplot as plt
# import tensorflow as tf

# (train_images, train_labels), (test_images, test_labels)	= tf.keras.datasets.mnist.load_data()

# print(train_images.shape)

# print(train_labels)
# print(test_images.shape)

# plt.imshow(train_images[0], cmap="Greys")

# model = tf.keras.models.Sequential()

# model.add(tf.keras.layers.Dense(512, activation='relu', input_shape=(784,)))
# model.add(tf.keras.layers.Dense(10, activation='sigmoid'))

# model.compile(optimizer='rmsprop',
#                 loss='mse',
#                 metrics=['accuracy'])            

# train_images = train_images.reshape((60000, 784))
# train_images = train_images.astype('float32') / 255.0

# test_images = test_images.reshape((10000, 784))
# test_images = test_images.astype('float32') / 255.0

# train_labels = tf.keras.utils.to_categorical(train_labels)
# test_labels = tf.keras.utils.to_categorical(test_labels)

# model.fit(train_images, train_labels, epochs=5, batch_size=128)

# test_loss, test_acc = model.evaluate(test_images, test_labels)
# print('테스트 정확도:', test_acc)

# history = model.fit(train_images, train_labels, epochs=5, batch_size=128)
# loss = history.history['loss']
# acc = history.history['accuracy']
# epochs = range(1, len(loss)+1)

# plt.plot(epochs, loss, 'b', label='Training Loss')
# plt.plot(epochs, acc, 'r', label='Accuracy')
# plt.xlabel('epochs')
# plt.ylabel('loss/acc')
# plt.show()

# import cv2 as cv

# image = cv.imread('C:/Users/Moon/Desktop/소스코드/chap07/test.png', cv.IMREAD_GRAYSCALE)
# image = cv.resize(image, (28, 28))
# image = image.astype('float32')
# image = image.reshape(1, 784)
# image = 255-image
# image /= 255.0

# plt.imshow(image.reshape(28, 28),cmap='Greys')
# plt.show()

# pred = model.predict(image.reshape(1, 784), batch_size=1)
# print("추정된 숫자=", pred.argmax())

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import cv2

# MNIST 데이터셋 로드
(train_images, train_labels), (test_images, test_labels) = tf.keras.datasets.mnist.load_data()

# 데이터 전처리
train_images = train_images.reshape((60000, 784)).astype('float32') / 255.0
test_images = test_images.reshape((10000, 784)).astype('float32') / 255.0

train_labels = tf.keras.utils.to_categorical(train_labels)
test_labels = tf.keras.utils.to_categorical(test_labels)

# 모델 정의
model = tf.keras.models.Sequential([
    tf.keras.layers.Dense(512, activation='relu', input_shape=(784,)),
    tf.keras.layers.Dense(10, activation='softmax')  # ✅ softmax 적용
])

# 모델 컴파일 (손실 함수 변경)
model.compile(optimizer='rmsprop',
              loss='categorical_crossentropy',  # ✅ 분류 문제에 적합한 손실 함수
              metrics=['accuracy'])

# 모델 학습
history = model.fit(train_images, train_labels, epochs=10, batch_size=128)

# 모델 평가
test_loss, test_acc = model.evaluate(test_images, test_labels)
print('테스트 정확도:', test_acc)

# 학습 그래프 출력
loss = history.history['loss']
acc = history.history['accuracy']
epochs = range(1, len(loss) + 1)

plt.plot(epochs, loss, 'b', label='Training Loss')
plt.plot(epochs, acc, 'r', label='Accuracy')
plt.xlabel('Epochs')
plt.ylabel('Loss/Accuracy')
plt.legend()
plt.show()

# 이미지 예측
image_path = 'C:/Users/Moon/Desktop/test.jpg'
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

if image is None:
    print("⚠️ 이미지 로드 실패! 경로를 확인하세요.")
else:
    image = cv2.resize(image, (28, 28))
    image = image.astype('float32') / 255.0  # 0~1 정규화
    image = image.reshape(1, 784)

    plt.imshow(image.reshape(28, 28), cmap='Greys')
    plt.show()

    pred = model.predict(image)
    print("추정된 숫자 =", pred.argmax())  # 가장 높은 확률의 숫자 출력

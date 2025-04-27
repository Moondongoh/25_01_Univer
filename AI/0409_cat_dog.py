#==================================================================
# 1번 필터 16개 커널 3*3짜리 은닉층 추가 // catdog05040901_1.h5
# 2번 배치 사이즈 20 > 32로 바꾸기 //
# 3번 에포크 5 > 10으로 바꾸기 //
# 4번 커널사이즈를 바꿔? //
# 5번 데이터 셋 개수 차이를 두고 단, 파라미터 값은 유지하고 개수만 차이를 두기 //
#==================================================================


#from google.colab import drive
#drive.mount('/content/drive')
#==================================================================
import numpy as np
import os
import tensorflow as tf
#from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras import models, layers

# GPU 사용 가능한지 확인
gpus = tf.config.list_physical_devices('GPU')
if gpus:
    print("✅ GPU 사용 중:", gpus)
else:
    print("❌ GPU를 사용할 수 없습니다. CPU 사용 중.")

model = tf.keras.models.Sequential()
# model.add(layers.Conv2D(16,(3,3), activation='relu', input_shape=(128,128,3))) # 1번 필터 16개 커널 3*3짜리 은닉층 추가
# model.add(layers.MaxPooling2D(2,2))
model.add(layers.Conv2D(32,(3,3), activation='relu', input_shape=(128,128,3)))
model.add(layers.MaxPooling2D(2,2))
model.add(layers.Conv2D(64,(3,3), activation='relu'))
model.add(layers.MaxPooling2D(2,2))
model.add(layers.Flatten())
model.add(layers.Dense(units=512, activation='relu'))
model.add(layers.Dense(units=1, activation='sigmoid'))
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
#==================================================================
#base_dir = 's/content/drive/MyDrive/PetImage'
base_dir = 'D:/Git_Hub/dogs-vs-cats'
#os.listdir(base_dir)
train_dir = os.path.join(base_dir, 'train')
test_dir = os.path.join(base_dir, 'test')

train_datagen = tf.keras.preprocessing.image.ImageDataGenerator(
    validation_split=0.3,   
    rescale=1.0/255.
)

validation_datagen = tf.keras.preprocessing.image.ImageDataGenerator(
    validation_split=0.3,
    rescale=1.0/255.
)

train_generator = train_datagen.flow_from_directory(train_dir,
                                                  batch_size=20, # 2번 배치 사이즈 20 > 32로 바꾸기
                                                  subset='training',
                                                  seed=1234,
                                                  class_mode='binary',
                                                  target_size=(128, 128))

validation_generator =  validation_datagen.flow_from_directory(train_dir,
                                                       batch_size=20, # 2번 배치 사이즈 20 > 32로 바꾸기
                                                       subset='validation',
                                                       seed=1234,
                                                       class_mode='binary',
                                                       target_size=(128, 128)) 

history = model.fit(train_generator,
                    validation_data=validation_generator,
                    epochs=5, # 3번 에포크 5 > 10으로 바꾸기
                    verbose=1) # 0: 훈련과정 출력 안함, 1: 훈련과정 출력함)
#==================================================================
model.save(base_dir + '/catdog05040901_3.h5')
#==================================================================
import matplotlib.pyplot as plt
plt.plot(history.history['accuracy'])
plt.plot(history.history['val_accuracy'])
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.legend(['Train', 'Validation'], loc='upper left')
plt.show()
#==================================================================
test_datagen = tf.keras.preprocessing.image.ImageDataGenerator(
    rescale=1.0/255.
)

test_generator = test_datagen.flow_from_directory(test_dir,
                                                  class_mode='binary',
                                                  target_size=(128, 128))
test_loss, test_acc = model.evaluate(test_generator)
print('\n테스트 정확도:', test_acc)
#==================================================================

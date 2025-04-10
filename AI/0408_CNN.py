import os
import tensorflow as tf
from tensorflow.keras import models, layers

#base_dir = '/content/drive/MyDrive/PetImages'
base_dir = 'D:/Git_Hub/dogs-vs-cats'
test_dir = os.path.join(base_dir, 'test')
model = tf.keras.models.load_model(base_dir + '/catdog05040901.h5')

test_datagen = tf.keras.preprocessing.image.ImageDataGenerator(
    rescale=1.0/255.
)

test_generator = test_datagen.flow_from_directory(test_dir,
                                                  class_mode='binary',
                                                  target_size=(128, 128))
test_loss, test_acc = model.evaluate(test_generator)
print('\n테스트 정확도:', test_acc)
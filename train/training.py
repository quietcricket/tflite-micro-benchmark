# TensorFlow and tf.keras
import tensorflow as tf
from tensorflow import keras

import binascii
import logging

# Check the version of tensorflow (should be 2.8.0)
print(tf.__version__)

# To silent verbose
tf.autograph.set_verbosity(0)
logging.getLogger("tensorflow").setLevel(logging.ERROR)


# MNIST download
mnist = keras.datasets.mnist
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
# 60,000 training data and 10,000 test data of 28x28 pixel images
print("train_images shape", train_images.shape)
print("train_labels shape", train_labels.shape)
print("test_images shape", test_images.shape)
print("test_labels shape", test_labels.shape)


# Normalize the input image so that each pixel value is between 0 to 1.
train_images = train_images / 255.0
test_images = test_images / 255.0
train_labels = tf.keras.utils.to_categorical(train_labels, 10)
test_labels = tf.keras.utils.to_categorical(test_labels, 10)
print('Datasets are normalized')


model = keras.Sequential([
    keras.layers.InputLayer(input_shape=(28, 28)),
    keras.layers.Reshape(target_shape=(28, 28, 1)),
    keras.layers.Conv2D(filters=12, kernel_size=(3, 3), activation=tf.nn.relu),
    keras.layers.MaxPooling2D(pool_size=(2, 2)),
    keras.layers.Flatten(),
    keras.layers.Dense(10),
    keras.layers.Activation(tf.nn.softmax)
])


model.compile(loss="mse", optimizer="adam", metrics=["accuracy"])


# Output the summary of the model
model.summary()


# Training the model
model.fit(x=train_images, y=train_labels,
          batch_size=128, epochs=2, verbose=1, validation_split=0.1)

model.save('model.h5')
# Evaluate the model using all images in the test dataset.
test_loss, test_acc = model.evaluate(x=test_images, y=test_labels, verbose=1)
print('Accuracy = %f' % test_acc)


# Convert the Keras model to a quantized TFLite model.
converter = tf.lite.TFLiteConverter.from_keras_model(model)


def representative_dataset_gen():
    for i in range(100):
        input_image = tf.cast(test_images[i], tf.float32)
        input_image = tf.reshape(input_image, [1, 28, 28])
        yield ([input_image])


converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.representative_dataset = representative_dataset_gen
converter.inference_input_type = tf.uint8
converter.inference_output_type = tf.uint8
tflite_model = converter.convert()


# Show the quantized model size in KBs.
tflite_model_size = len(tflite_model) / 1024
print('Quantized model size = %dKBs.' % tflite_model_size)
# Save the model to disk
open('qmodel.tflite', "wb").write(tflite_model)


# Output the quantized tflite model to a c-style header
def convert_to_c_array(bytes):
    hexstr = binascii.hexlify(bytes).decode("UTF-8")
    hexstr = hexstr.upper()
    array = ["0x" + hexstr[i:i + 2] for i in range(0, len(hexstr), 2)]
    array = [array[i:i + 10] for i in range(0, len(array), 10)]
    return ",\n  ".join([", ".join(e) for e in array])


tflite_binary = open('qmodel.tflite', 'rb').read()
ascii_bytes = convert_to_c_array(tflite_binary)
header_file = "const unsigned char model_tflite[] = {\n  " + ascii_bytes + \
    "\n};\nunsigned int model_tflite_len = " + str(len(tflite_binary)) + ";"
with open("../src/model.h", "w") as f:
    f.write(header_file)

import tensorflow as tf

model = tf.keras.models.load_model("modelo_edge.keras")

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]

tflite_model = converter.convert()

with open("modelo.tflite", "wb") as f:
    f.write(tflite_model)

print("Modelo TFLite criado")

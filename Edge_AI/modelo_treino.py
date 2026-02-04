import tensorflow as tf
import numpy as np

# Dados de exemplo - APENAS TEMPERATURA
# X = [temperatura]
X = np.array([
    [22],
    [24],
    [26],
    [30],
    [32],
    [35],
], dtype=np.float32)

y = np.array([0, 0, 0, 1, 1, 1], dtype=np.float32)

# Normalização
X[:, 0] = X[:, 0] / 50.0    # temperatura

model = tf.keras.Sequential([
    tf.keras.layers.Dense(8, activation='relu', input_shape=(1,)),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

model.compile(
    optimizer='adam',
    loss='binary_crossentropy',
    metrics=['accuracy']
)

model.fit(X, y, epochs=200, verbose=0)
print("Modelo treinado")


model.save("modelo_edge.keras")
print("Modelo guardado em 'modelo_edge.keras'")
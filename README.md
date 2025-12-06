🚗 Robot Autónomo con Evasión Inteligente
Arduino + Sensores IR + Ultrasonido + L9110S

Este proyecto implementa un sistema de navegación autónoma para un minirobot basado en Arduino, incorporando evasión inteligente, prevención de oscilaciones y un modo opcional de aprendizaje automático ligero (ε-greedy).
El objetivo es lograr un comportamiento más eficiente y “cognitivo”, evitando ciclos repetitivos y tomando mejores decisiones en esquinas o zonas cerradas.

✨ Características Principales
🧠 1. Evasión Inteligente (“Mirar y Elegir”)

El robot utiliza el sensor ultrasónico para “asomarse” hacia la derecha e izquierda mediante breves pivotes.
Tras medir en ambos lados:
Compara las distancias detectadas,
Elige el lado con mayor espacio,
Realiza un giro preciso para salir de la zona peligrosa.
Esto reduce giros innecesarios y mejora la fluidez en esquinas.

🔄 2. Anti-Oscilación / Anti-Atasco

Si el robot necesita evadir varias veces en pocos segundos, significa que está atrapado o indeciso.
En ese caso, el sistema detecta el patrón y ejecuta un:

➡️ U-turn corto (giro amplio y decidido)

Esto le permite abandonar rápidamente el área problemática.

🤖 3. (Opcional) Aprendizaje Liviano — ε-greedy

Cuando se activa con:

#define USE_LEARNING 1


El robot comienza a aprender:
Identifica patrones de sensores (IR y ultrasonido).
Registra qué acción funcionó mejor en el pasado.
Probabilísticamente explora nuevas acciones (ε-greedy).
Mejora con el tiempo para evitar repetir errores.
Este módulo es muy ligero y compatible con microcontroladores de recursos limitados.

🧩 Configuración de Pines
🔌 Motores (Driver L9110S)
Señal	Pin Arduino
A_1B	5
A_1A	6
B_1B	9
B_1A	10
👁️ Sensores IR Laterales
Sensor	Pin	Estado
rightIR	7	0 = obstáculo, 1 = libre
leftIR	8	0 = obstáculo, 1 = libre
📡 Sensor Ultrasónico HC-SR04
Señal	Pin
trigPin	3
echoPin	4

🚀 Cómo Usarlo
1️⃣ Subir el Sketch
Carga el código en tu Arduino tal como está.
Cuando el robot arranca:
Se moverá en línea recta mientras no haya obstáculos.
Utilizará los IR para evitar colisiones laterales.
Usará el ultrasonido para decidir rutas en esquinas.

2️⃣ Comportamiento Esperado

✔️ En zonas abiertas:
El robot avanza recto de forma estable.

✔️ En esquinas:
Realiza pivotes cortos, analiza ambos lados y elige automáticamente la mejor ruta.

✔️ En caso de atasco:
Si detecta múltiples evasiones consecutivas, ejecuta un U-turn corto para liberarse.

✔️ Con aprendizaje activado:
El robot recuerda maniobras exitosas y optimiza su comportamiento con el tiempo.

🛠️ Ajustes Rápidos
🔹 1. Aumentar prevención

Ajusta la distancia de seguridad:

D_STOP = 30–35;   // recomendado para entornos estrechos

🔹 2. Movimiento más suave

Reduce aceleraciones bruscas:
Disminuir SPEED_BACK o SPEED_TURN

🔹 3. Salir mejor del atasco

Puedes mejorar el giro de escape incrementando:

EVADE_MAX   // aumentar a 4
delay en hardEscape()  // extender giro más amplio

📦 Dependencias

Arduino UNO / SunFounder R3
Driver de motores L9110S
Motores TT
Sensores IR (obstáculos laterales)
Sensor ultrasónico HC-SR04

🧭 Futuras Mejoras (Opcionales)

Mapa de calor de recorridos
Interfaz Bluetooth para control manual
Servo para “radar” ultrasónico
Algoritmos más avanzados como Follow-the-Gap o Q-learning real


📜 Licencia

Puedes usar, modificar y distribuir este software libremente mientras mantengas los créditos del autor.

🤝 Contribuciones

¡Contribuciones, mejoras o PRs son bienvenidos!
Si deseas integrar visión artificial o TinyML, estaré encantado de ayudarte a extender el proyecto.

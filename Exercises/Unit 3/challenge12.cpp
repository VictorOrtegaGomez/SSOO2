#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <chrono>

/*
pasar el semáforo en la petición para desbloquearlo luego desde el sistema de pago
las peticiones llegan a una cola y con su variable de condición comprueba si la cola está vacía. cuando se envie la solicitud a la cola se hará el notify_one
crear estructura que encapsule la peticion (id hilo, saldo, semaforo)
para pasar el semáforo usar punteros (mirar punteros inteligentes en c++)
*/
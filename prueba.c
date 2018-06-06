/*--------------------------------Inicio código-------------------------------------*/
#include <QTRSensors.h> //Biblioteca para la lectura y calibración de los sensores de contraste

//Declaración de constantes
#define NUM_SENSORS	5 // Número de sensores
#define NUM_SAMPLES_PER_SENSOR 4	// Número de muestras por sensor
#define NO_EMITTER_PIN	2 // Controlador de emisores LED's infrarrojos

//Los sensores reflectivos para detectar contraste de encuentran conectados en las entradas
//Analógicas A0 a A4
QTRSensorsAnalog	qtra((unsigned	char[])	{0,	1,	2,	3,	4},	NUM_SENSORS,
NUM_SAMPLES_PER_SENSOR, NO_EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS]; //Vector para almacenar señal de sensores

unsigned int sensors[5]; // Matriz para armazenar valores dos sensores

int MotorA = 11; //Alias para Motor B
int MotorB = 8; //Alias para Motor A

//Declaración de variables
int total = 0;
float average = 0;
int index = 0;
int last_proportional;
int integral;

void setup(){

//Configuración de entradas y salidas digitales
delay(500);
pinMode(13, OUTPUT);

//Se prende el indicador LED conectado en el pin 13, para indicar que se
//inicia la calibración
digitalWrite(13, HIGH);
//Rotación del robot para calibración
analogWrite(MotorA, 90);
analogWrite(MotorB, 0);

for (int i = 0; i < 200; i++)	//Calibración durante 5 segundos
{
qtra.calibrate();	// Se hace las lecturas para obtener valores de máximos y mínimos
}
digitalWrite(13, LOW);	//Se apaga el LED indicando que se completo la programación

analogWrite(MotorA, 0);
analogWrite(MotorB, 0);
delay(1000);
 

}

void loop(){
while(1){
//Se obtiene el valor de cada uno de los sensores mediante la función qtra.readline();
unsigned int position = qtra.readLine(sensorValues);

//Determinación del término proporcional
int proportional = ((int)position) - 2000;

//Determinación del término derivativo
int derivative = proportional - last_proportional;

//Determinación del término integral
integral += proportional;

//Se almacena el estado anterior para determinar el próxmio término derivativo
last_proportional = proportional;

//Cálculo para determinar la velocidad de cada uno de los motores
int power_difference = proportional/10 + integral/10000 + derivative*3/2;
 
const int max = 180;
if(power_difference > max)
power_difference = max;
if(power_difference < -max)
power_difference = -max;
if(power_difference < 0)
set_motors(max+power_difference, max);
else
set_motors(max, max-power_difference);
 
}
 
}

// Accionamiento dos motores
void set_motors(int left_speed, int right_speed){
if(right_speed >= 0 && left_speed >= 0){
analogWrite(MotorA, left_speed);
analogWrite(MotorB, right_speed);
}
if(right_speed >= 0 && left_speed < 0){
left_speed = -left_speed;
analogWrite(MotorA, left_speed);
analogWrite(MotorB, 0);
}
if(right_speed < 0 && left_speed >= 0){
right_speed = -right_speed;
analogWrite(MotorA, 0);
analogWrite(MotorB, right_speed);
}
}

/*--------------------------------Fin de código-------------------------------------*/


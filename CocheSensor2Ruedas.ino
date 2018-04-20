#include <QTRSensorsRC.h>

#define NUM_SENSORS   5
#define TIMEOUT       2500  // tiempo de espera para dar resultado en uS
#define QTR_NO_EMITTER_PIN  12    //pin led on  //#define EMITTER_PIN  QTR_NO_EMITTER_PIN

//------------ Pines conectados Arduino------------------
#define led1          13
#define led2          11 
#define mot_Iup       //
#define mot_Dup       //
#define mot_IDown     //
#define mot_DDown     //
#define sensores      6
#define boton_1       //  //pin para boton
#define pin_pwm_i     6
#define pin_pwm_d     0

//pines correspondientes en este orde  A4-A3-A2-A1-A0 IR1-IR5
QTRSensorsRC qtrrc((unsigned char[]) {18, 17, 16, 15, 14},NUM_SENSORS, TIMEOUT, QTR_NO_EMITTER_PIN);
 
//variables para almacenar valores de sensores y posicion
unsigned int sensorValues[NUM_SENSORS];
unsigned int position=0;
 
/// variables para el pid
int  derivativo=0, proporcional=0, integral=0; //parametros
int  salida_pwm=0, proporcional_pasado=0;
 
 
//_______AQUI CAMBIEREMOS LOS PARAMETROS DE NUESTRO ROBOT_________________
int Vmax=180; //variable para la velocidad, el maximo es 255
float Kp=0.18;
float Kd=2;
float Ki=0.01;  //constantes
//variables para el control del sensado
int linea = 0;                //  0 para lineas negra, 1 para lineas blancas
int flanco_color = 0;      // aumenta o disminuye el valor del sensado
int en_linea = 700;         //valor al que considerara si el sensor esta en linea o no
int ruido = 150;          //valor al cual el valor del sensor es considerado como ruido
//________________________________________________________________________________
 
 void setup(){
	pinMode (mot_Iup, OUTPUT);
	pinMode (mot_Dup, OUTPUT);
	
	pinMode (mot_IDown, OUTPUT);
	pinMode (mot_DDown, OUTPUT);
	
	pinMode (pin_pwm_i, OUTPUT);
	pinMode (pin_pwm_d, OUTPUT);
	
	pinMode (led1, OUTPUT);
	pinMode (led2, OUTPUT);
	
	//calibracion del sistema--------------------
	for int i = 0; i < 50, i++){
		digitalWrite(led1, HIGH); //Led verde
		qtrrc.calibrate();
		digitalWrite led1, LOW);
	}	
	
	digitalWrite (led1, LOW); //cuando acabe la calibracion se apaga el led
	
	digitalWrite (led2 HIGH); //para indicar que el boton ya se puede pulsar 
	
	while(true){
		int x=digitalRead (boton_1); //leer y guardar el estado del boton
		if(x == 0){
			digitalWrite (led2, LOW); //si el led esta apagado indica que se ha presionado el boton
			digitalWrite (led1, HIGH); se enciende el led
			break;
		}
	}
	
 }
 
 void loop(){
	 
	 pid(linea, Vmax, kp, ki, flanco_color, en_linea, ruido);
	 
 }
 
 void pid (int linea, int Vmax, float kp, float ki,  float kd, int flanco_color){
	position = qtrrc.readLine(sensorValues, QTR_EMITTERS_ON, linea,flanco_color, en_linea, ruido); //0 para linea negra, 1 para linea blanca
	proporcional = (position) - 2000; // set point es 2000, asi obtenemos el error
	integral=integral + proporcional_pasado; //obteniendo integral
	derivativo = (proporcional - proporcional_pasado); //obteniedo el derivativo
	if (integral>1000) integral=1000; //limitamos la integral para no causar problemas
	if (integral<-1000) integral=-1000;
	salida_pwm =( proporcional * Kp ) + ( derivativo * Kd )+(integral*Ki); 
	if (  salida_pwm > Vmax )  salida_pwm = Vmax; //limitamos la salida de pwm
	if ( salida_pwm < -Vmax )  salida_pwm = -Vmax;
   
	if (salida_pwm < 0)
	{
	motores(Vmax+salida_pwm, Vmax);
	}
	if (salida_pwm >0)
	{
	motores(Vmax, Vmax-salida_pwm);
	}
 
	proporcional_pasado = proporcional;  
	}
 
void motores(int motor_IZQ, int motor_DER){
   
	  if ( motor_IZQ >= 0 )  //motor izquierdo
	 {
	  digitalWrite(motor_IZQ,HIGH); // con high avanza
	  analogWrite(pin_pwm_i,255-motor_IZQ); //se controla de manera inversa para mayor control
	 }
	 else
	 {
	  digitalWrite(motor_IZQ,LOW); //con low retrocede
	  motor_IZQ = motor_IZQ*(-1); //cambio de signo
	  analogWrite(pin_pwm_i,motor_IZQ); 
	 }
	 
	 
	  if ( motor_DER >= 0 ) //motor derecho
	 {
	  digitalWrite(motor_DER,HIGH);
	  analogWrite(pin_pwm_d,255-motor_DER);
	 }
	 else
	 {
	  digitalWrite(motor_DER,LOW);
	  motor_DER= motor_DER*(-1);
	  analogWrite(pin_pwm_d,motor_DER);
	 }

void freno_al_borde(int type, int flanco_comparación){
	if(type == 0){
		if (position <= 500){
			motores(-80,90=;
		}
	}
	
	while (true){
		qtrrc.read
	}
}	 
	 
 }
 

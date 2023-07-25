
// Essa aqui eu posso modificar


#include <virtuabotixRTC.h> // https://github.com/adafruit/RTClib
#include <Stepper.h>


//************************* Declaração de variaveis ***************************** //

int TIMEZONE = 3; // Your timezone, my location timezone is 1 (Central Europe)
int DST = 1; // Value = 1 if   summer time, in winter value = 0
virtuabotixRTC myRTC(6, 7, 8);
bool repeticao = false;
double grau_atual = 0;
bool permissao = false;



// Setup dos pinos:

int Pino_permissao = 5; // Pino que recebe o sinal de permissão para mudar a posição dos motores MANUALMENTE
int Pino_horario = 2; // Pino recebe o sinal para girar em sentido horario
int Pino_antihorario = 3; // Pino que recebe o sinal para girar em sentido antihorario
int Pino_gate = 4; // Pino que ativa o gate que permite corrente aos motores 
int Pino_horario_motor2 = 40; 
int Pino_antihorario_motor2 = 42;
int Horario; 
int Antihorario;
int Horario_motor2; 
int Antihorario_motor2;


//************************** Setup dos motores
// Setup dos motores 

// Define number of steps per rotation for the motor:
const int stepsPerRevolution = 2048;

// Wiring:
// Pin 9 to IN1 on the ULN2003 driver
// Pin 10 to IN2 on the ULN2003 driver
// Pin 11 to IN3 on the ULN2003 driver
// Pin 12 to IN4 on the ULN2003 driver
// Create stepper object called 'myStepper', note the pin order:

Stepper myStepper = Stepper(stepsPerRevolution, 9, 11, 10, 12);

// Wiring:
// Pin 22 to IN1 on the ULN2003 driver
// Pin 24 to IN2 on the ULN2003 driver
// Pin 26 to IN3 on the ULN2003 driver
// Pin 28 to IN4 on the ULN2003 driver
// Create stepper object called 'mySuujjtepper', note the pin order:

Stepper motor_2 = Stepper(stepsPerRevolution, 22, 26, 24, 28);



// ********************** SETUP DAS COORDENADAS  (bluetooth) **************** //

// Coordenadas da localização do usuario

float LONGITUDE = -43.213; // Your position to the Earth (West value - sign. Campinas-SP  == 47.21 west)
double LATITUDE = -22.88;  // Your position to the Earth (South value - sign. Campinas-SP == 22.88 south)


// insira os dados da estrela que quer encontrar (Right Ascension):

double hora_ra = 13;
double minutos_ra = 2 ;
double segundos_ra = 48;

// insira os dados da estrela que quer encontrar (Declination):

double star_declination = -11.083;

// ***************************************************************************// 


//************************ INICIO DO PROGRAMA ****************************** //

void setup()
{
  pinMode (Pino_horario, INPUT);
  pinMode (Pino_antihorario, INPUT);
  pinMode (Pino_horario_motor2, INPUT);
  pinMode (Pino_antihorario_motor2, INPUT);
  pinMode (Pino_gate, OUTPUT); 
  pinMode (Pino_permissao, INPUT);
  
  Serial.begin(9600);

  // Set the speed to 5 rpm:  
  myStepper.setSpeed(5);
  motor_2.setSpeed(5);

  //------------------------------------------------------------------------------
  //SETUP DO RTC (apenas acionar quando necessario reconfigurar)  
  //Wiring:  CLK -> 6 , DAT -> 7, Reset -> 8
  // format: seconds, minutes, hours, day of the week, day of the month, month, year
  // myRTC.setDS1302Time(40, 41, 15, 4, 13, 07, 2023); //Here you write your actual time/date as shown above 1
  // ------------------------------------------------------------------------------- 
}



void loop() {

  //COMEÇO DO CALCULO DO RASTREIO DO ASTRO
  
  double LST_em_graus = CalculateLST(); // Calcula o Local Sideral Time, em graus
  double RA_em_graus = RA_calculation(); // Calcula o Right Ascension da estrela, em graus
  
  double divergence = LST_em_graus - RA_em_graus; 

  if (repeticao == false ){

  double graus_motor_pitch = LATITUDE - star_declination; 
  
  Serial.print("Graus a rodar inicialmente motor pitch");
  Serial.print("  ");
  Serial.print(graus_motor_pitch);
  Serial.println();
  
  double  graus_motor_RA = divergence;
  

  Serial.print("Graus a rodar inicialmente motor Right Ascension");
  Serial.print("  ");
  Serial.print(graus_motor_RA);
  Serial.println();

  if (graus_motor_pitch > 90 || graus_motor_pitch < -90  || graus_motor_RA > 90 || graus_motor_RA < -90) 
  {
  Serial.print("Rotacao maior que a permitida");
  Serial.println();
  delay(5000);
  }

  else {
    
  // Atua o motor 1
  
  double motor2_revolutions = -graus_motor_pitch * double(5.6889);
  motor_2.step(motor2_revolutions);
  Serial.println("motor 2 foi girado");

  
  // Atua o motor 2 
  
  double motor_revolutions = graus_motor_RA * double(5.6889);
  
  myStepper.step(motor_revolutions);
  Serial.println("motor 1 foi girado pela primeira vez");
  grau_atual = divergence;
  }
  
  } 
  
  else
  {

    // Depois de alinhado o astros e os motores uma primeira vez, é apenas feita uma correção da orientacao 
    // nas proximas iterações

    Serial.println("Repetindo eternamente");
    double graus_a_rodar = divergence - grau_atual;
    
    Serial.print("Graus a rodar");
    Serial.print("  ");
    Serial.print(graus_a_rodar,6);
    Serial.println();
  
    if (graus_a_rodar > double(1) ) {

    digitalWrite (Pino_gate, HIGH);
    delay(50);
    myStepper.step(6); // o numero certo, quando se é pra rodar um grau, seriam 5.6889 revoluções, mas como tem q ser inteiro coloquei 6.
  
    Serial.println("Motor 1 gira 6 steps");  
    grau_atual = divergence; 
    delay(50);
    digitalWrite (Pino_gate, LOW);
    
    }
  
  }
  
  repeticao = true ;
  delay(1000);

  permissao = digitalRead(Pino_permissao);
  
   if (permissao == HIGH) {

    permissao = true;
    
  }

  
  else {

    permissao = false;

  }
  
}



// *************** Função que calcula o LST ******************************** // 


double CalculateLST() { // Calculate Local Sidereal Time
  
  myRTC.updateTime();

  double M, Y, D, MN, H, S;
  double A, B, C, E, F;

  M = (double) myRTC.month;
  Y = (double) myRTC.year;
  D = (double) myRTC.dayofmonth;
  MN = (double) myRTC.minutes;
  H = (double) myRTC.hours;
  S = (double) myRTC.seconds;

  if (M < 3) {
    M += 12;
    Y -= 1;
  }

  A = (long)Y / 100;
  B = (long)A / 4;
  C = (long)2 - A + B;
  E = (long)(365.25 * (Y + 4716));
  F = (long)(30.6001 * (M + 1));

  double LastJDN = C + D + E + F - 1524.5; // Julian day last midnight UT
  double Current_d = LastJDN - 2451545.0; //Days since Year 2000
  long Current_T = Current_d / 36525; // Julian centuries since Year 2000
  long NewJDN = LastJDN + H / 24; // Julian day today

  //Serial.println();
  //Serial.print("Julian Day Number..: ");
  //Serial.print(NewJDN); // Julian day today

  double Term1 = 6.697374558; // this line must be a double!
  double Term2 = 0.06570982441908 * Current_d;

  H = H + ((double)MN / 60) + ((double)S / 3600);

  float GMST;
  float Term3;

  Term3 = 0.00273790935 * H;
  Term3 += H;
  GMST = Term1 + Term2 + Term3; // Terms are found at http://aa.usno.navy.mil/faq/docs/GAST.php

  //add on longitude divided by 15 to get LST

  double LST = GMST + (LONGITUDE / 15); // longitude as hour angle.

  //reduce it to 24 format

  int LSTint;
  LSTint = (int)LST;
  LSTint /= 24;
  LST = LST - (double)LSTint * 24;

  int LST_H = (int)LST;
  int LST_M = ((LST - LST_H) * 60);
  int LST_S = (((LST - LST_H) * 60) - LST_M) * 60;

  int correcao_retirar_depois = 3; // coloquei esta correção dia 20/03/22 por que o LST está atrasado em 3hrs, não descobri o motivo ainda, assim que descobrir, retiro isso aq
  LST_H = LST_H + correcao_retirar_depois; 

  
  Serial.println();
  Serial.print("Local Sidereal time: ");
  Serial.print(LST_H);
  Serial.print(':');
  Serial.print(LST_M);
  Serial.print(':');
  Serial.println(LST_S);

  double double_LST_H =  double(LST_H);
  double double_LST_M =  double(LST_M);
  double double_LST_S =  double(LST_S);
  
  double LST_horas_em_graus = (double_LST_H)* double(15);
  double LST_minutos_em_graus =  (double_LST_M / double(60)) * double (15);
  double LST_segundos_em_graus = (double_LST_S /double(3600)) * double (15) ;

  double LST_em_graus = LST_horas_em_graus + LST_minutos_em_graus + LST_segundos_em_graus;
  
  Serial.println();
  Serial.print("LST em graus: ");
  Serial.print(LST_em_graus,6);
  Serial.println();

  return LST_em_graus;

}


// *************** Funcao que conver o RA em graus  ******************** //
double RA_calculation() { 

  double RA_em_graus = ((hora_ra)* double(15)) + ( ( minutos_ra/ double(60))* double(15)) + ( ( segundos_ra/ double(3600))* double(15));

  Serial.println();
  Serial.print( "Right Ascension em graus: ");
  Serial.print(RA_em_graus,6);
  Serial.println();

  return RA_em_graus;
  
}
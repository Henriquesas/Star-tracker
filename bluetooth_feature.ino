#include "BluetoothSerial.h"

const int ledPin = 2;
String receivedData;
String btvalues[7];
float timeHour, timeMin, timeSec, latitude, longitude, starCoord1, starCoord2;
int currentIndex = 0;
char c;

BluetoothSerial SerialBT;

void processCharacter(char receivedChar) {
  if (receivedChar == '%') {
    receivedData = "";  // Limpa a string ao encontrar o caractere de início
  } else if (receivedChar == '&') {
    extractValues();  // Extrai os valores ao encontrar o caractere de fim
  } else {
    receivedData += receivedChar;  // Concatena os caracteres para reconstruir a string
  }
}

void extractValues() {
  int startIndex = receivedData.indexOf('*') + 1;  // Encontra o primeiro asterisco após o caractere de início
  int endIndex;
  int index = 0;

  // Lida com o caso especial onde o primeiro valor está entre '%*' e o próximo asterisco
  if (startIndex > 0) {
    endIndex = receivedData.indexOf('*', startIndex);  // Encontra o próximo asterisco
    if (endIndex > 0) {
      btvalues[index] = receivedData.substring(startIndex, endIndex);  // Extrai o valor entre asteriscos
      index++;
      startIndex = endIndex + 1;  // Atualiza o índice de início para o próximo valor
    }
  }
  
  while (startIndex >= 0) {
    endIndex = receivedData.indexOf('*', startIndex);  // Encontra o próximo asterisco
    if (endIndex > 0) {
      btvalues[index] = receivedData.substring(startIndex, endIndex);  // Extrai o valor entre asteriscos
      index++;
      startIndex = endIndex + 1;  // Atualiza o índice de início para o próximo valor
    } else {
      break;
    }
  }
  
  latitude = btvalues[0].toFloat();
  longitude = btvalues[1].toFloat();
  timeHour = btvalues[2].toFloat();
  timeMin = btvalues[3].toFloat();
  timeSec = btvalues[4].toFloat();
  starCoord1 = btvalues[5].toFloat();
  starCoord2 = btvalues[6].toFloat();

  Serial.print("latitude: ");
  Serial.println(latitude);
  Serial.print("longitude: ");
  Serial.println(longitude);
  Serial.print("Hour: ");
  Serial.println(timeHour);
  Serial.print("Minute: ");
  Serial.println(timeMin);
  Serial.print("Second: ");
  Serial.println(timeSec);
  Serial.print("Star1: ");
  Serial.println(starCoord1);
  Serial.print("Star2: ");
  Serial.println(starCoord2);
}

void setup() {
  SerialBT.begin("Esp32-Bluetooth");
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (SerialBT.available()) {
    char receivedChar = SerialBT.read();
    processCharacter(receivedChar);
  }
}

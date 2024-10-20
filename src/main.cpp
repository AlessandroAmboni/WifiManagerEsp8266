#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>


//variáveis globais
#define BUTTON_PIN D7

// tempo limite para configuração do wifi do portal, utilizada na função onDemandAPMode, útil para caso haja quedas na rede wifi ou energia, para que o esp não fique preso no modo AP
const int timeout = 30;

// inicializa a biblioteca WiFiManager
WiFiManager wm;

// funcion declarations
void onDemandAPMode();
void printWifiInfo();

void setup() {
Serial.begin(115200);

delay(2000);
Serial.println("\nStarting...");

pinMode(BUTTON_PIN, INPUT_PULLUP);

// função para resetar as configurações de rede para testes
//wm.resetSettings();

// muda o ip do portal de configuração para 1.1.1.1
wm.setAPStaticIPConfig(IPAddress(1,1,1,1), IPAddress(1,1,1,1), IPAddress(255,255,255,0));

wm.setConfigPortalTimeout(timeout);
// inicializa a biblioteca WiFiManager e cria um ponto de acesso com nome "ESP8266 WiFi Configuration"
// ip padrão de acesso é 1.1.1.1
wm.autoConnect("ESP8266 AP");

}

void loop() {

  // se botão for pressionado ou o wifi não estiver conectado, entra no modo AP novamente
  if((digitalRead(BUTTON_PIN) == LOW) or (WiFi.status() != WL_CONNECTED)){
    delay(2000);
    Serial.println("Button pressed or WiFi not connected, entering AP mode \n");
    onDemandAPMode();
  }
  
  printWifiInfo();
  
  delay(5000);

}


// function definitions here:

void onDemandAPMode(){

  // define o tempo limite do portal (é utilizado duas vezes para caso seja necessário colocar valores diferentes)
  wm.setConfigPortalTimeout(timeout);

  if (!wm.startConfigPortal("ESP8266 AP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    //ESP.restart();
    return;
    delay(5000);
  }

  Serial.println("succesfully connected to WiFi");

}


// função para imprimir informações sobre a conexão wifi, para testes
void printWifiInfo(){
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected to WiFi " + WiFi.SSID() + "\n");

    Serial.println("Gateway: " + WiFi.gatewayIP().toString());
    Serial.println("IP Address: " + WiFi.localIP().toString());
    Serial.println("RSI: " + String(WiFi.RSSI()) + " dBm \n");
  } else {
    Serial.println("Not connected to WiFi");
  }
}

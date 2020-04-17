#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "colocar-o-nome-da-sua-rede";
const char* senha = "colocar-senha-da-sua-rede";
unsigned int localPort = 8888;
char pacote[10];

//separado com vírgula mesmo!
IPAddress ip(192,168,0,202);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

WiFiUDP conexao;

void setup() {
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  M5.begin();
  M5.Lcd.fillScreen(GREEN);
  M5.Lcd.setTextColor(PINK);
  M5.Lcd.setTextSize(2);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, senha);

  if (!WiFi.config(ip,gateway,subnet)){
    Serial.println("Falha ao configurar IP");
  }
  
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.println("Conexao Falhou!");
    delay(100);
  }
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println(WiFi.localIP());
  conexao.begin(localPort);
}

void loop() {
  int tamanhoPacote = conexao.parsePacket();
  
  if (tamanhoPacote){
    Serial.println(tamanhoPacote);    
    conexao.read(pacote, 5);
    Serial.println(pacote);
    M5.Lcd.println(pacote);
    
    if (strcmp("DW10H", pacote) == 0){
      digitalWrite(10, HIGH);
      Serial.println("Desligado");
      M5.Lcd.println("Desligado");
    }else if (strcmp("DW10L", pacote) == 0 ){
      digitalWrite(10, LOW);
      Serial.println("Ligado");
      M5.Lcd.println("Ligado");
    }    
  }
  conexao.flush();
}

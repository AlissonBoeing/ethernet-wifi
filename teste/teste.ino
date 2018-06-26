#include<ICMPPing.h>
#include<Ethernet.h>
#include <SPI.h>


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED //mac genérico pois não tem etiqueta na shield.
};
byte server[] = { 64, 233, 187, 99 }; // Google, para teste de conexão

IPAddress pingAddr(172,18,0,254); // router para testar ping

SOCKET pingSocket = 0;

char buffer[256];

ICMPPing ping(pingSocket, (uint16_t)random(0, 255));


EthernetClient client;

void setup() {
  
  pinMode(2, OUTPUT); //Pino para led vermelho (ambas conexões desligadas).
  pinMode(3, OUTPUT); //Pino para led verde (Conectado à ethernet).
  pinMode(4, OUTPUT); //Pino para led amarelo (Conectado ao wifi).
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) { //Inicia interface e espera obter ip pelo dhcp
    Serial.println("Não conseguiu obter endereço IP pelo DHCP");
  }
  

}

void loop() { 

  if (conectadoEthernet()) { //se conectado na interface ethernet acende led verde
    ShowIpEth();
    Serial.println("conectado eth");
    digitalWrite(3, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);

  /*} else if (conectadoWifi()) { //se conectado na interface wifi acende led amarelo
    Serial.println("conectado wifi");
    digitalWrite(4, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);*/
  } else { //se não tiver conectado acende led vermelho e tenta conectar novamente
    Serial.println("desconectado");
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    conecta();
  }

  
}

void conecta() {
  
  Ethernet.maintain(); //relativo ao dhclient
  delay(500);
  
 
}
bool conectadoEthernet() { //verifica se está conectado com um ping para o roteador (ifsc) 
  
  int i = 0, j = 0;
  while (i < 4) {
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS) {
    j++;
  } 
  i++;
  delay(500);
  }

  return j >= 2; //pelo menos dois sucessos de 4 tentativas

}

bool conectadoWifi() {
  
}

void ShowIpEth() { // mostra ip 
  Serial.println(Ethernet.localIP());
}

void ShowIpWifi(){
}


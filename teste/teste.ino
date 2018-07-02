// Baseado em https://www.filipeflop.com/blog/controle-modulo-rele-arduino/

#include <SPI.h>
#include <Ethernet.h>
String readString;

// Pinos dos relés:
// 1
int pino_rele1 = 3;
boolean ligado = true;
// 2
int pino_rele2 = 4;
boolean ligado_2 = true;

// Informações de MAC, endereco IP, gateway e máscara de rede
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F };
byte ip[] = { 191, 36, 8, 56 };
byte gateway[] = { 191, 36, 8, 62 };
byte subnet[] = { 255, 255, 255, 224};

EthernetServer server(80);

void setup()
{
  Serial.begin(9600);
  pinMode(pino_rele1, OUTPUT);
  pinMode(pino_rele2, OUTPUT);

  // Inicia o shild Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.println(Ethernet.localIP());
  Serial.println("FILIPEFLOP - Automacao Residencial");

  // Desliga os dois relés
  digitalWrite(pino_rele1, HIGH);
  digitalWrite(pino_rele2, HIGH);
}

void loop()
{

  EthernetClient client = server.available();
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == 'n')
        {
          // Controle do relé 1
          Serial.println(readString);
          // Liga
          if (readString.indexOf("?ligar") > 0)
          {
            digitalWrite(pino_rele1, LOW);
            Serial.println("Rele 1 Ligado");
            ligado = false;
          }
          else
          {
            // Desliga
            if (readString.indexOf("?desligar") > 0)
            {
              digitalWrite(pino_rele1, HIGH);
              Serial.println("Rele 1 Desligado");
              ligado = true;
            }
          }

          // Controle do relé 2
          Serial.println(readString);
          //
          if (readString.indexOf("?2_ligar") > 0)
          {
            digitalWrite(pino_rele2, LOW);
            Serial.println("Rele 2 Ligado");
            ligado_2 = false;
          }
          else
          {
            // Desliga
            if (readString.indexOf("?2_desligar") > 0)
            {
              digitalWrite(pino_rele2, HIGH);
              Serial.println("Rele 2 Desligado");
              ligado_2 = true;
            }
          }
          readString = "";

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head>");
          client.println("<title>FILIPEFLOP - Automacao Residencial</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<meta name='viewport' content='width=720, initial-scale=0.5' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://img.filipeflop.com/files/download/automacao/automacao_residencial.css' />");
          client.println("<script type='text/javascript' src='http://img.filipeflop.com/files/download/automacao/automacao_residencial.js'></script>");
          client.println("</head>");
          client.println("<body>");

          client.println("<table>");
          client.println("<tbody>");

          // Câmera IP do experimento
          client.println("<tr>");
          client.println("<td>");
          client.println("<p align='center'>");
          client.println("<img src='http://191.36.8.55/cgi-bin/mjpg/video.cgi' width='50%'/>");
          client.println("</p>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td>");
          client.println("<div id='wrapper'><img alt='FILIPEFLOP' src='http://img.filipeflop.com/files/download/automacao/logoFF.jpg'/><br/>");
          client.println("<div id='div1'>Rele 1</div>");
          client.println("<div id='div2'>Rele 2</div>");
          client.print("<div id='rele'></div><div id='estado' style='visibility: hidden;'>");
          client.print(ligado);
          client.println("</div>");
          client.println("<div id='botao'></div>");
          client.println("<div id='botao_2'></div>");
          client.print("<div id='rele_2'></div><div id='estado_2' style='visibility: hidden;'>");
          client.print(ligado_2);
          client.println("</div>");
          client.println("</div>");
          client.println("<script>AlteraRele1()</script>");
          client.println("<script>AlteraRele2()</script>");
          client.println("</div>");
          client.println("</td>");
          client.println("</tr>");

          client.println("</tbody>");
          client.println("</table>");
          client.println("</body>");
          client.println("</html>");

          delay(1);
          client.stop();
        }
      }
    }
  }
}

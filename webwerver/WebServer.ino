#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192, 168, 25, 20);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):

EthernetServer server(5080);

File webFile;

void setup() {
  
  Serial.begin(9600);
//  while (!Serial) {
    ; // Esperando inicializar a portta serial
//  }
  Serial.println("Ethernet WebServer inicialdo");

  // Startando o servidor web:
  Ethernet.begin(mac, ip);


  Serial.println("Inicializando o Cartão SD");

  if (!SD.begin(4)){
    Serial.println("Erro de inicialização do cartão Falhou.");
    return;    
  }
  Serial.println("Cartão SD Inicializado !!");

  if(!SD.exists("index.htm")){
     Serial.println("Arquivo HTML não Encontrado"); 
     return;
  }
  Serial.println("Sucesso Arquivo HTML encontrado.");

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  EthernetClient client = server.available();

  if(client){
    boolean CurrentLineIsBlank = true;
    while(client.connected()){
        if(client.available()){

          char c = client.read();

          if(c == '\n' && CurrentLineIsBlank){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();

            webFile = SD.open("index.htm");
            if(webFile){
              while(webFile.available()){
                client.write(webFile.read());
              }
              webFile.close();
            }
            break;
          }
          if(c == '\n'){
            CurrentLineIsBlank = true;
          }else if (c != '\r'){
            CurrentLineIsBlank = false;
          }
        }
    }

  delay(1);
  client.stop();
    
  }

}

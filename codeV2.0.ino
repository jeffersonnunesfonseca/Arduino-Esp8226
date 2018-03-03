#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

//192.168.25.10
//VARIAVEIS SENSOR
char vaz [10] ; //Variável para armazenar o valor em L/min
float vazao;
float media=0; //Variável para tirar a média a cada 1 minuto

volatile int Pulso=0; //Variável para a quantidade de pu lsos
int i=0; //Variável para contagem
int pino =3;

IPAddress server_addr(192,168,25,17); // IP DA MÁQUINA OU SERVIDOR DO MYSQL
char user[] = "jefferson"; //LOGIN DO MYSQL
char password[] = "123456"; // SENHA DO MYSQL
int port = 3307;

// CONEXÃO COM WIFI
char ssid[] = "WAN-GVT"; // SSID DA RED
char pass[] = "S1EC549294"; // SENHA DA REDE
//char INSERT_SQL[] = "INSERT INTO arduino.teste (msg) VALUES (teste)";


// QUERY INSERT
char INSERT_SQL[] = "INSERT INTO arduino.teste (msg) VALUES (%s)"; //%s é a variavel float que estou passando, onde lá embaixo será convertido para string
char query [128]; // variavel utilizada na conversão fo float para char, onde irá receber o INSERT_SQL
WiFiClient client;


MySQL_Connection conn((Client *)&client);

void setup() {

Serial.begin(115200);
  
  
// Iniciando wifi
WiFi.begin(ssid, pass);
while ( WiFi.status() != WL_CONNECTED ) {
delay ( 500 );
Serial.print ( "Tentando Conectar" );
}
Serial.println ( "" );
Serial.print ( "Conecado na rede : " );
Serial.println ( ssid );
Serial.print ( "IP : " );
Serial.println ( WiFi.localIP() );
//Fim conexão

Serial.println("DB - Connecting...");

if (conn.connect(server_addr, port, user, password)) {
  delay(500);
    Serial.println("Sucesso");
  pinMode(pino, INPUT);
  attachInterrupt(1, pulso, FALLING); //Configura o pino 2(Interrupção 0) para trabalhar como interrupção
  Serial.println("\n\nInicio\n\n"); //Imprime Inicio na serial

  }
  else {
    Serial.println("Falhou");
  } 
  delay(500);
  Serial.print("Conectando ao pino: ");
  Serial.println(pino);
  
 
    
}

void loop() {
// Iniciando conexão com o banco
MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  Pulso = 0;   //Zera a variável para contar os giros por segundos
  sei();      //Habilita interrupção
  delay (1000); //Aguarda 1 segundo
  cli();      //Desabilita interrupção
  
 
  vazao= Pulso / 7.5; //L/min com base no sensor de 1/2
  media=media+vazao; //armazena soma para retirada da media
  i++;
  
  Serial.print(vazao);
  Serial.print(" Litros por minuto - "); 
  Serial.print(i); //segundos
  Serial.println("seg");
  

//fazendo conversao do float para string
dtostrf(vazao, 1, 1, vaz);
//fazendo concatenação
sprintf(query, INSERT_SQL ,vaz);
//disparando query    
if(vazao>1){
cur_mem->execute(query);
}
  if(i==60)
  {
    media = media/60; 
    Serial.print("\nMedia por minuto = "); 
    Serial.print(media); 
    Serial.println(" Litros - "); 
    media = 0;
    i=0; 
    Serial.println("\n\nBegin\n\n"); //Imprime Inicio indicando que a contagem iniciou
    
  }
delete cur_mem;



}


void pulso ()
{ 
  Pulso++; //utilizado para a função  incrementado quando existir pulso
} 


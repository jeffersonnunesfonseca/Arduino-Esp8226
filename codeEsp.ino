#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
//192.168.25.10
float cont =0;
char teste[10];
IPAddress server_addr(192,168,25,17); // IP DA MÁQUINA OU SERVIDOR DO MYSQL
char user[] = "jefferson"; //LOGIN DO MYSQL
char password[] = "123456"; // SENHA DO MYSQL
int port = 3307;
// CONEXÃO COM WIFI
char ssid[] = "WAN-GVT"; // SSID DA RED
  char pass[] = "S1EC549294"; // SENHA DA REDE


// Sample query
//char INSERT_SQL[] = "INSERT INTO arduino.tb_dados_sensor (dados_sensor) VALUES (110)";
char INSERT_SQL[] = "INSERT INTO arduino.tb_dados_sensor (dados_sensor) VALUES (%s)"; //%s é a variavel float que estou passando, onde lá embaixo será convertido para string
char query [128]; // variavel utilizada na conversão fo float para char, onde irá receber o INSERT_SQL
WiFiClient client;


MySQL_Connection conn((Client *)&client);

void setup() {
Serial.begin(115200);
// Begin WiFi section
WiFi.begin(ssid, pass);
pinMode(13,OUTPUT);

// Wait for connection
while ( WiFi.status() != WL_CONNECTED ) {
delay ( 500 );
Serial.print ( "." );
}
Serial.println ( "" );
Serial.print ( "Connected to " );
Serial.println ( ssid );
Serial.print ( "IP address: " );
Serial.println ( WiFi.localIP() );
// End WiFi section
Serial.println("DB - Connecting...");

if (conn.connect(server_addr, port, user, password)) {
  delay(500);
    Serial.println("Sucesso");
  }
  else {
    Serial.println("Falhou");
  }   
}

void loop() {
Serial.println("Salvando.");
// Initiate the query class instance
MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

digitalWrite(13,HIGH);
delay(1000);
digitalWrite(13,LOW);
delay(1000);
cont = cont+1;
Serial.print("CONT =");
Serial.println(cont);
// Execute the query
dtostrf(cont, 1, 1, teste);
//fazendo concatenação
sprintf(query, INSERT_SQL ,teste);
cur_mem->execute(query);

// Note: since there are no results, we do not need to read any data
// Deleting the cursor also frees up memory used
delete cur_mem;
}


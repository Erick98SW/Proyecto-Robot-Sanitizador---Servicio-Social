#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <FS.h>

//"ROBOTECISC" Nombre de la red del router vinculado al robot **ssid**.
//"sanitizador" Contraseña de la red del router vinculado al robot **password**.

const char* ssid = "ROBOTECISC"; //Nombre de la red a usar.
const char* password = "sanitizador"; //Contraseña de la red a usar.

//Motor Derecha
int OUTPUT4 = 16; // D0 => 16 o //D0 => '16'
int OUTPUT3 = 0;  //D3 => '0' o //D1 o 5
                   //El marcador '' hace referencia a los pines si se usa el Arduino MEGA, si no estan marcados son pines del Arduino uno, pero para ambas tarjetas sirven cualquiera.
//Motor Izquierda
int OUTPUT2 = 2; //D2 => 4 o //D4 => '2'
int OUTPUT1 = 14; //D3 => 0 0 //D5 => '14'
long duracion = 0;

//Se define el puerto 81 para el WebSocket y puerto 80 para pagina web
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

void setup(void) {
  delay(1000);
  //Velocidad
  Serial.begin(115200);
  //Configuracion de Salidas
  pinMode (OUTPUT1, OUTPUT);
  pinMode (OUTPUT2, OUTPUT);
  pinMode (OUTPUT3, OUTPUT);
  pinMode (OUTPUT4, OUTPUT);

  //Establecer la conexion con el AP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Se establece la conexion y presenta la IP del cliente
  IPAddress myIP = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(myIP);

  //Inicia lectura del archivo
  SPIFFS.begin();
  //Inicia Websocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  //En caso de error 404 no se encuntra
  server.onNotFound([] () {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "Archivo no encontrado");
  });

  //Servidor Web iniciado
  server.begin();
  Serial.println("Servidor HTTP Iniciado");
}

void loop(void) {
  //Websocket a la espera de conexiones
  webSocket.loop();
  //Servidor Web a la espera de conexiones
  server.handleClient();
  delay(10);
}

//Funcion predefinida de un WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    //En caso de que un cliente se desconecte del websocket
    case WStype_DISCONNECTED: {
        Serial.printf("Usuario #%u - Desconectado\n", num);
        break;
      }
    //Cuando un cliente se conecta al websocket presenta la informacion del cliente conectado, IP y ID
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("Nueva conexión: %d.%d.%d.%d Nombre: %s ID: %u\n", ip[0], ip[1], ip[2], ip[3], payload, num);
        break;
      }
    case WStype_TEXT: {
        String entrada = "";
        //Se lee la entrada de datos y se concatena via el servidor Websocket
        for (int i = 0; i < lenght; i++) {
          entrada.concat((char)payload[i]);
        }
        //Se separan los datos de la posicion X y Y del Joystick
        String data = entrada;
        if (data) {
          int pos = data.indexOf(':');
          long x = data.substring(0, pos).toInt();
          long y = data.substring(pos + 1).toInt();
          //Imprime en monitor serial
          Serial.print("x:");
          Serial.print(x);
          Serial.print(", y:");
          Serial.println(y);
          //De acuerdo al valor de X y Y del Joystick, se ejcuta la funcion para habilitar los motores
          if (((x <= 50 && x > - 50) && (y <= 50 && y > - 50))) { //Parar
            parar();
          } else if (x > 50 && (y < 50 || y > - 50)) { //Derecha
            derecha();
          } else if (x < - 50 && (y < 50 || y > - 50)) { //Izquierda
            izquierda();
          } else if (y > 50 && (x < 50 || x > - 50)) { //Atras
            atras();
          } else if (y < - 50 && (x < 50 || x > - 50)) { //Adelnte
            adelante();
          }
        }
        break;
      }
  }
}

//Funcion para indentificar el tipo de contenido de los archivos del servidor web
String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

//Funcion para cargar el archivo del servidor web index.html
bool handleFileRead(String path) {
#ifdef DEBUG
  Serial.println("handleFileRead: " + path);
#endif
  if (path.endsWith("/")) path += "index.html";
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, getContentType(path));
    file.close();
    return true;
  }
  return false;
}

//Funciones para accionar los motores de acuerdo a los valores que se envian por medio del joystick
void adelante() {
  Serial.println("adelante");
  digitalWrite(OUTPUT1, 0);
  digitalWrite(OUTPUT2, 0);
  digitalWrite(OUTPUT3, 1);
  digitalWrite(OUTPUT4, 0);
}

void atras() {
  Serial.println("atras");
  digitalWrite(OUTPUT1, 1);
  digitalWrite(OUTPUT2, 0);
  digitalWrite(OUTPUT3, 0);
  digitalWrite(OUTPUT4, 0);
}

void derecha() {
  Serial.println("derecha");
  digitalWrite(OUTPUT1, 1);
  digitalWrite(OUTPUT2, 0);
  digitalWrite(OUTPUT3, 1);
  digitalWrite(OUTPUT4, 0);
}

void izquierda() {
  Serial.println("izquierda");
  digitalWrite(OUTPUT1, 0);
  digitalWrite(OUTPUT2, 1);
  digitalWrite(OUTPUT3, 1);
  digitalWrite(OUTPUT4, 0);
}

void parar() {
  Serial.println("parar");
  digitalWrite(OUTPUT1, 0);
  digitalWrite(OUTPUT2, 0);
  digitalWrite(OUTPUT3, 0);
  digitalWrite(OUTPUT4, 0);
}

//RECIBEN LA CADENA DE DATOS  
int Recibe1=8;
int Recibe2=9;
int Recibe3=10;
int Recibe4=11;

//LENCTURA DE DATOS
int P1=0;
int P2=0;
int P3=0;
int P4=0;

//PUENTE H
int A=3; //A  AZUL
int B=4; //B  AMARILLO
int C=5; //C  VERDE
int D=6; //D  NARANJA

//LUCES
int Luces=13;

void setup() {
  Serial.begin(9600);
  Serial.println("-----LISTO PARA TRABAJO-----");
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  analogWrite(A,127);
  analogWrite(B,127);
  analogWrite(C,127);
  analogWrite(D,127);
  delay(3000);

  // PINES RECIBE DATOS
pinMode(Recibe1,INPUT);
pinMode(Recibe2,INPUT);
pinMode(Recibe3,INPUT);
pinMode(Recibe4,INPUT);


//ENCENDIDO DE LUCES
pinMode(Luces,OUTPUT);
}
void loop() {
P1=digitalRead(Recibe1);
P2=digitalRead(Recibe2);
P3=digitalRead(Recibe3);
P4=digitalRead(Recibe4);

  Serial.print(P1);
  Serial.print(P2);
  Serial.print(P3);
  Serial.print(P4);
  Serial.print('\n');
   if (P3==0 && P2==0 && P1==1)
  {
    Serial.println("Presionaste arriba -->Adelante VELOCIDAD 1");
  analogWrite(A,155);
  analogWrite(B,135);
  analogWrite(C,158);
  analogWrite(D,130);
   }
   if (P3==0 && P2==1 && P1==0)
  {
    Serial.println("Presionaste arriba -->Adelante VELOCIDAD 2");
  analogWrite(A,180);
  analogWrite(B,130);
  analogWrite(C,185);
  analogWrite(D,130);    }
  if (P3==0 && P2==1 && P1==1)
  {
    Serial.println("Presionaste arriba -->Adelante VELOCIDAD 3");
  analogWrite(A,255);
  analogWrite(B,127);
  analogWrite(C,255);
  analogWrite(D,127);
   }
   if (P3==1 && P2==0 && P1==0)
    {  
      Serial.println("Presionaste abajo -->Reversa BAJA VELOCIDAD");
  analogWrite(A,0);
  analogWrite(B,127);
  analogWrite(C,0);
  analogWrite(D,127);
    }
   if (P3==0 && P2==0 && P1==0)
    {  
      Serial.println("Presionaste STOP --> ALTO TOTAL");
      analogWrite(A,127);
      analogWrite(C,127);
      analogWrite(D,127);
      analogWrite(B,127);
    }
     if (P3==1 && P2==0 && P1==1)
    {  
      Serial.println("Presionaste derecha --> Derecha BAJA VELOCIDAD");
  analogWrite(A,127);
  analogWrite(B,255);
  analogWrite(C,127);
  analogWrite(D,255);
    }
    if (P3==1 && P2==1 && P1==0)
    {  
      Serial.println("Presionaste izquierda --> Izquierda BAJA VELOCIDAD");
  analogWrite(A,127);
  analogWrite(B,0);
  analogWrite(C,127);
  analogWrite(D,0); 
    }
    if (P4==1)
    {  
      Serial.println("Encendido de luces");
      digitalWrite(Luces,HIGH);
    }
  else
  {
    Serial.println("Apagado de luces");
      digitalWrite(Luces,LOW);
    }
 
  //delay(10);
  }

#include <Keypad.h>
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
//CONFIGURANDO KEYPAD
const byte filas = 4; //4 FILAS
const byte columnas = 3; //4 COLUMNAS
//define the cymbols on the buttons of the keypads
char hexaKeys[filas][columnas] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[filas] = {33, 35, 37, 39}; //PINES CORRESPONDIENTES A FILAS
byte colPins[columnas] = {27, 29, 31};    //PINES CORRESPONDIENTES A COLUMNASo the column pinouts of the keypad
//INSTACIANDO OBJETO Keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, filas, columnas);

//INSTANCIANDO OBJETO SERVO
Servo servo1;

//ESTRUCTURA DE DATOS DE USUARIO
struct User {
  int pass;
  String name;
};
//INICIALIZANDO PINES DE LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//ASIGNANDO VALORES DE PINES
int PIN_BLUETOOTHVCC = 40,
    PIN_PULSADOR = 24,
    PIN_LEDVERDE = 26,
    PIN_LEDROJO = 30,
    PIN_BUZZER = 34,
    GRADOS;
char clave, customKey;
String texto, pass, usuario;
User usuario1;

User x;



void setup() {

  //INICIAR COMUNICACION SERIAL
  Serial.begin(9600);
  //SERVO
  servo1.attach(32, 500, 2400);
  //LCD
  lcd.begin(16, 2);
  loading();

  //PINES KAWAII
  pinMode(PIN_BLUETOOTHVCC, OUTPUT);
  pinMode(PIN_PULSADOR, INPUT);
  pinMode(PIN_LEDVERDE, OUTPUT);
  pinMode(PIN_LEDROJO, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  //ENCENDER MODULO HC-05
  digitalWrite(PIN_BLUETOOTHVCC, HIGH);


  //AGREGANDO DATOS A EEPROM
  setUsuario();
}

void loop() {
  lcd.setCursor(0, 1);
  customKey = customKeypad.getKey();
  if (customKey) {
    Serial.println(customKey);
    cadena(2);
    abrir(texto);

  }
  
    if (Serial.available()) {
      clave = Serial.read();

    

    if ( clave == '1' ) {

      digitalWrite(PIN_LEDVERDE, HIGH);

    } else if (clave == '0') {
      digitalWrite(PIN_LEDVERDE, LOW);
    }



    if (clave == '@') {
      cadena(1);
      delay(500);
      abrir(texto);

    }

    if (clave == '$') {
      cadena(1);
      lcd.print(texto);


    }

    if (clave == '&') {
      servo();

    }

  }

}

//METODO QUE PARA FORMAR CADENAS DE TEXTO
void cadena(int opcion) {
  texto = "";
  pass = "";

  switch (opcion) {
    case 1 :

      char c;
      delay(30);
      while (Serial.available())
      {
        c = Serial.read();  //Lee el dato entrante y lo almacena en una variable tipo char
        if (c != '*') {
          texto += c;            //Crea una cadena tipo String con los datos entrates

        } else {
          pass += c;
        }


      }

      if (texto.length() > 0) {  //Se verifica que la cadena tipo String tenga un largo mayor a cero
        Serial.println(texto);
        Serial.println(pass);

      }
      break;

    case 2:
      while (customKey != '#'){
       
        if(customKey != NULL){
          texto += customKey;
          Serial.println(customKey);
          }
          customKey = customKeypad.getKey();
      }
      if (texto.length() > 0) {  //Se verifica que la cadena tipo String tenga un largo mayor a cero
         Serial.println("test");
        Serial.println(texto);
      }
      break;


  }

}



//ESTE METODO ESCRIBE DATOS EN LA EEPROM
void setUsuario() {
  usuario1.pass = 1234;
  usuario1.name = "9632";
  EEPROM.put(0, usuario1);
  EEPROM.get(0, x);
  Serial.println(x.pass);
  Serial.println(x.name);
  Serial.print("EEPROM length: ");
  Serial.println(EEPROM.length());
}

//ESTE METODO VERIFICA LOS DATOS INGRESADOS PARA ABRIR LA CERRADURA
void abrir(String password) {
  usuario = x.name;
   Serial.println(texto.length());
   Serial.println(usuario.length());
  Serial.println(password.equals(usuario));

  if (password.equals(usuario)) {
    Serial.println("Ingreso correcto");
    lcd.print("Ingreso correcto!");
    digitalWrite(PIN_LEDVERDE, HIGH);
    digitalWrite(PIN_LEDROJO, LOW);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(50);
    digitalWrite(PIN_BUZZER, LOW);
    delay(50);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(50);
    digitalWrite(PIN_BUZZER, LOW);

  } else {
    lcd.print("Ingreso denegado!");
    digitalWrite(PIN_LEDROJO, HIGH);
    digitalWrite(PIN_LEDVERDE, LOW);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(350);
    digitalWrite(PIN_BUZZER, LOW);

  }

}

void loading() {
  String punto;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      lcd.print("Loading");
      lcd.print(punto);
      punto += ".";
      delay(500);
      lcd.clear();
    }
    punto = "";
    lcd.clear();
  }

}

void servo() {
  GRADOS = Serial.parseInt();

  if (GRADOS >= 0) {
    servo1.write(GRADOS);

  }
  delay(1000);

}

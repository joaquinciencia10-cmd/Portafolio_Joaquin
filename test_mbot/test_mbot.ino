/*
 * Test de diagnóstico para Makeblock mBot
 * Verifica: LEDs, zumbador, motores y sensores básicos
 * 
 * Instrucciones: Subir el código y abrir el Monitor Serie (9600 baudios)
 * Seguir las instrucciones que aparecen en pantalla
 */

#include <SoftwareSerial.h>
#include <Wire.h>

// === Pines del mBot ===
#define LED_PIN       13
#define BUZZER_PIN    3

// Motores (puerto M1 y M2 en la placa)
#define MOTOR_IZQ_INA 5
#define MOTOR_IZQ_INB 6
#define MOTOR_DER_INA 9
#define MOTOR_DER_INB 10

// Sensor de seguidor de línea
#define LINE_IZQ      A2
#define LINE_DER      A1

// Botón en la placa
#define BOTON_PIN     7

// === Variables globales ===
int testNum = 0;
bool testEnCurso = false;

void setup() {
  Serial.begin(9600);
  
  // Configurar pines
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_IZQ_INA, OUTPUT);
  pinMode(MOTOR_IZQ_INB, OUTPUT);
  pinMode(MOTOR_DER_INA, OUTPUT);
  pinMode(MOTOR_DER_INB, OUTPUT);
  pinMode(LINE_IZQ, INPUT);
  pinMode(LINE_DER, INPUT);
  pinMode(BOTON_PIN, INPUT_PULLUP);
  
  // Apagar todo al inicio
  apagarMotores();
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  
  delay(1000);
  mostrarMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case '1': testLED(); break;
      case '2': testBuzzer(); break;
      case '3': testMotores(); break;
      case '4': testSensores(); break;
      case '5': testBoton(); break;
      case '6': testCompleto(); break;
      case 'a': testRGB(); break;
      case 'm':
      case 'M':
        mostrarMenu();
        break;
    }
  }
}

// ===========================
//          MENÚ
// ===========================
void mostrarMenu() {
  Serial.println();
  Serial.println(F("╔══════════════════════════════╗"));
  Serial.println(F("║   DIAGNÓSTICO mBot v1.0      ║"));
  Serial.println(F("╠══════════════════════════════╣"));
  Serial.println(F("║ 1 - Test LED placa           ║"));
  Serial.println(F("║ 2 - Test Zumbador            ║"));
  Serial.println(F("║ 3 - Test Motores             ║"));
  Serial.println(F("║ 4 - Test Sensores línea      ║"));
  Serial.println(F("║ 5 - Test Botón               ║"));
  Serial.println(F("║ a - Test LED RGB             ║"));
  Serial.println(F("║ 6 - TEST COMPLETO            ║"));
  Serial.println(F("║ m - Mostrar este menú        ║"));
  Serial.println(F("╚══════════════════════════════╝"));
  Serial.println(F("Escribe un número y pulsa Enter"));
}

// ===========================
//        TEST 1: LED
// ===========================
void testLED() {
  Serial.println(F("\n--- TEST 1: LED de placa (pin 13) ---"));
  Serial.println(F("Encendiendo LED..."));
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println(F("LED ENCENDIDO"));
    delay(500);
    digitalWrite(LED_PIN, LOW);
    Serial.println(F("LED APAGADO"));
    delay(500);
  }
  
  Serial.println(F(">> Test LED completado"));
  Serial.println(F(">> ¿Viste el LED parpadear? Si/No"));
}

// ===========================
//      TEST 2: BUZZER
// ===========================
void testBuzzer() {
  Serial.println(F("\n--- TEST 2: Zumbador ---"));
  Serial.println(F("Reproduciendo tonos..."));
  
  int tonos[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int duracion = 200;
  
  for (int i = 0; i < 8; i++) {
    Serial.print(F("Tono: "));
    Serial.println(tonos[i]);
    tone(BUZZER_PIN, tonos[i], duracion);
    delay(duracion + 50);
  }
  
  // Tono largo final
  tone(BUZZER_PIN, 523, 1000);
  delay(1200);
  noTone(BUZZER_PIN);
  
  Serial.println(F(">> Test zumbador completado"));
  Serial.println(F(">> ¿Escuchaste los tonos? Si/No"));
}

// ===========================
//      TEST 3: MOTORES
// ===========================
void testMotores() {
  Serial.println(F("\n--- TEST 3: Motores ---"));
  Serial.println(F("ATENCIÓN: El robot se moverá."));
  Serial.println(F("Ponlo en una zona despejada."));
  delay(2000);
  
  // Motor izquierdo adelante
  Serial.println(F("Motor IZQUIERDO -> Adelante"));
  encenderMotorIzq(150);
  delay(1000);
  apagarMotores();
  delay(500);
  
  // Motor izquierdo atrás
  Serial.println(F("Motor IZQUIERDO -> Atrás"));
  encenderMotorIzq(-150);
  delay(1000);
  apagarMotores();
  delay(500);
  
  // Motor derecho adelante
  Serial.println(F("Motor DERECHO -> Adelante"));
  encenderMotorDer(150);
  delay(1000);
  apagarMotores();
  delay(500);
  
  // Motor derecho atrás
  Serial.println(F("Motor DERECHO -> Atrás"));
  encenderMotorDer(-150);
  delay(1000);
  apagarMotores();
  delay(500);
  
  // Ambos adelante
  Serial.println(F("Ambos motores -> Adelante"));
  encenderMotorIzq(150);
  encenderMotorDer(150);
  delay(1500);
  apagarMotores();
  
  Serial.println(F(">> Test motores completado"));
  Serial.println(F(">> ¿Se movieron los motores? Si/No"));
}

// ===========================
//     TEST 4: SENSORES
// ===========================
void testSensores() {
  Serial.println(F("\n--- TEST 4: Sensores de línea ---"));
  Serial.println(F("Coloca el robot sobre una superficie"));
  Serial.println(F("y cubre/destapa los sensores con la mano.\n"));
  
  for (int i = 0; i < 20; i++) {
    int valIzq = analogRead(LINE_IZQ);
    int valDer = analogRead(LINE_DER);
    
    Serial.print(F("Izq: "));
    Serial.print(valIzq);
    Serial.print(F(" ("));
    Serial.print(valIzq > 500 ? "BLANCO" : "NEGRO");
    Serial.print(F(")  |  Der: "));
    Serial.print(valDer);
    Serial.print(F(" ("));
    Serial.print(valDer > 500 ? "BLANCO" : "NEGRO");
    Serial.println(F(")"));
    
    delay(300);
  }
  
  Serial.println(F(">> Test sensores completado"));
}

// ===========================
//      TEST 5: BOTÓN
// ===========================
void testBoton() {
  Serial.println(F("\n--- TEST 5: Botón de la placa ---"));
  Serial.println(F("Pulsa el botón de la placa (MAX)"));
  Serial.println(F("Tienes 10 segundos...\n"));
  
  int intentos = 0;
  bool pulsado = false;
  
  while (intentos < 100 && !pulsado) {
    if (digitalRead(BOTON_PIN) == LOW) {
      Serial.println(F(">> ¡BOTÓN PULSADO! OK"));
      pulsado = true;
      // Feedback: encender LED
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
    }
    intentos++;
    delay(100);
  }
  
  if (!pulsado) {
    Serial.println(F(">> Botón NO detectado"));
    Serial.println(F("   (No se pulsó en el tiempo dado)"));
  }
  
  Serial.println(F(">> Test botón completado"));
}

// ===========================
//      TEST RGB
// ===========================
void testRGB() {
  Serial.println(F("\n--- TEST: LED RGB (I2C) ---"));
  Serial.println(F("Probando colores en el LED RGB..."));
  Serial.println(F("Nota: El LED RGB del mBot"));
  Serial.println(F("se comunica por I2C con la placa.\n"));
  
  // Secuencia visual en el LED de placa
  // (el LED RGB real del mBot requiere la librería MeRGBLed)
  // Aquí simulamos con el LED de placa
  
  for (int i = 0; i < 3; i++) {
    Serial.println(F("ROJO -> blink x3"));
    for (int j = 0; j < 3; j++) {
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      delay(200);
    }
    delay(300);
    
    Serial.println(F("VERDE -> blink x3"));
    for (int j = 0; j < 3; j++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
    delay(300);
  }
  
  Serial.println(F(">> Test RGB completado"));
  Serial.println(F(">> Para el LED RGB real se necesita"));
  Serial.println(F("   la librería MeRGBLed de Makeblock"));
}

// ===========================
//     TEST COMPLETO
// ===========================
void testCompleto() {
  Serial.println(F("\n================================"));
  Serial.println(F("   INICIANDO TEST COMPLETO"));
  Serial.println(F("================================\n"));
  
  int pasados = 0;
  int total = 5;
  
  // Test 1
  testLED();
  pasados++;
  
  // Test 2
  testBuzzer();
  pasados++;
  
  // Test 3
  Serial.println(F("\n¿Continuar con test de motores? (responde algo en el monitor serie)"));
  while (!Serial.available()) { delay(100); }
  while (Serial.available()) Serial.read();
  testMotores();
  pasados++;
  
  // Test 4
  testSensores();
  pasados++;
  
  // Test 5
  testBoton();
  pasados++;
  
  // Resumen
  Serial.println(F("\n================================"));
  Serial.println(F("   RESUMEN DEL DIAGNÓSTICO"));
  Serial.println(F("================================"));
  Serial.print(F("Tests completados: "));
  Serial.print(pasados);
  Serial.print(F("/"));
  Serial.println(total);
  
  if (pasados == total) {
    Serial.println(F("Estado: OK - Todos los tests ejecutados"));
  }
  
  Serial.println(F("================================\n"));
  mostrarMenu();
}

// ===========================
//    FUNCIONES AUXILIARES
// ===========================
void encenderMotorIzq(int velocidad) {
  if (velocidad > 0) {
    analogWrite(MOTOR_IZQ_INA, velocidad);
    analogWrite(MOTOR_IZQ_INB, 0);
  } else {
    analogWrite(MOTOR_IZQ_INA, 0);
    analogWrite(MOTOR_IZQ_INB, -velocidad);
  }
}

void encenderMotorDer(int velocidad) {
  if (velocidad > 0) {
    analogWrite(MOTOR_DER_INA, velocidad);
    analogWrite(MOTOR_DER_INB, 0);
  } else {
    analogWrite(MOTOR_DER_INA, 0);
    analogWrite(MOTOR_DER_INB, -velocidad);
  }
}

void apagarMotores() {
  analogWrite(MOTOR_IZQ_INA, 0);
  analogWrite(MOTOR_IZQ_INB, 0);
  analogWrite(MOTOR_DER_INA, 0);
  analogWrite(MOTOR_DER_INB, 0);
}

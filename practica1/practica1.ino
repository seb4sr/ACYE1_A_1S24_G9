#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <Key.h>
#include <Keypad.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEV 3
#define DATA 10
#define CS 11
#define CLK 12

String pass_display;
String pass;
String new_pass;
String new_pass_ast;
String pass_correcta="AC09C124";
//String pass_correcta = "8";
String estado = "ESTADO ARMADO";
String operacion_str = "";
String operacion_display = "";
String error = "false";
int intentos = 0;
String mensaje = "";
int estadofinal = 1;

unsigned long tiempoUltimaOperacion = 0;
const unsigned long intervaloOperacion = 30000;

const textEffect_t animacion2[] = {
  PA_PRINT, PA_SCROLL_UP, PA_SLICE, PA_SCAN_HORIZ, PA_OPENING_CURSOR, PA_SCROLL_DOWN_RIGHT,
  PA_WIPE, PA_GROW_UP, PA_CLOSING_CURSOR, PA_SCROLL_UP_LEFT, PA_MESH, PA_OPENING,
  PA_SCROLL_UP_RIGHT, PA_BLINDS, PA_DISSOLVE, PA_CLOSING, PA_RANDOM, PA_WIPE_CURSOR,
  PA_GROW_DOWN, PA_SCAN_VERT, PA_SCROLL_DOWN_LEFT
};

MD_Parola MATRICES = MD_Parola(HARDWARE_TYPE, DATA, CLK, CS, MAX_DEV);

bool inicio = true;
bool correcta = false;

const byte filas = 4;
const byte columnas = 4;

char teclas[filas][columnas] = {
  { '7', '8', '9', 'A' },
  { '4', '5', '6', 'B' },
  { '1', '2', '3', 'C' },
  { '*', '0', '#', 'D' }
};

byte filasPines[filas] = { 5, 4, 3, 2 };
byte columnasPines[columnas] = { 9, 8, 7, 6 };

Keypad teclado = Keypad(makeKeymap(teclas), filasPines, columnasPines, filas, columnas);

bool esDigitoString(const String& str) {
  // Iterar a través de cada caracter del string
  for (int i = 0; i < str.length(); i++) {
    // Verificar si el caracter actual no es un dígito
    if (!isdigit(str.charAt(i))) {
      // Si encontramos un caracter que no es un dígito, retornamos falso
      return false;
    }
  }
  // Si todos los caracteres son dígitos, retornamos verdadero
  return true;
}

int calculadora(String expresion) {

  int resultado = 0;
  int aux_result = 0;

  String numero1 = "";
  String numero2 = "";

  String operacion_simplificada = expresion;
  String operacion_escribir = "";

  bool opero = false;

  int contador_operaciones = 0;
  int contador_operaciones2 = 0;
  //-------------------------
  for (int i = 0; i < expresion.length(); i++) {

    char caracter = expresion.charAt(i);
    //if (caracter == '*' || caracter == '/'){
    if (caracter == '*') {
      contador_operaciones += 1;
    }
  }
  //Serial.println("CAN OP: " + String(contador_operaciones));

  for (int i = 0; i < contador_operaciones;) {
    int digitoInt1 = "0";
    int digitoInt2 = "0";
    numero1 = "";
    numero2 = "";
    for (int j = 0; j < operacion_simplificada.length(); j++) {

      char caracter = operacion_simplificada.charAt(j);

      if (caracter == '*') {
        if (opero == false) {
          for (int k = j - 1; k < operacion_simplificada.length(); k--) {

            char caracter_j = operacion_simplificada.charAt(k);

            if (k != -1) {

              if (caracter_j == '+' || caracter_j == '/' || caracter_j == '-' || caracter_j == '*') {

                //Serial.println("Se guardo un digito");
                k = operacion_simplificada.length() + 1;

              } else {
                numero1 += caracter_j;
              }
            }
          }
          for (int p = j + 1; p < operacion_simplificada.length(); p++) {

            char caracter_j = operacion_simplificada.charAt(p);

            if (caracter_j == '+' || caracter_j == '/' || caracter_j == '-' || caracter_j == '*') {

              //Serial.println("Se guardo un digito");
              p = operacion_simplificada.length();

            } else {

              numero2 += caracter_j;
              j++;
            }
          }


          String numero1_fix = "";
          for (int b = numero1.length() - 1; b >= 0; b--) {
            numero1_fix += numero1.charAt(b);
          }
          //Serial.println("NA fix "+numero1_fix);
          //Serial.println("NB " + numero2);
          digitoInt1 = numero1_fix.toInt();
          digitoInt2 = numero2.toInt();
          aux_result = digitoInt1 * digitoInt2;
          for (int r = 0; r < numero1.length(); r++) {
            if (operacion_escribir != 0) {

              int longitud = operacion_escribir.length();
              operacion_escribir = operacion_escribir.substring(0, longitud - 1);
            }
          }

          operacion_escribir += String(aux_result);
          i++;
          //Serial.println("i" + String(i));
          //Serial.println("TEMP RESULT" + String(aux_result));
          opero = true;
          //j=operacion_simplificada.length();
        } else {
          operacion_escribir += caracter;
        }
      } else {
        operacion_escribir += caracter;
      }
    }
    opero = false;
    //Serial.println("OPS ESCRI " + operacion_escribir);
    operacion_simplificada = operacion_escribir;
    operacion_escribir = "";
    //Serial.println("OPSIMPLI " + operacion_simplificada);
  }


  for (int i = 0; i < expresion.length(); i++) {

    char caracter = expresion.charAt(i);
    //if (caracter == '*' || caracter == '/'){
    if (caracter == '/') {
      contador_operaciones2 += 1;
    }
  }
  //Serial.println("CAN OP2: " + String(contador_operaciones2));

  for (int i = 0; i < contador_operaciones2;) {
    int digitoInt1 = "0";
    int digitoInt2 = "0";
    numero1 = "";
    numero2 = "";
    for (int j = 0; j < operacion_simplificada.length(); j++) {

      char caracter = operacion_simplificada.charAt(j);

      if (caracter == '/') {
        if (opero == false) {
          for (int k = j - 1; k < operacion_simplificada.length(); k--) {

            char caracter_j = operacion_simplificada.charAt(k);

            if (k != -1) {

              if (caracter_j == '+' || caracter_j == '/' || caracter_j == '-' || caracter_j == '*') {

                //Serial.println("Se guardo un digito");
                k = operacion_simplificada.length() + 1;

              } else {
                numero1 += caracter_j;
              }
            }
          }
          for (int p = j + 1; p < operacion_simplificada.length(); p++) {

            char caracter_j = operacion_simplificada.charAt(p);

            if (caracter_j == '+' || caracter_j == '/' || caracter_j == '-' || caracter_j == '*') {

              //Serial.println("Se guardo un digito");
              p = operacion_simplificada.length();

            } else {

              numero2 += caracter_j;
              j++;
            }
          }


          String numero1_fix = "";
          for (int b = numero1.length() - 1; b >= 0; b--) {
            numero1_fix += numero1.charAt(b);
          }
          //Serial.println("NA fix "+numero1_fix);
          //Serial.println("NB " + numero2);
          digitoInt1 = numero1_fix.toInt();
          digitoInt2 = numero2.toInt();
          aux_result = digitoInt1 / digitoInt2;
          for (int r = 0; r < numero1.length(); r++) {
            if (operacion_escribir != 0) {

              int longitud = operacion_escribir.length();
              operacion_escribir = operacion_escribir.substring(0, longitud - 1);
            }
          }

          operacion_escribir += String(aux_result);
          i++;
          //Serial.println("i" + String(i));
          //Serial.println("TEMP RESULT" + String(aux_result));
          opero = true;
          //j=operacion_simplificada.length();
        } else {
          operacion_escribir += caracter;
        }
      } else {
        operacion_escribir += caracter;
      }
    }
    opero = false;
    //Serial.println("OPS ESCRI " + operacion_escribir);
    operacion_simplificada = operacion_escribir;
    operacion_escribir = "";
    //Serial.println("OPSIMPLI " + operacion_simplificada);
  }
  //SUMAS RESTAS RESULTADO FINAL
  for (int i = 0; i < operacion_simplificada.length(); i++) {

    char caracter = operacion_simplificada.charAt(i);

    if (caracter == '-') {
      //Serial.println("ENTRO 1");
      String numero_restar = "";
      int numero_restar_int = 0;
      for (int p = i + 1; p < operacion_simplificada.length(); p++) {

        char caracter_j = operacion_simplificada.charAt(p);

        if (caracter_j == '+' || caracter_j == '/' || caracter_j == '-' || caracter_j == '*') {

          //Serial.println("Se guardo un digito resta ");
          p = operacion_simplificada.length();

        } else {

          numero_restar += caracter_j;
          i++;
        }
      }
      //Serial.println("A " + numero_restar);
      numero_restar_int = numero_restar.toInt();
      //Serial.println(numero_restar_int);
      resultado -= numero_restar_int;
      //Serial.println(resultado);
    }
    if (caracter == '+') {
      //Serial.println("ENTRO 2");
      String numero_sumar = "";
      int numero_sumar_int = 0;
      for (int p = i + 1; p < operacion_simplificada.length(); p++) {

        char caracter_j = operacion_simplificada.charAt(p);

        if (caracter_j == '+' || caracter_j == '/' || caracter_j == '-' || caracter_j == '*') {

          //Serial.println("Se guardo un digito resta ");
          p = operacion_simplificada.length();

        } else {

          numero_sumar += caracter_j;
          i++;
        }
      }
      //Serial.println("B " + numero_sumar);
      numero_sumar_int = numero_sumar.toInt();
      //Serial.println(numero_sumar_int);
      resultado += numero_sumar_int;
      //Serial.println(resultado);
    }
    if (i == 0 && isdigit(caracter)) {
      //Serial.println("ENTRO 3");
      String numero_sumar = "";
      int numero_sumar_int = 0;
      for (int p = i; p < operacion_simplificada.length(); p++) {

        char caracter_j = operacion_simplificada.charAt(p);

        if (caracter_j == '+' || caracter_j == '/' || caracter_j == '-' || caracter_j == '*') {

          //Serial.println("Se guardo un digito resta ");
          p = operacion_simplificada.length();

        } else {

          numero_sumar += caracter_j;
        }
      }
      //Serial.println("C " + numero_sumar);
      numero_sumar_int = numero_sumar.toInt();
      //Serial.println(numero_sumar_int);
      resultado += numero_sumar_int;
      //Serial.println(resultado);
    }
  }
  return resultado;
}



void setup() {
  MATRICES.begin();
  MATRICES.setIntensity(5);
  //MATRICES.displayClear();
  MATRICES.print(pass_display);

  Serial.begin(9600);
}

void loop() {

  char teclaIngresada = teclado.getKey();
  if (inicio) {
    Serial.println("---------------------");
    Serial.println("---> ESTADO ARMADO---");
    Serial.println("---------------------");
    Serial.println(" ");
    inicio = false;
  }
  if (teclaIngresada) {

    //Serial.print(intentos);
    if (intentos < 3) {

      if (estado.equals("ESTADO ARMADO")) {

        if (teclaIngresada == '*') {

          Serial.println("---------------------");
          Serial.println("---> ESTADO ARMADO---");
          Serial.println("-->CARACTER INVALIDO-");
          Serial.println("---------------------");
          Serial.println(" ");

        } else {
          if (teclaIngresada == '#') {

            if (pass.equals(pass_correcta)) {
              estado = "ESTADO DESBLOQUEADO";
              //Serial.println(estado);
              Serial.println("---------------------");
              Serial.println(">ESTADO DESBLOQUEADO-");
              Serial.println("---------------------");
              Serial.println(" ");
              pass = "";
              pass_display = "";
              MATRICES.print(pass_display);
              intentos = 0;
            } else {

              Serial.println("---------------------");
              Serial.println("---> ESTADO ARMADO---");
              Serial.println(">PASSWORD INCORRECTA-");
              Serial.println("---------------------");
              Serial.println(" ");
              pass = "";
              pass_display = "";
              MATRICES.print(pass_display);

              intentos += 1;
              if (intentos == 1) {
                pinMode(A0, OUTPUT);
                digitalWrite(A0, HIGH);
              }
              if (intentos == 2) {
                pinMode(A0, OUTPUT);
                digitalWrite(A0, HIGH);
                pinMode(A1, OUTPUT);
                digitalWrite(A1, HIGH);
              }
              if (intentos == 3) {
                pinMode(A0, OUTPUT);
                digitalWrite(A0, HIGH);
                pinMode(A1, OUTPUT);
                digitalWrite(A1, HIGH);
                pinMode(A2, OUTPUT);
                digitalWrite(A2, HIGH);
              }
            }

          } else {

            if (pass_display.length() == 4) {
              pass_display.remove(0, 1);
            }
            pass_display += "*";
            Serial.println("--->" + pass_display);
            pass += teclaIngresada;
            MATRICES.print(pass_display);
          }
        }
      } else if (estado.equals("ESTADO DESBLOQUEADO")) {
        if (teclaIngresada == '*') {
          Serial.println("---------------------");
          Serial.println("->ESPERANDO PASSWORD-");
          Serial.println("---------------------");
          Serial.println(" ");
          estado = "ESPERANDO PASSWORD";
        } else if (isdigit(teclaIngresada) || teclaIngresada == 'B') {
          Serial.println("---------------------");
          Serial.println("->ESPERANDO OPERACION-");
          Serial.println("---------------------");
          Serial.println();
          estado = "ESPERANDO OPERACION";
          if (teclaIngresada == 'B') {
            operacion_str += "-";
            operacion_display += "-";
            MATRICES.print(operacion_display);
          } else {
            operacion_str += teclaIngresada;
            operacion_display += teclaIngresada;
            MATRICES.print(operacion_display);
          }
        }
      } else if (estado.equals("ESPERANDO PASSWORD")) {
        //Serial.println("ESTOY ESPERANDO PASSWORD CADA TECLA");
        if (teclaIngresada == '#') {

          Serial.println("---------------------");
          Serial.println("->EVALUANDO PASSWORD-");
          Serial.println("---------------------");
          Serial.println();
          if (new_pass.length() == 8 || new_pass.length() == 6) {

            if (new_pass.length() == 8) {
              char caracter_examinado = new_pass[0];
              if (isalpha(caracter_examinado)) {  //letra
                caracter_examinado = new_pass[1];
                if (isdigit(caracter_examinado)) {  //numero
                  caracter_examinado = new_pass[2];
                  if (isalpha(caracter_examinado)) {  //letra
                    caracter_examinado = new_pass[3];
                    if (isalpha(caracter_examinado)) {  //letra
                      caracter_examinado = new_pass[4];
                      if (isdigit(caracter_examinado)) {  //numero
                        caracter_examinado = new_pass[5];
                        if (isdigit(caracter_examinado)) {  //numero
                          caracter_examinado = new_pass[6];
                          if (isalpha(caracter_examinado)) {  //letra
                            caracter_examinado = new_pass[7];
                            if (isdigit(caracter_examinado)) {  //numero
                              Serial.println("---------------------");
                              Serial.println("--->NUEVA PASSWORD---");
                              Serial.println("----->ALMACENADA-----");
                              Serial.println("---------------------");
                              Serial.println(" ");
                              pass_correcta = new_pass;
                              new_pass = "";
                              intentos = 0;
                              estado = "ESTADO ARMADO";
                              inicio = true;
                              correcta = true;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              } else if (isdigit(caracter_examinado)) {  //numero
                char caracter_examinado = new_pass[1];
                if (isalpha(caracter_examinado)) {  //letra
                  caracter_examinado = new_pass[2];
                  if (isdigit(caracter_examinado)) {  //numero
                    caracter_examinado = new_pass[3];
                    if (isdigit(caracter_examinado)) {  //numero
                      caracter_examinado = new_pass[4];
                      if (isalpha(caracter_examinado)) {  //letra
                        caracter_examinado = new_pass[5];
                        if (isalpha(caracter_examinado)) {  //letra
                          caracter_examinado = new_pass[6];
                          if (isdigit(caracter_examinado)) {  //numero
                            caracter_examinado = new_pass[7];
                            if (isalpha(caracter_examinado)) {  //letra
                              Serial.println("---------------------");
                              Serial.println("--->NUEVA PASSWORD---");
                              Serial.println("----->ALMACENADA-----");
                              Serial.println("---------------------");
                              Serial.println(" ");
                              pass_correcta = new_pass;
                              new_pass = "";
                              intentos = 0;
                              estado = "ESTADO ARMADO";
                              inicio = true;
                              correcta = true;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
              if (correcta == false) {
                Serial.println("---------------------");
                Serial.println("--->NUEVA PASSWORD---");
                Serial.println("------>DENEGADA------");
                Serial.println("-->PATRON INCORRECTO-");
                Serial.println("---------------------");
                new_pass = "";
                new_pass_ast = "";
                intentos = 0;
                estado = "ESPERANDO PASSWORD";
                Serial.println("---------------------");
                Serial.println("->ESPERANDO PASSWORD-");
                Serial.println("---------------------");
                Serial.println(" ");
              }
            } else if (new_pass.length() == 6) {
              char caracter_examinado = new_pass[0];
              if (isdigit(caracter_examinado)) {  //numero
                caracter_examinado = new_pass[1];
                if (isalpha(caracter_examinado)) {  //letra
                  caracter_examinado = new_pass[2];
                  if (isdigit(caracter_examinado)) {  //numero
                    caracter_examinado = new_pass[3];
                    if (isalpha(caracter_examinado)) {  //letra
                      caracter_examinado = new_pass[4];
                      if (isdigit(caracter_examinado)) {  //numero
                        caracter_examinado = new_pass[5];
                        if (isdigit(caracter_examinado)) {  //numero
                          Serial.println("---------------------");
                          Serial.println("--->NUEVA PASSWORD---");
                          Serial.println("----->ALMACENADA-----");
                          Serial.println("---------------------");
                          Serial.println(" ");
                          pass_correcta = new_pass;
                          new_pass = "";
                          intentos = 0;
                          estado = "ESTADO ARMADO";
                          inicio = true;
                          correcta = true;
                        }
                      }
                    }
                  }
                }
              } else if (isalpha(caracter_examinado)) {  //letra
                char caracter_examinado = new_pass[1];
                if (isdigit(caracter_examinado)) {  //numero
                  caracter_examinado = new_pass[2];
                  if (isalpha(caracter_examinado)) {  //letra
                    caracter_examinado = new_pass[3];
                    if (isdigit(caracter_examinado)) {  //numero
                      caracter_examinado = new_pass[4];
                      if (isalpha(caracter_examinado)) {  //letra
                        caracter_examinado = new_pass[5];
                        if (isdigit(caracter_examinado)) {  //numero
                          Serial.println("---------------------");
                          Serial.println("--->NUEVA PASSWORD---");
                          Serial.println("----->ALMACENADA-----");
                          Serial.println("---------------------");
                          Serial.println(" ");
                          pass_correcta = new_pass;
                          new_pass = "";
                          intentos = 0;
                          estado = "ESTADO ARMADO";
                          inicio = true;
                          correcta = true;
                        }
                      }
                    }
                  }
                }
              }
              if (correcta == false) {
                Serial.println("---------------------");
                Serial.println("--->NUEVA PASSWORD---");
                Serial.println("------>DENEGADA------");
                Serial.println("-->PATRON INCORRECTO-");
                Serial.println("---------------------");
                Serial.println("");
                new_pass = "";
                new_pass_ast = "";
                intentos = 0;
                estado = "ESPERANDO PASSWORD";
                Serial.println("---------------------");
                Serial.println("->ESPERANDO PASSWORD-");
                Serial.println("---------------------");
                Serial.println(" ");
              }
            }
          } else {
            Serial.println("---------------------");
            Serial.println("--->NUEVA PASSWORD---");
            Serial.println("------>DENEGADA------");
            Serial.println("--->INCORRECT SIZE---");
            Serial.println("---------------------");
            Serial.println("");
            new_pass = "";
            new_pass_ast = "";
            intentos = 0;
            estado = "ESPERANDO PASSWORD";
            Serial.println("---------------------");
            Serial.println("->ESPERANDO PASSWORD-");
            Serial.println("---------------------");
            Serial.println(" ");
            //inicio = true;
          }

        } else {

          new_pass += teclaIngresada;
          new_pass_ast += "*";
          Serial.print("Tecla: ");
          Serial.println(new_pass_ast);
          //Serial.println("NEW: " + new_pass);
        }
      } else if (estado.equals("ESPERANDO OPERACION")) {
        if (teclaIngresada == '*') {
          operacion_display = "";
          MATRICES.print(operacion_display);
        } else {
          if (teclaIngresada == '#') {
            int resultadoFinal = calculadora(operacion_str);
            Serial.print("Resultado ");
            Serial.println(resultadoFinal);

            //temporizador de operaciones
            unsigned long tiempoActual = millis();
            bool animacion = true;
            int conteo = 0;

            // Verifica si ha pasado el tiempo suficiente desde la última operación
            while (animacion) {
              int potenciometro = analogRead(A3);
              int velocidad = map(potenciometro, 1023, 0, 400, 15);

              MATRICES.setSpeed(velocidad);

              int switch1 = digitalRead(A4);
              int switch2 = digitalRead(A5);

              if (switch2 == 1) {
                if (switch1 != estadofinal) {
                  if (switch1 == 1) {
                    mensaje = "Resultado: "+String(resultadoFinal);
                    MATRICES.write(mensaje.c_str());
                    MATRICES.setTextEffect(PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
                    estadofinal = 1;
                  } else {
                    mensaje = "Resultado: "+String(resultadoFinal);
                    MATRICES.write(mensaje.c_str());
                    MATRICES.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_LEFT);
                    estadofinal = 0;
                  }
                }
              } else {
                mensaje = "Resultado: "+String(resultadoFinal);
                MATRICES.write(mensaje.c_str());
                int indiceEfecto = random(0, sizeof(animacion2));
                MATRICES.setTextEffect(animacion2[indiceEfecto], animacion2[indiceEfecto]);
              }
              if (MATRICES.displayAnimate()) {
                MATRICES.displayClear();
                MATRICES.displayReset();
              }
                conteo +=1;
                if(conteo == 5000){
                  animacion = false;
                }
            }

            estado = "ESPERANDO OPERACION";
            operacion_str = "";
            operacion_display = "";
            //teclaIngresada="*";
          }
          if (teclaIngresada == 'A') {
            operacion_str += "+";
            operacion_display += "+";
            MATRICES.print(operacion_display);
            Serial.println("--> OPERACION: " + operacion_str);
          } else if (teclaIngresada == 'B') {
            operacion_str += "-";
            operacion_display += "-";
            MATRICES.print(operacion_display);
            Serial.println("--> OPERACION: " + operacion_str);
          } else if (teclaIngresada == 'C') {
            operacion_str += "*";
            operacion_display += "*";
            MATRICES.print(operacion_display);
            Serial.println("--> OPERACION: " + operacion_str);
          } else if (teclaIngresada == 'D') {
            operacion_str += "/";
            operacion_display += "/";
            MATRICES.print(operacion_display);
            Serial.println("--> OPERACION: " + operacion_str);
          } else {
            if (teclaIngresada == '#') {

            } else {
              operacion_str += teclaIngresada;
              operacion_display += teclaIngresada;
              MATRICES.print(operacion_display);
              Serial.println("--> OPERACION: " + operacion_str);
            }
          }
        }
      }
    } else {
      estado = "MAXIMO DE INTENTOS FALLIDOS";
      //Serial.println(estado);
      Serial.println("---------------------");
      Serial.println("->MAXIMO DE INTENTOS-");
      Serial.println("----->FALLIDOS-------");
      Serial.println("---------------------");
      Serial.println(" ");
      delay(15000);
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      estado = "ESTADO ARMADO";
      Serial.println("---------------------");
      Serial.println("---> ESTADO ARMADO---");
      Serial.println("---------------------");
      intentos = 0;
    }
  }

  //MATRICES.displayAnimate();
  //int velocidad = 100;
  //MATRICES.setSpeed(velocidad);
  //MATRICES.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_RIGHT);
}

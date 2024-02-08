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
String pass_correcta="AC09C124";
String estado="ESTADO ARMADO";
int intentos =0;
MD_Parola MATRICES = MD_Parola(HARDWARE_TYPE, DATA, CLK, CS, MAX_DEV);
bool inicio = true;

const byte filas = 4;
const byte columnas = 4;

char teclas[filas][columnas] = {
  {'7', '8', '9', 'A'},
  {'4', '5', '6', 'B'},
  {'1', '2', '3', 'C'},
  {'*', '0', '#', 'D'}
};  

byte filasPines[filas] = {5,4,3,2};
byte columnasPines[columnas] = {9,8,7,6};

Keypad teclado = Keypad(makeKeymap(teclas), filasPines, columnasPines, filas, columnas);


void setup() {
  MATRICES.begin();
  MATRICES.setIntensity(5);
  //MATRICES.displayClear();
  MATRICES.print(pass_display);

  Serial.begin(9600);
}

void loop() {
  
  char teclaIngresada = teclado.getKey();
  if (inicio){
    Serial.println("---------------------");
    Serial.println("---> ESTADO ARMADO---");
    Serial.println("---------------------");
    Serial.println(" ");
    inicio = false;
  }
  if(teclaIngresada){
    
    //Serial.print(intentos);
    if(intentos < 3){
      
      if(estado.equals("ESTADO ARMADO")){
        //Serial.print(estado);
        if(teclaIngresada=='*'){
          
          Serial.println("---------------------");
          Serial.println("---> ESTADO ARMADO---");
          Serial.println("-->CARACTER INVALIDO-");
          Serial.println("---------------------");
          Serial.println(" ");

        } else{
          if(teclaIngresada=='#'){
            
            if(pass.equals(pass_correcta)){
              estado = "ESTADO DESBLOQUEADO";
              //Serial.println(estado);
              Serial.println("---------------------");
              Serial.println(">ESTADO DESBLOQUEADO-");
              Serial.println("---------------------");
              Serial.println(" ");
              pass ="";
              pass_display ="";
              MATRICES.print(pass_display);
              intentos = 0;
            }else{
              
              Serial.println("---------------------");
              Serial.println("---> ESTADO ARMADO---");
              Serial.println(">PASSWORD INCORRECTA-");
              Serial.println("---------------------");
              Serial.println(" ");
              pass ="";
              pass_display ="";
              MATRICES.print(pass_display);
              
              intentos +=1;
              if(intentos == 1){
                pinMode(A0,OUTPUT);
                digitalWrite(A0,HIGH);
              }
              if(intentos == 2){
                pinMode(A0,OUTPUT);
                digitalWrite(A0,HIGH);
                pinMode(A1,OUTPUT);
                digitalWrite(A1,HIGH);
              }
              if(intentos == 3){
                pinMode(A0,OUTPUT);
                digitalWrite(A0,HIGH);
                pinMode(A1,OUTPUT);
                digitalWrite(A1,HIGH);
                pinMode(A2,OUTPUT);
                digitalWrite(A2,HIGH);
              }
            }

          }else{
            //Serial.print("Tecla Presionada: ");
            //Serial.println(teclaIngresada);
            //mensaje = "resultado";
            //mensaje += teclaIngresada;
            if (pass_display.length() == 4) {
              pass_display.remove(0,1);
            }
            pass_display += "*";
            pass += teclaIngresada;
            MATRICES.print(pass_display);
            //Serial.println(pass_display);
            //Serial.println(pass);
          } 
        }
      } else if(estado.equals("ESTADO DESBLOQUEADO")){
          if(teclaIngresada == '*'){
            Serial.println("---------------------");
            Serial.println("->ESPERANDO PASSWORD-");
            Serial.println("---------------------");
            Serial.println(" ");
            estado = "ESPERANDO PASSWORD";
          }
      } else if (estado.equals("ESPERANDO PASSWORD")){
        Serial.println("ESTOY ESPERANDO PASSWORD CADA TECLA");
        if (teclaIngresada=='#'){
          Serial.println("---------------------");
          Serial.println("->EVALUANDO PASSWORD-");
          Serial.println("---------------------");
          if (new_pass.length() == 8 || new_pass.length() == 6){

            if(new_pass.length()==8){
               char caracter_examinado = new_pass[0];
              if(isalpha(caracter_examinado)){//letra
                 caracter_examinado = new_pass[1];
                if(isdigit(caracter_examinado)){//numero
                   caracter_examinado = new_pass[2];
                  if(isalpha(caracter_examinado)){//letra
                     caracter_examinado = new_pass[3];
                    if(isalpha(caracter_examinado)){//letra
                       caracter_examinado = new_pass[4];
                      if(isdigit(caracter_examinado)){//numero
                         caracter_examinado = new_pass[5];
                        if(isdigit(caracter_examinado)){//numero
                           caracter_examinado = new_pass[6];
                          if(isalpha(caracter_examinado)){//letra
                             caracter_examinado = new_pass[7];
                            if(isdigit(caracter_examinado)){//numero
                              Serial.println("---------------------");
                              Serial.println("--->NUEVA PASSWORD---");
                              Serial.println("----->ALMACENADA-----");
                              Serial.println("---------------------");
                              Serial.println(" ");
                              pass_correcta = new_pass;
                              new_pass ="";
                              intentos=0;
                              estado = "ESTADO ARMADO";
                              inicio = true;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              } else if(isdigit(caracter_examinado)){//numero
                char caracter_examinado = new_pass[1];
                if(isalpha(caracter_examinado)){//letra
                   caracter_examinado = new_pass[2];
                  if(isdigit(caracter_examinado)){//numero
                     caracter_examinado = new_pass[3];
                    if(isdigit(caracter_examinado)){//numero
                       caracter_examinado = new_pass[4];
                      if(isalpha(caracter_examinado)){//letra
                         caracter_examinado = new_pass[5];
                        if(isalpha(caracter_examinado)){//letra
                           caracter_examinado = new_pass[6];
                          if(isdigit(caracter_examinado)){//numero
                             caracter_examinado = new_pass[7];
                            if(isalpha(caracter_examinado)){//letra
                              Serial.println("---------------------");
                              Serial.println("--->NUEVA PASSWORD---");
                              Serial.println("----->ALMACENADA-----");
                              Serial.println("---------------------");
                              Serial.println(" ");
                              pass_correcta = new_pass;
                              new_pass ="";
                              intentos=0;
                              estado = "ESTADO ARMADO";
                              inicio = true; 
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }else{
                  Serial.println("---------------------");
                  Serial.println("--->NUEVA PASSWORD---");
                  Serial.println("------>DENEGADA------");
                  Serial.println("-->PATRON INCORRECTO-");
                  Serial.println("---------------------");
                  pass_correcta = new_pass;
                  new_pass ="";
                  intentos=0;
                  estado = "ESPERANDO PASSWORD";
                  Serial.println("---------------------");
                  Serial.println("->ESPERANDO PASSWORD-");
                  Serial.println("---------------------");
                  Serial.println(" ");
                }
            }
            else if(new_pass.length()==6){
              char caracter_examinado = new_pass[0];
              if(isdigit(caracter_examinado)){//numero
                 caracter_examinado = new_pass[1];
                if(isalpha(caracter_examinado)){//letra
                   caracter_examinado = new_pass[2];
                  if(isdigit(caracter_examinado)){//numero
                     caracter_examinado = new_pass[3];
                    if(isalpha(caracter_examinado)){//letra
                       caracter_examinado = new_pass[4];
                      if(isdigit(caracter_examinado)){//numero
                         caracter_examinado = new_pass[5];
                        if(isdigit(caracter_examinado)){//numero
                              Serial.println("---------------------");
                              Serial.println("--->NUEVA PASSWORD---");
                              Serial.println("----->ALMACENADA-----");
                              Serial.println("---------------------");
                              Serial.println(" ");
                              pass_correcta = new_pass;
                              new_pass ="";
                              intentos=0;
                              estado = "ESTADO ARMADO";
                              inicio = true; 
                        }
                      }
                    }
                  }
                }
              } else if(isalpha(caracter_examinado)){//letra
                char caracter_examinado = new_pass[1];
                if(isdigit(caracter_examinado)){//numero
                   caracter_examinado = new_pass[2];
                  if(isalpha(caracter_examinado)){//letra
                     caracter_examinado = new_pass[3];
                    if(isdigit(caracter_examinado)){//numero
                       caracter_examinado = new_pass[4];
                      if(isalpha(caracter_examinado)){//letra
                         caracter_examinado = new_pass[5];
                        if(isdigit(caracter_examinado)){//numero
                              Serial.println("---------------------");
                              Serial.println("--->NUEVA PASSWORD---");
                              Serial.println("----->ALMACENADA-----");
                              Serial.println("---------------------");
                              Serial.println(" ");
                              pass_correcta = new_pass;
                              new_pass ="";
                              intentos=0;
                              estado = "ESTADO ARMADO";
                              inicio = true; 
                            }
                          }
                        }
                      }
                    }
              }else{
                  Serial.println("---------------------");
                  Serial.println("--->NUEVA PASSWORD---");
                  Serial.println("------>DENEGADA------");
                  Serial.println("-->PATRON INCORRECTO-");
                  Serial.println("---------------------");
                  Serial.println("");
                  pass_correcta = new_pass;
                  new_pass ="";
                  intentos=0;
                  estado = "ESPERANDO PASSWORD";
                  Serial.println("---------------------");
                  Serial.println("->ESPERANDO PASSWORD-");
                  Serial.println("---------------------");
                  Serial.println(" ");
                }
            } 
          }
            else{
                  Serial.println("---------------------");
                  Serial.println("--->NUEVA PASSWORD---");
                  Serial.println("------>DENEGADA------");
                  Serial.println("--->INCORRECT SIZE---");
                  Serial.println("---------------------");
                  Serial.println("");
                  pass_correcta = new_pass;
                  new_pass ="";
                  intentos=0;
                  estado = "ESPERANDO PASSWORD";
                  pass_correcta = new_pass;
                  new_pass ="";
                  intentos=0;
                  estado = "ESPERANDO PASSWORD";
                  Serial.println("---------------------");
                  Serial.println("->ESPERANDO PASSWORD-");
                  Serial.println("---------------------");
                  Serial.println(" ");
                  //inicio = true; 
            }
          
        } else{
          
          new_pass+=teclaIngresada;
          Serial.print("Tecla: ");
          Serial.println(teclaIngresada);
          Serial.println("NEW: " + new_pass);
        }
      }
    } else{
      estado = "MAXIMO DE INTENTOS FALLIDOS";
      //Serial.println(estado);
      Serial.println("---------------------");
      Serial.println("->MAXIMO DE INTENTOS-");
      Serial.println("----->FALLIDOS-------");
      Serial.println("---------------------");
      Serial.println(" ");
      delay(15000);
      digitalWrite(A0,LOW);
      digitalWrite(A1,LOW);
      digitalWrite(A2,LOW);
      estado ="ESTADO ARMADO";
      Serial.println("---------------------");
      Serial.println("---> ESTADO ARMADO---");
      Serial.println("---------------------");
      intentos =0;
    }
    
  }
  
  //MATRICES.displayAnimate();
//int velocidad = 100;
  //MATRICES.setSpeed(velocidad);
  //MATRICES.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_RIGHT);
  
  


}

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
String pass_correcta="AC09C124";
String estado="ESTADO ARMADO";
int intentos =0;
MD_Parola MATRICES = MD_Parola(HARDWARE_TYPE, DATA, CLK, CS, MAX_DEV);

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
  if(teclaIngresada){
    if(intentos < 3){
      if(estado.equals("ESTADO ARMADO")){
        if(teclaIngresada=='*'){
          Serial.println("ESTADO BLOQUERADO (CARACTER INVALIDO)");

        } else{
          if(teclaIngresada=='#'){
            
            if(pass.equals(pass_correcta)){
              estado = "ESTADO DESBLOQUEADO";
              Serial.println(estado);
              pass ="";
              pass_display ="";
              MATRICES.print(pass_display);
              intentos = 0;
            }else{
              Serial.println("ESTADO ARMADO (PASSWORD INCORRECTA)");
              pass ="";
              pass_display ="";
              MATRICES.print(pass_display);
              intentos +=1;
            }

          }else{
            Serial.print("Tecla Presionada: ");
            Serial.println(teclaIngresada);
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
            Serial.println("ESPERANDO PASSWORD");
          }
      }
    } else{
      estado = "MAXIMO DE INTENTOS FALLIDOS";
      Serial.println(estado);
      delay(15000);
      estado ="ESTADO ARMADO";
      Serial.println(estado);
      intentos =0;
    }
  }
  
  //MATRICES.displayAnimate();
//int velocidad = 100;
  //MATRICES.setSpeed(velocidad);
  //MATRICES.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_RIGHT);
  
  


}

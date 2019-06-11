
/*
Desligar DEBUG quando usar o MIDI para no enviar os debugs como 
lixo a ser interpretado pelo Receptor MIDI
*/
//#define DEBUG



//#define LED_PIN 13
#define WAIT 11  //miliseconds

/*
Ao ler os dados pela serial usb com a biblioteca do termios, preciso
enviar CHAR de tamanho fixo
*/
#define MIDI_NOTE_ON 0x90  //channel 1
#define MIDI_NOTE_OFF 0x80 //channel 1
#define MIDI_C4 0x30 //48 apenas para identificar como C4
#define MIDI_VELOCITY_MIN 0x0
#define MIDI_VELOCITY_MAX 0x7F

byte lastState[14]; //12 para notas . o 13º pode ser oitava ou algum controle de OCT+/-, ligado OCT++, off OCT=0?

void setup(){
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  #if defined DEBUG
  Serial.print("["); Serial.print(millis()); Serial.print("] ");
  Serial.println("==> SETUP");
  #endif
  for (int i=2; i < 14 ; i++){
    pinMode(i, INPUT);
    lastState[i] = LOW;
  }  
//  Serial.println("BUF");
//  Serial.println(Serial.availableForWrite());
  //TESTE INICIALIZACAO
  /*
  for (int i=2; i < 14 ; i++){
    msg(MIDI_NOTE_ON, MIDI_C4 + i-2, 0x7F);
    delay(200);
  }
    for (int i=13; i >=2 ; i++){
    msg(MIDI_NOTE_OFF, MIDI_C4 + i, (byte) 0x00 );
    delay(200);
  }
  */
  

}

void loop(){
  for (int i=2; i < 14 ; i++) {
    if ( digitalRead(i) == HIGH && lastState[i] == LOW ){
      lastState[i] = HIGH;
      //msg(MIDI_NOTE_ON, MIDI_C4 , 0x7F);
      #if defined DEBUG
      Serial.print("\t\tNOTE ON => "); Serial.println(i);      
      #else
      msg(MIDI_NOTE_ON, MIDI_C4 + i-2, MIDI_VELOCITY_MAX);
      #endif
    } 
    
    if ( digitalRead(i) == LOW && lastState[i] == HIGH ){
      lastState[i] = LOW;
      //msg(MIDI_NOTE_OFF, MIDI_C4 , 0x00 );
      #if defined DEBUG
      Serial.print("NOTE OFF => "); Serial.println(i);
      #else
      msg(MIDI_NOTE_OFF, MIDI_C4 + i-2 , MIDI_VELOCITY_MIN );
      #endif
    }
    delay(WAIT);
  } 
}

//void loop(){
//  if ( digitalRead(1) == HIGH ){
//      msg(MIDI_NOTE_ON, MIDI_C4 + 1, 0x7F); delay(WAIT);
//  } else {
//    //msg(MIDI_NOTE_OFF, MIDI_C4 + 1, (byte) 0x00 ); delay(WAIT);
//  }
//}

/*
void loop(){

  for (int i=0; i < 12 ; i++) {
   //push button defaults to OPEN, or LOW digital level. When pressed => HIGH digital level.
    if ( digitalRead(i) == HIGH 
    //sem a condiço abaixo disparou um burst de notas, mesmo tendo resistor de 10k em todos os pinos
    && (i==10 || i==7 || i==3) //apenas 3 e 7 estao respondendo, enviando uma pancada de notas
    ){
      
      #if defined HARD_DEBUG
      Serial.print("["); Serial.print(millis()); Serial.print("] ");
      Serial.print("[PIN=OLD_STATE] ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(state[i]);
      #endif
      if ( state[i] == LOW ){
        //faz nada, deixa continuar soando
        
      } else {
        state[i] = LOW;
        msg(MIDI_NOTE_ON, MIDI_C4 + i, 0x7F);
        delay(WAIT);
        #if defined DEBUG
        Serial.print("["); Serial.print(millis()); Serial.print("] ");
        Serial.print("Send NOTE ON pin: ");
        Serial.println(i);
        #endif
      }
      
 
    } else {
      if ( state[i] == LOW ){
        state[i] = HIGH;
        msg(MIDI_NOTE_OFF, MIDI_C4 + i, (byte) 0x00 );
        #if defined DEBUG
        Serial.print("["); Serial.print(millis()); Serial.print("] ");
        Serial.println("Send NOTE OFF");
        #endif
        delay(WAIT);
      }
    }
    setLedState(state[i]);
  } 
 
 

}
*/

void msg(byte cmd, byte data1, byte data2){
//  Serial.write(cmd);
//  Serial.write(data1);
//  Serial.write(data2); 
//  Serial.println();
//  Serial.flush();
//enviando como CHAR e com \n para ler os dados pela serial usando "termios"
//vou enviar CHARs de tamanho fixo
//no receptor eu leio o dobro de chars
    Serial.print(cmd, DEC);  //128 ou 144 : 3 chars
    Serial.print(data1, DEC); // 48 a 60  : 2 chars
    if(data2 == MIDI_VELOCITY_MIN ){
      Serial.print(0, DEC);
      Serial.print(0, DEC);
      Serial.print(0, DEC);
    } else {
      Serial.println(data2, DEC); // 000 a 127 : 3 chars
    }
//Serial.print("0x90");
//Serial.print("0x30");
//Serial.println("0x7F");
//Serial.flush();
}

/*
void msg(byte cmd, byte data1, byte data2){
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2); 
}
*/

void setLedState(byte b) {
  /*
    if (b == LOW)
      digitalWrite(LED_PIN, LOW);
    else
      digitalWrite(LED_PIN, HIGH);
      */
}


/*
 8 Step Sequencer for Arduino Nano

 Features:

  - 78 square wave pitches
  - Adjustable duration for each step
  - Adjustable tempo
  - Ability to save own sequences and replay them on Preset Mode. First 4 sequences will be stored on EEPROM, others will be
  erased once the device is turned off
  - 3 Modes:
    - Sequencer Playing Mode - Each push button, combined with either the Pitch potentiometer or Duration potentiometer;
    can be used to adjust the Pitch and Duration of each step of the sequencer;
    - Sequencer Recording Mode - On this mode, pressing any of the push buttons will save the current sequencer pithches and durations,
    which can be replayed by pressing the same buttons when in Preset Mode. Limited to 8 sequences;
    - Preset Mode - 8 sequences can be played with the push of each individual push button.

 Future additions:

  - It is possible to store more sequences as presets to EEPROM with higher memory Arduino;
  - Add on an OLED to display the Mode that is selected;
  - Add a spectrum analyzer for the output sound;
  - It is possible to add polyphonic sounds for a more deep sound system.

 Settings:

  - Sequencer Playing Mode - Switch0 Pin = LOW, Switch1 Pin = LOW;
  - Sequencer Recording Mode - Switch 0 = HIGH, Switch1 Pin = LOW;
  - Preset Mode - Switch 0 = LOW, Switch1 Pin = HIGH;

  Missing:

  - Only Preset 0,1 and 2 toggle the button as a switch. This is due to low memory on Arduino
  
*/


#include "pitches.h"
#include <EEPROM.h>

int notes[]={
  0,        NOTE_B0,  NOTE_C1,  NOTE_CS1, NOTE_D1,  NOTE_DS1, NOTE_E1,  NOTE_F1,  NOTE_FS1, NOTE_G1,  NOTE_GS1, NOTE_A1,  NOTE_AS1, NOTE_B1,  
  NOTE_C2,  NOTE_CS2, NOTE_D2,  NOTE_DS2, NOTE_E2,  NOTE_F2,  NOTE_FS2, NOTE_G2,  NOTE_GS2, NOTE_A2,  NOTE_AS2, NOTE_B2,  NOTE_C3,  NOTE_CS3, 
  NOTE_D3,  NOTE_DS3, NOTE_E3,  NOTE_F3,  NOTE_FS3, NOTE_G3,  NOTE_GS3, NOTE_A3,  NOTE_AS3, NOTE_B3,  NOTE_C4,  NOTE_CS4, NOTE_D4,  NOTE_DS4, 
  NOTE_E4,  NOTE_F4,  NOTE_FS4, NOTE_G4,  NOTE_GS4, NOTE_A4,  NOTE_AS4, NOTE_B4,  NOTE_C5,  NOTE_CS5, NOTE_D5,  NOTE_DS5, NOTE_E5,  NOTE_F5,
  NOTE_FS5, NOTE_G5,  NOTE_GS5, NOTE_A5,  NOTE_AS5, NOTE_B5,  NOTE_C6,  NOTE_CS6, NOTE_D6,  NOTE_DS6, NOTE_E6,  NOTE_F6,  NOTE_FS6, NOTE_G6,  
  NOTE_GS6, NOTE_A6,  NOTE_AS6, NOTE_B6,  NOTE_C7,  NOTE_CS7, NOTE_D7,  NOTE_DS7, NOTE_E7,  NOTE_F7,  NOTE_FS7, NOTE_G7,  NOTE_GS7, NOTE_A7,  
  NOTE_AS7, NOTE_B7,  NOTE_C8,  NOTE_CS8, NOTE_D8,  NOTE_DS8
            };

int speakerPin1 = 3;      // orange wire
int speakerPin2 = 11;     // green wire

#define N_BUTTONS 8

// Input variables definitions

int button_pins[N_BUTTONS] = {17, 10, 9, 8, 7, 6, 5, 4}; // Putting button_pins into an array

int Switches_pins[] = {12, 2};

int Pitches[N_BUTTONS];         // Define array to hold the pitch. Setting it to 0.

long Durations[N_BUTTONS];      // Define array to hold the duration of the note.


int current_button;
uint32_t note_started;

int PitchPreset0[N_BUTTONS];
long DurationsPreset0[N_BUTTONS];
int PitchPreset1[N_BUTTONS];
long DurationsPreset1[N_BUTTONS];
int PitchPreset2[N_BUTTONS];
long DurationsPreset2[N_BUTTONS];
int PitchPreset3[N_BUTTONS];
long DurationsPreset3[N_BUTTONS];
int PitchPreset4[N_BUTTONS];
long DurationsPreset4[N_BUTTONS];
int PitchPreset5[N_BUTTONS];
long DurationsPreset5[N_BUTTONS];
int PitchPreset6[N_BUTTONS];
long DurationsPreset6[N_BUTTONS];
int PitchPreset7[N_BUTTONS];
long DurationsPreset7[N_BUTTONS];

int PitchPreset0addr[] = {0,1,2,3,4,5,6,7};  // EEPROM adresses to store PitchPreset0 values;
int DurationsPreset0addr[] = {8,9,10,11,12,13,14,15};  // EEPROM adresses to store DurationsPreset0 values;
int PitchPreset1addr[] = {16,17,18,19,20,21,22,23};
int DurationsPreset1addr[] = {24,25,26,27,28,29,30,31}; 
int PitchPreset2addr[] = {32,33,34,35,36,37,38,39};  
int DurationsPreset2addr[] = {40,41,42,43,44,45,46,47}; 
int PitchPreset3addr[] = {48,49,50,51,52,53,54,55}; 
int DurationsPreset3addr[] = {56,57,58,59,60,61,62,63}; 

int buttonState0 = 0; // variable for READING the pushbutton status
int variable_buttonState0 = 0;
int buttonState1 = 0; 
int variable_buttonState1 = 0;
int buttonState2 = 0; 
int variable_buttonState2 = 0;
 

void setup() {

Serial.begin(9600);


for(int pin : button_pins) pinMode(pin, INPUT_PULLUP); ////////////////////// Define the input pins for button_pins with internal pull up resistor.
                                                       ///////////////////// This means these inputs will have a value of 1 when not pressed, and 
for(int pin : Switches_pins) pinMode (pin, INPUT_PULLUP);//////////////////  0 when when pressed




}

void loop() {

int tempo = (analogRead(A2))/8;

for (int button=0; button<N_BUTTONS ; button++){
  if((digitalRead(button_pins[button])==LOW)&&(digitalRead(Switches_pins[1])==LOW)&&(digitalRead(Switches_pins[0])==LOW)){  // This ensures that when on Preset Play mode it will not change 
                                                                                      //the values of the sequencer pitches and durations
    
    Pitches[button] = analogRead(A0)/13;   // As we have 78 pitches we divide the max Potentiometer value (1023) by 78. This equals 13.
    Durations[button] = analogRead(A1);
   }
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  PRESET MODE  //////////////////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


if ((digitalRead(Switches_pins[1])==HIGH)){
  
  noTone(speakerPin1);

  
///////////////////////////////////////////////////////////////////// PRESET 0 //////////////////////////////

  buttonState0 = digitalRead(button_pins[0]);                     // Code to set push button as a switch
  
  if (buttonState0 == LOW) { 
    variable_buttonState0 = !variable_buttonState0;
    delay(300);
  }

  if (variable_buttonState0 == HIGH) {

    for (int beat=0; beat<N_BUTTONS ; beat++){                    // Code to retrieve Pitch and Durations values from EEPROM
      PitchPreset0[beat] = EEPROM.read(PitchPreset0addr[beat]);
      DurationsPreset0[beat] = EEPROM.read(DurationsPreset0addr[beat]);
    }
    
  // Are we done with the current note?
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset0[current_button] + tempo) {

        // Then move to the next note.
        ++current_button;

        // Finished the tune?
        if (current_button == N_BUTTONS) {
            current_button = 0;  // then restart from the beginning
        }

        // Play the note.
        tone(speakerPin2, notes[PitchPreset0[current_button]], DurationsPreset0[current_button]);

        // Take note of when this started.
        note_started = now;
    }
  }

/////////////////////////////////////////////////////////////// PRESET 1 /////////////////////////////////////

  buttonState1 = digitalRead(button_pins[1]);                     // Code to set push button as a switch
  
  if (buttonState1 == LOW) { 
    variable_buttonState1 = !variable_buttonState1;
    delay(300);
  }

  if (variable_buttonState1 == HIGH) {
    for (int beat=0; beat<N_BUTTONS ; beat++){
      PitchPreset1[beat] = EEPROM.read(PitchPreset1addr[beat]);
      DurationsPreset1[beat] = EEPROM.read(DurationsPreset1addr[beat]);
    }    
    
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset1[current_button] + tempo) {
      ++current_button;
        if (current_button == N_BUTTONS) {current_button = 0;}      
      tone(speakerPin2, notes[PitchPreset1[current_button]], DurationsPreset1[current_button]);
      note_started = now;
    }
  }

/////////////////////////////////////////////////////////////// PRESET 2 /////////////////////////////////////

  buttonState2 = digitalRead(button_pins[2]);                     // Code to set push button as a switch
  
  if (buttonState2 == LOW) { 
    variable_buttonState2 = !variable_buttonState2;
    delay(300);
  }

  if (variable_buttonState2 == HIGH) {
    for (int beat=0; beat<N_BUTTONS ; beat++){
      PitchPreset2[beat] = EEPROM.read(PitchPreset2addr[beat]);
      DurationsPreset2[beat] = EEPROM.read(DurationsPreset2addr[beat]);
    }    
    
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset2[current_button] + tempo) {
      ++current_button;
        if (current_button == N_BUTTONS) {current_button = 0;}      
      tone(speakerPin2, notes[PitchPreset2[current_button]], DurationsPreset2[current_button]);
      note_started = now;
    }
  }


/////////////////////////////////////////////////////////////// PRESET 3 /////////////////////////////////////

  if(digitalRead(button_pins[3])==LOW){
    for (int beat=0; beat<N_BUTTONS ; beat++){
      PitchPreset3[beat] = EEPROM.read(PitchPreset3addr[beat]);
      DurationsPreset3[beat] = EEPROM.read(DurationsPreset3addr[beat]);
    }    
    
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset3[current_button] + tempo) {
      ++current_button;
        if (current_button == N_BUTTONS) {current_button = 0;}      
      tone(speakerPin2, notes[PitchPreset3[current_button]], DurationsPreset3[current_button]);
      note_started = now;
    }
  }

/////////////////////////////////////////////////////////////// PRESET 4 /////////////////////////////////////

  if(digitalRead(button_pins[4])==LOW){
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset4[current_button] + tempo) {
      ++current_button;
        if (current_button == N_BUTTONS) {current_button = 0;}      
      tone(speakerPin2, notes[PitchPreset4[current_button]], DurationsPreset4[current_button]);
      note_started = now;
    }
  }

/////////////////////////////////////////////////////////////// PRESET 5 /////////////////////////////////////

  if(digitalRead(button_pins[5])==LOW){
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset2[current_button] + tempo) {
      ++current_button;
        if (current_button == N_BUTTONS) {current_button = 0;}      
      tone(speakerPin2, notes[PitchPreset5[current_button]], DurationsPreset5[current_button]);
      note_started = now;
    }
  }

/////////////////////////////////////////////////////////////// PRESET 6 /////////////////////////////////////

  if(digitalRead(button_pins[6])==LOW){
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset6[current_button] + tempo) {
      ++current_button;
        if (current_button == N_BUTTONS) {current_button = 0;}      
      tone(speakerPin2, notes[PitchPreset6[current_button]], DurationsPreset6[current_button]);
      note_started = now;
    }
  }

/////////////////////////////////////////////////////////////// PRESET 7 /////////////////////////////////////

  if(digitalRead(button_pins[7])==LOW){
    unsigned long now = millis();
    if (now - note_started >= DurationsPreset7[current_button] + tempo) {
      ++current_button;
        if (current_button == N_BUTTONS) {current_button = 0;}      
      tone(speakerPin2, notes[PitchPreset7[current_button]], DurationsPreset7[current_button]);
      note_started = now;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  SEQUENCER MODE PLAYING  /////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////     

if ((digitalRead(Switches_pins[1])==LOW)&&(digitalRead(Switches_pins[0])==LOW)){

  // Are we done with the current note?
    unsigned long now = millis();
    if (now - note_started >= Durations[current_button] + tempo) {

        // Then move to the next note.
        ++current_button;

        // Finished the tune?
        if (current_button == N_BUTTONS) {
            current_button = 0;  // then restart from the beginning
        }

        // Play the note.
        tone(speakerPin1, notes[Pitches[current_button]], Durations[current_button]);

        // Take note of when this started.
        note_started = now;
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  SEQUENCER MODE RECORDING  /////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if ((digitalRead(Switches_pins[1])==LOW)&&(digitalRead(Switches_pins[0])==HIGH)){
  
  for (int beat=0; beat<N_BUTTONS ; beat++){
  
    if((digitalRead(button_pins[0]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 0
      //PitchPreset0[beat] = Pitches[beat];
      //DurationsPreset0[beat] = Durations[beat];
      
      EEPROM.write(PitchPreset0addr[beat], Pitches[beat]);           // Writes Pitches[] values to the corresponding EEPROM addresses in PitchPreset0addr[]
      EEPROM.write(DurationsPreset0addr[beat], Durations[beat]);     // Writes Durations[beat] values to the corresponding EEPROM addresses in DurationsPreset0addr[]
    }

    if((digitalRead(button_pins[1]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 1
      //PitchPreset1[beat] = Pitches[beat];
      //DurationsPreset1[beat] = Durations[beat];}

      EEPROM.write(PitchPreset1addr[beat], Pitches[beat]);        
      EEPROM.write(DurationsPreset1addr[beat], Durations[beat]);     
    }

    if((digitalRead(button_pins[2]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 2
      //PitchPreset2[beat] = Pitches[beat];
      //DurationsPreset2[beat] = Durations[beat];}  

      EEPROM.write(PitchPreset2addr[beat], Pitches[beat]);        
      EEPROM.write(DurationsPreset2addr[beat], Durations[beat]);     
    }
    
    if((digitalRead(button_pins[3]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 3
      //PitchPreset3[beat] = Pitches[beat];
      //DurationsPreset3[beat] = Durations[beat];}

      EEPROM.write(PitchPreset3addr[beat], Pitches[beat]);        
      EEPROM.write(DurationsPreset3addr[beat], Durations[beat]);     
    }

    if((digitalRead(button_pins[4]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 4
      PitchPreset4[beat] = Pitches[beat];
      DurationsPreset4[beat] = Durations[beat];}

    if((digitalRead(button_pins[5]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 5
      PitchPreset5[beat] = Pitches[beat];
      DurationsPreset5[beat] = Durations[beat];}  

    if((digitalRead(button_pins[6]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 6
      PitchPreset6[beat] = Pitches[beat];
      DurationsPreset6[beat] = Durations[beat];}  

    if((digitalRead(button_pins[7]))==LOW){      // RECORD SEQUENCE VALUES (PITCHES AND DURATIONS) WHEN PRESSING BUTTON 7
      PitchPreset7[beat] = Pitches[beat];
      DurationsPreset7[beat] = Durations[beat];}  
  }  
 }
}

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
  

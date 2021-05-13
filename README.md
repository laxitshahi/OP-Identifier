# Object and Property Identifier using Arduino Due
## Motivation
    Wanted to create a system which would assist visually impaired individuals wtih:
        - Identifying specific objects 
        - Identifying specific properties of objects
    The solution?	
        - A Multi-Purpose System geared towards aiding individuals with visual impairments
    System uses:
        - 2 sensors to retrieve data 
        - 2 actuators to output the properties and object names


## Diagram and Schematic

## Components
**RGB Color Sensor**
    Main role: Identify/sense change in color, or color of an object
    
    Functionality:
        - Senses color light using photodiodes
        - Current is converted into a frequency 
        - The output is a square wave, which is proportional to light intensity
        - The intensity is read and calibrated to detect colors using the Arduino 

    Connections:
        - S0-S3 connect to ports 4-7 respectively
        - Out connects to port 9
            - Outputs Data from Color sensor to Arduino
        - Led connects to port 10
            - Turns on LED on sensor to improve clarity
        - VCC connected to 5V
            - Power source
        - GND & OE connect to ground port of Arduino


**Weight Sensor**
    Main role: Measure the weight of an object for the Arduino to read

    Functionality:
    - Load cell detects change in voltage when it is deformed
        - This causes the strain to change
    - Using this proportionally, we can record a weight value through   calibration 
        - Calibration is done using the HX711_ADC library 

    Connections:
    Load Cell
    - Red, Black, Green, White wires connect to the E+,E-,A-,A+ ports on Hx711 respectively
    Hx711 Amplifier
    - GND connects to ground on Arduino
    - DT and SCK connect to ports 22 and 23 on the Arduino
    - VCC connects to 5V 

**LCD and Speaker**
    LCD  
    Main role: Visually output the value for weight, color, or name/category of a specific object
    Functionality:
    - Utilizing the LiquidCrystal library, custom words and images can be displayed on the LCD through the Arduino    

    Simple Speakers
    Main role: Audibly (in morse code) output the value for weight, color, or category of a specific object
    How it works:
    Utilizes the Arduino Due implementation of Tone, 
    A Library that allows specific tones and frequencies of objects to be played through speaker
    Allows specific information to be played through the speaker 


## Functionality 
    The OP-Identifier uses 2 sensors and 2 acutators to indentify both the catagory/name and properties of an object.
    Makes use of the 

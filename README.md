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
    RGB Color Sensor
    Main role: Identify/sense change in color, or color of an object
    Functionality:
        - Senses color light using photodiodes
        - Current is converted into a frequency 
        - The output is a square wave, which is proportional to light intensity
        - The intensity is read and calibrated to detect colors using the Arduino 

## Functionality 
    The OP-Identifier uses 2 sensors and 2 acutators to indentify both the catagory/name and properties of an object.
    Makes use of the 

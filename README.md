# Object and Property Identifier using Arduino Due
## Motivation
    Wanted to create a system that would assist visually impaired individuals with:
        - Identifying specific objects 
        - Identifying specific properties of objects
    The solution?   
        - A Multi-Purpose System geared towards aiding individuals with visual impairments
    System uses:
        - 2 sensors to retrieve data 
        - 2 actuators to output the properties and object names


## Diagram and Schematic

## Functionality 
The OP-Identifier uses 2 sensors and 2 actuators to identify both the category/name and properties of an object.

Using the color sensor, it detects different light intensities, which in turn allows for the detection of certain colors
The Weight sensor, which is made of a load cell and hx711 amplifier, detects deformations. Using a strain to voltage proportionality, the weight of an object can be accurately measured, through the assistance of the hx711 amplifier

The functionality of the speaker is to output morse code for the color/category/name of an object
The LCD is used to display the weight/color/category/name of an object

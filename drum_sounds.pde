import ddf.minim.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.signals.*;
import ddf.minim.spi.*;
import ddf.minim.ugens.*;

import processing.serial.*; 
import processing.sound.*; 

Serial myPort; 
String inString; // String to store incoming data

SoundFile sound1, sound2, sound3, sound4;

void setup() {
 size(500, 500); 

 myPort = new Serial(this, "COM9", 115200);

 // Loading sound files
 sound1 = new SoundFile(this, "tom.mp3");
 sound2 = new SoundFile(this, "snare.mp3");
 sound3 = new SoundFile(this, "hihat.mp3");
 sound4 = new SoundFile(this, "cymbal.mp3");
 
 println("Sounds loaded");
}

void draw() {
    background(255); 

    // Checking for incoming data
    if (myPort.available() > 0) {
        inString = myPort.readStringUntil('\n'); 
        
        // Check if inString is not null before calling trim()
        if (inString != null) {
            inString = inString.trim();

            println("Received: " + inString);

            if (inString.equals("tom")) {
                sound1.play(); // Play toms
                println("Playing toms");
            } else if (inString.equals("snare")) {
                sound2.play(); // Play sound 1
                println("Playing snare");
            } else if (inString.equals("hihat")) {
                sound3.play(); // Play sound 2
                println("Playing hihat");
            } else if (inString.equals("cymbal")) {
                sound3.play(); // Play sound 3
                println("Playing cymbal");
            }
        //    else {
        //        // Stop any playing sound
        //        println("Stopping all sounds");
        //        sound1.stop();
        //        sound2.stop();
        //        sound3.stop();
        //        sound4.play(); 
        //    }
        //} else {
        //    // Handle the case where inString is null
        //    println("Received: [null]");
        //}
    }
}
}

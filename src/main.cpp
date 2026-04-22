#include <Arduino.h>
#include "model.h" // malli


const int LDR1_PIN = 33;
const int LDR2_PIN = 32;
const int LDR3_PIN = 35;
const int REDLED_PIN = 27;
const int GREENLED_PIN = 26;

// mallin alustus
Eloquent::ML::Port::OneClassSVM edgeAI;


float fVal1 = 0;
float fVal2 = 0;
float fVal3 = 0;

 // lasketaan mediaani
int getMedian(int a, int b, int c) {
    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}

void setup() {
    Serial.begin(115200);
    pinMode(REDLED_PIN, OUTPUT);
    pinMode(GREENLED_PIN, OUTPUT);
   

    // alkulukemat suodatukseen
    fVal1 = analogRead(LDR1_PIN);
    fVal2 = analogRead(LDR2_PIN);
    fVal3 = analogRead(LDR3_PIN);


}

void loop() {
    //suodatus kerrotaan vanhaa lukua 0.8 ja uutta 0.2 
    //eli vanha arvo vähenee ja uutta otetaan enemmän mukaan
    fVal1 = 0.8 * fVal1 + 0.2 * analogRead(LDR1_PIN);
    fVal2 = 0.8 * fVal2 + 0.2 * analogRead(LDR2_PIN);
    fVal3 = 0.8 * fVal3 + 0.2 * analogRead(LDR3_PIN);
    
    int val1 = (int)fVal1;
    int val2 = (int)fVal2;
    int val3 = (int)fVal3;
    
    // arvojen mediaani
    int mediaani = getMedian(val1, val2, val3);
    
    // annetaan mallille sensorien erot mediaanista ja jos poikkeaa enemmänkuin opetusdatassa niin on poikkeavaa
    float features[3] = {
        (float)abs(val1 - mediaani),
        (float)abs(val2 - mediaani),
        (float)abs(val3 - mediaani)
    };
    
    // palauttaa 1 jos vika muuten 0 
    int fault_detected = edgeAI.predict(features);
    
   // tulostus
    Serial.print(" LDR1 : "); Serial.print(val1);
    Serial.print(",") ;
    Serial.print(" LDR2 : "); Serial.print(val2);
    Serial.print(",");
    Serial.print(" LDR3: "); Serial.print(val3);
    Serial.print(",");
    Serial.print(" mediaani: ");
    Serial.print(mediaani);
    
    //vika
    if (fault_detected == 1) {
        Serial.println(" sensorissa vika ");
        digitalWrite(REDLED_PIN, HIGH); 
        digitalWrite(GREENLED_PIN, LOW);   
     
    //ok 
    } else {
        Serial.println(" sensorit ok ");
        digitalWrite(REDLED_PIN, LOW); 
        digitalWrite(GREENLED_PIN, HIGH); 
    }
    
    // 10hz = 10 per sek
    delay(100); 
}
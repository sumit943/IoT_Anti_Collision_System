
/**
 * ------------------------------------------------------------------
--------------------------------------------------
 * Example sketch/program showing how to read data from more than one
PICC to serial.
* ------------------------------------------------------------------
--------------------------------------------------
 * This is a MFRC522 library example; for further details and other
examples see: https://github.com/miguelbalboa/rfid
 *
7 * Example sketch/program showing how to read data from more than one
PICC (that is: a RFID Tag or Card) using a
8 * MFRC522 based RFID Reader on the Arduino SPI interface.
9 *
10 * Warning: This may not work! Multiple devices at one SPI are
difficult and cause many trouble!! Engineering skill
11 * and knowledge are required!
12 *
13 * @license Released into the public domain.
14 *
15 * Typical pin layout used:
16 * ------------------------------------------------------------------
-----------------------
17 * MFRC522 Arduino Arduino Arduino
Arduino Arduino
18 * Reader/PCD Uno/101 Mega Nano v3
Leonardo/Micro Pro Micro
19 * Signal Pin Pin Pin Pin Pin
Pin
20 * ------------------------------------------------------------------
-----------------------
21 * RST/Reset RST 9 5 D9
RESET/ICSP-5 RST
22 * SPI SS 1 SDA(SS) ** custom, take a unused pin, only
HIGH/LOW required **
23 * SPI SS 2 SDA(SS) ** custom, take a unused pin, only
HIGH/LOW required **
24 * SPI MOSI MOSI 11 / ICSP-4 51 D11 ICSP-4
16
 * SPI MISO MISO 12 / ICSP-1 50 D12 ICSP-1

 * SPI SCK SCK 13 / ICSP-3 52 D13 ICSP-3




 *
 */
  #include <SPI.h>
  #include<MFRC522.h>
  
  #define RST_PIN         9          // Configurable, see typical pin layout above
  #define SS_1_PIN        10         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
  #define SS_2_PIN        8          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1
  
  #define NR_OF_READERS   2
  
  byte ssPins[] = {SS_1_PIN, SS_2_PIN};
  
  MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.
  
  /**
   * Initialize.
   */
  void setup() {
    pinMode(4,OUTPUT);
    Serial.begin(9600); // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  
    SPI.begin();        // Init SPI bus
  
    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
      mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
      Serial.print(F("Reader "));
      Serial.print(reader);
      Serial.print(F(": "));
      mfrc522[reader].PCD_DumpVersionToSerial();
    }
  }
  
  /**
   * Main loop.
   */
  void loop() {
      //int* b=(int *)malloc(sizeof(int)*4);
    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
      // Look for new cards
      
      if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
        Serial.print(F("Reader "));
        Serial.print(reader);
        // Show some details of the PICC (that is: the tag/card)
        Serial.print(F(": Card UID:"));
        dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
        Serial.println();
        Serial.print(F("PICC type: "));
        MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
        Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));
  
        // Halt PICC
        mfrc522[reader].PICC_HaltA();
        // Stop encryption on PCD
        mfrc522[reader].PCD_StopCrypto1();
      } //if (mfrc522[reader].PICC_IsNewC
    } //for(uint8_t reader
  }
  
  /**
   * Helper routine to dump a byte array as hex values to Serial.
   */
  int* dump_byte_array(byte *buffer, byte bufferSize) {
    int c[]={176,240,158,94};
    int count=0;
    for (byte i = 0; i < bufferSize; i++) {
      //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
    }
    Serial.println();
    for (byte i = 0; i < bufferSize; i++) {
      //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      if(buffer[i]==c[i]){
        count++;
      }
    }
    int flag=0;
    if(count==4){
      digitalWrite(4,HIGH);
    }else{
      digitalWrite(4,LOW);
    }
  }


#include <Wire.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <MFRC522.h>
#include <NfcAdapter.h>

U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

const String storedString = "SecretPassword";

#define RST_PIN 4     // Reset pin
#define SS_PIN 5     // Slave select pin 

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
NfcAdapter nfc = NfcAdapter(&mfrc522);  // Create NfcAdapter instance

String key = "";
String salt = "";
String iv = "" ;

void setup() {

  #ifdef U8G2LIB_H
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setFont(u8g2_font_profont12_tf);  // Change font to ProFont12 (slightly larger size)
  u8g2.drawStr(0, 30, "Connect to the device");  // Shift the text lower
  u8g2.sendBuffer();
  #endif

  SPI.begin();         // Initialize SPI bus
  mfrc522.PCD_Init();  // Initialize MFRC522
 
  while (!Serial) {}  // Wait for serial connection to be established
}

void loop() {
  if (authenticateSerialInput()) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 30, "Connection made");  // Shift the text lower
    u8g2.sendBuffer();
    delay(2000);  // Display "Connection made" for 2 seconds
    executeTask();
  }
}

bool authenticateSerialInput() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equals(storedString)) {
      return true;
    } else {
      Serial.println("Authentication failed. Please provide the correct password.");
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_profont12_tf);  // Change font to ProFont12 (slightly larger size)
      u8g2.drawStr(0, 30, "Connect to the device");  // Shift the text lower
      u8g2.sendBuffer();
      delay(2000);  // Display "Connect to the device" for 2 seconds
    }
  }
  return false;
}

bool authenticateDataRequest() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.equals("SEND")) {
      return true;
    }
  }
  return false;
}

void executeTask() {
  while (true) {
  if(authenticateDataRequest()){
    Serial.println(key);
    Serial.println(salt);
    Serial.println(iv);
  }
  readNdefTag();
}
}

void readNdefTag() {
  // Check if a tag is present
  if (nfc.tagPresent()) {
    // Read the tag and retrieve the NdefMessage
    NfcTag tag = nfc.read();
    NdefMessage message = tag.getNdefMessage();

    // Get the number of records in the NdefMessage
    int recordCount = message.getRecordCount();

    // Iterate over each record in the NdefMessage
    for (int i = 0; i < recordCount; i++) {
      NdefRecord record = message.getRecord(i);
      
      // Check if the record is a text record manually
      byte tnf = record.getTnf();
      if (tnf == 0x01 && record.getTypeLength() == 1 && record.getType()[0] == 0x54) {
        // Get the payload bytes of the record
        const byte *payload = record.getPayload(); // if something breaks it is this bastard
        int payloadLength = record.getPayloadLength();
        
        // Convert the payload bytes to a String
        String text;
        for (int j = 3; j < payloadLength; j++) {
          text += (char)payload[j];
        }

        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_profont12_tf);  // Change font to ProFont12 (slightly larger size)
        u8g2.drawStr(0, 30, "scanning..");  // Shift the text lower
        u8g2.sendBuffer();
        
        // store the records in variable
        if(i == 0){
          key=text;

           u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_profont12_tf);  // Change font to ProFont12 (slightly larger size)
        u8g2.drawStr(0, 30, "scanning..");  // Shift the text lower
        u8g2.sendBuffer();
        
        }
        else if(i==1){
          salt=text;
        
           u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_profont12_tf);  // Change font to ProFont12 (slightly larger size)
        u8g2.drawStr(0, 30, "scanning...");  // Shift the text lower
        u8g2.sendBuffer();
        
        }
        else if(i==2){
          iv=text;
        
           u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_profont12_tf);  // Change font to ProFont12 (slightly larger size)
        u8g2.drawStr(0, 30, "scan complete!");  // Shift the text lower
        u8g2.sendBuffer();
        delay(1000);
        }
        else{
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_profont12_tf);  // Change font to ProFont12 (slightly larger size)
        u8g2.drawStr(0, 30, "Additional unknown");  // Shift the text lower
        u8g2.sendBuffer();
        }
      }
    }
  }
}
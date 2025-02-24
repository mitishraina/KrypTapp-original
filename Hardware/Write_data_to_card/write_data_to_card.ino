#include <SPI.h>
#include <MFRC522.h>
#include <NfcAdapter.h>

#define RST_PIN 4     // Reset pin
#define SS_PIN 5     // Slave select pin 

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
NfcAdapter nfc = NfcAdapter(&mfrc522);  // Create NfcAdapter instance

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  SPI.begin();         // Initialize SPI bus
  mfrc522.PCD_Init();  // Initialize MFRC522

  Serial.println("Place your tag near the reader...");
}

void loop() {
  // Check if a tag is present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
    writeNdefTag("nWpycXeI7o0//EABOO/weA==", "ae602827301badabbae071a1543321d3", "616d93ec4c8a864c35d260e2037288b5");
  }
}

void writeNdefTag(const char* data1, const char* data2, const char* data3) {
  // Read the tag and retrieve the NdefMessage
  NfcTag tag = nfc.read();
  NdefMessage message = tag.getNdefMessage();

  // Create a new NdefMessage
  NdefMessage newMessage = NdefMessage();

  // Add the desired data strings as text records to the new NdefMessage
  newMessage.addTextRecord(data1);
  newMessage.addTextRecord(data2);
  newMessage.addTextRecord(data3);

  // Write the new NdefMessage to the tag
  bool success = nfc.write(newMessage);

  if (success) {
    Serial.println("NDEF message written to tag successfully.");
  } else {
    Serial.println("Error writing NDEF message to tag.");
  }
}
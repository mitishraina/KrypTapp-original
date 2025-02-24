#include <SPI.h>
#include <MFRC522.h>
#include <NfcAdapter.h>

#define RST_PIN 4     // Reset pin
#define SS_PIN 5     // Slave select pin 

MFRC522 mfrc522(SS_PIN, RST_PIN);      // Create MFRC522 instance
NfcAdapter nfc = NfcAdapter(&mfrc522); // Create NfcAdapter instance

void setup()
{
  Serial.begin(9600); // Initialize serial communication
  SPI.begin();        // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522

  Serial.println("Place your tag near the reader...");
}

void loop()
{
  readNdefTag();
}

void readNdefTag()
{
  // Check if a tag is present
  if (nfc.tagPresent())
  {
    // Read the tag and retrieve the NdefMessage
    NfcTag tag = nfc.read();
    NdefMessage message = tag.getNdefMessage();

    // Get the number of records in the NdefMessage
    int recordCount = message.getRecordCount();

    // Iterate over each record in the NdefMessage
    for (int i = 0; i < recordCount; i++)
    {
      NdefRecord record = message.getRecord(i);

      // Check if the record is a text record manually
      byte tnf = record.getTnf();
      if (tnf == 0x01 && record.getTypeLength() == 1 && record.getType()[0] == 0x54)
      {
        // Get the payload bytes of the record
        byte *payload = record.getPayload();
        int payloadLength = record.getPayloadLength();

        // Convert the payload bytes to a String
        String text;
        for (int j = 3; j < payloadLength; j++)
        {
          text += (char)payload[j];
        }

        // Print the text content on the serial monitor
        Serial.print("Record ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(text);
      }
    }
  }
}
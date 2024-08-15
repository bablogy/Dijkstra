
#include <SD.h>
#include <SPI.h>

File myFile;

void setup()
{
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("graph0.txt")) {
    Serial.println("graph0.txt exists.");
  }
  else {
    Serial.println("graph0.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Creating graph0.txt...");
  myFile = SD.open("graph0.txt", FILE_WRITE);
   //SD.remove("graph0.txt");
 
myFile.write("000 085 217 000 173 000 000 000 000 000\n");
myFile.write("085 000 000 000 000 080 000 000 000 000\n");
myFile.write("217 000 000 000 000 000 186 103 000 000\n");
myFile.write("000 000 000 000 000 000 000 183 000 000\n");
myFile.write("173 000 000 000 000 000 000 000 000 502\n");
myFile.write("000 080 000 000 000 000 000 000 250 000\n");
myFile.write("000 000 186 000 000 000 000 000 000 000\n");
myFile.write("000 000 103 183 000 000 000 000 000 167\n");
myFile.write("000 000 000 000 000 250 000 000 000 084\n");
myFile.write("000 000 000 000 502 000 000 167 084 000");
  myFile.close();

  // Check to see if the file exists: 
  if (SD.exists("graph0.txt")) {
    Serial.println("graph0.txt exists.");
  }
  else {
    Serial.println("graph0.txt doesn't exist.");  
  }

  // delete the file:
  //Serial.println("Removing graph0.txt...");
  //SD.remove("graph0.txt");

   }

void loop()
{
  // nothing happens after setup finishes.
}

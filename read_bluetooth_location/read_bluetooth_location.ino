/*The ability to pipe Latitude/Longitude from a phone to Android via UART
 * 
 * By MoCo Makers - mocomakers.com
 * 
 * For use with 
 * Adafruit Bluefruit LE UART Friend module
 * & The Adafruit Bluefruit LE Connect mobile app
 * 
 * 
 * Bluetooth UART -> Ardunio
 *            TX0 -> RX
 *            CTS -> GND
 *            GND -> GND
 *            VIN -> 5V
 * --Set Bluetooth UART to UART mode
 * --Setting CTS to ground is the hardware way of establishing a UART pass-through mode
 * --Note, you can't program an Arduino via USB, if the RX line is connected to the module, 
 *   unplug it till after the upload is complete
 *   
 *In the mobile app, Connect to your device and select 'Controller'. Go down to the Location
 *data stream and turn it on. Then, open the Arduino IDE's 'Serial Monitor'.
 *   
 *Inspired by https://github.com/adafruit/Adafruit_BluefruitLE_nRF51/tree/master/examples/controller
 */

String readString;

float parsefloat(uint8_t *buffer) 
{
  float f;
  memcpy(&f, buffer, 4);
  return f;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Tracking is beginning.."); //An optional introduction
}

void loop() {

  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    readString += c;
  }

  if (readString.length() >0) {
    readString=readString;
    char charBuf[14] = "";
    readString.toCharArray(charBuf, 14);  //   ! + L + 4 bytes Lat + 4 bytes Long  + 4 bytes Alt = 14 Bytes
                                          // Alt is missing in mobile app

    float lat, lon, alt;
    
    lat = parsefloat(charBuf+2);
    lon = parsefloat(charBuf+6);
    alt = parsefloat(charBuf+10);
    
    Serial.print("GPS Location\t");
    Serial.print("Lat: "); Serial.print(lat, 6); // 5 digits of precision!
    Serial.print('\t');
    Serial.print("Lon: "); Serial.print(lon, 6); // 5 digits of precision!

    //The mobile app is only transmitting enough bytes for Lat and Long. 
    //This is left here for future reference.
    //Serial.print('\t'); Serial.print(alt, 6); Serial.println(" meters");

    //This shows a 'text-based' version of the UART data
    //Serial.println(readString);

    readString="";
  }
}

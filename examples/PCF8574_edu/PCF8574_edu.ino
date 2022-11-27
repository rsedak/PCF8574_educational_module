/* PCF8574 educational module
 *  
 * Created 2022 by Robert Sedak
 * 
 * This example code is in the public domain.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR(S) OR 
 *  CONTRIBUTOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
 *  THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
 #include <Wire.h>

const int PCF_address = 0x27;
const byte interruptPin = 2;
volatile byte PCF_int = LOW;
byte PCF_state = 0;

void setup() {
  // initialize digital pin 2 as an input with enabled internal pull up resistor
  pinMode(interruptPin, INPUT_PULLUP);
  // start I2C communication
  Wire.begin();
  // on every port D2 state change run function pcf_ISV()
  attachInterrupt(digitalPinToInterrupt(interruptPin), pcf_ISV, CHANGE);
  // test if PCF module LEDs are working
  for (byte i = 1; i <= 5; i++){
    PCF_state = 0B00001111; // turn on all PCF module LEDs
    PCF_write(PCF_address, PCF_state);
    delay(100);
    PCF_state = 0B11111111; // turn off all PCF module LEDs
    PCF_write(PCF_address, PCF_state);
    delay(100);
  }
} // end setup()

void loop() {
  // if any of switches had been pressed
  if (PCF_int) {
    PCF_state = PCF_read(PCF_address);  // read switches state
    PCF_state = PCF_state << 4;         // copy switch states to LED ports
    PCF_state = PCF_state | 0B00001111; // define switch ports are input ports
    PCF_write(PCF_address, PCF_state);  // send 
  }
}

void pcf_ISV(){
  PCF_int = !PCF_int;  // change state on any switch press
} // end pcf_ISV()

void PCF_write(int PCF_address, byte PCF_state){
  // send ports state to PCF
  Wire.beginTransmission(PCF_address);
  Wire.write(PCF_state);
  Wire.endTransmission();
} // end PCF_write()

byte PCF_read(int PCF_address){
  // read ports state from PCF
  byte PCF_state;
  Wire.requestFrom(PCF_address, 1);
  while (Wire.available()) {
    PCF_state = Wire.read(); 
  }
  return PCF_state;
} // end PCF_read()

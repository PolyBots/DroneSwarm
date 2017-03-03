#define led 5
#define push_button 2


/*  tone - (pin, frequency)*/
/*  noTone - (pin)*/

//  s1 = whip antenna
//  s2 = chip antenna


bool push_button_energy = LOW;
bool lock_out = true;

unsigned long previous_count = 0;
unsigned long debounce_time = 50;
unsigned long current_count = millis();

void check_button();
void  transmit_to_master(char *data_to_transmit, int number_of_bytes);


void setup()
{
  pinMode(push_button, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(1000);

  current_count = millis();
}


void loop()
{
  check_button();
  if(Serial.available())
  {
    check_incoming_data();
  }
  
}


void check_button()
{
  
  char  turn_on[6] = "s2_on";
  char  turn_off[7] = "s2_off";
  
  current_count = millis();

  if (((push_button_energy = digitalRead(push_button)) == HIGH) && (lock_out == true) && ((current_count - previous_count) > debounce_time))
  {
    lock_out = !lock_out;
    
    transmit_to_master(turn_on, 6);
    
    current_count = millis();
    previous_count = current_count;
  }  
  
  current_count = millis();
  if (((push_button_energy = digitalRead(push_button)) == LOW) && (lock_out == false) && ((current_count - previous_count) > debounce_time))
  {
    lock_out = !lock_out;
      
    transmit_to_master(turn_off, 7);
    
    current_count = millis();
    previous_count = current_count;
  }
}


void  transmit_to_master(char *data_to_transmit, int number_of_bytes)
{
  int x;
  
  for(x = 0; x <= (number_of_bytes - 1); x++)
  {
    Serial.write(data_to_transmit[x]);
  }
}


void check_incoming_data()
{
  char incoming_data[20] = "";

  Serial.readBytesUntil('\0', &(*incoming_data), 20);

  if(strcmp(incoming_data, "s2_on") == 0)
  {
    digitalWrite(led, HIGH);
    Serial.println("turn on");
  }
  if(strcmp(incoming_data, "s2_off") == 0)
  {
    digitalWrite(led,LOW);
    Serial.println("turn off");
  }
}

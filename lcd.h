
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

void InitialiseLCD()
{
    //setup lcd
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Mr Choi's Oven");
}

void OvenStateMachine(float temperature, bool doneFlag)
{
   lcd.clear();

   if(temperature < 150 && doneFlag == false)
   {
     lcd.print("preheating:");
   } 
   else if(temperature >= 150 && temperature < 180)
   {
      lcd.print("Soaking:"); 
   } 
   else if(temperature >=180 && temperature < 215)
   {
      lcd.print("Reflowing:");   
   }
   else if(temperature > 215)
   {
       lcd.print("Coolng:");   
   }
   else if(doneFlag == true)
   {
     lcd.print("Completed" );
   }
   
   float tempKelvin = temperature + 273.15;
   lcd.print(temperature);
   lcd.print(" ");
   lcd.print(tempKelvin);
   lcd.print(" Kelvin");
}
void ClearLCD()
{
 lcd.clear(); 
}

void PrintMessage(String message)
{
  lcd.print(message);
}

// #defines will be used to store certain analog values
#define sensor1 pin_number
#define sensor2  pin_number
#define thresh_exp 200
//variable description
//
int val;
int sens1;
int sens2;
int th1;
void setup() 
{
	pinMode(sensor1,INPUT);
	pinMode(sensor2,INPUT);
	sens1=analogRead(sensor1);
	sens2=analogRead(sensor2); 
			
}

void loop() 
{
 


}

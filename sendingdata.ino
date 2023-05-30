#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

unsigned long timing; // Перемен  ная для хранения точки отсчета
int pause = 5000; //Пауза опроса датчика 

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(10,0,1,35);  // IP of the MySQL *server* here
char user[] = "root";           // MySQL user login username
char password[] = "secret";     // MySQL user login password

// Sample query
char INSERT_DATA[] = "INSERT INTO test_arduino.hello_sensor (message, sensor_num, value) VALUES ('%s',%d,%s)";
char query[128];
char temperature[10];

EthernetClient client;
MySQL_Connection conn((Client *)&client);


void setup() {
   Serial.begin(9600);     // запускаем монитор порта
   pinMode(A1, INPUT); // к входу A1 подключаем потенциометр

   Serial.begin(115200);
   while (!Serial); // wait for serial port to connect
   Ethernet.begin(mac_addr);
   Serial.println("Connecting...");
   if (conn.connect(server_addr, 3306, user, password)) 
   {
    Serial.println("Connected");
    }
}

void loop() {


if (millis() - timing > pause){
  // Initiate the query class instance
   MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
   // Save
   dtostrf(50.125, 1, 1, temperature);
   sprintf(query, INSERT_DATA, "test sensor", 24, temperature);
   // Execute the query
   cur_mem->execute(query);
   // Note: since there are no results, we do not need to read any data
   // Deleting the cursor also frees up memory used
   delete cur_mem;
   Serial.println("Data recorded.");
   int val = sumFunction();
   Serial.println(val);
    }
    else
   Serial.println("Connection failed.");
    conn.close();
}



int sumFunction() {
  if (millis() - timing > 5000){ // Вместо 10000 подставьте нужное вам значение паузы 
  timing = millis(); 
  Serial.println ("Sending data");
  int val = analogRead(A1); // считываем данные с порта A1
  //Serial.println(val);             // выводим данные на монитор порта
  return val;
 }
}




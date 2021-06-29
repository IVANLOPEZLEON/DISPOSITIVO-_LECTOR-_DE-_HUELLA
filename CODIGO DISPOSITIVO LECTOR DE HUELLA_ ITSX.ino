#include <Wire.h>                          // Activa las resistencias internas de Pull-UP
#include <LiquidCrystal_I2C.h>             // Libreria  para comunicacion i2c
LiquidCrystal_I2C lcd(0x27,16,2);          //  Constructor de la clase, configura el hardware.         
                                                    
#include <Adafruit_Fingerprint.h>          // librerias para el lector de huella As608
#include <SoftwareSerial.h>                 
int getFingerprintIDez();                   
SoftwareSerial mySerial(10, 11);           // Estos dos pines nos serviran para recibir y transmitir informacion con el lector de huella                             
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// VARIABLES GLOBALES DE ENTRADAS Y SALIDAS////////////////
int con_servicios = 13;                   // pin escanear huella y activar servicios
int add_id_pin = 12;                      // pin para agregar nuevas huellas
int sin_servicios = 9;                    // pin para apagar servicios          
int relay= 2;                             // pín digital para salida de señal activar servicios
int relay2=3;                             // pin digital para salida de señal  apagar servicios

// EDITAR VARIABLES//
int admi_ID = 18;                       // Cambio de usuario administrador

//////////////////////////////////////////////////////////////////////////////////////////////////////


bool servicio = false;
int counter = 0;
int id_ad_counter = 0;
bool id_ad = false;
uint8_t num = 1;
bool id_selected = false;
uint8_t id;
bool first_lectura = false;
bool usuario_principal = false;
bool add_usuario = false;
bool no_servicios = true;




void setup() {
  Serial.begin(57600);              //Comienza comunicacion serial del lector de huella TX RX data.
           
   
  lcd.init();                      //  Prepara el LCD para su uso.
  lcd.backlight();                 //   Métodos para encender / apagar la iluminación de fondo
  lcd.setCursor(0,0);              //   Permite mover el cursor a la posición indicada en sus parámetros.
  lcd.print("      ITSX    ");     //   Mensaje a desplegar
  lcd.setCursor(0,1);              //   Permite mover el cursor a la posición indicada en sus parámetros.
  lcd.print(" -BIENVENIDOS- ");    //    Mensaje 2 a desplegar


// SE DEFINEN LAS ENTRAS O SALIDAS //
  pinMode (relay,OUTPUT); 
  pinMode (relay2,OUTPUT);
  pinMode(con_servicios,INPUT);  
  pinMode(add_id_pin,INPUT);  
  pinMode(sin_servicios,INPUT); 
  finger.begin(57600);             //Comienza comunicacion serial del lector de huella TX RX data.
  


}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////VOID LOOP////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {
/////////////////////////////////////////APAGADO DE SERVICIOS///////////////////////////////////
  if(digitalRead(sin_servicios))                       
  {
    no_servicios = true;
    digitalWrite(relay2,HIGH); 
    digitalWrite(relay,LOW); 

    lcd.init();                     
    lcd.setCursor(0,0);
    lcd.print(" SIN SERVICIOS!");
    lcd.setCursor(0,1);
    lcd.print("                "); 
    delay(500);     
    lcd.setCursor(0,0);
    lcd.print("PRESIONA ESCANER  ");
    lcd.setCursor(0,1);
    lcd.print(" -SIN SERVICIOS- ");
   
  }
 
   


////////////////////////////////ESCANEAR HUELLA/////////////////////////////////////////////////
 if(digitalRead(con_servicios) && !id_ad)
 {
  
  servicio = true;
  
  lcd.init();                      
  lcd.setCursor(0,0);
  lcd.print("INGRESA HUELLA ");
  lcd.setCursor(0,1);
  lcd.print("ESCANEANDO--------");
 }
  
 while(servicio && counter <= 60)
 {
  getFingerprintID();
  delay(100); 
  counter = counter + 1;
  if(counter == 10)
  {
    lcd.setCursor(0,0);
    lcd.print("INGRESA HUELLA  ");
    lcd.setCursor(0,1);
    lcd.print("ESCANEANDO  ------");
  }

  if(counter == 20)
  {
    lcd.setCursor(0,0);
    lcd.print("INGRESA HUELLA ");
    lcd.setCursor(0,1);
    lcd.print("ESCANEANDO   ----");
  }

  if(counter == 40)
  {
    lcd.setCursor(0,0);
    lcd.print("INGRESA HUELLA");
    lcd.setCursor(0,1);
    lcd.print("ESCANEANDO     --");
  }

  if(counter == 50)
  {
    lcd.setCursor(0,0);
    lcd.print("INGRESA HUELLA");
    lcd.setCursor(0,1);
    lcd.print("ESCANEANDO");
  }
  if(counter == 59)
  {
    lcd.init();
    lcd.setCursor(0,0);
    lcd.print(" TIEMPO LIMITE! ");
    lcd.setCursor(0,1);
    lcd.print("PRUEBA DE NUEVO");
    
    delay(1000);
    if(no_servicios)
    {
      lcd.setCursor(0,0);
      lcd.print("PRESIONA ESCANER");
      lcd.setCursor(0,1);
      lcd.print("-SIN SERVICIOS- ");   
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("SERVICIOS");
      lcd.setCursor(0,1);
      lcd.print("HABILITADOS ");  
    }
   }
   
 }
 servicio = false;
 counter = 0;
///////////////////////////////TERMINA LA PARTE DEL ESCANEO



/////////////////////////////////////////AGREGAR NUEVO USUARIO///////////////////////////////////////////
if(digitalRead(add_id_pin) && !id_ad)
 {

  add_usuario= true;
  lcd.init();                     
  lcd.setCursor(0,0);
  lcd.print("HUELLA ADMIN");
  lcd.setCursor(0,1);
  lcd.print("PON TU HUELLA");  
  
  while (id_ad_counter < 40 && !usuario_principal)
  {
    getFingerprintID();
    delay(100);  
    id_ad_counter = id_ad_counter+1;
    if(!add_usuario)
    {
      id_ad_counter = 40;
    }
  }
  id_ad_counter = 0;
  add_usuario = false;

  if(usuario_principal)
  { 
    lcd.init();                     
    lcd.setCursor(0,0);
    lcd.print("AGREGAR NUEVO ID A ");
    lcd.setCursor(0,1);
    lcd.print(" BASE DE DATOS  ");  
    delay(1500);
  
    numero_num(num);  
    id_ad = true;
    
  }
  else
  { 
    lcd.init();                      
    lcd.setCursor(0,0);
    lcd.print("ERROR");
    lcd.setCursor(0,1);
    lcd.print("NO ID# DE ADMIN");  
    delay(1500); 
    if(no_servicios)
    {
      lcd.setCursor(0,0);
      lcd.print("PRESIONA ESCANER");
      lcd.setCursor(0,1);
      lcd.print(" -SIN SERVICIOS- ");   
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("SERVICIOS");
      lcd.setCursor(0,1);
      lcd.print(" HABILITADOS  ");  
    }
    id_ad = false;
    
  }
 }

if(digitalRead(con_servicios) && id_ad)
 {  
  
  id=num;
  while (!  getFingerprintEnroll() );
  id_ad = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" NUEVO ID GUARDADO  ");
  lcd.setCursor(4,1);
  lcd.print("as ID #"); 
  lcd.setCursor(11,1);
  lcd.print(id);  
  delay(3000);
  if(no_servicios)
    {
      lcd.setCursor(0,0);
      lcd.print("PRESIONA ESCANER ");
      lcd.setCursor(0,1);
      lcd.print("-SIN SERVICIOS-");   
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("SERVICIOS");
      lcd.setCursor(0,1);
      lcd.print("HABILITADOS");  
    }
  add_usuario = false;
  usuario_principal = false;
  id_ad = false;
 
  
 }

if(digitalRead(add_id_pin) && id_ad)
 {
  num = num + 1;
  if(num > 17)
  {
    num=1;
  }
  numero_num(num);  
 }

}//termina ciclo loop


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


/*Esta funcion permite almacenar ahsta 17 ID donde quiera el usuario guardalo*/
void numero_num(uint8_t)
{
  if (num == 1)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID ");
    lcd.setCursor(0,1);
    lcd.print(">1  2   3   4   ");  
    delay(500);
  }
  if (num == 2)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 1 >2   3   4   ");  
    delay(500);
  }
  if (num == 3)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 1  2  >3   4   ");  
    delay(500);
  }
  if (num == 4)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 1  2   3  >4   ");  
    delay(500);
  }
  if (num == 5)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(">5  6   7   8   ");  
    delay(500);
  }
  if (num == 6)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 5 >6   7   8   ");  
    delay(500);
  }
  if (num == 7)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 5  6  >7   8   ");   
    delay(500);
  }
  if (num == 8)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 5  6   7  >8   ");   
    delay(500);
  }
  if (num == 9)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(">9  10  11  12  ");  
    delay(500);
  }
  if (num == 10)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 9 >10  11  12  ");  
    delay(500);
  }
  if (num == 11)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 9  10 >11  12  ");  
    delay(500);
  }
  if (num == 12)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 9  10  11 >12  ");  
    delay(500);
  }
  if (num == 13)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(">13  14  15  16 ");  
    delay(500);
  }
  if (num == 14)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 13 >14  15  16 ");  
    delay(500);
  }
  if (num == 15)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 13  14 >15  16 ");  
    delay(500);
  }
  if (num == 16)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 13  14  15 >16 ");  
    delay(500);
  }
  if (num == 17)
  {
    lcd.setCursor(0,0);
    lcd.print("SELECCIONE ID");
    lcd.setCursor(0,1);
    lcd.print(" 14  15  16 >17 ");  
    delay(500);
  }
}






/*ESTA FUNCION LEERA LA HUELLA IMPRESA DE LA MEMORIA DEL LECTOR DE HUELLA*/

uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();               
  switch (p)
  { 
    case FINGERPRINT_OK:                           // Imagen tomada
    break;
    case FINGERPRINT_NOFINGER: return p;           // huella no detectada
    case FINGERPRINT_PACKETRECIEVEERR: return p;   //error de comunicacion
    case FINGERPRINT_IMAGEFAIL: return p;          // error de imagen
    default: return p;                             // error desconocido
  } 
// 

  p = finger.image2Tz();
  switch (p) 
  {
    case FINGERPRINT_OK: break;                   // imagen convertida
    case FINGERPRINT_IMAGEMESS: return p;         //imagen muy desordenada
    case FINGERPRINT_PACKETRECIEVEERR: return p;  // error de comunicacion
    case FINGERPRINT_FEATUREFAIL: return p;       //No se pudieron encontrar las funciones de huellas dactilares
    case FINGERPRINT_INVALIDIMAGE: return p;      //No se pudieron encontrar las funciones de huellas dactilares
    default: return p;                            // error desconocido
  }



p = finger.fingerFastSearch();

if (p == FINGERPRINT_OK)
{
  servicio = false;
  counter = 0;
  if(add_usuario)
  {
    if(finger.fingerID == admi_ID)
    {
     usuario_principal = true;
      id_ad = false;
    }
    else
    {
      add_usuario = false;
      usuario_principal = false;
      id_ad = false;
    }
    
  }
  else
  {
  digitalWrite(relay,HIGH); 
  digitalWrite(relay2,LOW); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" USUARIO   ");
  
  lcd.setCursor(0,1);
  lcd.print(" ID: #");
  
  lcd.setCursor(6,1);
  lcd.print(finger.fingerID);

  lcd.setCursor(9,1);
  lcd.print("%: ");

  lcd.setCursor(12,1);
  lcd.print(finger.confidence);

 no_servicios = false;
  delay(4000);
  lcd.setCursor(0,0);
  lcd.print("SERVICIOS");
  lcd.setCursor(0,1);
  lcd.print("HABILITADOS");
  delay(50);
  }
}//termina  escaner 

else if(p == FINGERPRINT_NOTFOUND)
{
  servicio = false;
  id_ad = false;
  counter = 0;
  lcd.setCursor(0,0);
  lcd.print("   ERROR    ");
  lcd.setCursor(0,1);
  lcd.print("INTENTA DE NUEVO");
  add_usuario = false;
  usuario_principal = false;  

  delay(2000);
  if(no_servicios)
    {
      digitalWrite(relay2,HIGH); 
      digitalWrite(relay,LOW); 
      lcd.setCursor(0,0);
      lcd.print("PRESIONE ESCANER");
      lcd.setCursor(0,1);
      lcd.print(" -SIN SERVICIOS- ");   
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("SERVICIOS");
      lcd.setCursor(0,1);
      lcd.print("HABILITADOS");  
    }
  delay(2);
  return p;
}
}

// REGRESA  -1 SI FALLA,POR EL CONTRARIO  REGRESA EL  ID #


int getFingerprintIDez() {
uint8_t p = finger.getImage();
if (p != FINGERPRINT_OK) return -1;
p = finger.image2Tz();
if (p != FINGERPRINT_OK) return -1;
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK) return -1;
// ENCONTRADO UNA COINCIDENCIA
return finger.fingerID;
}









//////////////////////////////////////////










/*This function will add new ID to the database*/
 uint8_t getFingerprintEnroll() {

  int p = -1;
  if(!first_lectura)
  {
  lcd.setCursor(0,0);
  lcd.print("AGREGAR NUEVO ID#");
  lcd.setCursor(14,0);
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("PON TU HUELLA");  
  }
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.setCursor(0,0);
      lcd.print(" IMAGEN TOMADA!  ");
      lcd.setCursor(0,1);
      lcd.print("                ");  
      delay(100);
      first_lectura = true;
      break;
    case FINGERPRINT_NOFINGER:
      lcd.setCursor(0,0);
      lcd.print("AGREGAR NUEVO ID#");
      lcd.setCursor(14,0);
      lcd.print(id);
      lcd.setCursor(0,1);
      lcd.print(" PON TU HUELLA"); 
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.setCursor(0,0);
      lcd.print("  Comunicacion  ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.setCursor(0,0);
      lcd.print("     -Image     ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      break;
    default:
      lcd.setCursor(0,0);
      lcd.print("    -DESCONOCIDO    ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.setCursor(0,0);
      lcd.print("IMAGEN CONVERTIDA!");
      lcd.setCursor(0,1);
      lcd.print("                ");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.setCursor(0,0);
      lcd.print("Imagen muy tardada!");
      lcd.setCursor(0,1);
      lcd.print("                ");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.setCursor(0,0);
      lcd.print("  Comunication  ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.setCursor(0,0);
      lcd.print(" No HUELLA ");
      lcd.setCursor(0,1);
      lcd.print(" CARACTERTISTICAS NO ");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.setCursor(0,0);
      lcd.print(" No HUELLA ");
      lcd.setCursor(0,1);
      lcd.print("CARACTERISTICAS NO ");
      delay(1000);
      return p;
    default:
      lcd.setCursor(0,0);
      lcd.print("    -DESCONOCIDO   ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      return p;
  }
  
  lcd.setCursor(0,0);
  lcd.print(" REMUEVE  TU HUELLA! ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  lcd.setCursor(0,1);
  lcd.print("ID# ");
  lcd.setCursor(4,1);
  lcd.print(id);
  p = -1;
  lcd.setCursor(0,0);
  lcd.print("PONER EN MISMO  ");
  lcd.setCursor(0,1);
  lcd.print("LUGAR LA HUELLA  ");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.setCursor(0,0);
      lcd.print("  IMAGEN TOMADA!  ");
      lcd.setCursor(0,1);
      lcd.print("                "); 
      break;
    case FINGERPRINT_NOFINGER:
      lcd.setCursor(0,0);
      lcd.print("PONER EN  MISMO ");
      lcd.setCursor(0,1);
      lcd.print("LUGAR  HUELLA   ");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.setCursor(0,0);
      lcd.print("  Comunication  ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.setCursor(0,0);
      lcd.print("     -Imagen     ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      break;
    default:
      lcd.setCursor(0,0);
      lcd.print("    -Desconocido   ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.setCursor(0,0);
      lcd.print("IMAGEN CONVERTIDA!");
      lcd.setCursor(0,1);
      lcd.print("                ");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.setCursor(0,0);
      lcd.print("Image too messy!");
      lcd.setCursor(0,1);
      lcd.print("                ");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.setCursor(0,0);
      lcd.print("  Comunication  ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.setCursor(0,0);
      lcd.print(" No huella ");
      lcd.setCursor(0,1);
      lcd.print(" CARACTERISTICAS NO ");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.setCursor(0,0);
      lcd.print("  Comunication  ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      return p;
    default:
      lcd.setCursor(0,0);
      lcd.print("    -Unknown    ");
      lcd.setCursor(0,1);
      lcd.print("     ERROR!     ");
      delay(1000);
      return p;
  }
  
  // OK converted!
 lcd.setCursor(0,0);
 lcd.print(" Creating model ");
 lcd.setCursor(0,1);
 lcd.print("for ID# ");
 lcd.setCursor(8,1);
 lcd.print(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.setCursor(0,0);
    lcd.print(" IMPRESION LISTA! ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.setCursor(0,0);
    lcd.print("  Comunication  ");
    lcd.setCursor(0,1);
    lcd.print("     ERROR!     ");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.setCursor(0,0);
    lcd.print("HUELLA MUERTA ");
    lcd.setCursor(0,1);
    lcd.print("no registrada      ");
    delay(1000);
    return p;
  } else {
    lcd.setCursor(0,0);
    lcd.print("    -Unknown    ");
    lcd.setCursor(0,1);
    lcd.print("     ERROR!     ");
    delay(1000);
    return p;
  }   
  
  lcd.setCursor(0,1);
  lcd.print("ID# ");
  lcd.setCursor(4,1);
  lcd.print(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.setCursor(0,0);
    lcd.print("     Stored     ");
    lcd.setCursor(0,1);
    lcd.print("                ");
   
    delay(1000);
    first_lectura = false;
    id_ad = false;
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.setCursor(0,0);
    lcd.print("  Comunication  ");
    lcd.setCursor(0,1);
    lcd.print("     ERROR!     ");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.setCursor(0,0);
    lcd.print("Could not store ");
    lcd.setCursor(0,1);
    lcd.print("in that location");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.setCursor(0,0);
    lcd.print("Error writing to");
    lcd.setCursor(0,1);
    lcd.print("flash           ");
    delay(1000);
    return p;
  } else {
    lcd.setCursor(0,0);
    lcd.print("    -Unknown    ");
    lcd.setCursor(0,1);
    lcd.print("     ERROR!     ");    
    delay(1000);
    return p;
  }   
}




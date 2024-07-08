int indexVariables;
int indexVariablesNext;

//String apn_entel = "auto"; //auto apn

bool GPSretry = false; //variable aux por si falla el GPS

void setup() {

    // código para setup
  
    sk.StopPSM();

}

void loop() {


    //decide si ya se intentó una traza GPS previamente o no
    if (GPSretry == false){
        sk.StartGPS(); //inicializa el uso de GPS para obtener una traza


    }

    delay(5000);
    String GPSmsg = sk.PositionGPS();
    
    Serial.println(GPSmsg);

    indexVariables = GPSmsg.indexOf(",");
    indexVariablesNext = GPSmsg.indexOf(",", indexVariables+1);

    lat = GPSmsg.substring(indexVariables+1, indexVariablesNext);        // Latitude

    indexVariables = indexVariablesNext;
    indexVariablesNext = GPSmsg.indexOf(",", indexVariables+1);
    lon = GPSmsg.substring(indexVariables+1, indexVariablesNext);      // Longitude

    String tempmsg = "{\"latitude\":" + lat + "; \"longitude\": "+ lon +"}"; //temporal message

    Serial.println(tempmsg);



    //realiza el intento de envío de una traza gps exitosa
    if (GPSmsg == "Null"){
        GPSretry = true;
        delay(500); //reintenta 
    }

    

    else{ //termina gps, inicializa narrowband nuevamente para envío de mensaje
        sk.EndGPS();
        delay(5000);

        

        // codigo para enviar el mensaje

        GPSretry = false;
        delay(5000);
    }

    
    }

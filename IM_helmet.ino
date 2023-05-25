
#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <ESP32Servo.h>

//const char* ssid = "login";
//const char* password = "password"; // home network credentials

const char* ssid = "login";
const char* password = "password"; // access-point network credentials

#define EYES 26
#define MASK 25

Servo servoLEFT;
Servo servoRIGHT;

AsyncWebServer server(80);

// Search for parameters in HTTP POST request
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<meta charset="UTF-8">
<html>
  <head>
    <link href="styleIM.css" rel="stylesheet" type="text/css" />
    <script src="IMfunc.js"></script>
  </head>
  
  <body>
    <header>
      <div class="headtext">
          <h1>STEROWANIE MK85</h1>
      </div>
    </header>
    <div id="wrapper">
      
       <div class="helmet">
        <svg width="300" height="500" viewBox="0 0 900 1200" fill="none" xmlns="http://www.w3.org/2000/svg">
          <g id="rysunek 1">
            <g id="BAZA">
            <path id="path3289" d="M396.85 46.3753V1089.22H271.117C271.117 1089.22 180.745 1026.2 147.45 977.448C147.45 977.448 79.6704 789.569 84.4268 709.899L67.7793 693.251L55.8882 456.618L71.3466 439.971L67.7793 417.378L107.02 165.286C107.02 165.286 199.146 46.3753 396.85 46.3753V46.3753ZM396.85 46.3753V1089.22H522.584C522.584 1089.22 612.956 1026.2 646.251 977.448C646.251 977.448 714.03 789.569 709.274 709.899L725.922 693.251L737.813 456.618L722.354 439.971L725.922 417.378L686.681 165.286C686.681 165.286 594.555 46.3753 396.85 46.3753V46.3753Z" fill="#BC0202" stroke="#BC0202" stroke-width="0.999999"/>
            <path id="path3289-0" d="M396.85 46.375C199.145 46.375 107.02 165.287 107.02 165.287L116.533 219.984L329.23 349.098L462.516 350.4L678.104 219.984L686.682 165.287C686.682 165.287 594.554 46.375 396.85 46.375Z" fill="#8B0800" stroke="#8B0800" stroke-width="0.999999"/>
            <path id="path3368" d="M271.117 1089.22L317.492 1041.66H397.163L396.85 1089.22H271.117ZM523.208 1089.22L476.833 1041.66H397.163L397.475 1089.22H523.208Z" fill="#8B0800" stroke="#8B0800" stroke-width="0.999999"/>
            <path id="path3403" d="M271.117 1089.22L244.362 1030.96L302.034 989.339L317.492 1041.66L271.117 1089.22ZM522.584 1089.22L549.339 1030.96L491.667 989.339L476.208 1041.66L522.584 1089.22Z" fill="#A40202" stroke="#A40202" stroke-width="0.999999"/>
            <path id="path3407" d="M71.3457 439.971L84.4277 709.899C85.2037 725.911 86.1136 740.935 87.1309 755.088C99.5799 844.733 147.449 977.449 147.449 977.449C136.747 923.272 145.73 806.354 145.73 806.354L173.877 767.316L71.3457 439.971ZM722.355 439.971L709.273 709.899C708.497 725.911 707.587 740.935 706.57 755.088C694.121 844.733 646.252 977.449 646.252 977.449C656.954 923.272 647.97 806.354 647.97 806.354L619.824 767.316L722.355 439.971Z" fill="#CA2F2F" stroke="#CA2F2F"/>
            <path id="path3356" d="M211.662 917.398L145.73 806.354L120.977 690.873L199.234 781.97L217.607 896.588L211.662 917.398ZM582.039 917.398L647.97 806.354L672.724 690.873L594.467 781.97L576.093 896.588L582.039 917.398Z" fill="#262626" stroke="#262626" stroke-width="0.999999"/>
            <path id="path3362" d="M696.225 620.121L722.354 439.971L725.922 417.378L720.379 381.769L696.742 452.365L666.184 479.806L696.225 620.121ZM97.4762 620.121L71.3466 439.971L67.7793 417.378L73.3222 381.769L96.9592 452.365L127.517 479.806L97.4762 620.121Z" fill="#262626" stroke="#262626" stroke-width="0.999999"/>
            <path id="path3296-3" d="M275.873 112.371C275.873 112.371 148.045 147.449 116.533 219.984L127.83 479.807L98.1016 620.121L121.289 690.873C121.289 690.873 219.391 809.188 217.607 896.588L211.662 917.398L256.254 958.422L288.359 920.371H505.967L538.072 958.422L582.664 917.398L576.719 896.588C574.935 809.188 673.035 690.873 673.035 690.873L696.225 620.121L666.496 479.807L677.793 219.984C646.282 147.449 518.451 112.371 518.451 112.371L488.725 305.006C483.968 354.354 451.268 352.57 451.268 352.57H397.475L397.162 890.643L396.85 352.57H343.059C343.059 352.57 310.358 354.354 305.602 305.006L275.873 112.371Z" fill="black" stroke="black" stroke-width="0.999999"/>
            <path id="path3302" d="M396.85 916.804L288.359 920.371L256.253 958.423L211.662 917.398L199.771 981.61L244.362 1030.96L302.034 989.339L349.004 986.367L356.733 970.908H397.163L396.85 916.804ZM397.475 916.804L505.966 920.371L538.072 958.423L582.664 917.398L594.555 981.61L549.963 1030.96L492.292 989.339L445.322 986.367L437.592 970.908H397.163L397.475 916.804Z" fill="#F0C215" stroke="#F0C215" stroke-width="0.999999"/>
            <path id="path3302-7" d="M397.475 916.803L397.162 970.908H437.592L445.322 986.367L491.674 989.301L505.967 920.371L397.475 916.803ZM397.162 970.908L396.85 916.822L288.359 920.371L302.033 989.34L349.004 986.367L356.732 970.908H397.162Z" fill="#F6D96B" stroke="#F6D96B" stroke-width="0.999999"/>
            </g>
            <g class ="MASKA" id="MASKA">
            <path id="path3296" d="M396.85 352.57H343.058C343.058 352.57 310.357 354.354 305.601 305.006L275.873 112.37C275.873 112.37 148.044 147.449 116.532 219.985L127.829 479.805L98.101 620.12L121.289 690.872C121.289 690.872 219.39 809.189 217.607 896.588L255.658 932.261L284.791 890.643H397.162L396.85 352.57ZM396.92 352.57L450.712 352.521C450.712 352.521 483.414 354.274 488.125 304.922L517.676 112.259C517.676 112.259 645.537 147.22 677.115 219.726L666.058 479.557L695.916 619.845L672.793 690.618C672.793 690.618 574.801 809.025 576.665 896.423L538.646 932.131L509.475 890.539L397.104 890.643L396.92 352.57Z" fill="#F6D96B" stroke="#F6D96B" stroke-width="0.999999"/>
            <path id="path4558" d="M98.1017 620.119L121.289 690.871V690.873C121.295 690.881 219.391 809.191 217.608 896.588L255.658 932.262L284.791 890.643C284.791 890.643 254.532 702.81 98.1017 620.119V620.119ZM695.599 620.119L672.412 690.871V690.873C672.408 690.881 574.31 809.191 576.093 896.588L538.042 932.262L508.91 890.643C508.91 890.643 539.169 702.81 695.599 620.119V620.119Z" fill="#F0C215" stroke="#F6D96B" stroke-width="0.999999"/>
            <path id="eyesShadow" d="M396.85 580.88L308.574 575.529L241.984 568.394L178.366 545.801L155.179 538.072L146.26 572.348C146.26 572.348 174.205 630.228 307.385 608.824L318.681 586.826L397.163 588.609L396.85 580.88ZM397.475 580.88L485.751 575.529L552.341 568.394L615.959 545.801L639.147 538.072L648.065 572.348C648.065 572.348 620.121 630.228 486.94 608.824L475.644 586.826L397.163 588.609L397.475 580.88Z" fill="#7C7C7C" stroke="#7C7C7C" stroke-width="0.999999"/>
            <path id="eyes" d="M308.574 575.529C308.574 575.529 203.932 561.26 178.366 545.801L166.475 568.394C166.475 568.394 202.461 610.013 299.656 596.339L308.574 575.529ZM485.127 575.529C485.127 575.529 589.768 561.26 615.334 545.801L627.225 568.394C627.225 568.394 591.24 610.013 494.045 596.339L485.127 575.529Z" fill="white" stroke="white"/>
            </g>
          </g>
        </svg>   
        </div>

        <div class="buttons">
        %BUTTONPLACEHOLDER%
        </div> 
        
    </div>
  </body>
</html>
)rawliteral";

String outputState(int output){
  if(digitalRead(output))
    {return "checked";}
  else 
    {return "";}
  return "";
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    buttons+= "<div class=\"button-wrapper\"><p><b>Maska: </b></p><label class=\"switch\"><input onclick=\"OpenClose()\" type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"25\" " + outputState(25) + "><span class=\"slider\"></span></label></div>";
    buttons+= "<div class=\"button-wrapper\"><p><b>Oczy: </b></p><label class=\"switch\"><input onclick=\"OnOff()\" type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"26\" " + outputState(26) + "><span class=\"slider\"></span></label></div>";
    return buttons;
  }
  return String();
}

void setup(){
Serial.begin(115200);
pinMode(EYES,OUTPUT);
digitalWrite(EYES,LOW);
servoLEFT.attach(33);
//servoLEFT.write(1);
servoRIGHT.attach(32);
//servoRIGHT.write(180);

if(!SPIFFS.begin()){
  //Serial.println("An Error has occurred while mounting SPIFFS");
  return;
}

//WiFi.mode(WIFI_STA); //zmienić na WIFI_AP (aby access point)
//WiFi.begin(ssid, password);
WiFi.softAP(ssid, password);
/*
while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  //Serial.println("Connecting to WiFi..");
}*/
//Serial.println(WiFi.localIP());
Serial.print("IP adress: ");
Serial.println(WiFi.softAPIP());

 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  
server.on("/IMfunc.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/IMfunc.js", "text/javascript");
  });
  
server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      int Pin = inputMessage1.toInt();
      int Status = inputMessage2.toInt();
      if (Pin == 26){digitalWrite(EYES,Status);}
      if (Pin == 25)
        {
          if(Status == 1){servoLEFT.write(120);servoRIGHT.write(60);} //120,60
          else {servoLEFT.write(1);servoRIGHT.write(180);}
        }
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    request->send(200, "text/plain", "OK");
  });
  
  
  server.on("/styleIM.css", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/styleIM.css", "text/css");
  });
  
  server.begin();
}

void loop(){}

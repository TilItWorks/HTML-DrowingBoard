/*This script was created by Manciu Lucas and Busila Adrian
For the script to work you will have to install all the libraries that are included below,
and modify ssid and password section with you SSID and PASSWORD of you router.

Behavior: 
On wifi connection the built in led will start pulsating,
when wifi is connected and everything is initialised the led will turn off. 
After this you can connect to the ip printed in serial monitor.

Project Description:
On connection on the ip from the serial monitor, you can draw on the right part.
If you dont want to write an sqwer you can click on the joicestick and the if you pass through the squares you will not draw.
If you want to change the collor, press down the joice stick for 2 seccond and on the left side, one of the collor component will be evidentiate,
with the rigth and left, now, you can change the amount of the specific collor, and with up an down, you moove throo collor components.
To go back to the drowing bord you need to hold the joicestic button for 2 seconds.

OBS:
all of the class can connect to the same ip, if the device is on the smane natwork, and can visualize the drawing in the realtime.*/

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <EasyButton.h>


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

char ssid[] = "SSID";          //////////MODIFI THIS
char password[] = "PASSWORD";  ///MODIFI THIS

///////////////////////////////////////////////////////FrontSide
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Proiect SuP</title>
  </head>
  <style>
    body {
      display: flex;
      flex-direction: column;
      justify-content: center;
      font-size: 2em;
      font-family: "Times New Roman", Times, serif;
      background-color: rgb(212, 232, 249);
      height: 700px;
    }
    #main_container {
      height: 650px;
      display: grid;
      grid-template-columns: 25% 75%;
      grid-template-rows: 100%;
    }
    #make_color {
      display: flex;
      flex-direction: column;
      padding: 1px;
      margin: 2px;
      border-style: solid;
      border-radius: 10px;
      border-width: 1px;
    }
    #paint_aria {
      padding: 1px;
      margin: 2px;
      border-style: solid;
      border-radius: 10px;
      border-width: 1px;
    }
    #out {
      height: 20px;
      width: 40px;
      background: red;
    }
    #name {
      display: flex;
      align-items: center;
      justify-content: center;
    }
    #paint_aria {
      display: grid;
      padding: 10px;
      align-items: center;
      justify-content: center;
      grid-template-columns: repeat(180, 5px);
      grid-template-rows: repeat(100, 5px);
    }
    .print_sqwer {
      display: flex;
      height: 5px;
      background-color: white;
      align-items: center;
      justify-content: center;
    }
    .p_val {
      display: flex;
      flex-direction: row;
      height: 40px;
      padding: 5px;
      align-content: center;
    }
    .p_val p {
      height: auto;
      padding: 5px;
      margin: 0%;
    }
    .add {
      border-width: 1px;
      border-style: solid;
      border-color: red;
    }
  </style>
  <body>
    <p id="name">PROIECT</p>
    <div id="main_container">
      <div id="make_color">
        <div class="p_val">
          <p id="red_select">Red:</p>
          <p id="red_val">255</p>
        </div>
        <div class="p_val">
          <p id="green_select">Green:</p>
          <p id="green_val">35</p>
        </div>
        <div class="p_val">
          <p id="blue_select">Blue:</p>
          <p id="blue_val">69</p>
        </div>
        <div id="out"></div>
      </div>
      <div id="paint_aria"></div>
    </div>
  </body>
  <script>
    var lin_old = 100;
    var increment_old = 0;
    var init_val;
    div_aria = document.getElementById("paint_aria");
    var children_div_aria = div_aria.children;

    function js_atribut(atribut, value, poz) {}
    make_color = document.getElementById("make_color");
    var make_color_children = make_color.children;
    window.onload = function () {
      dimension = 180 * 100;
      for (var i = 0; i < dimension; i++) {
        const box6 = document.createElement("div");
        box6.classList.add("print_sqwer");
        div_aria.appendChild(box6);
      }
      initWebSocket();
    };

    // var gateway = `ws://192.168.0.227/ws`;
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    var vector = ["red_val", "green_val", "blue_val"];
    var out_aria = document.getElementById("out");

    function initWebSocket() {
      console.log("Trying to open a WebSocket connection...");
      websocket = new WebSocket(gateway);
      websocket.onopen = onOpen;
      websocket.onclose = onClose;
      websocket.onmessage = onMessage; // <-- add this line
    }
    function onOpen(event) {
      console.log("Connection opened");
    }
    function onClose(event) {
      console.log("Connection closed");
      setTimeout(initWebSocket, 2000);
    }

    function onMessage(event) {
      var state = event.data;
      var data = JSON.parse(state);

      var caz = data["caz"];
      console.log(data);
      if (caz == 0) {
        var poz = data["main_poz"];
        var print = data["print"];

        let out_aria_color = window.getComputedStyle(out_aria).backgroundColor;
        if (print) {
          var culoare = window.getComputedStyle(
            children_div_aria[poz]
          ).backgroundColor;
          children_div_aria[poz].style.backgroundColor = culoare;
          children_div_aria[poz].style.borderStyle = "solid";
          children_div_aria[poz].style.borderWidth = "1px";
        } else {
          children_div_aria[poz].style.backgroundColor = out_aria_color;
          children_div_aria[poz].style.filter = "brightness(200%)";
          children_div_aria[poz].style.borderColor = "black";
        }

        ///clear hilight
        try {
          children_div_aria[poz - 1].style.filter = "brightness(100%)";
          children_div_aria[poz + 1].style.filter = "brightness(100%)";
          children_div_aria[poz - 180].style.filter = "brightness(100%)";
          children_div_aria[poz + 180].style.filter = "brightness(100%)";

          children_div_aria[poz - 1 - 180].style.filter = "brightness(100%)";
          children_div_aria[poz - 1 + 180].style.filter = "brightness(100%)";
          children_div_aria[poz + 1 - 180].style.filter = "brightness(100%)";
          children_div_aria[poz + 1 + 180].style.filter = "brightness(100%)";
        } catch (error) {}

        try {
          children_div_aria[poz - 1].style.borderStyle = "none";
          children_div_aria[poz + 1].style.borderStyle = "none";
          children_div_aria[poz - 180].style.borderStyle = "none";
          children_div_aria[poz + 180].style.borderStyle = "none";

          children_div_aria[poz - 1 - 180].style.borderStyle = "none";
          children_div_aria[poz - 1 + 180].style.borderStyle = "none";
          children_div_aria[poz + 1 - 180].style.borderStyle = "none";
          children_div_aria[poz + 1 + 180].style.borderStyle = "none";
        } catch (error) {}
        for (var i = 0; i < 3; i++) {
          make_color_children[i].classList.remove("add");
        }
        lin_old = 100;
      } else if (caz == 1) {
        var color_str = "rgb(";
        var lin = data["lin"];
        var increment = data["col"];

        lin = lin % 3;
        var para = document.getElementById(vector[lin]);
        if (lin != lin_old) {
          init_val = parseInt(para.innerHTML);
          lin_old = lin;
        }
        if (init_val + increment < 0) {
          init_val = 255 - increment;
        }
        var val_reglata = (init_val + increment) % 256;

        para.innerHTML = val_reglata;

        for (var i = 0; i < 3; i++) {
          make_color_children[i].classList.remove("add");
          para = document.getElementById(vector[i]);
          if (i != 2) {
            color_str += para.innerHTML + ", ";
          } else {
            color_str += para.innerHTML + ")";
            out_aria.style.backgroundColor = color_str;
          }
        }
        make_color_children[lin].classList.add("add");
      }
    }
  </script>
</html>
)rawliteral";
///////////////////////////////////////////////////////ServerSIDE
DynamicJsonDocument js(1024);
DynamicJsonDocument js_in(1024);

int plex = 15, buton = 3, led = 2;  // pini
int stare_buton = 0, timp_buton = 0, timp_buton_old = millis();
int x_poz = 0, y_poz = 0;          // analogice
int x_poz_old = 0, y_poz_old = 0;  //analogice
int col_mem = 0, lin_mem = 0;
int lin = 0, col = 0, caz = 0, caz_old = 0, print = 0, print_old = 0;
int main_poz = 0, lin_old = 0, col_old = 0, main_poz_old = 0;

bool mem_flag = false, enter_flag = false;
bool timp_passed = false;

DynamicJsonDocument jsonBuffer(1024);
WiFiClient espClient;

EasyButton button(buton);
//////////////////////////////////////////////////Prefidining functions
void onPressedForDuration();
void onPressed();
void get_poz();
void get_direction();
void get_caz();
void limitare();

void notifyClients() {
  char data[200];
  size_t len = serializeJson(js, data);
  ws.textAll(data, len);  //send all data to front-end
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {  //get all the data from the fron-end
  int i;
  char jsone_data[200];
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

    for (i = 0; i < len; i++) {
      jsone_data[i] = (char)data[i];
    }
    deserializeJson(js_in, jsone_data);
    int a = js_in["new_col"];
    col = a;
    col_old = a;
    //Serial.print(a);
    //Serial.println();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,  //WebSocket events
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

//////////////////////////////SETUP
void setup() {
  Serial.begin(115200);
  pinMode(plex, OUTPUT);
  pinMode(buton, INPUT);
  pinMode(led, OUTPUT);

  digitalWrite(led, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("This is the ip of the server: ");
  digitalWrite(led, HIGH);
  Serial.println(WiFi.localIP());

  

  initWebSocket();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
  button.begin();
  button.onPressedFor(2000, onPressedForDuration);
  button.onPressed(onPressed);
}
void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  ws.cleanupClients();
  get_poz();
  get_direction();
  button.read();
  if (lin != lin_old || col != col_old || caz != caz_old || print != print_old) {
    if (!caz) {
      col_mem = col;
      lin_mem = lin;
      mem_flag = false;
    } else {
      mem_flag = true;
    }
    limitare();
    main_poz = lin * 180 + col;

    js["main_poz"] = main_poz;
    js["lin"] = lin;
    js["col"] = col;
    js["caz"] = caz;
    js["print"] = print;
    notifyClients();

    lin_old = lin;
    col_old = col;
    caz_old = caz;
    print_old = print;
  }
  delay(100);
}

void get_poz() {
  digitalWrite(plex, LOW);
  x_poz = analogRead(A0);
  digitalWrite(plex, HIGH);
  y_poz = analogRead(A0);
}

void get_direction() {
  if (!((x_poz >= 547 - 5 && x_poz <= 547 + 5) && (y_poz >= 532 - 5 && y_poz <= 532 + 5))) {
    if (x_poz == 1024 && y_poz < 10) {  //dreapta jos
      lin++;
      col++;
      delay(10);
    } else if (x_poz == 1024 && y_poz == 1024) {  //stanga jos
      lin++;
      col--;
      delay(10);
    } else if (x_poz < 120 && y_poz < 120) {  //dreapta sus
      col++;
      lin--;
      delay(10);
    } else if (x_poz < 120 && y_poz == 1024) {  //stanga sus
      col--;
      lin--;
      delay(10);
    } else if (x_poz == 1024 && (y_poz >= 547 - 5 || y_poz <= 547 + 5)) {  //jos
      lin++;
      if (caz) {
        col = 0;
      }
    } else if (x_poz < 10 && (y_poz >= 542 - 5 || y_poz <= 542 + 5)) {  //sus
      lin--;
      if (caz) {
        col = 0;
      }
    } else if ((x_poz >= 547 - 5 || x_poz == 547 + 5) && y_poz < 9) {  //dreapta
      col++;
    } else if ((x_poz >= 547 - 5 || x_poz == 547 + 5) && y_poz == 1024) {  //stanga
      col--;
    }
    x_poz_old = x_poz;
    y_poz_old = y_poz;
  }
}
void limitare() {
  if (lin < 0) {
    lin = 0;
  }
  if (!caz) {
    if (col < 0) {
      col = 0;
    }
  }
  if (lin > 100) {
    lin = 100;
  }
  if (caz) {
    if (col > 255) {
      col = 255;
    }
  } else {
    if (col > 180) {
      col = 180;
    }
  }
}
void onPressedForDuration() {
  lin = 0;
  col = 0;
  caz = (caz + 1) % 2;
  if (!caz && mem_flag) {  /////////////////////
    col = col_mem;
    lin = lin_mem;
  }
}
void onPressed() {
  print = (print + 1) % 2;
}

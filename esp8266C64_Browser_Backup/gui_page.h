const char gui[] PROGMEM = 
  "<html><head>\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1' />\n"
  "<meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate' />\n"
  "<meta http-equiv='Pragma' content='no-cache' />\n"
  "<meta http-equiv='Expires' content='0' />\n"
  "<script src='VICII.js'></script>\n"
  "</head>\n"
  "<body>\n<h1 style='font-family:Lucida Console;font-size:32px;margin:0px;padding:0px'>VIC-II client</h1>\n"
  "<div style='width:384px;font-size:12px;'><div style='float:right;'>for use with ESP8266 based C64 CPU emulator</div><br><div style='float:right;'><a href='http://www.esp8266.com/viewtopic.php?f=32&t=4596&start=5'>info at esp8266.com forum</a></div></div>\n"
  "<div id='fps' style='background-color:#FFFFAA;border:1px solid black;width:42px;height:24px;text-align:center;line-height:22px;font-size:12px;'>10 fps</div>"
  "<canvas id='vicoutput' style='background-color:#A4A4FE;border:1px;width:384px;height:264px;'></canvas>\n"
  "<div id='Log' style='background-color:#FFFFAA;width:378px;height:80px;border:1px solid black;font-family:Lucida console;font-size:10px;padding:2px;overflow:auto;'></div>\n"
  "<div id='PopUp'\n"
  " style='display: none; position: absolute; left: 30px; top: 50px; border: solid black 1px; padding: 10px; background-color: rgb(240,240,240); text-align: justify; font-size: 12px; width: 360px;'\n"
  " onclick='document.getElementById('PopUp').style.display = 'none''>\n"
  //"<span id='PopUpText' style='font-family:Lucida Console;letter-spacing: 2px;'>TEXT</span>\n"
  "</div>\n"
  "</body></html>\n";


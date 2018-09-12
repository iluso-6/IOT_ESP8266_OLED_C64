const char gui[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name='viewport' content='width=device-width, initial-scale=1' />
<meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate' />
<meta http-equiv='Pragma' content='no-cache' />
<meta http-equiv='Expires' content='0' />
  <script src='VICII.js'></script>
  </head>
  <body>
  <div id='fps' style='background-color:#FFFFAA;border:1px solid black;width:42px;height:24px;text-align:center;line-height:22px;font-size:12px;'>10 fps</div>
  <canvas id='vicoutput' style='background-color:#A4A4FE;border:1px;width:384px;height:264px;'></canvas>
  <div id='Log' style='background-color:#FFFFAA;width:378px;height:80px;border:1px solid black;font-family:Lucida console;font-size:10px;padding:2px;overflow:auto;'></div>
  </body>
  </html>
)=====";

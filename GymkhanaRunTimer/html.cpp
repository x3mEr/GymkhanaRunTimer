#include <Arduino.h>
#include "Html.h"

const char* CHtml::head = "\
<!DOCTYPE html>\n\
<html>\n\
<head>\n\
<title>GymkhanaRunTimer</title>\n";

const char* CHtml::style = "\
<style>\n\
body{zoom:1;margin:0px 5px 0px 5px;font:1.0em Arial;color:#eee;background:#2a3649;}\n\
table{width:100%;box-shadow:5px 5px 9px #111;}\n\
table,th,td{margin:0px 0px 5px 0px;text-align:center;color:#111;background:#eee;border:1px solid black;border-spacing:0;border-collapse:collapse;}\n\
th{padding:6px;font-weight:bold;color:#eee;background:linear-gradient(to bottom,#888,#000);}\n\
td{padding:3px;border-bottom:none;}\n\
.but,button{outline:none;width:168px;font:1.0em Arial;display:inline-block;white-space:nowrap;background:linear-gradient(to bottom,#fff,#777);border:1px solid #777;padding:12px;margin:12px 0px 12px 3px;text-shadow:1px 1px 0 rgba(255,255,255,0.5);color:#111;text-decoration:none;border-radius:.2em;box-shadow:3px 3px 6px #333;}\n\
.but:active,button:active{box-shadow:inset 2px 2px 4px #333;transform: translateY(2px);}\n\
p{margin:5px;}\n\
input{font:1.0em Arial;}\n\
.b{font:10.0em Arial;text-shadow:2px 2px 2px #fff;}\n\
.f{font:2.0em Arial;}\n\
.h{font:3.0em Arial;}\n\
.w30{width:30%;}\n\
.w95{width:95%;}\n\
.g{background:#0f0;}\n\
</style>\n";

const char* CHtml::script = "\
<script type=\"text/javascript\" charset=\"utf-8\"></script>\n\
<script>\n\
var connection = new WebSocket('ws://'+location.hostname+':81/',['arduino']);\n\
var sound = new AudioContext();\n\
function beep(){var osc=sound.createOscillator();osc.type='triangle';osc.frequency.value=880;var env=sound.createGain();env.connect(sound.destination);env.gain.exponentialRampToValueAtTime(0.00001,sound.currentTime+0.5);osc.connect(env);osc.start(0);};\n\
function timerOnFinish(txt){var list=txt.split(' ');beep();document.getElementById('l').innerHTML=list[1];document.getElementById('t').innerHTML=list[3];var res = list[1]+'.&ensp;'+list[3];document.getElementById('sum').innerHTML += res+'<br>';if (list.length == 5){ document.getElementById('rec').innerHTML = res;}};\n\
var timeStart = new Date().getTime();\n\
var pointer;\n\
function timerOnStart(){var now = new Date().getTime();var distance = now-timeStart; var mins  = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));var secs  = Math.floor((distance % (1000 * 60)) / 1000);var msecs = Math.floor(distance % 1000); var str = mins + ':'; if (secs < 10) str += '0'; str += secs + '.'; if (msecs < 100) str += '0'; if (msecs < 10)  str += '0'; str += msecs; document.getElementById('t').innerHTML = str;};\n\
function startTimer(){pointer = setInterval(timerOnStart, 50);};\n\
function stopTimer(){clearInterval(pointer);};\n\
function reset(){console.log('Reset');document.getElementById('l').innerHTML='0';document.getElementById('t').innerHTML='-:--.---';document.getElementById('rec').innerHTML='';document.getElementById('sum').innerHTML='';connection.send('Reset');};\n\
function thold(){var txt='Thold '+document.getElementById('th').value;console.log(txt);connection.send(txt);};\n\
connection.onopen=function(){connection.send('Connect '+new Date());};\n\
connection.onmessage=function(msg){console.log('Server: ',msg.data);if (msg.data.startsWith('R')) stopTimer();if (msg.data.startsWith('Start')){document.getElementById('l').innerHTML = msg.data.split(' ')[1]; timeStart = new Date().getTime(); startTimer();} if (msg.data.startsWith('Finish')){stopTimer(); timerOnFinish(msg.data);} if (msg.data.startsWith('Thold')) document.getElementById('th').value=parseInt(msg.data.substr(6));};\n\
connection.onerror=function(error){console.log('WebSocket Error '+error);};\n\
var countdownfunction = setInterval(timerOnFinish(txt), 1000);\n\
</script>\n";

const char* CHtml::body = "\
<body>\n\
<table>\n\
<tr>\n\
<th colspan=\"2\">MotoGymkhana39</th>\n\
</tr>\n\
<tr>\n\
  <td class=\"h w30\">Run:</td>\n\
  <td class=\"h\">Time:</td>\n\
</tr>\n\
<tr>\n\
  <td class=\"b\"><p id=\"l\">-</p></td>\n\
  <td class=\"b\"><p id=\"t\">-:--.---</p></td>\n\
</tr>\n\
<tr>\n\
  <td><input class=\"but f\" type=\"button\" value=\"Reset\" onclick=\"reset();\"/></td>\n\
  <td class=\"f\">Threshold: <input type=\"number\" id=\"th\" value=\"3\" min=\"0\" max=\"120\" onchange=\"thold();\"/> s</td>\n\
</tr>\n\
<tr>\n\
<td class=\"g f\">Best time:</td>\n\
<td class=\"g f\"><p id=\"rec\"></p></td>\n\
</tr>\n\
<tr>\n\
<td class=\"f\">List:</td>\n\
<td><p class=\"f\" id=\"sum\"></p></td>\n\
</tr>\n\
</table>\n\
</body>\n";


String CHtml::getPage() {
  return String(head) + script + style + "</head>\n" + body + "</html>\n";
}

String CHtml::getHead() {
  String res = String(head) + style;
  Serial.print("getHead() size: ");
  Serial.println(res.length());
  return res;
}

String CHtml::getScript() {
  String res = String(script);
  Serial.print("getScript() size: ");
  Serial.println(res.length());
  return res;
}

String CHtml::getBody() {
  String res = String("</head>\n") + body + "</html>\n\n";
  Serial.print("getBody() size: ");
  Serial.println(res.length());
  return res;
}

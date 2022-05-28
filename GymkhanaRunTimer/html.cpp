#include <Arduino.h>
#include "html.h"

const char* CHtml::head = R"html(
<!DOCTYPE html>
<html>
<head>
  <title>GymkhanaRunTimer</title>
  <link rel="icon" type="image/x-icon" href="data:image/png;base64,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">
)html";

const char* CHtml::style = R"html(
<style>
body{
  zoom: 1;
  margin: 0px 5px 0px 5px;
  font: 1.0em Arial;
  color: #eee; background: #2a3649;
}

table{
  width: 100%;
  box-shadow: 5px 5px 9px #111;
}

table, th, td{
  margin: 0px 0px 5px 0px;
  text-align: center;
  color: #111;
  background: #eee;
  border: 1px solid black;
  border-spacing: 0;
  border-collapse: collapse;
}

th{
  padding: 6px;
  font-weight: bold;
  color: #eee;
  background: linear-gradient(to bottom, #888, #000);
}

td{
  padding: 3px;
  border-bottom: none;
}

.but, button{
  outline: none;
  width: 168px;
  font: 1.0em Arial;
  display: inline-block;
  white-space: nowrap;
  background: linear-gradient(to bottom, #fff, #777);
  border: 1px solid #777;
  padding: 12px;
  margin: 12px 0px 12px 3px;
  text-shadow: 1px 1px 0 rgba(255, 255, 255, 0.5);
  color: #111;
  text-decoration: none;
  border-radius: .2em;
  box-shadow: 3px 3px 6px #333;
}

.but:active, button:active{
  box-shadow: inset 2px 2px 4px #333;
  transform: translateY(2px);
}

p{margin:5px;}

input{font: 1.0em Arial;}

.b{
  font: 10.0em Arial;
  text-shadow: 2px 2px 2px #fff;
}

.f{font: 2.0em Arial;}

.h{font: 3.0em Arial;}

.tname{font: 2.0em Arial;}

.w30{width: 30%;}

.w95{width: 95%;}

.g{background: #0f0;}

.led {
  height: 40px;
  width: 40px;
  background-color: #bbb;
  border-radius: 50%;
  display: inline-block;
}
</style>
)html";

const char* CHtml::script = R"html(
<script type="text/javascript" charset="utf-8"></script>
<script>
var connection = new WebSocket('ws://' + location.hostname + ':81/',['arduino']);
var sound = new AudioContext();
var LedState = new AudioContext();

function beep(){
  var osc = sound.createOscillator();
  osc.type = 'triangle';
  osc.frequency.value = 880;
  var env = sound.createGain();
  env.connect(sound.destination);
  env.gain.exponentialRampToValueAtTime(0.00001, sound.currentTime + 0.5);
  osc.connect(env);
  osc.start(0);
};

function timerOnFinish(txt){
  var list = txt.split(' ');
  beep();
  document.getElementById('l').innerHTML = list[1];
  document.getElementById('t').innerHTML = list[2];
  var res = list[1] + '.&ensp;' + list[2];
  document.getElementById('sum').innerHTML += res + '<br>';
  if (list.length == 4)
    document.getElementById('rec').innerHTML = res;
};

var timeStart = new Date().getTime();

var pointer;

function OnStartTimer(){
  var now = new Date().getTime();
  var distance = now - timeStart;
  var mins = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
  var secs = Math.floor((distance % (1000 * 60)) / 1000);
  var msecs = Math.floor(distance % 1000);
  var str = mins + ':';
  if (secs < 10) str += '0';
  str += secs + '.';
  if (msecs < 100) str += '0';
  if (msecs < 10)  str += '0';
  str += msecs;
  document.getElementById('t').innerHTML = str;
};

function startTimer(){
  pointer = setInterval(OnStartTimer, 50);
};

function stopTimer(){
  clearInterval(pointer);
};

function reset(){
  console.log('Reset');
  stopTimer();
  document.getElementById('l')  .innerHTML = '-';
  document.getElementById('t')  .innerHTML = '-:--.---';
  document.getElementById('rec').innerHTML = '';
  document.getElementById('sum').innerHTML = '';
  connection.send('Reset');
};

function thold(){
  var txt = 'Thold ' + document.getElementById('th').value;
  console.log(txt);
  connection.send(txt);
};

connection.onopen = function(){
  connection.send('Connect ' + new Date());
};

connection.onmessage = function(msg){
  console.log('Server: ', msg.data);
  if (msg.data.startsWith('Reset'))
    stopTimer();

  if (msg.data.startsWith('Start')){
    document.getElementById('l').innerHTML = msg.data.split(' ')[1];
    timeStart = new Date().getTime();
    startTimer();
  }

  if (msg.data.startsWith('Finish')){
    stopTimer();
    timerOnFinish(msg.data);
  }

  if (msg.data.startsWith('Thold'))
    document.getElementById('th').value = parseInt(msg.data.substr(6));

  if (msg.data.startsWith('LedState')){
    LedState = parseInt(msg.data.substr(9));
    if (LedState == 1)
      document.getElementById('led').style.backgroundColor = "green";
    else
      document.getElementById('led').style.backgroundColor = "red";
  }
};

connection.onerror = function(error){
  console.log('WebSocket Error ' + error);
};

</script>
)html";

const char* CHtml::body = R"html(
<body>
<table>
<tr>
<th colspan="2"; class="tname">MotoGymkhana39</th>
</tr>
<tr>
  <td class="h w30"><span class="led" id="led"></span>&nbsp;&nbsp;Run #</td>
  <td class="h">Time:</td>
</tr>
<tr>
  <td class="b"><p id="l">-</p></td>
  <td class="b"><p id="t">-:--.---</p></td>
</tr>
<tr>
  <td><input class="but f" type="button" value="Reset" onclick="reset();"/></td>
  <td class="f">Threshold: <input type="number" id="th" value="3" min="0" max="120" onchange="thold();"/> s</td>
</tr>
<tr>
<td class="g f">Best time:</td>
<td class="g f"><p id="rec"></p></td>
</tr>
<tr>
<td class="f">List:</td>
<td><p class="f" id="sum"></p></td>
</tr>
</table>
</body>
)html";

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

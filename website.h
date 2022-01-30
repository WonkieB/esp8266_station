const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     background-color:whitesmoke;
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    table, th, td {
      border:3px solid black;
    }
    td, th {
      text-align: right;
      padding: 8px;
    }
    td:nth-child(even) {
      text-align: center;
    }
    .styled-table {
      background-color:F7F7F7;
      border-collapse: collapse;
      margin: 0px auto;
      font-size: 2.9em;
      font-family: Arial;
      min-width: 400px;
      box-shadow: 0 0 200px rgba(0, 0, 0, 0.15);
 	}
    .styled-table thead tr {
    background-color: #009879;
    color: #ffffff;
    text-align: left;
}
.styled-table th,
.styled-table td {
    padding: 12px 15px;
}
.styled-table tbody tr {
    border-bottom: 1px solid #dddddd;
}


.styled-table tbody tr:last-of-type {
    border-bottom: 2px solid darkslategrey;
}
.styled-table tbody tr.active-row {
    font-weight: bold;
    color: #009879;
}

.basic-grey {
	margin-left:auto;
	margin-right:auto;
    max-width: 800px;
    background: #F7F7F7;
    padding: 25px 15px 25px 10px;
    font: 22px Arial;
    color: #888;
    text-shadow: 1px 1px 1px #FFF;
    border:1px solid #E4E4E4;
}

.basic-grey h1 {
    font-size: 35px;
    padding: 0px 0px 10px 40px;
    display: block;
    border-bottom:1px solid #E4E4E4;
    margin: -10px -15px 30px -10px;;
    color: black;
}
.basic-grey h1>span {
    display: block;
    font-size: 11px;
}

.basic-grey label {
    display: block;
    margin: 0px auto;
}
.basic-grey label>span {
    float: left;
    width: 20%;
    text-align: right;
    padding-right: 10px;
    margin-top: 10px;
    color: #888;
}
.basic-grey input[type="text"], .basic-grey input[type="email"], .basic-grey textarea, .basic-grey select {
	border: 2px solid #DADADA;
	color: #888;
	height: 30px;
	margin-bottom: 16px;
	margin-right: 6px;
	margin-top: 2px;
	outline: 0 none;
	padding: 3px 3px 3px 5px;
	width: 70%;
	font-size: 22px;
	line-height:15px;
	box-shadow: inset 0px 1px 4px #ECECEC;
	-moz-box-shadow: inset 0px 1px 4px #ECECEC;
	-webkit-box-shadow: inset 0px 1px 4px #ECECEC;
}
.basic-grey .button {
	align: "left";
    margin-left: -140px;
	background: #E27575;
	border: none;
	padding: 10px 25px 10px 25px;
	color: #FFF;
	box-shadow: 1px 1px 5px #B6B6B6;
	border-radius: 3px;
	text-shadow: 1px 1px 1px #9E3F3F;
	cursor: pointer;
}
.basic-grey .button:hover {
    background: #CF7A7A
}

  </style>
</head>
<body>
  <h2>SiSK LAB 2021</h2>
  <p>

  <table class="styled-table" align="center">
    <tr>
      <th>Temperature:</th>
      <td><span id="time">%TEMPERATURE%</span>&deg;C</td>

    </tr>
    <tr>
      <th>Humidity:</th>
      <td><span id="time">%HUMIDITY%</span>%</td>
    </tr>
    <tr>
      <th>Time:</th>
      <td><span id="time">%TIME%</span> sec</td>
    </tr>
  </table>
  <p>
  
   <form action="/get" class="basic-grey">
    <h1>Configuration</h1>
    <label>
    <span>SSID: </span><input type="text" name="SSID" placeholder="SSID of your WiFi network">
    </label>
    <label>
    <span>WiFi password: </span><input type="text" name="WiFi_Password" placeholder="Your WiFi password">
    </label>
    <label>
    	<span> </span> 
        <input type="submit" class="button" value="Submit" /> 
    </label>  
  </form>
</body>

<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("time").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/time", true);
  xhttp.send();
}, 1000 ) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)rawliteral";

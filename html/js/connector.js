var connection;
var lastMeasure = new Object();
if(!("serial" in navigator)) {
    document.getElementById("noserial").style.display = "block";
}
else {
    connection = SimpleWebSerial.setupSerialConnection({
        requestAccessOnPageLoad: true,
        baudRate: 115200
    });

    connection.on("value", value => {
        lastMeasure['rpm'] = Math.round(value['erpm']*200/getAsInt("num_magnets"));
        document.getElementById('rpm').innerText = lastMeasure['rpm'];

        lastMeasure['current'] = (value['current']/100.0)
        document.getElementById('current').innerText=lastMeasure['current'].toFixed(1);

        lastMeasure['voltage'] = (value['voltage']/100.0);
        document.getElementById('voltage').innerText= lastMeasure['voltage'].toFixed(1);

        lastMeasure['thrust'] =value['thrust'];
        document.getElementById('thrust').innerText = lastMeasure['thrust'].toFixed(1);

        lastMeasure['temp'] = value['temp'];
        document.getElementById('temp').innerText =lastMeasure['temp'];

        lastMeasure['consumed'] = value['consumed'];
        document.getElementById('consumed').innerText = value['consumed'];
    });
    connection.on("term", value => {
        var log = JSON.stringify(value);
        var ta = document.getElementById('term');
        ta.innerText = ta.innerText+"\n"+log;
        ta.scrollTop = ta.scrollHeight;
    });
}

function tare() {
    connection.send("cmd", {tare: 1});
}

function setThrottle(val) {
    document.getElementById('throttle').innerHTML = val.toFixed(1);
    connection.send("cmd", {throttle:val});
    //console.log(val);
}

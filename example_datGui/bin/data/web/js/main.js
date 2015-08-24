var socket;

var messageDiv;
var statusDiv;

$(document).ready( function() {
	
	document.getElementById("brow").textContent = " " + BrowserDetect.browser + " "
		+ BrowserDetect.version +" " + BrowserDetect.OS +" ";

	messageDiv = document.getElementById("messages");
	statusDiv = document.getElementById("status");
});

// setup web socket
function setupSocket(){

	// setup websocket
	socket = new WebSocket(get_appropriate_ws_url());

	// open
	try {
		socket.onopen = function() {
			statusDiv.style.backgroundColor = "#40ff40";
			statusDiv.textContent = " websocket connection opened ";
			var myJson = {type:"initRequest"};	
			socket.send(JSON.stringify(myJson));
		} 

		// received message
		socket.onmessage =function got_packet(msg) {
			messageDiv.innerHTML = msg.data + "<br />" + messageDiv.innerHTML;
			// console.log(msg.data);
			śyncGui(msg.data);
		}

		socket.onclose = function(){
			statusDiv.style.backgroundColor = "#ff4040";
			statusDiv.textContent = " websocket connection CLOSED ";
		}
	} catch(exception) {
		alert('<p>Error' + exception);  
	}
}

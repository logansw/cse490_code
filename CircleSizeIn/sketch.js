let spawnRate = 0;
let serial; // the Serial object
let pHtmlMsg; // used for displaying messages via html (optional)
let serialOptions;

let xPos = [];
let yPos = [];
let speed = [];

function setup() {
  createCanvas(400, 400);

  // Setup Web Serial using serial.js
  serial = new Serial();
  serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
  serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
  serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
  serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

  // If we have previously approved ports, attempt to connect with them
  serial.autoConnectAndOpenPreviouslyApprovedPort(serialOptions);

  // Add in a lil <p> element to provide messages. This is optional
  pHtmlMsg = createP("Click anywhere on this page to open the serial connection dialog");

  xPos.push(1);
  yPos.push(10);
  speed.push(10);
}

function onSerialErrorOccurred(eventSender, error) {
  console.log("onSerialErrorOccurred", error);
  pHtmlMsg.html(error);
}

function onSerialConnectionOpened(eventSender) {
  console.log("onSerialConnectionOpened");
  pHtmlMsg.html("Serial connection opened successfully");
}

function onSerialConnectionClosed(eventSender) {
  console.log("onSerialConnectionClosed");
  pHtmlMsg.html("onSerialConnectionClosed");
}

function onSerialDataReceived(eventSender, newData) {
  console.log("onSerialDataReceived", newData);
  pHtmlMsg.html("onSerialDataReceived: " + newData);

  // Parse the incoming value as a float
  spawnRate = parseFloat(newData);
}

function mouseClicked() {
  if (!serial.isOpen()) {
    serial.connectAndOpen(null, serialOptions);
  }
}

function draw() {
  background(100);

  noStroke();
  fill(250);

  let circleDiameter = 5;

  count = Math.round(spawnRate * 200);
  xPos.length = count;
  yPos.length = count;
  speed.length = count;

  for (i = 0; i < count; i++) {
    if (xPos[i] === undefined) {
      xPos[i] = Math.random() * width;
      yPos[i] = Math.random() * height;
      speed[i] = Math.random() * 5 + 5;
    }

    circle(xPos[i], yPos[i], circleDiameter);
    yPos[i] += speed[i];
    if (yPos[i] > height) {
      xPos[i] = Math.random() * width;
      yPos[i] = 0;
      speed[i] = Math.random() * 5 + 5;
    }
  }
}
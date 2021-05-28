// This is a basic web serial template for p5.js. Please see:
// https://makeabilitylab.github.io/physcomp/communication/p5js-serial
//
// By Jon E. Froehlich
// @jonfroehlich
// http://makeabilitylab.io/
//

let video;
let poseNet;
let human = null;

let pHtmlMsg;
let serialOptions = { baudRate: 115200  };
let serial;

function setup() {
  createCanvas(640, 480);

  video = createCapture(VIDEO);
  video.hide();

  // PoseNet initialization and subscription
  poseNet = ml5.poseNet(video, onPoseNetModelReady);
  poseNet.on("pose", onPoseDetected);

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

  loadEyes();
  loadNoses();
}

function onPoseNetModelReady(){
  print("onPoseNetModelReady");
  pHtmlMsg.html("onPoseNetModelReady");
}

function onPoseDetected(poses){
  human = poses[0];

  if(human && serial.isOpen()){
    let xNormalized = human.pose.nose.x / width;
    let yNormalized = human.pose.nose.y / height;

    let strDataOutput = nf(xNormalized, 0, 3) + "," + nf(yNormalized, 0, 3);
    serial.writeLine(strDataOutput);
  }
}

async function draw() {
  background(100);

  image(video, 0, 0);

  // image(eyes[0], 0, 0, 50, 50);

  if(human){
    drawEye(human.pose.leftEye.x, human.pose.leftEye.y, eyes[0]);
    drawEye(human.pose.rightEye.x, human.pose.rightEye.y, eyes[0]);

    drawNose(human.pose.nose.x, human.pose.nose.y, noses[0]);
  }else{
    noStroke();
    fill(255);
    text("Waiting for PoseNet model", 30, 30);
  }
}

function drawEye(x, y, eyeImage){
  let eyeSize = 40;

  image(eyeImage, x - (eyeSize / 2), y - (eyeSize / 2), eyeSize, eyeSize);
}

function drawNose(x, y, noseImage) {
  let noseSize = 60;

  image(noseImage, x - (noseSize / 2), y - (noseSize / 2), noseSize, noseSize);
}

/**
 * Callback function by serial.js when there is an error on web serial
 *
 * @param {} eventSender
 */
 function onSerialErrorOccurred(eventSender, error) {
  console.log("onSerialErrorOccurred", error);
  pHtmlMsg.html(error);
}

/**
 * Callback function by serial.js when web serial connection is opened
 *
 * @param {} eventSender
 */
function onSerialConnectionOpened(eventSender) {
  console.log("onSerialConnectionOpened");
  pHtmlMsg.html("Serial connection opened successfully");
}

/**
 * Callback function by serial.js when web serial connection is closed
 *
 * @param {} eventSender
 */
function onSerialConnectionClosed(eventSender) {
  console.log("onSerialConnectionClosed");
  pHtmlMsg.html("onSerialConnectionClosed");
}

/**
 * Callback function serial.js when new web serial data is received
 *
 * @param {*} eventSender
 * @param {String} newData new data received over serial
 */
function onSerialDataReceived(eventSender, newData) {
  console.log("onSerialDataReceived", newData);
  pHtmlMsg.html("onSerialDataReceived: " + newData);
}

/**
 * Called automatically by the browser through p5.js when mouse clicked
 */
function mouseClicked() {
  if (!serial.isOpen()) {
    serial.connectAndOpen(null, serialOptions);
  }
}
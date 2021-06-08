let faceapi;
let video;
let detections;

const MIN_WIDTH = 1.0;
const MAX_WIDTH = 4.0;
const MIN_HEIGHT = 1.0;
const MAX_HEIGHT = 4.0;

const features = {
  BROWS: 0,
  EYES: 1,
  NOSE: 2,
  MOUTH: 3,
}

let feature = features.BROWS;

let indices = [0, 0, 0, 0];
let widths = [1.0, 1.0, 1.0, 1.0];
let heights = [1.0, 3.0, 1.0, 1.0];
let maximums;

let serialOptions = { baudRate: 115200 };
let serial;

// by default all options are set to true
const detectionOptions = {
  withLandmarks: true,
  withDescriptors: false,
};

function setup() {
  createCanvas(640, 480);

  // Video loading
  video = createCapture(VIDEO);
  video.hide();

  // Initialize faceapi
  faceapi = ml5.faceApi(video, detectionOptions, modelReady);

  // Serial set up
  serial = new Serial();
  serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
  serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
  serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
  serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

  // If we have previously approved ports, attempt to connect with them
  serial.autoConnectAndOpenPreviouslyApprovedPort(serialOptions);

  loadEyes();
  loadNoses();
  loadBrows();
  loadMouths();

  maximums = [brows.length - 1, eyes.length - 1, noses.length - 1, mouths.length - 1];
}

function modelReady() {
  console.log("ready!");
  console.log(faceapi);
  faceapi.detect(gotResults);
}

function gotResults(err, result) {
  if (err) {
    console.log(err);
    return;
  }
  detections = result;

  background(255);
  image(video, 0, 0, width, height);
  if (detections) {
    if (detections.length > 0) {
      drawLandmarks(detections);
    }
  }
  faceapi.detect(gotResults);
}

function drawLandmarks(detections) {
  for (let i = 0; i < detections.length; i++) {
    let detection = detections[i];
    drawBrow(detection.parts.leftEyeBrow);
    drawBrow(detection.parts.rightEyeBrow);
    drawEye(detection.parts.leftEye);
    drawEye(detection.parts.rightEye);
    drawNose(detection.parts.nose);
    drawMouth(detection.parts.mouth);
  }
}

function drawBrow(part) {
  let bounds = getDimensions(part);
  let w1 = bounds[2] - bounds[0];
  let h1 = bounds[3] - bounds[1];
  let w2 = w1 * widths[features.BROWS];
  let h2 = h1 * heights[features.BROWS];

  image(brows[indices[features.BROWS]], bounds[0] - ((w2 - w1) / 2), bounds[1] - ((h2 - h1) / 2), w2, h2);
}

function drawEye(part) {
  let bounds = getDimensions(part);
  let w1 = bounds[2] - bounds[0];
  let h1 = bounds[3] - bounds[1];
  let w2 = w1 * widths[features.EYES] * 2;
  let h2 = h1 * heights[features.EYES] * 2;

  image(eyes[indices[features.EYES]], bounds[0] - ((w2 - w1) / 2), bounds[1] - ((h2 - h1) / 2), w2, h2);
}

function drawNose(part) {
  let bounds = getDimensions(part);

  let w1 = bounds[2] - bounds[0];
  let h1 = bounds[3] - bounds[1];
  let w2 = 2 * w1 * widths[features.NOSE];
  let h2 = h1 * heights[features.NOSE];
  image(noses[indices[features.NOSE]], bounds[0] - ((w2 - w1) / 2), bounds[1] - ((h2 - h1) / 2), w2, h2);
}

function drawMouth(part) {
  let bounds = getDimensions(part);
  let w1 = bounds[2] - bounds[0];
  let h1 = bounds[3] - bounds[1];
  let w2 = w1 * widths[features.MOUTH];
  let h2 = h1 * heights[features.MOUTH];
  image(mouths[indices[features.MOUTH]], bounds[0] - ((w2 - w1) / 2), bounds[1] - ((h2 - h1) / 2), w2, h2);
}

function getDimensions(points) {
  let xMin = 10000;
  let xMax = 0;
  let yMin = 10000;
  let yMax = 0;
  for (let i = 0; i < points.length; i++) {
    let x = points[i]._x;
    let y = points[i]._y;

    if (x < xMin)
      xMin = x;
    if (x > xMax)
      xMax = x;
    if (y < yMin)
      yMin = y;
    if (y > yMax)
      yMax = y;
  }

  return [xMin, yMin, xMax, yMax];
}

/**
 * Callback function by serial.js when there is an error on web serial
 *
 * @param {} eventSender
 */
 function onSerialErrorOccurred(eventSender, error) {
  console.log("onSerialErrorOccurred", error);
}

/**
 * Callback function by serial.js when web serial connection is opened
 *
 * @param {} eventSender
 */
function onSerialConnectionOpened(eventSender) {
  console.log("onSerialConnectionOpened");
}

/**
 * Callback function by serial.js when web serial connection is closed
 *
 * @param {} eventSender
 */
function onSerialConnectionClosed(eventSender) {
  console.log("onSerialConnectionClosed");
}

/**
 * Callback function serial.js when new web serial data is received
 *
 * @param {*} eventSender
 * @param {String} newData new data received over serial
 */
function onSerialDataReceived(eventSender, newData) {
  console.log("onSerialDataReceived", newData);

  let hasChanged = false;

  if (newData == "left" && indices[feature] > 0) {
    indices[feature]--;
    hasChanged = true;
  } else if (newData == "right" && indices[feature] < maximums[feature]) {
    indices[feature]++;
    hasChanged = true;
  } else if (newData == "up" && feature > features.BROWS) {
    feature--;
    hasChanged = true;
  } else if (newData == "down" && feature < features.MOUTH) {
    feature++;
    hasChanged = true;
  }

  if (newData == "increase_width") {
    widths[feature] += 0.1;
    if (widths[feature] > MAX_WIDTH) {
      widths[feature] = MAX_WIDTH;
    }
  } else if (newData == "decrease_width") {
    widths[feature] -= 0.1;
    if (widths[feature] < MIN_WIDTH) {
      widths[feature] = MIN_WIDTH;
    }
  } else if (newData == "increase_height") {
    heights[feature] += 0.1;
    if (heights[feature] > MAX_HEIGHT) {
      heights[feature] = MAX_HEIGHT;
    }
  } else if (newData == "decrease_height") {
    heights[feature] -= 0.1;
    if (heights[feature] < MIN_HEIGHT) {
      heights[feature] = MIN_HEIGHT;
    }
  }

  if (hasChanged) {
    let message = "";
    switch (feature) {
      case features.BROWS:
        message = message + "Brow -- ";
        if (indices[features.BROWS] == 0) {
          message = message + "  0 >";
        } else if (indices[features.BROWS] == 9) {
          message = message + "< 9  ";
        } else {
          message = message + "< " + indices[features.BROWS] + " >";
        }
        break;
      case features.EYES:
        message = message + "Eye --- ";
        if (indices[features.EYES] == 0) {
          message = message + "  0 >";
        } else if (indices[features.EYES] == 9) {
          message = message + "< 9  ";
        } else {
          message = message + "< " + indices[features.EYES] + " >";
        }
        break;
      case features.NOSE:
        message = message + "Nose -- ";
        if (indices[features.NOSE] == 0) {
          message = message + "  0 >";
        } else if (indices[features.NOSE] == 9) {
          message = message + "< 9  ";
        } else {
          message = message + "< " + indices[features.NOSE] + " >";
        }
        break;
      case features.MOUTH:
        message = message + "Mouth - ";
        if (indices[features.MOUTH] == 0) {
          message = message + "  0 >";
        } else if (indices[features.MOUTH] == 9) {
          message = message + "< 9  ";
        } else {
          message = message + "< " + indices[features.MOUTH] + " >";
        }
        break;
    }
    serial.writeLine(message);
  }
}

/**
 * Called automatically by the browser through p5.js when mouse clicked
 */
function mouseClicked() {
  if (!serial.isOpen()) {
    serial.connectAndOpen(null, serialOptions);
  }
}
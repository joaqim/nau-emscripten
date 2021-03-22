// helper function

const RADIUS = 0;

var x = 0;
var y = 0;

// setup of the canvas

var canvas = document.querySelector("canvas");

// pointer lock object forking for cross browser

canvas.requestPointerLock =
  canvas.requestPointerLock || canvas.mozRequestPointerLock;

document.exitPointerLock =
  document.exitPointerLock || document.mozExitPointerLock;

canvas.onclick = function () {
  canvas.requestPointerLock();
};

// pointer lock event listeners

// Hook pointer lock state change events for different browsers
document.addEventListener("pointerlockchange", lockChangeAlert, false);
document.addEventListener("mozpointerlockchange", lockChangeAlert, false);

function lockChangeAlert() {
  if (
    document.pointerLockElement === canvas ||
    document.mozPointerLockElement === canvas
  ) {
    console.log("The pointer lock status is now locked");
    document.addEventListener("mousemove", updatePosition, {
      capture: true,
      passive: true,
    });
  } else {
    console.log("The pointer lock status is now unlocked");
    document.removeEventListener("mousemove", updatePosition, {
      capture: true,
      passive: true,
    });
  }
}

var tracker = document.getElementById("tracker");

function updatePosition(e) {
  x += e.movementX;
  y += e.movementY;
  if (x > canvas.width + RADIUS) {
    x = -RADIUS;
  }
  if (y > canvas.height + RADIUS) {
    y = -RADIUS;
  }
  if (x < -RADIUS) {
    x = canvas.width + RADIUS;
  }
  if (y < -RADIUS) {
    y = canvas.height + RADIUS;
  }
  tracker.textContent = "X position: " + x + ", Y position: " + y;
}

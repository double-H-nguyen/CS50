originalImageURL = "https://images.pexels.com/photos/220201/pexels-photo-220201.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260";
tennisImageURL = "https://cdn.pixabay.com/photo/2020/11/27/18/59/tennis-5782695_1280.jpg";
spaceImageURL = "https://images.pexels.com/photos/1169754/pexels-photo-1169754.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260";
gameImageURL = "https://images.pexels.com/photos/4792731/pexels-photo-4792731.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260";

document.addEventListener("DOMContentLoaded", function () {
  let tennisBtn = document.querySelector("#tennis-btn");
  let spaceBtn = document.querySelector("#space-btn");
  let gameBtn = document.querySelector("#game-btn");

  // if hover over button, change background image to relevant image
  tennisBtn.addEventListener("mouseenter", function () {
    changeBackgroundImage(tennisImageURL);
  });

  spaceBtn.addEventListener("mouseenter", function () {
    changeBackgroundImage(spaceImageURL);
  });

  gameBtn.addEventListener("mouseenter", function () {
    changeBackgroundImage(gameImageURL);
  });

  // if mouse is not hovering button, then go back to original image
  tennisBtn.addEventListener("mouseleave", function () {
    changeBackgroundImage(originalImageURL);
  });

  spaceBtn.addEventListener("mouseleave", function () {
    changeBackgroundImage(originalImageURL);
  });

  gameBtn.addEventListener("mouseleave", function () {
    changeBackgroundImage(originalImageURL);
  });
});

function changeBackgroundImage(imageUrl) {
  bodyElement = document.body;
  bodyElement.style.background = `url(${imageUrl})`;
  bodyElement.style.backgroundPosition = "center";
  bodyElement.style.backgroundSize = "cover";
  bodyElement.style.backgroundRepeat = "no-repeat";
  bodyElement.style.transition = "0.1s linear";
}

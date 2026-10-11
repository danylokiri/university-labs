document.addEventListener("DOMContentLoaded", function () {
  const images = document.querySelectorAll(".slider-img");
  const controlls = document.querySelectorAll(".controlls");
  let imageIndex = 0;
  const toggleButton = document.getElementById("autoToggle");
  const closeButton = document.querySelector(".close_slider");
  const dotsContainer = document.querySelector(".slider-dots");
  let interval;
  let sliderVisible = true;

function show(index) {
  images[imageIndex].classList.remove("active");
  images[index].classList.add("active");
  updateDots(index);
  imageIndex = index;
}

function nextSlide() {
  let index = imageIndex + 1;
  if (index >= images.length) {
    index = 0;
  }
  show(index);
}

function toggleSlider() {
  const slider = document.querySelector(".slider-general");
  if (sliderVisible) {
    slider.style.display = "none";
    closeButton.innerHTML = "Show";
  } else {
    slider.style.display = "block";
    closeButton.innerHTML = "Hide";
  }
  sliderVisible = !sliderVisible;
}

function updateDots(index) {
  const dots = document.querySelectorAll(".dot");
  dots.forEach((dot, i) => {
    dot.classList.toggle("active", i === index);
  });
}

function createDots() {
  images.forEach((_, i) => {
    const dot = document.createElement("span");
    dot.classList.add("dot");
    dot.addEventListener("click", () => show(i));
    dotsContainer.appendChild(dot);
  });
}

closeButton.addEventListener("click", toggleSlider);

controlls.forEach((e) => {
  e.addEventListener("click", (event) => {
    if (event.target.classList.contains("prev")) {
      let index = imageIndex - 1;

      if (index < 0) {
        index = images.length - 1;
      }

      show(index);
    } else if (event.target.classList.contains("next")) {
      nextSlide();
    }
  });
});

show(imageIndex);
createDots();

toggleButton.addEventListener("click", (event) => {
  if (event.target.classList.contains("start")) {
    event.target.innerHTML = "Auto off";
    clearInterval(interval);
  } else {
    event.target.innerHTML = "Auto on";
    interval = setInterval(nextSlide, 3000);
  }
  event.target.classList.toggle("start");
});

  const canvas = document.getElementById("myCanvas");
  const ctx = canvas.getContext("2d");
  let isDrawing = false;

  // Установка начальных параметров рисования
  ctx.lineWidth = 10; // Начальная толщина линии
  ctx.strokeStyle = "#000000"; // Начальный цвет линии

      function clearCanvas() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
      }

      function changeBrushSize(size) {
        ctx.lineWidth = size;
      }

      function changeBrushColor(color) {
        ctx.strokeStyle = color;
      }

      function addBackgroundImage() {
        const input = document.getElementById("imageInput");
        const file = input.files[0];
        const reader = new FileReader();

        reader.onload = function (e) {
          const img = new Image();
          img.src = e.target.result;

          img.onload = function () {
            ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
          };
        };

        reader.readAsDataURL(file);
      }

      function saveCanvas() {
        const dataURL = canvas.toDataURL();
        const link = document.createElement("a");
        link.href = dataURL;
        link.download = "canvas_image.png";
        link.click();
      }

      function copyCanvas() {
        const tempCanvas = document.createElement("canvas");
        tempCanvas.width = canvas.width;
        tempCanvas.height = canvas.height;
        const tempCtx = tempCanvas.getContext("2d");
        tempCtx.drawImage(canvas, 0, 0);

        tempCanvas.toBlob(function (blob) {
          const clipboardData = new ClipboardEvent("").clipboardData || new DataTransfer();
          clipboardData.items.add(new File([blob], "copied_image.png"));
          document.dispatchEvent(new ClipboardEvent("paste", { clipboardData }));
        });

        alert("Canvas content copied to clipboard!");
      }

      canvas.addEventListener("mousedown", (e) => {
        isDrawing = true;
        draw(e);
      });

      canvas.addEventListener("mouseup", () => {
        isDrawing = false;
        ctx.beginPath();
      });

      canvas.addEventListener("mousemove", (e) => draw(e));

      function draw(e) {
        if (!isDrawing) return;

        const rect = canvas.getBoundingClientRect();
        ctx.lineCap = "round";
        ctx.lineTo(e.clientX - rect.left, e.clientY - rect.top);
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(e.clientX - rect.left, e.clientY - rect.top);
      }
    });

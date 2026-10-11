document.addEventListener("DOMContentLoaded", function () {
  const canvas = document.getElementById("myCanvas");
  const ctx = canvas.getContext("2d");
  let isDrawing = false;
  let points = [];
  let brushColor = "#000000"; // Начальный цвет кисти
  let brushSize = 10; // Начальный размер кисти

  function clearCanvas() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    points = [];
  }

  function changeBrushSize(size) {
    brushSize = size;
    ctx.lineWidth = size;
  }

  function changeBrushColor(color) {
    brushColor = color;
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

  function draw(e) {
    if (!isDrawing) return;

    const rect = canvas.getBoundingClientRect();
    ctx.lineCap = "round";
    ctx.lineTo(e.clientX - rect.left, e.clientY - rect.top);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo(e.clientX - rect.left, e.clientY - rect.top);

    points.push({
      x: e.clientX - rect.left,
      y: e.clientY - rect.top,
      color: brushColor,
      size: brushSize,
    });
  }
function redrawCanvas() {
  clearCanvas();  // Clear the canvas before redrawing

  points.forEach((point, index) => {
    ctx.lineCap = "round";
    ctx.lineWidth = point.size;
    ctx.strokeStyle = point.color;

    if (index === 0) {
      ctx.beginPath();
      ctx.moveTo(point.x, point.y);
    } else {
      ctx.lineTo(point.x, point.y);
      ctx.stroke();
    }
  });
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

  // TOOLBAR
  const toolBar = document.getElementById("toolbar");

  function createButton(label, iconClass, onClick) {
    const button = document.createElement("button");
    button.classList.add("btn");
    button.innerHTML = `<i class="${iconClass}"></i> ${label}`;
    button.addEventListener("click", onClick);
    toolBar.appendChild(button);
  }

  // Clear button
  createButton("Clear", "fas fa-ban", clearCanvas);

  // Copy button
  createButton("Copy", "far fa-copy", copyCanvas);

  // Download button
  createButton("Download", "fas fa-download", function () {
    const dataUrl = canvas.toDataURL("image/png").replace(/^data:image\/[^;]/, 'data:application/octet-stream');
    const link = document.createElement("a");
    link.href = dataUrl;
    link.download = "canvas_image.png";
    link.click();
  });

  // Brush Color input
  const colorPicker = document.createElement("input");
  colorPicker.type = "color";
  colorPicker.id = "color-picker";
  colorPicker.addEventListener("input", function () {
    changeBrushColor(this.value);
  });
  toolBar.appendChild(colorPicker);

  // Brush Size input
  const brushSizeInput = document.createElement("input");
  brushSizeInput.type = "range";
  brushSizeInput.id = "brushSize";
  brushSizeInput.min = 1;
  brushSizeInput.max = 50;
  brushSizeInput.value = 10;
  brushSizeInput.addEventListener("input", function () {
    changeBrushSize(this.value);
  });
  toolBar.appendChild(brushSizeInput);

  // Background button
  createButton("Background", "fas fa-image", function () {
    const imageUrl = prompt("Enter image URL:");
    if (imageUrl) {
      const img = new Image();
      img.src = imageUrl;
      img.onload = function () {
        ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
      };
    }
  });

  // Timestamp button
  createButton("Timestamp", "far fa-clock", function () {
    const timestamp = new Date().toLocaleString();
    ctx.font = "16px Arial";
    ctx.fillStyle = "#000000";
    ctx.fillText(timestamp, 10, 20);
  });

  // Restore button
  createButton("Restore", "fas fa-undo", function () {
    const storedPoints = JSON.parse(localStorage.getItem('points')) || [];
    points = storedPoints;
    redrawCanvas();
  });

  // Save button
  createButton("Save", "fas fa-save", function () {
    localStorage.setItem('points', JSON.stringify(points));
  });
});

let result = document.getElementById('result');

function display(val) {
  result.value += val;
}

function clearScreen() {
  result.value = '';
}

function calculate() {
  try {
    result.value = eval(result.value);
  } catch (err) {
    alert('Invalid');
  }
}

function backspace() {
    var value = document.getElementById("result").value;
    document.getElementById("result").value = value.substr(0, value.length - 1);
}
function OpenClose()
{
  var checkBox = document.getElementById("25");
  var servo = document.getElementById("MASKA");
  if (checkBox.checked == true){
    servo.classList.add("open");
  } else {
    servo.classList.remove("open");
  }
}

function OnOff()
{
  var checkBox = document.getElementById("26");
  var oczy = document.getElementById("eyes");
  if (checkBox.checked == true){
    oczy.style.fill='cyan';
  } else {
    oczy.style.fill='white';
  }
}

 function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}


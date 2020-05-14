import { WebSocketService } from "./sockets.js";

/** @type HTMLInputElement */
const textInput = document.getElementById("txbuff");

/** @type HTMLButtonElement */
const sendBtn = document.getElementById("sendBtn");

/** @type HTMLTextAreaElement */
const textArea = document.getElementById("rxConsole");

const socket = new WebSocketService({
  host: window.location.hostname,
  port: 80,
  onOpen: () => {
    textInput.removeAttribute("disabled");
    textInput.focus();
    sendBtn.removeAttribute("disabled");
  },
  onClose: () => {
    textInput.setAttribute("disabled", "disabled");
    sendBtn.setAttribute("disabled", "disabled");
  },
  onMessage: evt => {
    const incoming = document.createElement("div");
    incoming.className = "incoming"
    try {
      incoming.innerText = JSON.stringify(JSON.parse(evt.data), undefined, 2);
      incoming.className = "incoming json"
    } catch (error) {
      incoming.innerText = evt.data;
    }
    textArea.appendChild(incoming);
  }
});

const storageKey = "ROVER_CONSOLE_HISTORY";
const storedHistory = localStorage.getItem(storageKey);
let history = storedHistory ? JSON.parse(storedHistory) : [];
let historyPointer = history.length;

function enterpressed() {
  if (!textInput.value){
    return;
  }
  const outgoing = document.createElement("div");
  outgoing.className = "outgoing";
  outgoing.innerText = textInput.value;
  textArea.appendChild(outgoing);
  textArea.scrollTop = textArea.scrollHeight;
  history.push(textInput.value);
  historyPointer = history.length + 1;
  history = history.slice(Math.max(history.length -100, 0))
  localStorage.setItem(storageKey, JSON.stringify(history))
  socket.send(textInput.value);
  textInput.value = "";
}

/**
 * 
 * @param {HTMLInputElement} input 
 */
const selectInputTextValue = (input) => {
  setTimeout(()=>{
    input.setSelectionRange(0, input.value.length)
  }, 10)

}

textInput.onkeydown = (event) => {
  if (event.key === "Enter") {
    enterpressed();
  } else if (event.key === "ArrowUp"){
    historyPointer= Math.max(historyPointer - 1, 0);
    textInput.value = history[historyPointer] || "";
    selectInputTextValue(event.currentTarget);
  } else if (event.key === "ArrowDown") {
    historyPointer= Math.min(historyPointer + 1, history.length);
    textInput.value = history[historyPointer] || "";
    selectInputTextValue(event.currentTarget);
  } else {
    historyPointer = history.length + 1;
  }
};


textArea.onclick = ()=>textInput.focus()

sendBtn.onclick = () => enterpressed();

export class WebSocketService {
  createSocket() {
    this.socket = new WebSocket(
      `ws://${this.options.host}:${this.options.port}/ws`
    );
    this.socket.onopen = () => {
      this.options.onOpen && this.options.onOpen();
      console.log("WebSocket connected");
    };

    this.socket.onclose = () => {
      this.options.onCloce && this.options.onCloce();
      console.log("WebSocket disconnected. Reconnecting...");
      try {
        this.createSocket();
      } catch (e) {
        console.warn("Cannot reconnect", e);
      }
    };

    this.socket.onmessage = evt => {
      this.options.onMessage && this.options.onMessage(evt);
    };
  }

  constructor(
    /** @type {{ host: string, port: number, onOpen: ()=>void, onCloce: ()=>void, onMessage: (evt: MessageEvent) }} */
    options
  ) {
    this.options = options;
    this.createSocket();
    window.addEventListener("unload", ()=>{
      this.socket?.close();
      console.log("Disconnecting due unload...")
    })
  }

  send(message) {
    if (this.socket.readyState === 1) return this.socket.send(message);
  }
}

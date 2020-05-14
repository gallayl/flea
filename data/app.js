import { WebSocketService } from "./sockets.js"
import { MovementService } from "./movement.js"


const socket = new WebSocketService({
    host: window.location.hostname,
    port: 80
})

const movement = new MovementService({
    socket
})

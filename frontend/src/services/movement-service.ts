import { Injectable } from '@furystack/inject'
import { WebSocketService } from './websocket-service'

@Injectable({ lifetime: 'singleton' })
export class MovementService {
  move(throttle: number, steer: number) {
    this.webSocket.socket.send(`move ${Math.round(throttle)} ${Math.round(steer)}`)
  }

  constructor(private readonly webSocket: WebSocketService) {}
}

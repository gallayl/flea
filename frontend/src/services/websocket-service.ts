import { Injectable, Injector } from '@furystack/inject'
import { ScopedLogger } from '@furystack/logging'
import { PathHelper, ObservableValue } from '@furystack/utils'
import { EnvironmentService } from './environment-service'

export interface WebSocketEvent {
  type: 'incoming' | 'outgoing' | 'connection'
  data: string
}

@Injectable({ lifetime: 'singleton' })
export class WebSocketService {
  public isConnected = new ObservableValue<boolean>(false)

  public eventStream: WebSocketEvent[] = []

  public send(data: string) {
    if (this.socket.readyState === WebSocket.OPEN) {
      this.socket.send(data)
      this.eventStream.push({ type: 'outgoing', data })
    }
  }

  private socket: WebSocket
  private logger: ScopedLogger

  private onConnect = (() => {
    this.logger.verbose({
      message: 'Connected',
      data: { socket: this.socket },
    })
    this.isConnected.setValue(true)
  }).bind(this)
  private onDisconnect = (() => {
    this.logger.verbose({
      message: 'Disconnected',
      data: { socket: this.socket },
    })
    this.isConnected.setValue(false)
    this.disposeSocket(this.socket)
    this.socket = this.createSocket()
  }).bind(this)
  private onOpen() {
    this.logger.verbose({ message: 'Opened', data: { socket: this.socket } })
  }
  private onClose() {
    this.logger.verbose({ message: 'Closed', data: { socket: this.socket } })
  }
  private onError = (() => {
    this.logger.warning({
      message: 'Socket Error',
      data: { socket: this.socket },
    })
  }).bind(this)
  private onMessage = ((ev: MessageEvent) => {
    this.logger.verbose({
      message: 'Message received',
      data: { socket: this.socket, data: ev.data },
    })
    this.eventStream.push({ type: 'outgoing', data: ev.data.toString() })
  }).bind(this)

  private createSocket() {
    const socket = new WebSocket('ws://' + PathHelper.joinPaths(this.env.site, 'ws'))
    socket.addEventListener('connect', this.onConnect)
    socket.addEventListener('disconnect', this.onDisconnect)
    socket.addEventListener('open', this.onOpen)
    socket.addEventListener('close', this.onClose)
    socket.addEventListener('error', this.onError)
    socket.addEventListener('message', this.onMessage)
    return socket
  }

  private disposeSocket(socket: WebSocket) {
    socket.removeEventListener('connect', this.onConnect)
    socket.removeEventListener('disconnect', this.onDisconnect)
    socket.removeEventListener('open', this.onOpen)
    socket.removeEventListener('close', this.onClose)
    socket.removeEventListener('error', this.onError)
    socket.removeEventListener('message', this.onMessage)
  }

  constructor(private env: EnvironmentService, injector: Injector) {
    this.logger = injector.logger.withScope('WebSocketService')
    this.socket = this.createSocket()
  }
}

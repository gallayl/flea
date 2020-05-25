import { Injectable, Injector } from '@furystack/inject'
import { ScopedLogger } from '@furystack/logging'
import { PathHelper, ObservableValue } from '@furystack/utils'
import { EnvironmentService } from './environment-service'

@Injectable({ lifetime: 'singleton' })
export class WebSocketService {
  socket: WebSocket
  logger: ScopedLogger
  isConnected = new ObservableValue<boolean>(false)

  onConnect = (() => {
    this.logger.verbose({
      message: 'Connected',
      data: { socket: this.socket },
    })
    this.isConnected.setValue(true)
  }).bind(this)
  onDisconnect = (() => {
    this.logger.verbose({
      message: 'Disconnected',
      data: { socket: this.socket },
    })
    this.isConnected.setValue(false)
  }).bind(this)
  onOpen() {
    this.logger.verbose({ message: 'Opened', data: { socket: this.socket } })
  }
  onClose() {
    this.logger.verbose({ message: 'Closed', data: { socket: this.socket } })
  }
  onError = (() => {
    this.logger.warning({
      message: 'Socket Error',
      data: { socket: this.socket },
    })
  }).bind(this)
  onMessage = (() => {
    this.logger.verbose({
      message: 'Message received',
      data: { socket: this.socket },
    })
  }).bind(this)

  createSocket() {
    const socket = new WebSocket('ws://' + PathHelper.joinPaths(this.env.site, 'ws'))
    socket.addEventListener('connect', this.onConnect)
    socket.addEventListener('disconnect', this.onDisconnect)
    socket.addEventListener('open', this.onOpen)
    socket.addEventListener('close', this.onClose)
    socket.addEventListener('error', this.onError)
    socket.addEventListener('message', this.onMessage)
    return socket
  }

  disposeSocket(socket: WebSocket) {
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

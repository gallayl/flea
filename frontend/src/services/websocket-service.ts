import { Injectable, Injected } from '@furystack/inject'
import { getLogger, type ScopedLogger } from '@furystack/logging'
import { ObservableValue, PathHelper } from '@furystack/utils'
import { EnvironmentService } from './environment-service'

export interface WebSocketEvent<T = unknown> {
  type: 'incoming' | 'outgoing' | 'connection'
  date: Date
  dataObject?: T
  data: string
}

@Injectable({ lifetime: 'singleton' })
export class WebSocketService {
  public isConnected = new ObservableValue<boolean>(false)

  public eventStream: WebSocketEvent[] = []

  public lastMessage = new ObservableValue<Omit<WebSocketEvent, 'date'>>({
    data: 'null',
    type: 'connection',
    dataObject: null,
  })

  public send(data: string) {
    if (this.socket.readyState === WebSocket.OPEN) {
      this.socket.send(data)
      this.lastMessage.setValue({ type: 'outgoing', data })
    }
  }

  declare socket: WebSocket

  public init() {
    this.socket = this.createSocket()
  }

  @Injected((injector) => getLogger(injector).withScope('WebSocketService'))
  declare logger: ScopedLogger

  private onConnect = (() => {
    void this.logger.verbose({
      message: 'Connected',
      data: { socket: this.socket },
    })
    this.isConnected.setValue(true)
  }).bind(this)

  private onDisconnect = (() => {
    void this.logger.verbose({
      message: 'Disconnected',
      data: { socket: this.socket },
    })
    this.isConnected.setValue(false)
    this.disposeSocket(this.socket)
    this.socket = this.createSocket()
    this.lastMessage.setValue({ type: 'connection', data: 'Socket disconnected' })
  }).bind(this)

  private onOpen = (() => {
    void this.logger.verbose({ message: 'Opened', data: { socket: this.socket } })
    this.lastMessage.setValue({ type: 'connection', data: 'Socket opened' })
  }).bind(this)

  private onClose = (() => {
    void this.logger.verbose({ message: 'Closed', data: { socket: this.socket } })
    this.lastMessage.setValue({ type: 'connection', data: 'Socket closed' })
  }).bind(this)

  private onError = (() => {
    void this.logger.warning({
      message: 'Socket Error',
      data: { socket: this.socket },
    })
    this.lastMessage.setValue({ type: 'connection', data: 'Socket Error' })
  }).bind(this)

  private onMessage = ((ev: MessageEvent) => {
    try {
      const data = (ev.data as string).toString()

      const newMessage = {
        type: 'incoming',
        data,
        dataObject: JSON.parse(data),
        date: new Date(),
      } satisfies WebSocketEvent

      this.eventStream.push(newMessage)

      this.lastMessage.setValue(newMessage)
    } catch (error) {
      this.lastMessage.setValue({ type: 'incoming', data: (ev.data as string).toString(), dataObject: null })
    }
    void this.logger.verbose({
      message: 'Message received',
      data: { socket: this.socket, data: ev.data },
    })
  }).bind(this)

  private createSocket() {
    const socket = new WebSocket(`ws://${PathHelper.joinPaths(this.env.site, 'ws')}`)
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

  public async [Symbol.asyncDispose]() {
    if (this.socket) {
      this.disposeSocket(this.socket)
    }
    this.lastMessage[Symbol.dispose]()
  }

  @Injected(EnvironmentService)
  declare env: EnvironmentService
}

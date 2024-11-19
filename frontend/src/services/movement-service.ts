import { Injectable, Injected } from '@furystack/inject'
import { getLogger, ScopedLogger } from '@furystack/logging'
import { WebSocketService } from './websocket-service'
import { ClientSettings } from './client-settings'
import { ObservableValue } from '@furystack/utils'

@Injectable({ lifetime: 'singleton' })
export class MovementService {
  @Injected((injector) => getLogger(injector).withScope('movementService'))
  declare logger: ScopedLogger

  @Injected(WebSocketService)
  declare webSocket: WebSocketService

  @Injected(ClientSettings)
  declare _settings: ClientSettings

  stop() {
    this.logger.verbose({ message: 'Stopped' })
    this.webSocket.send('move 0 0')
  }

  private currentValues = new ObservableValue<{ throttle: number; steer: number; sent: boolean }>({
    throttle: 0,
    steer: 0,
    sent: false,
  })

  public sendInterval = setInterval(() => {
    const { throttle, steer, sent } = this.currentValues.getValue()
    if (!sent) {
      this.webSocket.send(`move ${Math.round(throttle)} ${Math.round(steer)}`)
      this.currentValues.setValue({ throttle, steer, sent: true })
    }
  }, 150)

  async move(throttle: number, steer: number) {
    this.logger.verbose({ message: 'Movement change', data: { throttle, steer } })
    this.currentValues.setValue({ throttle, steer, sent: false })
    // this.webSocket.send(`move ${Math.round(throttle)} ${Math.round(steer)}`)
  }
}

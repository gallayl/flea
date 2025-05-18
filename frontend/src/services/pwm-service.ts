import { Injectable, Injected } from '@furystack/inject'
import { ObservableValue } from '@furystack/utils'
import { getLogger, type ScopedLogger } from '@furystack/logging'
import { WebSocketService } from './websocket-service'

export type PwmValues = [
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
  number,
]

@Injectable({ lifetime: 'singleton' })
export class PwmService {
  @Injected((injector) => getLogger(injector).withScope('pwmService'))
  declare private logger: ScopedLogger

  @Injected(WebSocketService)
  declare webSocket: WebSocketService

  public readonly pwmValues: ObservableValue<PwmValues> = new ObservableValue<PwmValues>([
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  ])

  public setPwmAll(values: PwmValues) {
    this.pwmValues.setValue(values)
    this.webSocket.send(`pwm ${values.map((v, i) => `${i}=${v}`).join(';')}`)
  }

  public setPwm(channel: number, value: number) {
    this.pwmValues.setValue(this.pwmValues.getValue().map((v, i) => (i === channel ? value : v)) as PwmValues)
    this.webSocket.send(`pwm ${channel}=${value}`)
  }
}

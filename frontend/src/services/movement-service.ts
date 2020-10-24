import { Injectable, Injector } from '@furystack/inject'
import { ScopedLogger } from '@furystack/logging'
import { WebSocketService } from './websocket-service'
import { ClientSettings } from './client-settings'
import Semaphore from 'semaphore-async-await'

@Injectable({ lifetime: 'singleton' })
export class MovementService {

  private lock = new Semaphore(1)

  private logger: ScopedLogger

  stop() {
    this.logger.verbose({ message: 'Stopped' })
  }

  async move(throttle: number, steer: number) {
    await this.lock.acquire()
    this.logger.verbose({ message: 'Movement change', data: { throttle, steer } })
    this.webSocket.send(`move ${Math.round(throttle)} ${Math.round(steer)}`)
  }

  constructor(private readonly webSocket: WebSocketService, _settings: ClientSettings, injector: Injector) {
    this.logger = injector.logger.withScope('MovementService')
    this.webSocket.lastMessage.subscribe(m => {
      if (m.type !== 'outgoing'){
        this.lock.release()
    }})
  }
}

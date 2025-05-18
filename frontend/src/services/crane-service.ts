import { Injectable, Injected } from '@furystack/inject'
import { getLogger, type ScopedLogger } from '@furystack/logging'
import { ObservableValue } from '@furystack/utils'
import { ClientSettings } from './client-settings'
import { PwmService } from './pwm-service'

@Injectable({ lifetime: 'singleton' })
export class CraneService {
  @Injected((injector) => getLogger(injector).withScope('movementService'))
  declare logger: ScopedLogger

  @Injected(ClientSettings)
  declare _settings: ClientSettings

  @Injected(PwmService)
  declare pwmService: PwmService

  private readonly channels = {
    baseRotation: 0,
    claw: 1,
    clawRotation: 2,
    upperAxisRotation: 3,
    lowerAxisRotation: 4,
  }

  public readonly clawState = new ObservableValue<'open' | 'close' | 'unknown'>('unknown')

  public async openClaw() {
    void this.logger.verbose({ message: 'Opening claw' })
    this.pwmService.setPwm(this.channels.claw, 150)
    this.clawState.setValue('open')
  }

  public async closeClaw() {
    void this.logger.verbose({ message: 'Closing claw' })
    this.pwmService.setPwm(this.channels.claw, 330)
    this.clawState.setValue('close')
  }

  public readonly clawRotation = new ObservableValue<number>(0)
  public async rotateClaw(degree: number) {
    const minPwm = 100
    const maxPwm = 500

    const minAngle = -45
    const maxAngle = 45

    const calculatedPwm = Math.round(((degree - minAngle) / (maxAngle - minAngle)) * (maxPwm - minPwm) + minPwm)

    this.clawRotation.setValue(degree)
    void this.logger.verbose({ message: `Rotating claw to ${degree} degrees (${calculatedPwm} pulse)` })
    this.pwmService.setPwm(this.channels.clawRotation, calculatedPwm)
  }

  public readonly upperAxisRotation = new ObservableValue<number>(0)
  public async rotateUpperAxis(degree: number) {
    const minPwm = 100
    const maxPwm = 500

    const minAngle = -90
    const maxAngle = 90

    const calculatedPwm = Math.round(((degree - minAngle) / (maxAngle - minAngle)) * (maxPwm - minPwm) + minPwm)

    this.upperAxisRotation.setValue(degree)
    void this.logger.verbose({ message: `Rotating upper axis to ${degree} degrees (${calculatedPwm} pulse)` })
    this.pwmService.setPwm(this.channels.upperAxisRotation, calculatedPwm)
  }

  public readonly lowerAxisRotation = new ObservableValue<number>(0)
  public async rotateLowerAxis(degree: number) {
    const minPwm = 100
    const maxPwm = 500

    const minAngle = -90
    const maxAngle = 90

    const calculatedPwm = Math.round(((degree - minAngle) / (maxAngle - minAngle)) * (maxPwm - minPwm) + minPwm)

    this.lowerAxisRotation.setValue(degree)
    void this.logger.verbose({ message: `Rotating lower axis to ${degree} degrees (${calculatedPwm} pulse)` })
    this.pwmService.setPwm(this.channels.lowerAxisRotation, calculatedPwm)
  }

  public readonly baseRotation = new ObservableValue<number>(0)
  public async rotateBase(degree: number) {
    const minPwm = 100
    const maxPwm = 500

    const minAngle = -90
    const maxAngle = 90

    const calculatedPwm = Math.round(((degree - minAngle) / (maxAngle - minAngle)) * (maxPwm - minPwm) + minPwm)

    this.baseRotation.setValue(degree)
    void this.logger.verbose({ message: `Rotating base to ${degree} degrees (${calculatedPwm} pulse)` })
    this.pwmService.setPwm(this.channels.baseRotation, calculatedPwm)
  }
}

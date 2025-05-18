import { Shade, createComponent } from '@furystack/shades'
import { CraneService } from '../services/crane-service'
import { EnvironmentService } from '../services/environment-service'
import { Button, Input, Paper } from '@furystack/shades-common-components'

export const JoystickPage = Shade({
  shadowDomName: 'flea-joystick-page',
  render: ({ injector, useObservable }) => {
    const craneService = injector.getInstance(CraneService)
    const env = injector.getInstance(EnvironmentService)

    const [clawState] = useObservable('clawState', craneService.clawState)
    const [clawRotation] = useObservable('clawRotation', craneService.clawRotation)
    const [baseRotation] = useObservable('baseRotation', craneService.baseRotation)
    const [upperAxisRotation] = useObservable('upperAxisRotation', craneService.upperAxisRotation)
    const [lowerAxisRotation] = useObservable('lowerAxisRotation', craneService.lowerAxisRotation)

    return (
      <div style={{ width: '100%', height: '100%', position: 'relative' }}>
        {/* <img
          src={`http://${env.site}/stream`}
          alt="webcam-image"
          style={{ objectFit: 'cover', width: '100%', height: '100%', zIndex: '-1' }}
        /> */}
        <div style={{ position: 'absolute', top: '0', left: '0', width: '100%', height: '100%', marginTop: '100px' }}>
          <Paper style={{ display: 'flex', flexDirection: 'row', alignItems: 'center' }}>
            <h2>Claw</h2>
            <Button
              style={{ width: '130px' }}
              onclick={() => {
                if (clawState === 'open') {
                  void craneService.closeClaw()
                } else {
                  void craneService.openClaw()
                }
              }}>
              {clawState === 'open'
                ? '🔒 Close'
                : clawState === 'close'
                  ? '🔓 Open'
                  : clawState === 'moving'
                    ? '⌛ Moving...'
                    : '🔓 Open'}
            </Button>
            <Input
              type="range"
              min="-90"
              max="90"
              step="1"
              value={clawRotation.toFixed(0)}
              oninput={(e) => {
                const { value } = e.target as HTMLInputElement
                void craneService.rotateClaw(Number(value))
              }}
              style={{ width: '100%' }}
            />
          </Paper>
          <Paper style={{ display: 'flex', flexDirection: 'row', alignItems: 'center' }}>
            <h2>Axis</h2>
            <Input
              type="range"
              min="-90"
              max="90"
              step="1"
              value={upperAxisRotation.toFixed(0)}
              oninput={(e) => {
                const { value } = e.target as HTMLInputElement
                void craneService.rotateUpperAxis(Number(value))
              }}
              style={{ width: '100%' }}
            />
            <Input
              type="range"
              min="-90"
              max="90"
              step="1"
              value={lowerAxisRotation.toFixed(0)}
              oninput={(e) => {
                const { value } = e.target as HTMLInputElement
                void craneService.rotateLowerAxis(Number(value))
              }}
              style={{ width: '100%' }}
            />
          </Paper>
          <Paper style={{ display: 'flex', flexDirection: 'row', alignItems: 'center' }}>
            <h2>Base</h2>
            <Input
              type="range"
              min="-90"
              max="90"
              step="1"
              value={baseRotation.toFixed(0)}
              oninput={(e) => {
                const { value } = e.target as HTMLInputElement
                void craneService.rotateBase(Number(value))
              }}
              style={{ width: '100%' }}
            />
          </Paper>
        </div>
      </div>
    )
  },
})

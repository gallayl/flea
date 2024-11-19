import { NippleComponent } from '@furystack/shades-nipple'
import { Shade, createComponent } from '@furystack/shades'
import { MovementService } from '../services/movement-service'
import { EnvironmentService } from '../services/environment-service'

export const JoystickPage = Shade({
  shadowDomName: 'flea-joystick-page',
  render: ({ injector }) => {
    const movementService = injector.getInstance(MovementService)
    const env = injector.getInstance(EnvironmentService)

    return (
      <div style={{ width: '100%', height: '100%', position: 'relative' }}>
        <img
          src={`http://${env.site}/stream`}
          alt="webcam-image"
          style={{ objectFit: 'cover', width: '100%', height: '100%', zIndex: '-1' }}
        />
        <NippleComponent
          style={{ position: 'absolute', top: '0', left: '0', width: '100%', height: '100%' }}
          managerOptions={{}}
          onEnd={() => movementService.stop()}
          onMove={(_ev, data) => {
            const speed = Math.round(data.force * 20 * (data.direction?.y === 'down' ? -1 : 1)) // +/- 300
            const steer = Math.round(Math.cos(data.angle.radian) * 110) // +/-50
            movementService.move(speed, steer)
          }}
        />
      </div>
    )
  },
})

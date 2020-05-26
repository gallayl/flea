import { NippleComponent } from '@furystack/shades-nipple'
import { Shade, createComponent } from '@furystack/shades'
import { MovementService } from '../services/movement-service'

export const JoystickPage = Shade<unknown, { movementService: MovementService }>({
  getInitialState: ({ injector }) => ({ movementService: injector.getInstance(MovementService) }),
  render: ({ getState }) => {
    return (
      <div style={{ width: '100%', height: '100%' }}>
        <NippleComponent
          managerOptions={{}}
          onEnd={() => getState().movementService.stop()}
          onMove={(_ev, data) => {
            const speed = data.force * (data.direction?.y === 'down' ? -1 : 1)
            const steer = Math.cos(data.angle.radian) * 90
            getState().movementService.move(speed, steer)
          }}
        />
      </div>
    )
  },
})

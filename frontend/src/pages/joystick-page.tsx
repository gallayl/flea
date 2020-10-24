import { NippleComponent } from '@furystack/shades-nipple'
import { Shade, createComponent } from '@furystack/shades'
import { MovementService } from '../services/movement-service'
import { EnvironmentService } from '../services/environment-service'

export const JoystickPage = Shade<unknown, { movementService: MovementService }>({
  getInitialState: ({ injector }) => ({ movementService: injector.getInstance(MovementService) }),
  render: ({ getState, injector }) => {
    return (
      <div style={{ width: '100%', height: '100%' }}>
        <img src={`http://${injector.getInstance(EnvironmentService).site}/cam`} alt="webcam-image" style={{objectFit: 'cover', width: '100%', height: '100%', zIndex: '-1'}} onload={(ev)=>{
          (ev.target as HTMLImageElement).src = `http://${injector.getInstance(EnvironmentService).site}/cam?refresh=${Math.random()}`
        }} onerror={(ev)=>{
          ((ev as any).target as HTMLImageElement).src = `http://${injector.getInstance(EnvironmentService).site}/cam?refresh=${Math.random()}`
        }}/>
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

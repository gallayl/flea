import { NippleComponent } from '@furystack/shades-nipple'
import { Shade, createComponent } from '@furystack/shades'
import { WebSocketService } from '../services/websocket-service'

export const JoystickPage = Shade<unknown, { websocketService: WebSocketService }>({
  getInitialState: ({ injector }) => ({ websocketService: injector.getInstance(WebSocketService) }),
  render: () => {
    return (
      <div style={{ width: '100%', height: '100%' }}>
        <NippleComponent managerOptions={{}} />
      </div>
    )
  },
})

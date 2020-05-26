import { Shade, createComponent } from '@furystack/shades'
import { Input, Button } from '@furystack/shades-common-components'
import { WebSocketService } from '../services/websocket-service'

export const ConsolePage = Shade<unknown, { webSocketService: WebSocketService }>({
  getInitialState: ({ injector }) => ({ webSocketService: injector.getInstance(WebSocketService) }),
  shadowDomName: 'flea-console-page',
  render: () => {
    return (
      <div
        style={{
          width: '100%',
          height: '100%',
          flexGrow: '1',
          display: 'flex',
          flexDirection: 'column',
        }}>
        <div style={{ flexGrow: '1' }}>console area</div>
        <div style={{ display: 'flex', flexDirection: 'row' }}>
          <Input autofocus style={{ display: 'block', flexGrow: '1', width: '100%' }} labelTitle="Command" />
          <Button title="Send">Send</Button>
        </div>
      </div>
    )
  },
})

import { Shade, createComponent } from '@furystack/shades'
import { WebSocketService } from '../services/websocket-service'

export const ConsolePage = Shade<unknown, { webSocketService: WebSocketService }>({
  getInitialState: ({ injector }) => ({ webSocketService: injector.getInstance(WebSocketService) }),
  shadowDomName: 'flea-console-page',
  render: () => {
    return <div style={{ width: '100%', height: '100%', flexGrow: '1' }}></div>
  },
})

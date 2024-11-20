import { Shade, createComponent } from '@furystack/shades'
import { Button } from '@furystack/shades-common-components'
import { ObservableValue } from '@furystack/utils'
import { WebSocketService } from '../services/websocket-service'

export const ConsoleEntryList = Shade({
  shadowDomName: 'flea-console-entries',
  constructed: ({ injector, element, useDisposable }) => {
    useDisposable('lastMessageSubscription', () =>
      injector.getInstance(WebSocketService).lastMessage.subscribe(() => {
        element.parentElement?.scrollTo({ top: element.firstElementChild?.scrollHeight || Number.MAX_SAFE_INTEGER })
      }),
    )
  },
  render: ({ injector, useObservable }) => {
    const [_lastMessage] = useObservable('lastMessage', injector.getInstance(WebSocketService).lastMessage)

    return (
      <div style={{ marginTop: '32px' }}>
        {injector.getInstance(WebSocketService).eventStream.map((event) => (
          <div style={{ display: 'flex', flexDirection: 'row', alignItems: 'center', fontFamily: 'monospace' }}>
            <div
              style={{
                fontSize: '20px',
                width: '40px',
                height: '40px',
                textAlign: 'center',
                verticalAlign: 'middle',
                lineHeight: '40px',
                flexShrink: '0',
                marginRight: '1em',
                color: event.type === 'incoming' ? '#aa2233' : '#22bb33',
              }}>
              {event.type === 'incoming' ? '<' : event.type === 'outgoing' ? '>' : '|'}
            </div>
            {event.dataObject ? (
              <code style={{ whiteSpace: 'pre-wrap' }}>{JSON.stringify(event.dataObject, undefined, 2)}</code>
            ) : (
              <div> {event.data} </div>
            )}
          </div>
        ))}
      </div>
    )
  },
})

export const ConsolePage = Shade({
  shadowDomName: 'flea-console-page',
  render: ({ useDisposable, injector }) => {
    const service = injector.getInstance(WebSocketService)

    const command = useDisposable('command', () => new ObservableValue<string>(''))

    return (
      <div
        style={{
          width: '100%',
          height: '100%',
          display: 'flex',
          flexDirection: 'column',
          overflow: 'hidden',
          color: '#bbb',
        }}>
        <div style={{ flexGrow: '1', overflow: 'auto', height: '100px', padding: '1em' }}>
          <ConsoleEntryList />
        </div>
        <form
          style={{ display: 'flex', flexDirection: 'row', width: '100%', flexShrink: '0' }}
          onsubmit={(ev) => {
            ev.preventDefault()
            service.send(command.getValue())
            ;(ev.target as HTMLFormElement).reset()
          }}>
          <input
            autofocus
            style={{ display: 'block', flexGrow: '1', width: '100%' }}
            placeholder="Command"
            onkeyup={(ev) => command.setValue((ev.target as HTMLInputElement).value)}
          />
          <Button title="Send" type="submit">
            Send
          </Button>
        </form>
      </div>
    )
  },
})

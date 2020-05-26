import { AppBar } from '@furystack/shades-common-components'
import { Shade, createComponent, Router, RouteLink } from '@furystack/shades'
import { JoystickPage } from '../pages/joystick-page'
import { ConsolePage } from '../pages/console-page'
import { UpdatePage } from '../pages/update-page'

export const Layout = Shade({
  shadowDomName: 'flea-layout',
  render: () => {
    return (
      <div style={{ width: '100%', height: '100%', overflow: 'hidden', display: 'flex', flexDirection: 'column' }}>
        <AppBar>
          <RouteLink href="/" style={{ marginRight: '2em' }}>
            Flea
          </RouteLink>
          &nbsp;
          <RouteLink href="/">Home</RouteLink>&nbsp;
          <RouteLink href="/console">Console</RouteLink>&nbsp;
          <RouteLink href="/update">Update</RouteLink>&nbsp;
          <div style={{ flex: '1' }} />
        </AppBar>
        <div style={{ width: '100%', height: '100%', flexGrow: '1' }}>
          <Router
            routes={[
              {
                url: '/',
                component: () => <JoystickPage />,
              },
              {
                url: '/console',
                component: () => <ConsolePage />,
              },
              {
                url: '/update',
                component: () => <UpdatePage />,
              },
            ]}
          />
        </div>
      </div>
    )
  },
})

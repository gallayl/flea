import { AppBar, AppBarLink } from '@furystack/shades-common-components'
import { Shade, createComponent, Router } from '@furystack/shades'
import { JoystickPage } from '../pages/joystick-page'
import { ConsolePage } from '../pages/console-page'
import { UpdatePage } from '../pages/update-page'

export const Layout = Shade({
  shadowDomName: 'flea-layout',
  render: () => {
    return (
      <div
        style={{
          width: '100%',
          height: '100%',
          overflow: 'hidden',
          display: 'flex',
          flexDirection: 'column',
          lineHeight: '18px',
        }}>
        <AppBar>
          <AppBarLink href="/" style={{ marginRight: '2em' }}>
            Flea
          </AppBarLink>
          &nbsp;
          <AppBarLink href="/">Home</AppBarLink>&nbsp;|&nbsp;
          <AppBarLink href="/console">Console</AppBarLink>&nbsp;|&nbsp;
          <AppBarLink href="/update">Update</AppBarLink>
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

import { AppBar, AppBarLink } from '@furystack/shades-common-components'
import { Shade, createComponent, Router, LazyLoad } from '@furystack/shades'

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
        <AppBar
          style={{
            height: '48px',
          }}>
          <AppBarLink href="/" style={{ marginRight: '2em' }}>
            🐜 Flea
          </AppBarLink>
          &nbsp;
          <AppBarLink href="/">🏡 Home</AppBarLink>
          <AppBarLink href="/joystick">🕹️ POV</AppBarLink>
          <AppBarLink href="/console">⌨️ Console</AppBarLink>
          <AppBarLink href="/update">⬆️ Update</AppBarLink>
          <div style={{ flex: '1' }} />
        </AppBar>
        <div style={{ width: '100%', height: '100%', flexGrow: '1' }}>
          <Router
            routes={[
              {
                url: '/',
                component: () => (
                  <LazyLoad
                    component={async () => {
                      const { HomePage } = await import('../pages/home-page')
                      return <HomePage />
                    }}
                    loader={<></>}
                  />
                ),
              },
              {
                url: '/joystick',
                component: () => (
                  <LazyLoad
                    component={async () => {
                      const { JoystickPage } = await import('../pages/joystick-page')
                      return <JoystickPage />
                    }}
                    loader={<></>}
                  />
                ),
              },
              {
                url: '/console',
                component: () => (
                  <LazyLoad
                    component={async () => {
                      const { ConsolePage } = await import('../pages/console-page')
                      return <ConsolePage />
                    }}
                    loader={<></>}
                  />
                ),
              },
              {
                url: '/update',
                component: () => (
                  <LazyLoad
                    component={async () => {
                      const { UpdatePage } = await import('../pages/update-page')
                      return <UpdatePage />
                    }}
                    loader={<></>}
                  />
                ),
              },
            ]}
          />
        </div>
      </div>
    )
  },
})

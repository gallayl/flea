import { createComponent, Shade } from '@furystack/shades'
import { Paper } from '@furystack/shades-common-components'

export const HomePage = Shade({
  shadowDomName: 'flea-home-page',
  render: () => {
    return (
      <Paper style={{ paddingTop: '48px' }}>
        <h1>Home Page</h1>
        <p>Welcome to the home page</p>
      </Paper>
    )
  },
})

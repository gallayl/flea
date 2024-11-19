import { Injector } from '@furystack/inject'
import { useLogging, VerboseConsoleLogger } from '@furystack/logging'
import { createComponent, initializeShadeRoot } from '@furystack/shades'
import { EnvironmentService } from './services/environment-service'
import { Layout } from './components/layout'
import { useThemeCssVariables, defaultDarkTheme } from '@furystack/shades-common-components'

const shadeInjector = new Injector()

useThemeCssVariables(defaultDarkTheme)

useLogging(shadeInjector, VerboseConsoleLogger)
shadeInjector.setExplicitInstance(
  {
    site: '192.168.0.38', // window.location.host,
  },
  EnvironmentService,
)

const rootElement: HTMLDivElement = document.getElementById('root') as HTMLDivElement
initializeShadeRoot({
  rootElement,
  injector: shadeInjector,
  jsxElement: <Layout />,
})

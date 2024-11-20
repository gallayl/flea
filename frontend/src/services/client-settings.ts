import { Injectable, Injected } from '@furystack/inject'
import { getLogger, type ScopedLogger } from '@furystack/logging'
import { ObservableValue } from '@furystack/utils'

const localStorageKey = 'FLEA_SETTINGS'

interface ClientSettingsValues {
  isPidEnabled: boolean
  throttleSensitivity: number
  steerSensitivity: number
}

@Injectable({ lifetime: 'singleton' })
export class ClientSettings {
  currentSettings = new ObservableValue<ClientSettingsValues>({
    isPidEnabled: true,
    throttleSensitivity: 32,
    steerSensitivity: 32,
  })

  @Injected((i) => getLogger(i).withScope('ClientSettings'))
  declare logger: ScopedLogger

  public init() {
    const settings = localStorage.getItem(localStorageKey)
    try {
      const value = JSON.parse(settings || '') as Partial<ClientSettingsValues>
      this.currentSettings.setValue({ ...this.currentSettings.getValue(), ...value })
    } catch (error) {
      void this.logger.warning({ message: 'Failed to parse stored settings. Resetting to defaults...' })
      localStorage.removeItem(localStorageKey)
    }
  }
}

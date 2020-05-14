import { WebSocketService } from './sockets.js'


let maxSpeedValue = 1;
document.getElementById('rangeSlider')?.addEventListener('change', (ev)=>{
    maxSpeedValue = parseInt(ev.currentTarget?.value || 1);
})


/**
 * @typedef MovementValues
 * @property {number} throttle
 * @property {number} steer
 * @property {number} maxSpeed
 */

/**
 * @typedef MovementServiceOptions
 * @property {WebSocketService} socket
 */
export class MovementService {

    state = {speed: 0, steer: 0}
    lastState = JSON.stringify(this.state);

    constructor(/** @type {MovementServiceOptions} */ options) {
        /** @type {string} */
        this.lastState = ""

        /** @type {MovementServiceOptions} */
        this.options = {
            ...options,
        };

        const nippleManager = nipplejs.create({
            zone: document.getElementById("joystickContainer"),
        })
        nippleManager.on("move", (_ev, data)=>{
            const speed = Math.round(data.force * maxSpeedValue) * (data.direction?.y === 'down' ? -1 : 1);
            const steer = Math.round((Math.cos(data.angle.radian) * 45));
            console.log({speed, steer})
            this.state = {speed, steer}
        })
        nippleManager.on("end", ()=>{
            this.options.socket.send(`move 0 0`)
            this.state = {speed: 0, steer: 0}
            this.lastState = JSON.stringify(this.state)
        });

        setInterval(() => {
            var stateStringified = JSON.stringify(this.state);
            if (this.lastState !== stateStringified) {
                this.options.socket.send(`move ${this.state.speed} ${this.state.steer}`)
                this.lastState = stateStringified
            }
        }, 50);
    }
}
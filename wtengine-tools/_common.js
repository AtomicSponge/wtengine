/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const readline = require('readline')

const showScriptInfo = () => {
    console.log(`wtengine-tools`)
    console.log(`https://www.wtfsystems.net/`)
}
exports.showScriptInfo = showScriptInfo

/**
 * Display an error message and exit script.
 * @param {String} message Message to display.
 */
const scriptError = (message) => {
    console.log(`Error:  ${message}  Exiting...`)
    process.exit(0)
}
exports.scriptError = scriptError

/**
 * Confirmation prompt
 * @param {String} message 
 * @returns {boolean}
 */
const confirmPrompt = (message) => {
    let confRes = false
    const rl = readline.createInterface({
        input: process.stdin, output: process.stdout
    })
    rl.question(`${message} [Y/n] `, (res) => {
        if(res[0] === 'n' || res[0] === 'N') confRes = false
        else confRes = true
        rl.close()
    })
    return confRes
}
exports.confirmPrompt = confirmPrompt

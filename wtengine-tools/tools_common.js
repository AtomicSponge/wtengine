/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @version 0.0.2
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const readline = require('readline')
const { stdin, stdout, exit } = require('process')

/**
 * Display an error message and exit script.
 * @param {String} message Message to display.
 */
const scriptError = (message) => {
    console.log(`Error:  ${message}  Exiting...`)
    exit(-1)
}
exports.scriptError = scriptError

/**
 * Confirmation prompt
 * @param {String} message 
 * @returns {boolean}
 */
const confirmPrompt = (message) => {
    const rl = readline.createInterface({ input: stdin, output: stdout })
    rl.question(`${message} [Y/n] `, (res) => {
        console.log(res)
    })
    rl.close()
    return true
}
exports.confirmPrompt = confirmPrompt

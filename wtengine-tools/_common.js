/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const readline = require('readline')

/**
 * Display script title & info.
 * @param {String} scriptName 
 */
const showScriptInfo = (scriptName) => {
    process.stdout.write(`\nwtengine-tools > ${scriptName}\n`)
    process.stdout.write(`https://www.wtfsystems.net/\n\n`)
}
exports.showScriptInfo = showScriptInfo

/**
 * Display an error message and exit script.
 * @param {String} message Message to display.
 */
const scriptError = (message) => {
    process.stdout.write(`Error:  ${message}  Exiting...\n`)
    process.exit(0)
}
exports.scriptError = scriptError

/**
 * Confirmation prompt (wip)
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

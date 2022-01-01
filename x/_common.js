/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const readline = require('readline/promises')

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
 * @param {String} message Message to display.
 * @returns {boolean} True if 'yes', else false
 */
const confirmPrompt = async (message) => {
    const rl = readline.createInterface({
        input: process.stdin, output: process.stdout
    })
    const res = await rl.question(`${message} [Y/n] `)
    rl.close()
    if(res[0] === 'N' || res[0] === 'n') return false
    return true
}
exports.confirmPrompt = confirmPrompt

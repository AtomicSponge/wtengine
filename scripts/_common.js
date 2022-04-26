/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const fs = require('fs')
const inquirer = require('inquirer')

const package = require('../package.json')

/**
 * Display script title & info.
 * @param {String} scriptName 
 */
const showScriptInfo = (scriptName) => {
    process.stdout.write(`\x1b[37m${package.name} > v${package.version} > \x1b[89m`)
    process.stdout.write(`\x1b[97m${scriptName}\x1b[39m\n`)
    process.stdout.write(`\x1b[36m\x1b[4m${package.url}\x1b[24m\x1b[89m\n`)
    process.stdout.write(`\x1b[0m\n`)
}
exports.showScriptInfo = showScriptInfo

/**
 * 
 * @returns 
 */
const checkSettings = () => {
    return false
}
exports.checkSettings = checkSettings

/**
 * 
 */
const createSettings = () => {
    fs.copyFile(`${__dirname}/_default_settings.json`, `${__dirname}/../settings.json`, fs.constants.COPYFILE_EXCL, (err) => {
        throw err
    })
}
exports.createSettings = createSettings

/**
 * 
 * @param {*} settings 
 * @returns 
 */
const saveSettings = (settings) => {
    return false
}
exports.saveSettings = saveSettings

/**
 * Display an error message and exit script.
 * @param {String} message Message to display.
 */
const scriptError = (message) => {
    process.stdout.write(`\x1b[31mError:  ${message}  Exiting...\x1b[89m\x1b[0m\n`)
    process.exit(0)
}
exports.scriptError = scriptError

/**
 * Confirmation prompt (wip)
 * @param {String} message Message to display.
 * @param {boolean} dvalue Default answer (Y - true | N - false)
 * @returns {boolean} True if 'yes', else false
 */
const confirmPrompt = (message, dvalue) => {
    if(dvalue === undefined) dvalue = true
    return inquirer.prompt([{
        default: dvalue,
        name: 'conf',
        type: 'confirm',
        message: `\x1b[33m${message}\x1b[89m\x1b[0m`
    }]).then(res => { return res.conf })
}
exports.confirmPrompt = confirmPrompt

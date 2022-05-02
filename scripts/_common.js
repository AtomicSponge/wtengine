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

const constants = {
    DEFAULT_LOCATION: `${__dirname}/_default_settings.json`,
    SETTINGS_LOCATION: `${__dirname}/../settings.json`,
}

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
 * Verify write access to engine settings file.
 * @returns {boolean} True if writable, else false.
 */
const checkSettings = () => {
    fs.access(constants.SETTINGS_LOCATION, fs.constants.W_OK, (err) => {
        return false
    })
    return true
}
exports.checkSettings = checkSettings

/**
 * Create user engine settings file.
 * On fail, display error and exit running script.
 */
const createSettings = () => {
    try {
        fs.copyFileSync(constants.DEFAULT_LOCATION, constants.SETTINGS_LOCATION, fs.constants.COPYFILE_EXCL)
    } catch (err) {
        scriptError(err)
    }
}
exports.createSettings = createSettings

/**
 * Save engine settings.
 * @param {JSON} settings Settings as JSON object.
 * On fail, display error and exit running script.
 */
const saveSettings = (settings) => {
    try {
        fs.writeFileSync(constants.SETTINGS_LOCATION, JSON.stringify(settings))
    } catch (err) {
        scriptError(err)
    }
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
 * @returns {boolean} True if default answer, else false
 */
const confirmPrompt = async (message, dvalue) => {
    if(dvalue === undefined) dvalue = true
    inquirer.prompt([{
        default: dvalue,
        name: 'conf',
        type: 'confirm',
        message: `\x1b[33m${message}\x1b[89m`
    }]).then(res => { return res.conf })
    process.stdout.write(`\x1b[0m\n\n`)
    return true
}
exports.confirmPrompt = confirmPrompt

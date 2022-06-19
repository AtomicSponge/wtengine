/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const fs = require('fs')
const spawn = require('child_process').spawn
const commandExistsSync = require('command-exists').sync
const inquirer = require('inquirer')

/**
 * Configuration settings
 */
const config = {
    checkApps: [ "cmake", "git" ],
    gitURLs: {
        "allegro": "https://github.com/liballeg/allegro5",
        "physfs": "https://github.com/icculus/physfs"
    }
}
exports.config = config

/**
 * Constants
 */
const constants = {
    CONFIG_SCRIPT:     `${__dirname}/wte-config`,
    SYSCHECK_SCRIPT:   `${__dirname}/wte-syscheck.js`,
    SETTINGS_LOCATION: `${__dirname}/../settings.json`,
    LOG_FILE: ``,
    LOG_LOCATION: ``
}
exports.constants = constants

/**
 * Font colors
 */
const colors = {
    RED:    `\x1b[31m`,
    GREEN:  `\x1b[32m`,
    YELLOW: `\x1b[33m`,
    BLUE:   `\x1b[34m`,
    CYAN:   `\x1b[36m`,
    DIM:    `\x1b[2m`,
    CLEAR:  `\x1b[0m`
}
exports.colors = colors

/**
 * Create an object with the current date and time that can be easily referenced.
 * @returns {Object} A date object with the current date and time.
 */
const getDate = () => {
    const _date = {}
    const date = new Date()
    _date.month = date.getMonth()
    _date.day = date.getDate()
    _date.year = date.getFullYear()
    _date.hour = date.getHours()
    _date.minutes = date.getMinutes()
    _date.seconds = date.getSeconds()
    return _date
}
exports.getDate = getDate

/**
 * Display an error message and exit script.
 * @param {String} message Message to display.
 */
const scriptError = (message) => {
    process.stdout.write(`${colors.RED}Error:  ${message}  Exiting...${colors.CLEAR}\n`)
    process.exit(1)
}
exports.scriptError = scriptError

/**
 * Clears the log file.
 */
const clearLog = () => {
    try {
        fs.unlinkSync(`${constants.LOG_LOCATION}/${constants.LOG_FILE}`)
    } catch (err) {}
}
exports.clearLog = clearLog

/**
 * Write a message to the log file.
 * @param {String} message String to write.
 * @throws Error on fail then exits script.
 */
const writeLog = (message) => {
    try {
        fs.appendFileSync(`${constants.LOG_LOCATION}/${constants.LOG_FILE}`, message)
    } catch (err) { scriptError(err) }
}
exports.writeLog = writeLog

/**
 * Converts script arguments from array to object.
 * @param {Array} args Argument array.
 * @param {Object} commands Object representing a list of commands.
 * @returns {Object} Argument object.
 */
 const parseArgs = (args, commands) => {
    _args = {}
    //  Build the object using supplied command names
    commands.forEach(command => {
        (command.name.includes('=') ?
            _args[command.name] = null : _args[command.name] = false)
    })
    //  Now parse the arguments
    args.forEach(arg => {
        var matchMe = null
        var newVal = null
        if(arg.includes('=')) {
            matchMe = arg.substring(0, arg.indexOf('='))
            newVal = arg.substring(arg.indexOf('=') + 1)
        } else {
            matchMe = arg
            newVal = true
        }
        commands.forEach(command => {
            command.flags.replace(/\s+/g, '').split(',').forEach(item => {
                if(item == matchMe) _args[command.name] = newVal
            })
        })
    })
    return _args
}
exports.parseArgs = parseArgs

/**
 * Confirmation prompt (wip)
 * @param {String} message Message to display.
 * @param {boolean} dvalue Default answer (Y - true | N - false)
 * @returns {boolean} True if default answer, else false
 */
const confirmPrompt = (message, dvalue) => {
    if(dvalue == undefined) dvalue = true
    return (async () => {
        const prompt = inquirer.prompt([{
            default: dvalue,
            name: 'conf',
            type: 'confirm',
            message: `${colors.YELLOW}${message}`
        }]).then(res => { return res.conf })
        console.log(prompt)
        return prompt
    })()
    /*const prompt = inquirer.prompt([{
        default: dvalue,
        name: 'conf',
        type: 'confirm',
        message: `${colors.YELLOW}${message}`
    }])
    process.stdout.write(`${colors.CLEAR}\n`)
    const res = await prompt.then(res => { return res.conf })
    return res*/
}
exports.confirmPrompt = confirmPrompt

/**
 * Verify write access to engine settings file.
 * @param {String} permissions File permissions to check, 'rwx' format.
 * Passing nothing checks if the file simply exists.
 * On fail, displays a script error and exit.
 */
const checkSettings = (permissions) => {
    let checkFlags = []
    if(permissions == undefined) checkFlags.push(fs.constants.F_OK)
    if(permissions.includes("r") || permissions.includes("R")) checkFlags.push(fs.constants.R_OK)
    if(permissions.includes("w") || permissions.includes("W")) checkFlags.push(fs.constants.W_OK)
    if(permissions.includes("x") || permissions.includes("X")) checkFlags.push(fs.constants.X_OK)

    if(checkFlags.length == 0) scriptError(`Unable to check settings file!  No proper tests requested!`)

    checkFlags.forEach(fFlag => {
        fs.access(constants.SETTINGS_LOCATION, fFlag, (err) => { scriptError(err) })
    })
}
exports.checkSettings = checkSettings

/**
 * Load engine settings.
 * @returns {JSON} Settings JSON object.  False on fail.
 */
const loadSettings = () => {
    try {
        const settings = fs.readFileSync(constants.SETTINGS_LOCATION)
        return JSON.parse(settings)
    } catch (err) {
        return false
    }
}
exports.loadSettings = loadSettings

/**
 * Save engine settings.
 * @param {JSON} settings Settings as JSON object.
 * On fail, display error and exit running script.
 */
const saveSettings = (settings) => {
    if(!(settings instanceof Object)) scriptError(`Settings format not valid.`)

    const oldSettings = loadSettings()
    if(oldSettings) settings = oldSettings.concat(settings)

    try {
        fs.writeFileSync(constants.SETTINGS_LOCATION, JSON.stringify(settings))
        process.stdout.write(`${colors.GREEN}Settings saved.${colors.CLEAR}\n`)
    } catch (err) {
        scriptError(err)
    }
}
exports.saveSettings = saveSettings

/**
 * Check for necessary applications.
 * On not found, displays a script error and exit.
 */
const checkApps = () => {
    process.stdout.write(`Checking for necessary applications...\n`)
    config.checkApps.forEach((appCheck) => {
        if(commandExistsSync(appCheck))
            process.stdout.write(`${colors.GREEN}  > '${appCheck}' found.${colors.CLEAR}\n`)
        else scriptError(`'${appCheck}' not found.`)
    })
    process.stdout.write(`${colors.CLEAR}\n`)
}
exports.checkApps = checkApps

/**
 * 
 * @param {*} process 
 * @returns 
 */
const onProcessExit = (process) => {
    return new Promise((resolve, reject) => {
        process.once('exit', (error) => {
            if(error) resolve(true)
            else reject(false)
        })
        process.once('error', (error) => {
            reject(error)
        })
    })
}
exports.onProcessExit = onProcessExit

/**
 * Run the system check script.
 * @returns {boolean} True if the script was successful, else false.
 */
const runSysCheckScript = async () => {
    const proc = spawn(constants.SYSCHECK_SCRIPT,
                       {stdio: [process.stdin, process.stdout, process.stderr]})
    if(await onProcessExit(proc) === true) return true
    else return false
}
exports.runSysCheckScript = runSysCheckScript

/**
 * Run the configuration script.
 * @returns {boolean} True if the script was successful, else false.
 */
const runConfigScript = async () => {
    const proc = spawn(constants.CONFIG_SCRIPT,
                       {stdio: [process.stdin, process.stdout, process.stderr]})
    if(await onProcessExit(proc) === true) return true
    else return false
}
exports.runConfigScript = runConfigScript
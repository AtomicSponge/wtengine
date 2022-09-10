/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

import fs from 'fs'
import { fileURLToPath } from 'url'
import { dirname } from 'path'
import { exec } from 'child_process'
import { spawn } from 'child_process'
import inquirer from 'inquirer'

const __filename = fileURLToPath(import.meta.url)
const __dirname = dirname(__filename)

const wtf = {}

let appInfo = fs.readFileSync('../package.json')
appInfo = JSON.parse(appInfo)

/**
 * Configuration settings
 */
const config = {
    checkApps: [ "cmake", "git" ],
    gitURLs: [
        { name: "allegro5", url: "https://github.com/liballeg/allegro5" },
        { name: "physfs", url: "https://github.com/icculus/physfs" }
    ]
}
wtf.config = config

/**
 * Constants
 */
const constants = {
    APP_NAME: `${appInfo['name']}`,
    APP_VERSION: `${appInfo['version']}`,
    APP_URL: `${appInfo['url']}`,
    ENGINE_ROOT_LOCATION:  __dirname.substring(0, __dirname.lastIndexOf(`/`)),
}
wtf.constants = constants

/**
 * Folder paths
 */
const paths = {
    ENGINE_BUILD_LOCATION: `${constants.ENGINE_ROOT_LOCATION}/wte-build`,
    ENGINE_BUILD_DEBUG_LOCATION: `${constants.ENGINE_ROOT_LOCATION}/wte-build-debug`,
    ENGINE_WORK_LOCATION: `${constants.ENGINE_ROOT_LOCATION}/wte-temp`,
    ENGINE_LOG_LOCATION: `${constants.ENGINE_ROOT_LOCATION}/wte-logs`,
    ENGINE_TEMP_LOCATION: `${constants.ENGINE_ROOT_LOCATION}/wte-temp`
}
wtf.paths = paths

/**
 * Files
 */
const files = {
    CONFIG_SCRIPT:     `${__dirname}/wte-config.mjs`,
    SYSCHECK_SCRIPT:   `${__dirname}/wte-syscheck.mjs`,
    SETTINGS_FILE: `${constants.ENGINE_ROOT_LOCATION}/settings.json`,
    LOG_FILE: ``  //  Set by script
}
wtf.files = files

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
wtf.colors = colors

/**
 * Show script info
 * @param {String} title Script title to use
 */
const scriptTitle = (title) => {
    process.stdout.write(`${colors.CYAN}${title}${colors.CLEAR} - `)
    process.stdout.write(`${colors.DIM}${colors.CYAN}${constants.APP_NAME}${colors.CLEAR} - `)
    process.stdout.write(`${colors.DIM}${colors.CYAN}ver ${constants.APP_VERSION}${colors.CLEAR}\n`)
    process.stdout.write(`${colors.DIM}${colors.YELLOW}${constants.APP_URL}${colors.CLEAR}\n`)
    process.stdout.write(`\n`)
}
wtf.scriptTitle = scriptTitle

/**
 * Display an error message and exit script.
 * @param {String} message Message to display.
 */
const scriptError = (message) => {
    process.stdout.write(`${colors.RED}Error:  ${message}  Exiting...${colors.CLEAR}\n`)
    process.exit(1)
}
wtf.scriptError = scriptError

/**
 * Clears the log file.
 * Will exit script if the log filename was not set.
 */
const clearLog = () => {
    if(files.LOG_FILE === '') scriptError(`Must set a log file in the script first!`)
    try {
        fs.unlinkSync(`${paths.ENGINE_LOG_LOCATION}/${files.LOG_FILE}`)
    } catch (err) {}
}
wtf.clearLog = clearLog

/**
 * Write a message to the log file.
 * Will exit script if the log filename was not set.
 * @param {String} message String to write.
 * @throws Error on fail then exits script.
 */
const writeLog = (message) => {
    if(files.LOG_FILE === '') scriptError(`Must set a log file in the script first!`)
    try {
        fs.appendFileSync(`${paths.ENGINE_LOG_LOCATION}/${files.LOG_FILE}`, message)
    } catch (err) { scriptError(err) }
}
wtf.writeLog = writeLog

/**
 * Converts script arguments from array to object.
 * @param {Array} args Argument array.
 * @param {Object} commands Object representing a list of commands.
 * @returns {Object} Argument object.
 */
const parseArgs = (args, commands) => {
    let _args = {}
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
wtf.parseArgs = parseArgs

/**
 * Confirmation prompt
 * @param {String} message Message to display.
 * @param {boolean} dvalue Default answer (Y - true | N - false)
 * @returns {boolean} True if default answer, else false
 */
const confirmPrompt = async (message, dvalue) => {
    if(dvalue == undefined) dvalue = true
    return await inquirer.prompt([{
        default: dvalue,
        name: 'conf',
        type: 'confirm',
        message: `${colors.YELLOW}${message}`
    }]).then(res => { return res.conf })
}
wtf.confirmPrompt = confirmPrompt

/**
 * Check if a folder exists
 * @param {String} folder 
 * @returns {boolean} True if the folder exists, else false.
 */
const checkFolder = (folder) => {
    try { fs.accessSync(folder) } catch (err) { return false }
    return true
}
wtf.checkFolder = checkFolder

/**
 * Check if a folder exists, then create it if one does not
 * @param {String} folder 
 * @throws Error on fail then exits script
 */
const makeFolder = (folder) => {
    try {
        fs.accessSync(folder)
    } catch (err) {
        try {
            fs.mkdirSync(folder)
        } catch (err) { scriptError(err) }
    }
}
wtf.makeFolder = makeFolder

/**
 * Verify access to engine settings file.  Passing nothing checks if the file simply exists.
 * @param {String} permissions File permissions to check, 'rwx' format.
 * @returns {boolean} True if tests succeded, else false
 */
const checkSettings = (permissions) => {
    let checkFlags = []
    if(permissions === undefined) checkFlags.push(fs.constants.F_OK)
    else {
        if(permissions.includes("r") || permissions.includes("R")) checkFlags.push(fs.constants.R_OK)
        if(permissions.includes("w") || permissions.includes("W")) checkFlags.push(fs.constants.W_OK)
        if(permissions.includes("x") || permissions.includes("X")) checkFlags.push(fs.constants.X_OK)
    }

    if(checkFlags.length == 0) scriptError(`Unable to check settings file!  No proper tests requested!`)

    var result = true
    checkFlags.forEach(fFlag => {
        try { fs.accessSync(files.SETTINGS_FILE, fFlag)
        } catch (err) { result = false }
    })
    return result
}
wtf.checkSettings = checkSettings

/**
 * Load engine settings.
 * @returns {JSON} Settings JSON object.  False on fail.
 */
const loadSettings = () => {
    try {
        const settings = fs.readFileSync(files.SETTINGS_FILE)
        return JSON.parse(settings)
    } catch (err) {
        return false
    }
}
wtf.loadSettings = loadSettings

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
        fs.writeFileSync(files.SETTINGS_FILE, JSON.stringify(settings))
        process.stdout.write(`${colors.GREEN}Settings saved.${colors.CLEAR}\n`)
    } catch (err) {
        scriptError(err)
    }
}
wtf.saveSettings = saveSettings

/**
 * Async version of array's forEach
 * @param {Array} array Array to iterate
 * @param {Function} callback Callback function
 */
const asyncForEach = async (array, callback) => {
    for(let index = 0; index < array.length; index++)
        await callback(array[index], index, array)
}
wtf.asyncForEach = asyncForEach

/**
 * Wait for a process to exit and return the result.
 * @param {Object} process The process object to watch.
 * @returns {Promise} A fulfilled promise with the result.
 */
 const onProcessExit = async (process, log) => {
    log = log || true
    return new Promise((resolve, reject) => {
        process.once('exit', (code) => {
            if(log && files.LOG_FILE !== '') writeLog(`Return code:  ${code}\n`)
            if(code === 0) resolve(true)
            else resolve(false)
        })
        process.once('error', (error) => {
            if(log && files.LOG_FILE !== '') writeLog(`Error:  ${error}\n`)
            reject(error)
        })
    })
}
wtf.onProcessExit = onProcessExit

/**
 * Run a system command.
 * Waits for the command to complete but does not show output.
 * @param {String} cmd Command to run.
 * @param {Object} opts Additional options.
 * @param {boolean} log Log the result of the command to the log file.  Defaults to true.
 * @returns {boolean} True if the command was successful, else false.
 */
 const runCommand = async (cmd, opts, log) => {
    opts = opts || {}
    opts.cwd = opts.cwd || process.cwd()
    opts.env = opts.env || process.env
    opts.timeout = opts.timeout || 0
    log = log || true

    if(log && files.LOG_FILE !== '') writeLog(`Running command:  ${cmd}\n`)

    const proc = exec(cmd, { cwd: opts.cwd, env: opts.env, windowsHide: true })
    if(await onProcessExit(proc, log).catch(err => { return false }) === true) return true
    else return false
}
wtf.runCommand = runCommand

/**
 * Run the system check script.
 * @param {Array} args Process arguments as an array.
 * @returns {boolean} True if the script was successful, else false.
 */
const runSysCheckScript = async (args) => {
    process.stdout.write(`\n`)
    const proc = spawn(files.SYSCHECK_SCRIPT, args,
                       { stdio: [ process.stdin, process.stdout, process.stderr ] })
    //
    if(await onProcessExit(proc) === true) return true
    else return false
}
wtf.runSysCheckScript = runSysCheckScript

/**
 * Run the configuration script.
 * @param {Array} args Process arguments as an array.
 * @returns {boolean} True if the script was successful, else false.
 */
const runConfigScript = async (args) => {
    process.stdout.write(`\n`)
    const proc = spawn(files.CONFIG_SCRIPT, args,
                       { stdio: [ process.stdin, process.stdout, process.stderr ] })
    if(await onProcessExit(proc) === true) return true
    else return false
}
wtf.runConfigScript = runConfigScript

export default wtf
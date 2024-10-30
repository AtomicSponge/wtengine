/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

import fs from 'node:fs'
import path from 'node:path'
import { exec } from 'node:child_process'

import inquirer from 'inquirer'
import { dim, green, yellow, cyan, stripColors } from 'kolorist'

import { __locale } from '@spongex/system-locale'
import { scriptError } from '@spongex/script-error'

import type { ChildProcess } from 'node:child_process'

const packageJSON = JSON.parse(
  fs.readFileSync(path.join(import.meta.dirname, '..', 'package.json')).toString()
)

/**
 * Configuration settings
 */
export const config = {
  checkApps: [ "cmake", "git" ],
  gitURLs: [
    { name: "allegro5", url: "https://github.com/liballeg/allegro5" },
    { name: "physfs",   url: "https://github.com/icculus/physfs" }
  ]
}

/**
 * Constants
 */
export const scriptInfo = {
  /** Name of script */
  NAME:     packageJSON['name'],
  /** Version of script */
  VERSION:  packageJSON['version'],
  /** Script documentation URL */
  URL:      packageJSON['homepage']
}

/**
 * PROJECT class
 * Used for setting the project root location
 * Must be set before referencing the path variables below
 */
export class PROJECT {
  /** Root location of the project */
  static #ROOT_LOCATION = ''
  /** Get the root location */
  static get ROOT_LOCATION():string { return PROJECT.#ROOT_LOCATION }
  /** Set the root location */
  static set ROOT_LOCATION(val:string) { PROJECT.#ROOT_LOCATION = val }
}

/**
 * Folder paths
 * Relevant to {@link PROJECT.#ROOT_LOCATION}
 */
export const paths = {
  /** Location for engine build artifacts */
  ENGINE_BUILD_LOCATION:  path.join(PROJECT.ROOT_LOCATION, 'wte-build'),
  /** Location for source downloads */
  ENGINE_TEMP_LOCATION:   path.join(PROJECT.ROOT_LOCATION, 'wte-temp')
}

/**
 * Files
 */
export const files = {
  /** Configuration script location */
  CONFIG_SCRIPT:    path.join(import.meta.dirname, 'wte-config.js'),
  /** System check script location */
  SYSCHECK_SCRIPT:  path.join(import.meta.dirname, 'wte-syscheck.js'),
  /** Settings file location relevant to {@link PROJECT.ROOT_LOCATION} */
  SETTINGS_FILE:    path.join(PROJECT.ROOT_LOCATION, 'wte.settings.json'),
  /** Log file location relevant to {@link PROJECT.ROOT_LOCATION} */
  LOG_FILE:         path.join(PROJECT.ROOT_LOCATION, 'wte.build.log')
}

/**
 * Show script info
 * @param title Script title to use
 * @param log Log to file or not.  Defaults to false
 */
export const scriptTitle = (title:string, log?:boolean):void => {
  log = log || false

  console.log(`${cyan(`${title}`)} - ` +
    dim(cyan(scriptInfo.NAME)) + ` - ` +
    dim(cyan(`ver ${scriptInfo.VERSION}`)))
  console.log(dim(yellow(`${scriptInfo.URL}\n`)))

  if(log) {
    writeLog(`${title} - ${scriptInfo.NAME} - ver ${scriptInfo.VERSION}\n`)
    writeLog(`${scriptInfo.URL}\n`)
    writeLog(`Ran at: ` + new Date().toLocaleString(__locale, { timeZoneName: 'short' }))
    writeLog(`\n\n`)
  }
}

/**
 * Clears the log file
 * Will exit script if the PROJECT.ROOT_LOCATION was not set
 * @throws Error on fail then exits script
 */
export const clearLog = ():void => {
  if(PROJECT.ROOT_LOCATION === '')
    scriptError(`Must set the project root path in the script first!`)
  try {
    fs.unlinkSync(files.LOG_FILE)
  } catch (error:any) { scriptError(error.message) }
}

/**
 * Write a message to the log file
 * Will exit script if the PROJECT.ROOT_LOCATION was not set
 * @param message String to write
 * @throws Error on fail then exits script
 */
export const writeLog = (message:string):void => {
  if(PROJECT.ROOT_LOCATION === '')
    scriptError(`Must set the project root path in the script first!`)
  try {
    fs.appendFileSync(files.LOG_FILE, stripColors(message))
  } catch (error:any) { scriptError(error.message) }
}

/**
 * Confirmation prompt
 * @param message Message to display
 * @param dvalue Default answer (Y - true | N - false)
 * @returns True if default answer, else false
 */
export const confirmPrompt = async (message:string, dvalue?:boolean):Promise<boolean> => {
  if(dvalue == undefined) dvalue = true
  return await inquirer.prompt([{
    default: dvalue,
    name: 'conf',
    type: 'confirm',
    message: yellow(`${message}`)
  }]).then(res => { return res.conf })
}

/**
 * Save engine settings
 * @param settings Settings as JSON object
 * On fail, display error and exit running script
 */
export const saveSettings = (settings:JSON) => {
  if(!(settings instanceof Object)) scriptError(`Settings format not valid.`)

  try {
    fs.writeFileSync(files.SETTINGS_FILE, JSON.stringify(settings))
    console.log(green(`Settings saved.`))
  } catch (error:any) {
    scriptError(error.message)
  }
}

/**
 * Load engine settings
 * @returns Settings JSON object.  False on fail
 */
export const loadSettings = ():string | boolean => {
  try {
    return JSON.parse(fs.readFileSync(files.SETTINGS_FILE).toString())
  } catch (error:any) {
    return false
  }
}

/**
 * Verify access to engine settings file
 * Passing nothing checks if the file simply exists
 * @param permissions File permissions to check, 'rwx' format
 * @returns True if tests succeded, else false
 */
export const checkSettings = (permissions:string):boolean => {
  let checkFlags = []
  if(permissions === undefined) checkFlags.push(fs.constants.F_OK)
  else {
    if(permissions.includes("r") || permissions.includes("R")) checkFlags.push(fs.constants.R_OK)
    if(permissions.includes("w") || permissions.includes("W")) checkFlags.push(fs.constants.W_OK)
    if(permissions.includes("x") || permissions.includes("X")) checkFlags.push(fs.constants.X_OK)
  }

  if(checkFlags.length == 0) scriptError(`Unable to check settings file!  No proper tests requested!`)

  let result = true
  checkFlags.forEach(fFlag => {
    try { fs.accessSync(files.SETTINGS_FILE, fFlag)
    } catch (error:any) { result = false }
  })
  return result
}

/** Options for runCommand */
interface runCommandOpts {
  /** Current working directory - default process.cwd() */
  cwd?:string
  /** Process environment - default process.env */
  env?:NodeJS.ProcessEnv
  /** Timeout value - default zero */
  timeout?:number
  /** Log file - default true */
  log?:boolean
}

/**
 * Run a system command
 * Waits for the command to complete but does not show output
 * @param cmd Command to run
 * @param opts Additional options to pass to 'exec'
 * @param log Log the result of the command to the log file.  Defaults to true
 * @returns True if the command was successful, else false
 */
export const runCommand = async (cmd:string, opts?:runCommandOpts) => {
  const options = {
    cwd: opts!.cwd || process.cwd(),
    env: opts!.env || process.env,
    timeout: opts!.timeout || 0
  }
  const log = opts!.log || true

  if(log) writeLog(`Running command:  ${cmd}\n`)

  return await new Promise ((resolve, _reject) => {
    exec(cmd, options, (error, stdout, stderr) => {
      if(log) {
        if(stdout !== ``) writeLog(`Output:\n${stdout}\n`)
        if(stderr !== ``) writeLog(`Error:\n${stderr}\n`)
      }
      if(error) resolve(false)
      resolve(true)
    })
  })
}

/**
 * Wait for a process to exit and return the result
 * @param process The process object to watch
 * @returns A fulfilled promise with the result
 */
export const onProcessExit = async (proc:ChildProcess, log?:boolean) => {
  log = log || true
  return new Promise((resolve, reject) => {
    proc.once('exit', (code) => {
      if(log) writeLog(`Return code:  ${code}\n`)
      if(code === 0) resolve(true)
      else resolve(false)
    })
    proc.once('error', (error) => {
      if(log) writeLog(`Error:  ${error.message}\n`)
      reject(error)
    })
  })
}

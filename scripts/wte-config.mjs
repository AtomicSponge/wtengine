#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

import fs from 'fs'
import wtf from './_common.mjs'
import inquirer from 'inquirer'

/**
 * Script menus.
 */
const menus = {
    //
}

/**
 * Script actions.
 */
const actions = {
    /**
     * Run initial setup.
     * @returns {JSON} New settings object
     */
    doSetup: () => {
        var settings = {}
        return settings
    },

    /**
     * Edit existing settings.
     * @param {JSON} settings JSON object of new settings
     * @returns {JSON} Modified settings object
     */
    doEdit: (settings) => {
        return settings
    }
}

/*
 * Main script
 */
wtf.scriptTitle(`WTEngine Configuration Utility`)

const args = wtf.parseArgs(process.argv, [
    { name: 'noSyscheck', flags: '--nosyscheck' },
    { name: 'clearCache', flags: '--clearcache' }
])

/*
 * Claer cache option - deletes cache files then exits
 */
if(args.clearCache) {
    const wipeFiles = (location, ignoreList) => {
        const fileList = fs.readdirSync(location, { withFileTypes: 'true' })
        fileList.forEach(item => {
            var ignoreMatch = false
            ignoreList.forEach(ignore => { if(item.name == ignore) ignoreMatch = true; return })
            if(ignoreMatch) return
            if(item.isDirectory()) fs.rmdirSync(`${location}/${item.name}`, { recursive: true })
            if(item.isFile()) fs.rmSync(`${location}/${item.name}`)
        })
    }

    const clearRes = await inquirer.prompt([{
        name: 'clearselect',
        type: 'rawlist',
        message: `${wtf.colors.YELLOW}Clear which cache folder?`,
        choices: [ `Engine Build`, `Engine Build Debug`, `Logs`, `Temp`, `All` ]
    }])

    const ignoreList = [ `README.md`, `.gitignore` ]
    if(clearRes.clearselect == `Engine Build` || clearRes.clearselect == `All`)
        wipeFiles(wtf.paths.ENGINE_BUILD_LOCATION, ignoreList)
    if(clearRes.clearselect == `Engine Build Debug` || clearRes.clearselect == `All`)
        wipeFiles(wtf.paths.ENGINE_BUILD_DEBUG_LOCATION, ignoreList)
    if(clearRes.clearselect == `Logs` || clearRes.clearselect == `All`)
        wipeFiles(wtf.paths.ENGINE_LOG_LOCATION, ignoreList)
    if(clearRes.clearselect == `Temp` || clearRes.clearselect == `All`)
        wipeFiles(wtf.paths.ENGINE_TEMP_LOCATION, ignoreList)

    process.exit(0)
}

var settings = wtf.loadSettings()
if(!settings) {
    process.stdout.write(`No settings file found, running setup...\n`)
    if(!args.noSyscheck) if(await wtf.confirmPrompt(`Run a system check first?`)) {
        if(!await wtf.runSysCheckScript())
            wtf.scriptError(`Problems running system check.\nPlease resolve issues then re-run config.`)
    }
    settings = actions.doSetup()
    wtf.saveSettings(settings)
} else {
    process.stdout.write(`Editing existing settings...\n`)
    const oldSettings = settings
    settings = actions.doEdit(settings)
    if((oldSettings !== settings) && await wtf.confirmPrompt(`Save changes?`)) wtf.saveSettings(settings)
}

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Config Done!${wtf.colors.CLEAR}\n\n`)
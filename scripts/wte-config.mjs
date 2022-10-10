#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

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

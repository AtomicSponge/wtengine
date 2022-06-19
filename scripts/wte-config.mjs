#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

 import wtf from './_common.cjs'
 import 'inquirer'

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
process.stdout.write(`${wtf.colors.CYAN}WTEngine Configuration Utility${wtf.colors.CLEAR}\n\n`)

var settings = wtf.loadSettings()
if(!settings) {
    process.stdout.write(`No settings file found, running setup...\n`)
    settings = actions.doSetup()
    wtf.saveSettings(settings)
} else {
    process.stdout.write(`Editing existing settings...\n`)
    oldSettings = settings
    settings = actions.doEdit(settings)
    if((oldSettings !== settings) && wtf.confirmPrompt(`Save changes?`)) wtf.saveSettings(settings)
}

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Config Done!${wtf.colors.CLEAR}\n\n`)

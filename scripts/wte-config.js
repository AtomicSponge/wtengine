#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

const wtf = require('./_common')
const inquirer = require('inquirer')

/**
 * Script actions.
 */
const actions = {
    /**
     * Run initial setup.
     * @param {JSON} settings 
     */
    doSetup: () => {
        var settings = {}
        return settings
    },

    /**
     * Edit existing settings.
     * @param {JSON} settings 
     */
    doEdit: (settings) => {
        return settings
    }
}

/*
 * Main script
 */
process.stdout.write(`${wtf.constants.CYAN}WTEngine Configuration Utility${wtf.constants.CLEAR}\n\n`)

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

process.stdout.write(`${wtf.constants.DIM}${wtf.constants.CYAN}Config Done!${wtf.constants.CLEAR}\n\n`)

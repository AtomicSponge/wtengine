#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
const wtf = require('./_common')

process.stdout.write('WTEngine Install\n\n')
process.stdout.write(`Creating engine settings file...\n`)

//  Create the settings file
wtf.createSettings()
if(!wtf.checkSettings()) wtf.scriptError(
    `Unable to access 'settings.json'.  Verify you have write access.`)
process.stdout.write('File created successfully.\n\n')

if(wtf.confirmPrompt('Run WTEngine configuration?'))
    require('child_process').fork(`${__dirname}/wte-config.js`)

#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
const wtf = require('./_common')

//  Create the settings file
wtf.createSettings()
if(!wtf.checkSettings()) wtf.scriptError(
    `Unable to access 'settings.json'.  Verify you have write access.`)

require('child_process').fork(`${__dirname}/wte-config.js`)

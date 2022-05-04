#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
const wtf = require('./_common')
const config = require('./_config')

process.stdout.write('WTEngine Install\n\n')
process.stdout.write(`Creating engine settings file...\n`)

//  Run system check

//  Run config
if(wtf.confirmPrompt('Run WTEngine configuration?'))
    require('child_process').fork(`${__dirname}/wte-config.js`)

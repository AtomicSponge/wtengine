#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
const wtf = require('./_common')
const config = require('./_config')

process.stdout.write(`WTEngine Install\n\n`)
process.stdout.write(`Creating engine settings file...\n`)

//  Run system check
if(wtf.confirmPrompt('Run WTEngine system check?'))
    wtf.runSysCheck()

//  Run config
if(wtf.confirmPrompt('Run WTEngine configuration?'))
    wtf.runConfig()

process.stdout.write(`Install done!\n`)

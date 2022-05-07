#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
const wtf = require('./_common')

process.stdout.write(`${wtf.constants.CYAN}WTEngine Install${wtf.constants.CLEAR}\n\n`)

//  Run system check
if(wtf.confirmPrompt('Run WTEngine system check?'))
    if(!wtf.runSysCheckScript())
        wtf.scriptError(`Problems running system check.\nPlease resolve issues then re-run install.`)

//  Run config
if(wtf.confirmPrompt('Run WTEngine configuration?'))
    if(!wtf.runConfigScript())
        wtf.scriptError(`Problems running configuration.\nPlease resolve issues then re-run install.`)

process.stdout.write(`Install done!\n\n`)

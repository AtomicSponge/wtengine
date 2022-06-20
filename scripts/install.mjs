#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
import wtf from './_common.cjs'
import 'inquirer'
 
process.stdout.write(`${wtf.colors.CYAN}WTEngine Install${wtf.colors.CLEAR}\n\n`)

//  Run system check
if(!wtf.checkSettings() && await wtf.confirmPrompt('Run WTEngine system check?'))
    if(!await wtf.runSysCheckScript())
        wtf.scriptError(`Problems running system check.\nPlease resolve issues then re-run install.`)

//  Run config
if(await wtf.confirmPrompt('Run WTEngine configuration?'))
    if(!await wtf.runConfigScript(['--nosyscheck']))
        wtf.scriptError(`Problems running configuration.\nPlease resolve issues then re-run install.`)

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Install done!${wtf.colors.CLEAR}\n\n`)
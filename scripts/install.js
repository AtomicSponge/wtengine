#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */
 
const wtf = require('./_common')
const inquirer = require('inquirer')

process.stdout.write(`${wtf.constants.CYAN}WTEngine Install${wtf.constants.CLEAR}\n\n`)

const res = (async function () {
    const res = await inquirer.prompt([{
        name: 'conf',
        type: 'confirm',
        message: `${wtf.constants.YELLOW}TEST`
    }]).then(res => { return res.conf })
    //await res.resolve()
    //console.log(res)
    return res
})()
console.log(res)

process.exit(0)

//  Run system check
if(wtf.confirmPrompt('Run WTEngine system check?'))
    if(!wtf.runSysCheckScript())
        wtf.scriptError(`Problems running system check.\nPlease resolve issues then re-run install.`)

//  Run config
if(wtf.confirmPrompt('Run WTEngine configuration?'))
    if(!wtf.runConfigScript())
        wtf.scriptError(`Problems running configuration.\nPlease resolve issues then re-run install.`)

process.stdout.write(`${wtf.constants.DIM}${wtf.constants.CYAN}Install done!${wtf.constants.CLEAR}\n\n`)

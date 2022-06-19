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
 
/*const res = (async function () {
    const res = await inquirer.prompt([{
        name: 'conf',
        type: 'confirm',
        message: `${wtf.colors.YELLOW}TEST`
    }]).then(res => { return res.conf })
    //await res.resolve()
    //console.log(res)
    return res
})()
console.log(res)*/

const res = await wtf.runSysCheckScript()
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

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Install done!${wtf.colors.CLEAR}\n\n`)
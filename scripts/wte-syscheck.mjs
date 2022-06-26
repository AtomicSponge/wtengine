#!/usr/bin/env node
/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

import commandExists from 'command-exists'
import wtf from './_common.cjs'

const workers = {
    /**
     * Check for necessary applications.
     * @returns True on success, else false
     */
    checkApps: () => {
        process.stdout.write(`Checking for necessary applications...\n`)
        var result = true
        wtf.config.checkApps.forEach(appCheck => {
            if(commandExists.sync(appCheck)) {
                process.stdout.write(`${wtf.colors.GREEN}  > '${appCheck}' found.${wtf.colors.CLEAR}\n`)
            } else {
                process.stdout.write(`${wtf.colors.RED}  > '${appCheck}' not found.${wtf.colors.CLEAR}\n`)
                result = false
            }
        })
        process.stdout.write(`${wtf.colors.CLEAR}\n`)
        return result
    }
}

wtf.scriptTitle(`WTEngine System Check Utility`)

workers.checkApps()

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}System check done!${wtf.colors.CLEAR}\n\n`)
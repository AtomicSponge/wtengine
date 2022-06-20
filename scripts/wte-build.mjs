#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

import wtf from './_common.cjs'
import 'inquirer'

process.stdout.write(`${wtf.colors.CYAN}WTEngine Build Utility${wtf.colors.CLEAR}\n\n`)

console.log(wtf.constants.WORK_FOLDER)
wtf.config.gitURLs.forEach(gitURL => {
    wtf.runCommand(`git clone ${gitURL.url}`, { cwd: wtf.constants.WORK_FOLDER })
})

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Build done!${wtf.colors.CLEAR}\n\n`)

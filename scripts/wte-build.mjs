#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

import wtf from './_common.cjs'
import 'inquirer'

wtf.constants.LOG_FILE = 'wte-build.log'

/**
 * Build script workers
 */
const workers = {
    /**
     * Batch run all git project commands.
     * @returns False if any commands fail, else true.
     */
    runGit: () => {
        var resA = []
        wtf.config.gitURLs.forEach(gitJob => {
            if(wtf.checkFolder(`${wtf.constants.WORK_FOLDER}/${gitJob.name}`)) {
                process.stdout.write(`Making sure ${gitJob.name} is up to date...  `)
                resA.push(wtf.runCommand(`git pull`, { cwd: `${wtf.constants.WORK_FOLDER}/${gitJob.name}` }))
                if(!resA[resA.length-1]) process.stdout.write(`ERROR!\n`)
                else process.stdout.write(`OK!\n`)
            }
            else {
                process.stdout.write(`Downloading ${gitJob.name} from ${gitJob.url}...  `)
                resA.push(wtf.runCommand(`git clone ${gitJob.url}`, { cwd: wtf.constants.WORK_FOLDER }))
                if(!resA[resA.length-1]) process.stdout.write(`ERROR!\n`)
                else process.stdout.write(`OK!\n`)
            }
        })
        var res = true
        resA.forEach(gitRes => { if(gitRes === false) res = false; return })
        return res
    }
}

/**
 * Build functions
 */
const build = {
    /**
     * Build the engine
     */
    engine: () => {
        if(!workers.runGit()) scriptError(`Error!  One or more repos failed to download!`)
    },

    /**
     * Build the project
     */
    project: () => {
        //
    }
}

/*
 * Main script
 */
wtf.scriptTitle(`WTEngine Build Utility`)

//  Parse command line arguments
const args = wtf.parseArgs(process.argv, [
    { name: 'buildEngine', flags: '--buildengine' },
])

const settings = wtf.loadSettings()
wtf.clearLog()

if(args.buildEngine) build.engine()
else build.project()

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Build done!${wtf.colors.CLEAR}\n\n`)
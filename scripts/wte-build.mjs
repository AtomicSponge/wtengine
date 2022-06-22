#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

import wtf from './_common.cjs'
import 'inquirer'

/**
 * Build script workers
 */
const workers = {
    /**
     * Batch run all git project commands.
     * @returns False if any commands fail, else true.
     */
    runGit: async () => {
        var resA = []
        await wtf.asyncForEach(wtf.config.gitURLs, async (gitJob) => {
            if(wtf.checkFolder(`${wtf.constants.WORK_FOLDER}/${gitJob.name}`)) {
                process.stdout.write(`Making sure ${gitJob.name} is up to date...  `)
                resA.push(await wtf.runCommand(`git pull`, { cwd: `${wtf.constants.WORK_FOLDER}/${gitJob.name}` }))
                if(!resA[resA.length-1]) process.stdout.write(`ERROR!\n`)
                else process.stdout.write(`OK!\n`)
            }
            else {
                process.stdout.write(`Downloading ${gitJob.name} from ${gitJob.url}...  `)
                resA.push(await wtf.runCommand(`git clone ${gitJob.url}`, { cwd: wtf.constants.WORK_FOLDER }))
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
    engine: async () => {
        wtf.constants.LOG_FILE = 'wte-build-engine.log'
        wtf.clearLog()
        if(!await workers.runGit()) scriptError(`Error!  One or more repos failed to download!`)
    },

    /**
     * Build the project
     */
    project: async () => {
        wtf.constants.LOG_FILE = 'wte-build-project.log'
        wtf.clearLog()
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
if(args.buildEngine) await build.engine()
else await build.project()

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Build done!${wtf.colors.CLEAR}\n\n`)
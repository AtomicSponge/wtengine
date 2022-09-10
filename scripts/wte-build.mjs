#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

import wtf from './_common.mjs'
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
    },

    /**
     * 
     */
    buildEngine: async (debug) => {
        let runCmd = ``
        if(debug) runCmd = `cmake --build ${wtf.constants.ENGINE_LOCATION}/wte-build --config Debug --target all`
        else runCmd =  `cmake --build ${wtf.constants.ENGINE_LOCATION}/wte-build --config Debug --target all`
        
        wtf.runCommand(runCmd, {}, true)
    }
}

/**
 * Build functions
 */
const build = {
    /**
     * Build the engine
     */
    engine: async (debugmode) => {
        wtf.constants.LOG_FILE = 'wte-build-engine.log'
        wtf.clearLog()
        wtf.writeLog(`WTEngine Build Script\n`)
        wtf.writeLog(`Starting Engine Build Process at ${new Date().toString()}\n\n`)

        //  Download necessary repos or check for updates.
        if(!await workers.runGit()) wtf.scriptError(`Error!  One or more repos failed to download!`)

        workers.buildEngine(debugmode)

        wtf.writeLog(`Engine Build Process completed at ${new Date().toString()}`)
    },

    /**
     * Build the project
     */
    project: async (debugmode) => {
        wtf.constants.LOG_FILE = 'wte-build-project.log'
        wtf.clearLog()
        wtf.writeLog(`WTEngine Build Script\n`)
        wtf.writeLog(`Starting Project Build Process at ${new Date().toString()}\n\n`)

        wtf.writeLog(`Project Build Process completed at ${new Date().toString()}`)
    }
}

/*
 * Main script
 */
wtf.scriptTitle(`WTEngine Build Utility`)

//  Parse command line arguments
const args = wtf.parseArgs(process.argv, [
    { name: 'buildEngine', flags: '--buildengine' },
    { name: 'buildProject', flags: '--buildproject' },
    { name: 'debugmode', flags: '--debug' },
])

if(!wtf.checkSettings()) scriptError(`No 'settings.json' file found!  Run 'npx wte-config' first!`)

const settings = wtf.loadSettings()

if(args.buildEngine || args.buildProject) {
    if(args.buildEngine) await build.engine(args.debugmode)
    if(args.buildProject) await build.project(args.debugmode)
} else {
    //option
}

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Build done!${wtf.colors.CLEAR}\n\n`)
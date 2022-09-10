#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtfngine
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
                resA.push(wtf.runCommand(`git pull`, { cwd: `${wtf.constants.WORK_FOLDER}/${gitJob.name}` }))
                if(!resA[resA.length-1]) process.stdout.write(`${wtf.colors.RED}ERROR!${wtf.colors.CLEAR}\n`)
                else process.stdout.write(`${wtf.colors.GREEN}OK!${wtf.colors.CLEAR}\n`)
            }
            else {
                process.stdout.write(`Downloading ${gitJob.name} from ${gitJob.url}...  `)
                resA.push(wtf.runCommand(`git clone ${gitJob.url}`, { cwd: wtf.constants.WORK_FOLDER }))
                if(!resA[resA.length-1]) process.stdout.write(`${wtf.colors.RED}ERROR!${wtf.colors.CLEAR}\n`)
                else process.stdout.write(`${wtf.colors.GREEN}OK!${wtf.colors.CLEAR}\n`)
            }
        })
        var res = true
        resA.forEach(gitRes => { if(gitRes === false) res = false; return })
        return res
    },

    /**
     * 
     */
    buildEngine: async (debugMode) => {
        let runCmd = ``
        if(debugMode) runCmd = `cmake --build /wtf-build --config Debug --target all --`
        else runCmd =  `cmake --build /wtf-build --config Debug --target all`
        
        wtf.runCommand(runCmd, { cwd: `${wtf.constants.ENGINE_ROOT_LOCATION}/` }, true)
    },

    /**
     * 
     */
    buildProject: async (debugMode) => {}
}

/**
 * Build functions
 */
const build = {
    /**
     * Build the engine
     */
    engine: async (debugMode) => {
        wtf.constants.LOG_FILE = 'wtf-build-engine.log'
        wtf.clearLog()
        wtf.writeLog(`wtfngine Build Script\n`)
        wtf.writeLog(`Starting Engine Build Process at ${new Date().toString()}\n\n`)

        //  Download necessary repos or check for updates.
        if(!await workers.runGit()) wtf.scriptError(`Error!  One or more repos failed to download!`)

        await workers.buildEngine(debugMode)

        wtf.writeLog(`Engine Build Process completed at ${new Date().toString()}`)
    },

    /**
     * Build the project
     */
    project: async (debugMode) => {
        wtf.constants.LOG_FILE = 'wtf-build-project.log'
        wtf.clearLog()
        wtf.writeLog(`wtfngine Build Script\n`)
        wtf.writeLog(`Starting Project Build Process at ${new Date().toString()}\n\n`)

        await workers.buildProject(debugMode)

        wtf.writeLog(`Project Build Process completed at ${new Date().toString()}`)
    }
}

/*
 * Main script
 */
wtf.scriptTitle(`wtfngine Build Utility`)

//  Parse command line arguments
const args = wtf.parseArgs(process.argv, [
    { name: 'buildEngine', flags: '--buildengine' },
    { name: 'buildProject', flags: '--buildproject' },
    { name: 'debugMode', flags: '--debug' },
])

if(!wtf.checkSettings()) scriptError(`No 'settings.json' file found!  Run 'npx wtf-config' first!`)

const settings = wtf.loadSettings()

if(args.buildEngine || args.buildProject) {
    if(args.buildEngine) await build.engine(args.debugMode)
    if(args.buildProject) await build.project(args.debugMode)
} else {
    //option
}

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Build done!${wtf.colors.CLEAR}\n\n`)
#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtfngine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

import wtf from './_common.mjs'
import inquirer from 'inquirer'

/*
 * Start script, set global variables
 */
wtf.scriptTitle(`WTEngine Build Utility`)

//  Parse command line arguments
const args = wtf.parseArgs(process.argv, [
    { name: 'buildEngine', flags: '--buildengine' },
    { name: 'buildProject', flags: '--buildproject' },
    { name: 'debugMode', flags: '--debug' },
])

//  Load settings
if(!wtf.checkSettings()) scriptError(`No 'settings.json' file found!  Run 'npx wte-config' first!`)
const settings = wtf.loadSettings()

settings.CMAKE_LOCATION = `/usr/bin/cmake`  // testing
settings.useNinja = true

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
            if(wtf.checkFolder(`${wtf.paths.ENGINE_TEMP_LOCATION}/${gitJob.name}`)) {
                process.stdout.write(`Making sure ${gitJob.name} is up to date...  `)
                resA.push(await wtf.runCommand(`git pull`, { cwd: `${wtf.paths.ENGINE_TEMP_LOCATION}/${gitJob.name}` }))
                if(!resA[resA.length-1]) process.stdout.write(`${wtf.colors.RED}ERROR!${wtf.colors.CLEAR}\n`)
                else process.stdout.write(`${wtf.colors.GREEN}OK!${wtf.colors.CLEAR}\n`)
            }
            else {
                process.stdout.write(`Downloading ${gitJob.name} from ${gitJob.url}...  `)
                resA.push(await wtf.runCommand(`git clone ${gitJob.url}`, { cwd: wtf.paths.ENGINE_TEMP_LOCATION }))
                if(!resA[resA.length-1]) process.stdout.write(`${wtf.colors.RED}ERROR!${wtf.colors.CLEAR}\n`)
                else process.stdout.write(`${wtf.colors.GREEN}OK!${wtf.colors.CLEAR}\n`)
            }
        })
        var res = true
        resA.forEach(gitRes => { if(gitRes === false) res = false; return })
        return res
    },

    /**
     * Runs the process of building the engine
     */
    buildEngine: async () => {
        //  Set up cmake project
        process.stdout.write(`\nConfiguring project... `)
        let runCmd = `${settings.CMAKE_LOCATION} --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Release -S${wtf.constants.ENGINE_ROOT_LOCATION} -B___SET_PATH___`

        //  Set output path (normal or debug)
        if(args.debugMode) runCmd = runCmd.replace(`___SET_PATH___`, wtf.paths.ENGINE_BUILD_DEBUG_LOCATION)
        else runCmd = runCmd.replace(`___SET_PATH___`, wtf.paths.ENGINE_BUILD_LOCATION)

        //  Set the debug flag if on
        if(args.debugMode) runCmd += ` -DWTE_BUILD_DEBUG:BOOL=ON`
        if(settings.useNinja) runCmd += " -G Ninja"

        //  Run the command
        if(!await wtf.runCommand(runCmd, { cwd: `${wtf.constants.ENGINE_ROOT_LOCATION}` }))
            wtf.scriptError(`Build command failed!`)
        process.stdout.write(`${wtf.colors.GREEN}Done!${wtf.colors.CLEAR}\n`)

        //  Run the build
        if(args.debugMode) process.stdout.write(`Starting engine debug build... `)
        else process.stdout.write(`\nStarting engine build... `)
        runCmd = `${settings.CMAKE_LOCATION} --build ___SET_PATH___ --config Release --target all --`

        //  Set output path (normal or debug)
        if(args.debugMode) runCmd = runCmd.replace(`___SET_PATH___`, wtf.paths.ENGINE_BUILD_DEBUG_LOCATION)
        else runCmd = runCmd.replace(`___SET_PATH___`, wtf.paths.ENGINE_BUILD_LOCATION)

        //  Run the command
        if(!await wtf.runCommand(runCmd, { cwd: `${wtf.constants.ENGINE_ROOT_LOCATION}` }))
            wtf.scriptError(`Build command failed!`)
        process.stdout.write(`${wtf.colors.GREEN}Done!${wtf.colors.CLEAR}\n`)
    },

    /**
     * Runs the process of building the project
     */
    buildProject: async () => {}
}

/**
 * Build functions
 */
const build = {
    /**
     * Build the engine
     */
    engine: async () => {
        wtf.files.LOG_FILE = 'wte-build-engine.log'
        wtf.clearLog()
        wtf.writeLog(`WTEngine Build Script\n`)
        wtf.writeLog(`Starting Engine Build Process at ${new Date().toString()}\n\n`)
        if(args.debugMode) wtf.writeLog(`ALERT!  Building engine debug mode!\n\n`)

        //  Download necessary repos or check for updates.
        if(!await workers.runGit()) wtf.scriptError(`Error!  One or more repos failed to download!`)

        await workers.buildEngine()

        wtf.writeLog(`\nEngine Build Process completed at ${new Date().toString()}`)
    },

    /**
     * Build the project
     */
    project: async () => {
        wtf.files.LOG_FILE = 'wte-build-project.log'
        wtf.clearLog()
        wtf.writeLog(`WTEngine Build Script\n`)
        wtf.writeLog(`Starting Project Build Process at ${new Date().toString()}\n\n`)
        if(args.debugMode) wtf.writeLog(`ALERT!  Building project debug mode!\n\n`)

        await workers.buildProject()

        wtf.writeLog(`\nProject Build Process completed at ${new Date().toString()}`)
    }
}

/*
 * Run script
 */
if(args.buildEngine || args.buildProject) {
    if(args.buildEngine) await build.engine()
    if(args.buildProject) await build.project()
} else {
    const buildRes = await inquirer.prompt([{
        name: 'buildselect',
        type: 'rawlist',
        message: `${wtf.colors.YELLOW}Build engine or project?`,
        choices: [ `Engine`, "Project" ]
    }])
    if(buildRes.buildselect == `Engine`) await build.engine()
    if(buildRes.buildselect == `Project`) await build.project()
}

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Build done!${wtf.colors.CLEAR}\n\n`)
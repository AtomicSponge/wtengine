#!/usr/bin/env node
/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const fs = require('fs')
const { parse } = require('csv/sync')
const { showInfo, confirmPrompt, scriptError } = require('./_common')

showInfo()

//if(!confirmPrompt('test?')) scriptError('flagrant error')

/**
 * Parse a CSV file.
 * @param {String} csvFilename 
 * @returns {Object} Parsed CSV file
 */
const readCSVData = (csvFilename) => {
    return parse(fs.readFileSync(csvFilename))
}

/**
 * Build a game script file.
 * @param {Object} gameData 
 * @returns {Object} 
 */
const buildScriptFile = (gameData) => {
    //  verify 6 columns
    //  time / sys / to / from / cmd / args
    return gameData
}

/*
 * Main script
 */
const args = process.argv.slice(2)

if(args[0] === undefined) scriptError('Please specify an input file.')
if(!fs.existsSync(args[0])) scriptError(`Input file '${args[0]}' does not exist.`)
if(args[1] === undefined) scriptError('Please specify an output file.')
if(fs.existsSync(args[1]) && !confirmPrompt(`Output file '${args[1]}' exists, overwrite?`))
    scriptError(`Output file '${args[1]}' already exists.`)

const inFile = args[0]
const outFile = args[1]

const inData = readCSVData(inFile)
const outData = buildScriptFile(inData)
console.log(outData)

try {
    //fs.writeFileSync(outFile, outData)
    console.log(`Wrote data file '${outFile}'`)
} catch(error) { scriptError(error) }

console.log('Done!')

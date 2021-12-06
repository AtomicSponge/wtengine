/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @version 0.0.2
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const fs = require('fs')
const { exit } = require('process')
const { confirmPrompt, scriptError } = require('./tools_common')
const csv = require('csv-parse')

confirmPrompt('test?')

/**
 * Parse a CSV file.
 * @param {String} csvFile 
 * @returns {Object} 
 */
const readCSVData = (csvFile) => {
    return csvFile
}

/**
 * Build a game script file.
 * @param {Object} data 
 * @returns {Object} 
 */
const buildScriptFile = (data) => {
    return data
}

/*
 * Main script
 */
const args = process.argv.slice(2)

if(args[0] === undefined) scriptError('Please specify an input file.')
if(!fs.existsSync(args[0])) scriptError('Input file does not exist.')
if(args[1] === undefined) scriptError('Please specify an output file.')
if(fs.existsSync(args[1])) {
    exit(0)
}

const inData = readCSVData(args[0])
const outData = buildScriptFile(inData)

try {
    fs.writeFileSync(args[1], outData)
} catch(error) { scriptError(error) }

console.log('Done!')

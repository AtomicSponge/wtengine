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
const csv = require('csv-parse')
const { exit } = require('process')

/**
 * Display an error message and exit script.
 * @param {String} message Message to display.
 */
const scriptError = (message) => {
    console.log(`Error:  ${message}`)
    console.log(`Exiting...`)
    exit(-1)
}

const readCSVData = (csvFile) => {}

const buildScriptFile = (filename, data) => {}

/*
 * Main script
 */

let args = process.argv.slice(2)

if(args[0] === undefined) scriptError('no infile')
if(args[1] === undefined) scriptError('no outfile')

const inData = readCSVData(args[0])
const outData = buildScriptFile(args[1], inData)

// save data...

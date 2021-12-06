/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @version 0.0.1
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const fs = require('fs')
const csv = require('csv-parse')
const { exit } = require('process')

const scriptError = (message) => {
    console.log(`Error:  ${message}`)
    console.log(`Exiting...`)
    exit(-1)
}

const buildScriptFile = () => {}

const readCSVData = (csvFile) => {}

/*
 * Main script
 */

let args = process.argv.slice(2)

if(args[0] === undefined) scriptError('no infile')
if(args[1] === undefined) scriptError('no outfile')

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
const { Buffer } = require('buffer')
const { parse } = require('csv/sync')
const { showScriptInfo, confirmPrompt, scriptError } = require('./_common')

showScriptInfo('wte-mkscript')

/*
 * Process command arguments
 */
const args = process.argv.slice(2)
if(args[0] === undefined) scriptError('Please specify an input file.')
if(!fs.existsSync(args[0])) scriptError(`Input file '${args[0]}' does not exist.`)
if(args[1] === undefined) scriptError('Please specify an output file.')
if(args[1].split('.')[1] === undefined) args[1] += '.sdf'
if(fs.existsSync(args[1]) && !confirmPrompt(`Output file '${args[1]}' exists, overwrite?`))
    scriptError(`Output file '${args[1]}' already exists.`)

/*
 * Parse the input file
 */
process.stdout.write(`Parsing data file '${args[0]}'...\n`)
let gameData = undefined
switch(args[0].split('.')[1].toLowerCase()) {
    case 'csv':
        gameData = parse(fs.readFileSync(args[0]))
        break
    default:
        scriptError(`File format '${args[0].split('.')[1]}' not supported.`)
}
if(gameData === undefined) scriptError('Error parsing game data.')

/*
 * Generate the data file buffer
 */
process.stdout.write(`Generating game data, one moment...\n`)
let rowCounter = Number(0)        //  Row counter for error reporting
let dataBuffer = Buffer.alloc(0)  //  Buffer to store binary file
gameData.forEach(row => {
    rowCounter++
    if(row.length !== 6) scriptError(`Row ${rowCounter} incorrect length.`)

    //  Write each message:  timer / sys / to / from / cmd / args
    dataBuffer = Buffer.concat([dataBuffer, Buffer.concat([
        Buffer.from(new Int32Array([row[0]])),
        Buffer.from(
            row[1] + '\x00' + row[2] + '\x00' + row[3] + '\x00' +
            row[4] + '\x00' + row[5] + '\x00'
        )
    ])])
})

/*
 * Write the data file buffer to file
 */
try {
    fs.writeFileSync(args[1], dataBuffer)
    process.stdout.write(`Wrote data file '${args[1]}' - ${rowCounter} commands\n`)
} catch(error) { scriptError(error) }

process.stdout.write('Done!\n')

#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

const wtf = require('./_common')
const path = require('path')
const fs = require('fs')
const { Buffer } = require('buffer')
const csv = require('csv/sync')

wtf.showScriptInfo(path.basename(__filename, path.extname(__filename)))

/*
 * Process command arguments
 */
const args = process.argv.slice(2)
if(args[0] === undefined) wtf.scriptError('Please specify an input file.')
if(!fs.existsSync(args[0])) wtf.scriptError(`Input file '${args[0]}' does not exist.`)
if(args[1] === undefined) args[1] = args[0].split('.')[0]
if(args[1].split('.')[1] === undefined) args[1] += '.sdf'
if(fs.existsSync(args[1]) && !wtf.confirmPrompt(`Output file '${args[1]}' exists, overwrite?`))
    wtf.scriptError(`Output file '${args[1]}' already exists.`)

/*
 * Parse the input file
 */
process.stdout.write(`Parsing data file '${args[0]}'...\n`)
let gameData = undefined
switch(args[0].split('.')[1].toLowerCase()) {
    case 'csv':
        gameData = csv.parse(fs.readFileSync(args[0]))
        break
    default:
        wtf.scriptError(`File format '${args[0].split('.')[1]}' not supported.`)
}
if(gameData === undefined || !(gameData instanceof Array))
    wtf.scriptError('Parsing game data failed.')

/*
 * Generate the data file buffer
 */
process.stdout.write(`Generating game data file '${args[1]}'...\n`)
let rowCounter = Number(0)        //  Row counter for error reporting
let dataBuffer = Buffer.alloc(0)  //  Buffer to store binary file
gameData.forEach(row => {
    rowCounter++
    if(row.length !== 6) wtf.scriptError(`Row ${rowCounter}: incorrect length.`)

    //  Write each message:  timer / sys / to / from / cmd / args
    const timerBuffer = Buffer.alloc(8)
    timerBuffer.writeBigInt64LE(BigInt.asIntN(64, row[0]))
    dataBuffer = Buffer.concat([dataBuffer, Buffer.concat([
        timerBuffer, Buffer.from(
            row[1] + '\x00' + row[2] + '\x00' + row[3] + '\x00' +
            row[4] + '\x00' + row[5] + '\x00'
        )
    ])])
})

//  Verify data generated
if(Buffer.byteLength(dataBuffer, 'utf8') == 0)
    wtf.scriptError('No data generated.')

/*
 * Write out the data file buffer
 */
try {
    fs.writeFileSync(args[1], dataBuffer)
    process.stdout.write(`\nWrote data file '${args[1]}'\n${rowCounter} total commands.\n`)
    process.stdout.write(`Size: ${Buffer.byteLength(dataBuffer, 'utf8')} bytes.\n\n`)
} catch(error) { wtf.scriptError(error) }

process.stdout.write('Done!\n')
process.exit(0)

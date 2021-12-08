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
const Buffer = require('buffer')
const { parse } = require('csv/sync')
const { showScriptInfo, confirmPrompt, scriptError } = require('./_common')

showScriptInfo()

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
 * @param {String} outFile 
 * @param {Object} gameData 
 */
const buildScriptFile = (outFile, gameData) => {
    let dataBlob = []
    let rowCounter = 0
    gameData.forEach(row => {
        rowCounter++
        if(row.length !== 6) scriptError(`Row ${rowCounter} incorrect length.`)
        // timer
        {const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[0])}
        // sys
        {const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[1])}
        // to
        {const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[2])}
        // from
        {const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[3])}
        // cmd
        {const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[4])}
        // args
        {const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[5])}
    })

    const size = 100  //  calc data blob size
    let dataBuffer = Buffer.alloc(size)
    //  write blob to buffer

    try {
        //fs.writeFileSync(outFile, dataBuffer)
        console.log(`Wrote data file '${outFile}'`)
    } catch(error) { scriptError(error) }
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

const gameData = readCSVData(inFile)
buildScriptFile(outFile, gameData)

console.log('Done!')

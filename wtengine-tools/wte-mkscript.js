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
    let rowCounter = Number(0)
    gameData.forEach(row => {
        rowCounter++
        if(row.length !== 6) scriptError(`Row ${rowCounter} incorrect length.`)
        let tempBlob = []
        // timer - awalys same size
        {const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[0])
        tempBlob.push({ size: 0, data: tempBuffer })}
        // sys
        {const len = 0
        const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[1])
        tempBlob.push({ size: len, data: tempBuffer })}
        // to
        {const len = 0
        const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[2])
        tempBlob.push({ size: len, data: tempBuffer })}
        // from
        {const len = 0
        const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[3])
        tempBlob.push({ size: len, data: tempBuffer })}
        // cmd
        {const len = 0
        const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[4])
        tempBlob.push({ size: len, data: tempBuffer })}
        // args
        {const len = 0
        const tempBuffer = Buffer.alloc(1)
        tempBuffer.write(row[5])
        tempBlob.push({ size: len, data: tempBuffer })}

        dataBlob.push(tempBlob)
    })

    let bufferSize = Number(0)
    dataBlob.forEach(row => {
        row.forEach(column => { bufferSize += column.size })
    })

    let dataBuffer = Buffer.alloc(bufferSize)
    dataBlob.forEach(row => {
        //row.forEach(column => { dataBuffer += column.data })
    })

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

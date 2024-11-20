#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module silvergun-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

import fs from 'node:fs'
import { Buffer } from 'node:buffer'

import { Command } from 'commander'
import { confirm } from '@inquirer/prompts'
import { parse as csvParse } from 'csv-parse/sync'
import { dim, cyan, yellow } from 'kolorist'

import { scriptError } from '@spongex/script-error'

/** Save version number - relevant to itself and not the script */
const SAVE_FILE_VERSION = 'v0.9.0'

/**
 * Create the game script binary file
 * @param inFile Input filename
 * @param outFile Output filename
 */
const createScriptData = (inFile:string, outFile:string):void => {
  /*
   * Parse the input file
   */
  console.log(`Parsing data file '${inFile}'...\n`)
  const gameData:any = (() => {
    switch (inFile.split('.')[1].toLowerCase()) {
      /* CSV file data */
      case 'csv':
        return csvParse(fs.readFileSync(inFile), { skip_empty_lines: true })
      /* JSON file data */
      case 'json':
        let tempGameData:any = []
        {const tempData = JSON.parse(fs.readFileSync(inFile).toString())
        Object.keys(tempData).forEach(key => { tempGameData.push(tempData[key]) })}
        return tempGameData
      case 'yml':
        scriptError('Not yet supported!')
        break
      /* Unsupported file types */
      default:
        scriptError(`File format '${inFile.split('.')[1]}' not supported!`)
    }
  })()

  if (gameData === null ||
      gameData === undefined ||
      !(gameData instanceof Array) ||
      gameData.length === 0)
    scriptError('Parsing game data failed!')

  console.log(`Parsed datafile '${inFile}.'`)
  console.log(`${gameData.length} rows read.\n`)

  /*
   * Generate the data file buffer
   */
  console.log(`Generating game data file '${outFile}'...`)

  /** Header to identify file stored in buffer */
  const fileHeader = Buffer.from('46445300', 'hex')
  /** Save file version stored in buffer */
  const fileVersion = Buffer.from(SAVE_FILE_VERSION)

  /** Row counter for error reporting */
  let rowCounter = Number(0)
  /** Buffer to store binary file */
  let dataBuffer = Buffer.alloc(0)

  gameData.forEach((row:any) => {
    rowCounter++
    if (row.length !== 6) scriptError(`Row ${rowCounter}: incorrect length!`)

    //  Write each message:  timer / sys / to / from / cmd / arg
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
  if (Buffer.byteLength(dataBuffer, 'utf8') === 0 || rowCounter === 0)
    scriptError('No data generated!')

  /*
   * Create final output buffer
   *
   * --- FILE FORMAT ---
   * fileHeader - 16 bits - File header for identification
   * fileVersion - 24 bits - Version number of the script file
   * commandCount - 32 bits - Total number of proceeding commands
   * dataBuffer - blob - List of all commands to run
   */
  /** Total number of commands in file */
  const commandCount = Buffer.alloc(4)
  commandCount.writeUInt8(rowCounter)
  /** File output buffer */
  const outBuffer = Buffer.concat([
    fileHeader, fileVersion, commandCount, dataBuffer
  ])

  /*
   * Write out the data file buffer
   */
  try {
    fs.writeFileSync(outFile, outBuffer)
    console.log(`\nWrote data file '${outFile}'\n${rowCounter} total commands.`)
    console.log(`Size: ${Buffer.byteLength(outBuffer, 'utf8')} bytes.\n`)
  } catch (error:any) { scriptError(error.message) }

  console.log(dim(cyan(`Script conversion done!\n`)))
}

/*
 * SCRIPT START
 */
console.log(cyan(`Silvergun Game Engine Script Utility`))
const program = new Command()
program
  .name('slv-mkscript')
  .description('Convert csv, json, or (soon) yml files to Silvergun Game Engine scripts')
  .argument('<inFile>', 'Input file - required')
  .argument('[outFile]', 'Output file - optional, will use input name if not provided')
  .action(async (inFile, outFile) => {
    if (!fs.existsSync(inFile)) scriptError(`Input file '${inFile}' does not exist.`)
    if (outFile === undefined) outFile = inFile.split('.').at(0)
    if (outFile.split('.').at(-1) === undefined || outFile.split('.').at(-1) !== '.sdf')
      outFile += '.sdf'

    await (async () => {
      if (
        fs.existsSync(outFile) &&
        !await confirm({ message: yellow(`Output file '${outFile}' exists, overwrite?`) })
      ) {
        scriptError(`Output file '${outFile}' already exists!`)
      }
    })()

    createScriptData(inFile, outFile)
  })
  program.showHelpAfterError()
await program.parseAsync()
process.exit(0)

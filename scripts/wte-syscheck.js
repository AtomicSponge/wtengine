#!/usr/bin/env node
/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

const wtf = require('./_common')
const config = require('./_config')

process.stdout.write(`${wtf.constants.CYAN}WTEngine System Check Utility${wtf.constants.CLEAR}\n\n`)

wtf.checkApps()

process.stdout.write(`${wtf.constants.DIM}${wtf.constants.CYAN}System check done!${wtf.constants.CLEAR}\n\n`)

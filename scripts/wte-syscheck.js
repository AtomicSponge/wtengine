#!/usr/bin/env node
/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

//const wtf = require('./_common')
import wtf from './_common.cjs'

process.stdout.write(`${wtf.colors.CYAN}WTEngine System Check Utility${wtf.colors.CLEAR}\n\n`)

wtf.checkApps()

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}System check done!${wtf.colors.CLEAR}\n\n`)
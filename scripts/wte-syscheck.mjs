#!/usr/bin/env node
/**
 * 
 * @author Matthew Evans
 * @module wtfsystems/wtengine-tools
 * @see README.md
 * @copyright MIT see LICENSE.md
 * 
 */

import wtf from './_common.cjs'

wtf.scriptTitle(`WTEngine System Check Utility`)

wtf.checkApps()

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}System check done!${wtf.colors.CLEAR}\n\n`)
#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

/**
 * 
 */
const checkApps = [ 'cmake', 'git' ]

const wtf = require('./_common')
const fs = require('fs')
const path = require('path')
const shell = require('shelljs')

//  Set path to node for shelljs
const nodePath = (shell.which('node').toString())
shell.config.execPath = nodePath

checkApps.forEach((appCheck) => {
    if(shell.which(appCheck)) console.log(`${appCheck} found.`)
    else wtf.scriptError(`${appCheck} not found.`)
})

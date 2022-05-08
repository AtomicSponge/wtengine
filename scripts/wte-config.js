#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

const wtf = require('./_common')

const fs = require('fs')
const path = require('path')
const shell = require('shelljs')
const inquirer = require('inquirer')

process.stdout.write(`${wtf.constants.CYAN}WTEngine Configuration Utility${wtf.constants.CLEAR}\n\n`)

const settings = wtf.loadSettings()
if(!settings) {
    process.stdout.write(`No settings file found, running setup...\n`)
} else {
    process.stdout.write(`settings exist... do other stuff\n`)
}

process.stdout.write(`${wtf.constants.DIM}${wtf.constants.CYAN}Config Done!${wtf.constants.CLEAR}\n\n`)

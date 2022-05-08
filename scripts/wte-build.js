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

process.stdout.write(`${wtf.constants.CYAN}WTEngine Build Utility${wtf.constants.CLEAR}\n\n`)

process.stdout.write(`${wtf.constants.DIM}${wtf.constants.CYAN}Build done!${wtf.constants.CLEAR}\n\n`)

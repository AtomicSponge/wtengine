#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

const wtf = require('./_common')
const config = require('./_config')

const fs = require('fs')
const path = require('path')
const shell = require('shelljs')
const inquirer = require('inquirer')

process.stdout.write(`${wtf.constants.CYAN}WTEngine Configuration Utility${wtf.constants.CLEAR}\n\n`)

process.stdout.write(`${wtf.constants.DIM}${wtf.constants.CYAN}Config Done!${wtf.constants.CLEAR}\n\n`)

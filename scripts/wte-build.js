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

process.stdout.write(`${wtf.colors.CYAN}WTEngine Build Utility${wtf.colors.CLEAR}\n\n`)

process.stdout.write(`${wtf.colors.DIM}${wtf.colors.CYAN}Build done!${wtf.colors.CLEAR}\n\n`)

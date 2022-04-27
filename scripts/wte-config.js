#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

const fs = require('fs')
const path = require('path')
const shell = require('shelljs')

const settings = require('../settings')
const config = require('./_config')

//  Set path to node for shelljs
const nodePath = (shell.which('node').toString())
shell.config.execPath = nodePath

process.stdout.write('done')
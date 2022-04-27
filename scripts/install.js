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

const config = require('./_config')

//  Set path to node for shelljs
const nodePath = (shell.which('node').toString())
shell.config.execPath = nodePath

wtf.createSettings()
wtf.checkSettings()

const settings = require('../settings')

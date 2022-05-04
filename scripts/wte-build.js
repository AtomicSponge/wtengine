#!/usr/bin/env node
/**
 * @author Matthew Evans
 * @module wtfsystems/wtengine
 * @see README.md
 * @copyright MIT see LICENSE.md
 */

const wtf = require('./_common')
const config = require('./_config')
//const settings = require('../settings')

const fs = require('fs')
const path = require('path')
const shell = require('shelljs')
const inquirer = require('inquirer')

wtf.checkApps()

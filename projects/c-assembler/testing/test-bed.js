#! /usr/bin/env node

const { exec } = require('node:child_process')
const { readFile } = require('node:fs/promises')
const { resolve } = require('node:path')
const { cwd } = require('node:process')
const { promisify } = require('node:util')

const execPrm = promisify(exec)

const PROJECT_NAME = require('../project.json').name
const RUN_COMMAND = (sourceFile) => `nx run ${PROJECT_NAME}:run --args="--input ${sourceFile}"`

const COLORS = {
    RED: '\x1b[31m',
    GREEN: '\x1b[32m',
    YELLOW: '\x1b[33m',
    RED_BACKGROUND: '\x1b[41m',
    GREEN_BACKGROUND: '\x1b[42m',
}
const CONSOLE_COLOR = (color) => `${color}%s\x1b[0m`;

; (async () => {
    try {
        const testsConfiguration = require('./inputs-configuration.json')

        let errorRate = 0

        for (const test of testsConfiguration) {
            const { expect, sourceFile, target } = test
            await execPrm(RUN_COMMAND(sourceFile))

            const obExpectFile = await readFile(resolve(cwd(), expect.fileOutput), 'utf-8')
            const postProcessExpectFile = await readFile(resolve(cwd(), expect.postProcessFileOutput), 'utf-8')
            const entryExpectFile = await readFile(resolve(cwd(), expect.entryFileOutput), 'utf-8')
            const externExpectFile = await readFile(resolve(cwd(), expect.externFileOutput), 'utf-8')

            const obTargetFile = await readFile(resolve(cwd(), target.fileOutput), 'utf-8')
            const postProcessTargetFile = await readFile(resolve(cwd(), target.postProcessFileOutput), 'utf-8')
            const entryTargetFile = await readFile(resolve(cwd(), target.entryFileOutput), 'utf-8')
            const externTargetFile = await readFile(resolve(cwd(), target.externFileOutput), 'utf-8')

            if (obExpectFile !== obTargetFile) {
                console.error(CONSOLE_COLOR(COLORS.RED), `Test failed: ob file is different`)
                errorRate++
            }

            if (postProcessExpectFile !== postProcessTargetFile) {
                console.error(CONSOLE_COLOR(COLORS.RED), `Test failed: post process file is different`)
                errorRate++
            }

            if (entryExpectFile !== entryTargetFile) {
                console.error(CONSOLE_COLOR(COLORS.RED), `Test failed: entry file is different`)
                errorRate++
            }

            if (externExpectFile !== externTargetFile) {
                console.error(CONSOLE_COLOR(COLORS.RED), `Test failed: extern file is different`)
                errorRate++
            }
        }

        if (!errorRate) {
            console.log(CONSOLE_COLOR(COLORS.GREEN_BACKGROUND), 'All tests passed successfully')
            return
        }

        console.error(CONSOLE_COLOR(COLORS.RED_BACKGROUND), `${errorRate} tests failed`)
        throw new Error('Tests failed')
    } finally {
    }
})()
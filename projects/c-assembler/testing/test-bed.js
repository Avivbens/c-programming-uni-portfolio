#! /usr/bin/env node

const { exec } = require('node:child_process')
const { readFile } = require('node:fs/promises')
const { resolve } = require('node:path')
const { cwd } = require('node:process')
const { promisify } = require('node:util')

const execPrm = promisify(exec)

const PROJECT_NAME = require('../project.json').name
const DIST_FOLDER = resolve(cwd(), 'projects', PROJECT_NAME, 'dist')
const TESTS_OUTPUT_FOLDER = resolve(cwd(), 'projects', PROJECT_NAME, 'testing', 'outputs')
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
            const { cases, sourceFile } = test
            await execPrm(RUN_COMMAND(sourceFile))

            for (const [caseName, fileName] of Object.entries(cases)) {
                const expectedContent = await readFile(resolve(TESTS_OUTPUT_FOLDER, fileName), 'utf-8')
                const targetContent = await readFile(resolve(DIST_FOLDER, fileName), 'utf-8')

                if (expectedContent !== targetContent) {
                    console.error(CONSOLE_COLOR(COLORS.RED), `Test failed: ${caseName} is different`)
                    errorRate++
                }
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
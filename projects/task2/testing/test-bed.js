#! /usr/bin/env node

const { exec } = require('node:child_process')
const { mkdtemp, writeFile } = require('node:fs/promises')
const { cwd, exit } = require('node:process')
const { promisify } = require('node:util')

const execPrm = promisify(exec)

const PROJECT_NAME = require('../project.json').name
const DIST_PATH = `dist/${PROJECT_NAME}`
const EXECUTABLE = `${DIST_PATH}/${PROJECT_NAME}`

const COLORS = {
    RED: '\x1b[31m',
    GREEN: '\x1b[32m',
    YELLOW: '\x1b[33m',
    RED_BACKGROUND: '\x1b[41m',
    GREEN_BACKGROUND: '\x1b[42m',
}
const CONSOLE_COLOR = (color) => `${color}%s\x1b[0m`

async function createTempFile() {
    const tempDirName = await mkdtemp('dist/test-')
    const tempFileName = `${tempDirName}/input.txt`
    await writeFile(tempFileName, '')

    return { tempDirName, tempFileName }
}

function extractResults(rawOutput) {
    const lines = rawOutput.split('\n')
    if (!lines.length) {
        exit(1)
    }

    const RESULTS_HEADER = `The results are: `
    const resultsIndex = lines.findIndex((line) => line.includes(RESULTS_HEADER))
    if (resultsIndex === -1) {
        exit(1)
    }

    const allResults = lines.slice(resultsIndex + 1)

    const parsedResults = allResults.filter(Boolean).map(s => s.trim()).map(Number)
    return parsedResults
}

async function cleanup(tempDirectory) {
    await execPrm(`rm -rf ${tempDirectory}`, { shell: true })
}

; (async () => {
    const { tempDirName, tempFileName } = await createTempFile()

    try {
        const testsConfiguration = require('./inputs-configuration.json')

        let errorRate = 0

        /* run tests */
        for (const test of testsConfiguration) {
            const { inputs, output } = test

            const { results } = output

            const injectInputs = `${inputs.length}\n${inputs.elements.join('\n')}`
            await writeFile(tempFileName, injectInputs)

            const { stdout } = await execPrm(`${cwd()}/${EXECUTABLE} < ${cwd()}/${tempFileName}`, { shell: true })
            const runResults = extractResults(stdout)

            if (JSON.stringify(runResults) !== JSON.stringify(results)) {
                console.error(`Test failed: ${JSON.stringify(runResults)} => ${JSON.stringify(results)}`)
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
        await cleanup(tempDirName)
    }
})()
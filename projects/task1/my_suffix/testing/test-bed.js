#! /usr/bin/env node

const { exec } = require('node:child_process')
const { mkdtemp, writeFile } = require('node:fs/promises')
const { resolve } = require('node:path')
const { cwd } = require('node:process')
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

function extractMatchSum(rawOutput) {
    const searchTerm = /^Number of matches found: (\d+)$/gm
    const match = searchTerm.exec(rawOutput)
    const [, result] = match ?? []

    return result ?? 0
}

function compareArrays(arr1, arr2) {
    if (arr1.length !== arr2.length) {
        return false
    }

    for (let i = 0; i < arr1.length; i++) {
        if (arr1[i] !== arr2[i]) {
            return false
        }
    }

    return true
}

function extractSuffixMatches(rawOutput) {
    const lines = rawOutput.split('\n')
    if (!lines.length) {
        return []
    }

    const res = lines.reduce((acc, line) => {
        const searchTerm = /^\*\s([a-zA-Z0-9]+)$/g
        const [, results] = searchTerm.exec(line) ?? []

        results && acc.push(results)
        return acc
    }, [])

    return res
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

            const { matches, suffixes } = output

            await writeFile(tempFileName, inputs.join('\n'))

            const program = resolve(cwd(), EXECUTABLE)
            const tempFile = resolve(cwd(), tempFileName)
            const command = `${program} < ${tempFile}`

            const { stdout } = await execPrm(command, { shell: true })
            const matchSum = extractMatchSum(stdout)
            const suffixMatch = extractSuffixMatches(stdout)

            if (String(matchSum) !== String(matches)) {
                console.error(`${inputs.join(', ')} - Match sum is not correct. Expected: ${matches}, got: ${matchSum}`)
                errorRate++
            }

            const isIdentical = compareArrays(suffixMatch, suffixes)
            /**
             * Ignore this test over CI
             */
            if (!isIdentical && !process.env.CI) {
                console.error(`${inputs.join(', ')} - Matches are not correct. Expected: ${JSON.stringify(suffixes)}, got: ${JSON.stringify(suffixMatch)}`)
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
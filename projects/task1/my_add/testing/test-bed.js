#! /usr/bin/env node

const { exec } = require('node:child_process')
const { mkdtemp, writeFile } = require('node:fs/promises')
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
    const name = await mkdtemp('dist/test-')
    const fileName = `${name}/input.txt`
    await writeFile(fileName, '')

    return fileName
}

function extractBinarySum(rawOutput) {
    const searchTerm = /^Sum of binaries: (\d+)$/gm
    const match = searchTerm.exec(rawOutput)
    const [, result] = match ?? []

    if (!result) {
        throw new Error('Invalid output')
    }

    return result
}

function extractDecimalSum(rawOutput) {
    const searchTerm = /^Sum of binaries - Decimal: (\d+)$/gm
    const match = searchTerm.exec(rawOutput)
    const [, result] = match ?? []

    if (!result) {
        throw new Error('Invalid output')
    }

    return result
}

; (async () => {
    /* build project */
    await execPrm(`npx nx run ${PROJECT_NAME}:build`)

    const testsConfiguration = require('./inputs-configuration.json')
    const tempTextFile = await createTempFile()

    let errorRate = 0

    /* run tests */
    for (const test of testsConfiguration) {
        const { inputs, output } = test

        const { decimal, binary } = output

        await writeFile(tempTextFile, inputs.join('\n'))

        const { stdout } = await execPrm(`${EXECUTABLE} < ${tempTextFile}`)
        const binarySum = extractBinarySum(stdout)
        const decimalSum = extractDecimalSum(stdout)

        if (String(binarySum) !== String(binary)) {
            console.error(`${inputs.join(', ')} - Binary sum is not correct. Expected: ${binary}, got: ${binarySum}`)
            errorRate++
        }

        if (String(decimalSum) !== String(decimal)) {
            console.error(`${inputs.join(', ')} - Decimal sum is not correct. Expected: ${decimal}, got: ${decimalSum}`)
            errorRate++
        }
    }

    if (!errorRate) {
        console.log(CONSOLE_COLOR(COLORS.GREEN_BACKGROUND), 'All tests passed successfully')
        return
    }

    console.error(CONSOLE_COLOR(COLORS.RED_BACKGROUND), `${errorRate} tests failed`)
    throw new Error('Tests failed')
})()
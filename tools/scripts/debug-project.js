#!/usr/bin/env node

const { exec } = require('node:child_process')
const { resolve, dirname } = require('node:path')
const { existsSync } = require('node:fs')
const { cwd } = require('node:process')
const { promisify } = require('node:util')
const { readdir } = require('node:fs/promises')

const execPrm = promisify(exec)

const ROOT_DIR = cwd()
const DEBUGGER_SYMLINK_PATH = `${ROOT_DIR}/dist/debugger`
const BUILD_SCRIPT = (projectName) => `npx nx run ${projectName}:build:debug`

/**
 * Search for an Nx project in the given directory
 * @param rootDir - the root directory of the project
 * @param dirPath - the directory to start looking for the Nx project
 * @returns the name of the Nx project or null if not found
 */
async function findNxProject(rootDir, dirPath) {
    if (!existsSync(dirPath)) {
        return null
    }

    if (dirPath === rootDir) {
        return null
    }

    const dirFiles = await readdir(dirPath)
    if (dirFiles.includes('project.json')) {
        const projectName = require(resolve(dirPath, 'project.json')).name
        return projectName
    }

    const parentDir = resolve(dirPath, '..')

    return findNxProject(rootDir, parentDir)
}

; (async () => {
    const [, , ...rest] = process.argv
    const [filePath] = rest

    const dirName = dirname(filePath)
    const nxProject = await findNxProject(ROOT_DIR, dirName)
    if (!nxProject) {
        console.error(`No Nx project found in ${dirName}`)
        process.exit(1)
    }

    const nxBuildCommand = BUILD_SCRIPT(nxProject)
    console.log(`Running ${nxBuildCommand}`)

    await execPrm(nxBuildCommand, { stdio: 'inherit' })

    const targetPath = `${ROOT_DIR}/dist/${nxProject}/${nxProject}`
    await execPrm(`ln -s -f ${targetPath} ${DEBUGGER_SYMLINK_PATH}`, { stdio: 'inherit' })
})()

/**
 * @type {import('lint-staged').Config}
 */
module.exports = process.env.CI
    ? {
        '**/*.(c|h)': 'npx clang-format --dry-run -Werror',
    }
    : {
        '**/*.(c|h)': 'npx clang-format -i',
        '**/*.(ts|json)': 'prettier --write',
    }

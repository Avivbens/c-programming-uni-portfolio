## Intro

Install dependencies to get started:

```bash
npm ci
```

## Debug

To debug a project, follow these steps:

-   Open the project in Visual Studio Code.
-   Place breakpoints in the code.
-   Open a file in the project.
-   Press `F5` to start debugging.

## Commit Message Guidelines

We follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) specification. Make sure your commit messages follow the format below:

```git
<type>(optional scope): <description>
```

Available types:

1. `chore` - changes that should not affect production user code, e.g. update dev-dependencies
1. `fix` - bug fixes, e.g. fix linting errors
1. `feat` - new features, e.g. add new command
1. `docs` - changes to documentation
1. `ci` - changes to CI configuration
1. For breaking changes, add a `BREAKING CHANGE` section to the commit message body:

```git
feat: <description>

BREAKING CHANGE: <description>
```

## Build

To build all affected projects, execute the following command:

```bash
npm run build
```

To build all projects, execute the following command:

```bash
npx nx run-many --all --target=build
```

## Run

To run a project, execute the following command:

```bash
nx run <project>:run
```

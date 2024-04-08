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

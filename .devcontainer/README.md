# DCC dev container

This folder configures a dev container so you can develop DCC inside Docker and attach Cursor (or VS Code) to it.

**Stack:** Pythia 8.317, HepMC 2.06.11, Ubuntu 22.04 (builder) → Debian trixie-slim (runtime). The dev image uses the builder stage plus clang/clang-tidy/git.

## Use from Cursor

1. **Reopen in Container**  
   Open the repo in Cursor, then run **“Dev Containers: Reopen in Container”**.  
   Cursor will build the `dev` stage of the root `Dockerfile` and start a container. Your workspace is mounted at `/dcc`.

2. **Attach to an existing container**  
   If you prefer to start the container yourself:
   ```bash
   docker build -t dcc:dev --target dev .
   docker run -d --name dcc-dev -v $(pwd):/dcc -w /dcc dcc:dev
   ```
   Then in Cursor: **“Dev Containers: Attach to Running Container…”** → select `dcc-dev`.

Inside the container you can run `make` and `./bin/main` (HepMC and Pythia are installed under `/home/jsk`).

## Image stages

- **builder**: Builds HepMC2, Pythia 8.317, and the DCC binary.
- **runtime**: Minimal image with only runtime libs and `bin/main`; use for production runs.
- **dev**: Same as builder plus dev tools; used by this devcontainer.

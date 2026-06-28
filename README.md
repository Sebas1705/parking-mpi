# Parking MPI

MPI-based parking lot simulator — concurrent vehicle processes managed by a central admin.

Three progressive versions of a multi-process parking simulation built with C++ and MPI.
Each vehicle (car or truck) runs as an independent MPI process that sends enter/exit
requests to an admin process coordinating access to shared parking spots.

---

## Versions

| Version | Directory | Description |
|---|---|---|
| v1 | `parking1/` | Cars only, single floor |
| v2 | `parking2/` | Cars + trucks, single floor (trucks occupy 2 spots) |
| v3 | `parking3/` | Cars + trucks, multi-floor |

---

## Repository Structure

```
parking-mpi/
├── parking1/
│   ├── Admin.cpp / Admin.h   # Admin process (cars only)
│   ├── Car.cpp               # Car process
│   ├── Makefile
│   └── hostfile.config
├── parking2/
│   ├── Admin.cpp / Admin.h   # Admin process (cars + trucks)
│   ├── Car.cpp               # Car process
│   ├── Truck.cpp             # Truck process (occupies 2 consecutive spots)
│   ├── Makefile
│   └── hostfile.config
├── parking3/
│   ├── Admin.cpp / Admin.h   # Admin process (multi-floor, cars + trucks)
│   ├── Car.cpp               # Car process
│   ├── Truck.cpp             # Truck process
│   ├── Makefile
│   └── hostfile.config
├── vehiculos/                # Standalone vehicle process implementations
├── Vehiculos/                # Alternate vehicle implementations
├── Examples/                 # MPI hello-world examples
│   ├── helloworld.c
│   └── helloworld.cpp
└── Makefile                  # Builds all three versions
```

---

## How it works

### Message protocol

Each vehicle sends a 2- or 3-integer message to the admin (rank 0):

```
[rank, mode, type]
  rank  — vehicle's MPI rank (its unique ID)
  mode  — 0 = enter, 1 = exit
  type  — 0 = Car, 1 = Truck  (v2 and v3 only)
```

The admin replies with `assigned = 0` (spot found) or `assigned = -1` (lot full).

### Vehicle lifecycle

1. Send enter request → wait for response.
2. If full (`assigned == -1`), retry immediately until a spot opens.
3. Once parked, sleep for a random 1–10 seconds (simulating dwell time).
4. Send exit request.
5. Repeat.

### Admin logic

- Maintains an `occupied[]` array (v1/v2) or `occupied[][]` matrix (v3) tracking which rank holds each spot.
- Trucks occupy two consecutive spots.
- v3 adds a floor dimension: `Admin(floors, spots_per_floor)`.

---

## Building and Running

### Prerequisites

- MPI implementation (e.g. OpenMPI): `mpiCC`, `mpirun`

### Build all versions

```bash
make
```

### Run a specific version

```bash
cd parking1 && make        # builds and runs: 1 admin + 10 cars
cd parking2 && make        # builds and runs: 1 admin + 4 cars + 4 trucks
cd parking3 && make        # builds and runs: 1 admin + 4 cars + 4 trucks, 4 floors
```

Each `make` target compiles and immediately launches via `mpirun`.
Edit `hostfile.config` to distribute processes across multiple machines.

### Run standalone vehicle processes

```bash
cd vehiculos && make       # builds Car and Truck executables
```

---

## Tech stack

| Technology | Role |
|---|---|
| C++ | Implementation language |
| MPI (OpenMPI) | Inter-process communication |
| `mpiCC` | Compiler wrapper |
| `mpirun` | Process launcher |

---

## Course context

Built for the *High Performance Computing* (Computación de Altas Prestaciones) course.
Demonstrates the MPI process model: processes communicate exclusively via message
passing with no shared memory, reflecting real distributed-system constraints.

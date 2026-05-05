# Prerequisites Guide

This file tells you what must already exist before pasting a snippet.

## Base template (recommended before any snippet)

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
```

## Common assumptions in this repository

- Many snippets are **not standalone** by design.
- If a snippet uses `ll`, `ld`, `pii`, `pll`, define them in your main template.
- Geometry snippets commonly rely on constants/functions from `Geometry/point.h` (`EPS`, `PI`, `dcmp`, `vec`, `dp`, `cp`, `length`).
- If a file has no `#include` lines, assume it expects the base template above.

## File-level dependencies (direct local includes)

- `Graph/2_sat.h`
  - requires `Graph/Tarjan/scc.h`
- `Graph/MST/kruskal.h`
  - requires `Graph/Data Structures/dsu.h`
- `Geometry/lines.h`
  - requires `Geometry/point.h`
- `Geometry/triangles.h`
  - requires `Geometry/point.h`
- `Geometry/polygon.h`
  - requires `Geometry/lines.h` (which itself requires `Geometry/point.h`)
- `Geometry/circles.h`
  - requires `Geometry/point.h`
  - requires `Geometry/triangles.h`
  - requires `Geometry/lines.h`

## Practical copy-paste order

- **Geometry stack**: `point.h` -> `lines.h` -> `triangles.h` -> `circles.h`/`polygon.h`
- **2-SAT stack**: `scc.h` -> `2_sat.h`
- **Kruskal stack**: `dsu.h` -> `kruskal.h`

## Optional platform note

- `Misc/bitmask.h` contains MSVC-specific handling for popcount intrinsics under `_MSC_VER`.

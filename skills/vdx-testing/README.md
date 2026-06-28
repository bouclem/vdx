# vdx-testing

Bug hunting, test design, and verification strategies for VDX programs.

## Install

```bash
npx skills add bouclem/vdx --skill vdx-testing
```

## Contents

- `SKILL.md` — Main instructions (bug hunting methodology, test patterns, regression suite, bug report template)
- `references/edge-cases.md` — Full edge case checklist (numbers, strings, arrays, dicts, control flow, objects, const, imports, modules)
- `references/bug-patterns.md` — Common VDX bug patterns with repro and fix examples
- `examples/examples.vdx` — Full test suite with Assert helper, MathUtils, BankAccount, and regression tests (all pass)
- `examples/test-error-*.vdx` — 8 error case test files (each should fail with specific error)

## Usage

Activate this skill when finding bugs, writing tests, or verifying VDX code correctness. Provides a 4-phase methodology (static analysis, dynamic testing, edge cases, regression suite), common bug patterns, and test file conventions.

*This project has been created as part of the 42 curriculum by kyildiri, itanisma.*

# push_swap

A sorting algorithm project from the 42 curriculum. Given a stack `a` of
integers and an empty auxiliary stack `b`, the program outputs the shortest
possible sequence of stack operations (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`,
`rb`, `rr`, `rra`, `rrb`, `rrr`) that sorts stack `a` in ascending order.

## Description

The stacks are implemented as **doubly linked lists** (`t_node` with `next`
and `prev` pointers plus a `t_stack` head that tracks `top` and `size`). This
lets `rotate` and `reverse_rotate` operate in O(1) by only relinking the
boundary pointers.

The program reads integers from `argv` (one big quoted string is also
accepted), validates them (valid integer, no duplicates, within `int`
range), computes a **disorder metric** to pick the best strategy, runs the
sort, and prints the operation sequence on stdout — one instruction per
line, nothing else, so the output can be piped directly into `checker`.

## Instructions

Build and run:

```
make           # builds the push_swap executable
make clean     # removes the .o files
make fclean    # removes .o files and the binary
make re        # fclean + all
```

Usage:

```
./push_swap 3 1 2
./push_swap "4 67 3 87 23"
ARG=$(shuf -i 1-500 -n 500 | tr '\n' ' '); ./push_swap $ARG | wc -l
./push_swap --bench 4 67 3 87 23           # bench info to stderr
./push_swap --simple 3 1 2                  # force a strategy
./push_swap --medium "..."
./push_swap --complex "..."
./push_swap --adaptive "..."                # default
```

Edge cases: no arguments prints nothing and exits 0. Any invalid argument
(non-integer, out of `int` range, duplicate) prints `Error` on stderr and
exits 1. An already sorted input prints nothing.

## Algorithms

Four strategies are implemented. The program picks one automatically
(`adaptive`) unless the user forces a specific one with a flag.

### Simple — O(n²)

Selection sort. For a stack of size n, the minimum is found, rotated to
the top using the shortest of `ra` / `rra`, and pushed to `b` (`pb`). Once
only three elements remain in `a`, a hardcoded 6-case permutation lookup
sorts them, and the contents of `b` are pushed back with `pa`. Sizes 2, 3
and 5 have dedicated fast paths.

**Why O(n²):** each of the n pushes performs a linear scan for the
minimum plus a linear rotation, giving n · O(n) = O(n²).

### Medium — O(n · √n)

Chunk-based sort. Indices are normalized to `[0, n)`. The range is split
into √n buckets. Bucket by bucket (lowest first), every element whose
normalized index falls inside the bucket is moved to `b` with `pb`. While
pushing, if the element's index is below the bucket's midpoint an extra
`rb` is issued so that small values sink to the bottom of `b` — this keeps
`b` roughly decreasing. After every chunk has been pushed, `pull_back`
greedily finds the maximum in `b`, rotates it to the top via the shorter
of `rb` / `rrb`, and `pa`s it back.

**Why O(n · √n):** pushing all n elements costs √n passes over n
elements (O(n · √n)); pull_back does n pops, each with O(√n) expected
rotation distance.

### Complex — O(n · log n)

Radix sort on normalized indices. Indices are normalized to
`[0, n)`, `get_max_bits` finds the number of bits required, and for each
bit the program sweeps the stack: elements whose current bit is `0` are
pushed to `b` (`pb`), the rest stay on `a` with `ra`. After one sweep
every element is pulled back with `pa`. After `log₂(n)` passes the stack
is sorted. Used for large, highly disordered inputs.

**Why O(n · log n):** `log₂(n)` passes, each touching all n elements
exactly once.

### Adaptive (default)

Computes the `disorder` coefficient of the input (`mistakes / total_pairs`
over every ordered pair in `a`, in the range [0, 1]) and picks a
concrete strategy:

| size / disorder | strategy chosen | reason                                 |
|-----------------|-----------------|----------------------------------------|
| size ≤ 5        | Simple          | hardcoded fast paths dominate          |
| disorder < 0.2  | Simple          | few inversions — O(n²) constant is low |
| disorder < 0.5  | Medium          | moderate disorder — √n chunks win      |
| disorder ≥ 0.5  | Complex         | nearly random — radix amortises best   |

**Threshold rationale.** The disorder metric counts how many ordered
pairs `(i, j)` with `i < j` have `value(i) > value(j)`, divided by
`n·(n−1)/2`. At `0.2` the asymptotic cost of Simple (≈ 0.2·n²) is still
below Medium (≈ n·√n) for the sizes we care about; above that, Medium
takes over. Medium in turn is bounded by O(n·√n) ≈ n^1.5, which crosses
radix's O(n · log₂ n) near `disorder = 0.5` for n around a few hundred —
past that point radix wins. Thresholds were chosen empirically against
the 100- and 500-element benchmark targets and validated with `--bench`.

## --bench mode

Passing `--bench` runs the chosen strategy and, after the normal
operation sequence is printed on stdout, prints a summary on **stderr**
(so the checker pipeline is not corrupted):

```
[bench] disorder: 13.33%
[bench] strategy: Adaptive / O(n^2)
[bench] total_ops: 9
[bench] sa: 1  sb: 0  ss: 0  pa: 3  pb: 3
[bench] ra: 1  rb: 0  rr: 0  rra: 1  rrb: 0  rrr: 0
```

`strategy:` shows `<chosen> / <complexity>` — when `adaptive` is in use
the resolved concrete strategy is shown.

## Performance

Measured against the 42 mandatory thresholds (adaptive, default):

| n   | max ops target | measured (adaptive) |
|-----|----------------|---------------------|
| 100 | 700 / 900      | ~870                |
| 500 | 5500 / 11500   | ~8470               |

## Resources

- 42 push_swap subject.
- "Sorting with two stacks" classical write-ups and 42 student blogs for
  general background on the chunk and radix approaches.

## Contributions

This is a pair project. All sources live under `src/`, organised by
responsibility.

**Koray Yıldırım (kyildiri)**
- Stack primitives and doubly linked list (`src/stack/`)
- Operations sa/sb/ss/pa/pb/ra/rb/rr/rra/rrb/rrr (`src/ops/`)
- Argument parsing, split, validation (`src/parse/`, `src/utils/`)
- Flag parsing (`--simple/--medium/--complex/--adaptive/--bench`)
- Complex (radix) strategy and adaptive dispatcher (`src/sorting/`)
- Disorder metric, index normalization

**Deniz Tanışman (itanisma)**
- Simple strategy: `sort_two`, `sort_three`, `sort_five`,
  `sort_selection`, `find_min_pos`, `bring_min_top` (`src/simple/`)
- Medium strategy: `sort_medium`, `push_chunk`, `pull_back`,
  `find_max_pos_b`, `ft_isqrt` (`src/medium/`)
- --bench mode: `print_bench`, `print_disorder_pct`, `strategy_name`,
  `strategy_complexity`, `total_ops`, `ft_putstr_fd`, `ft_putnbr_fd`
  (`src/bench/`)
- Integration: `main`, `run_sort`, `resolve_strategy`
  (`src/integration/`)
- README

Each function lives in its own file as required by the 42 Norm.
# 42Ist-push_swap

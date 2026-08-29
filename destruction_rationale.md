# Task 2.4 — Destruction of the root and single-release guarantee

## Ownership policy
An `EventGroup` **owns** every child added to it through `add(...)`. Ownership is
represented by a vector of raw `EventComponent*` pointers held only by the parent
group. No child is owned by two parents at the same time, and no leaf keeps an
owning pointer back up the tree. Ownership therefore forms a strict tree: every
component has exactly one owning parent, except the root, which the client owns.

## Why the whole subtree is released exactly once
`~EventGroup()` iterates its children and calls `delete` on each one. Because
`EventComponent` declares a **virtual destructor**, deleting a child through a
base-class pointer dispatches to the concrete destructor:

- deleting a leaf runs that leaf's destructor and frees it;
- deleting a child group runs `~EventGroup()` recursively, which deletes *that*
  group's children first.

Destruction thus recurses depth-first from the root down to the leaves. Since
each component has exactly one owning parent, each `delete` is reached through
exactly one path, so every object is freed once and only once. Deleting the root
(`delete expo;`) releases the entire owned subtree with no leaks and no double
free. This is confirmed with AddressSanitizer, which reports no leaked or
doubly-freed blocks.

## Transfers (runtime reorganisation)
A unit may be moved between groups at runtime. This must not cause a double free.
The rule is: **`release(...)` removes a child from a group's vector without
deleting it**, and the receiving group takes ownership through `add(...)`. After
`transferUnit(from, to, unit)`, only the destination group holds the owning
pointer, so the unit is still destroyed exactly once — by its new owner. The
`transferUnit` helper performs the Composite side of the move; the Observer side
(detaching from the old subject and attaching to the new one) is performed
alongside it once the Observer interface is finalised with Person B.

## Non-owning references
Any pointer that is *not* an ownership link (for example an observer registration
held by a subject) must be a **non-owning** pointer and must never be deleted by
the holder. Observers are destroyed by their owning group in the Composite tree,
not by the subjects that reference them, which is why registrations must be
removed (detached) before an observer is destroyed.

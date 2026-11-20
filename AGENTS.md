# CodePath Studio

CodePath Studio is a native Windows desktop learning app written in C++ with raylib.
Use the project skills for UI and UX work. Keep the existing build, rendering and
input model, and user settings unless the task explicitly calls for a change.

## Design workflow

1. Use `$frontend-design` to define visual direction, typography, hierarchy,
   composition, and the purpose of each animation.
2. Use `$ui-ux-pro-max` to research palettes, font pairings, keyboard navigation,
   contrast, and interaction states. Its Python catalog is design tooling only.
   Prefer generic search domains such as color, typography, and UX.
3. Use `$impeccable` to critique and polish the result. Read its relevant
   Markdown playbooks directly. For raylib work, use native screenshots and manual
   inspection instead of its launcher, browser automation, live editing, and hooks.

Translate applicable guidance into raylib layout coordinates, font measurement,
draw order, hit testing, input focus, and frame based animation. Skip guidance
that depends on React, HTML, CSS, DOM inspection, or web breakpoints.

For authorized UI changes, inspect the running app at 1100×700 and in F11
fullscreen. Check Windows display scaling, text clipping, keyboard focus and
mouse targets. Verify that motion communicates state changes, respects the
existing motion setting, and preserves the accuracy of the learning content.

## Skill sources

The vendored packages retain their upstream licenses. Each entry point includes
project specific scope guidance.

| Skill | Upstream snapshot |
| --- | --- |
| `frontend-design` | [Anthropic](https://github.com/anthropics/skills/tree/e4bb5d960d87126ff3b3a8af1a104b7db67560ba/skills/frontend-design) |
| `ui-ux-pro-max` | [NextLevelBuilder Codex template and catalog](https://github.com/nextlevelbuilder/ui-ux-pro-max-skill/tree/2de1134dc4d1cfe112934611ae5803ae17a1bd7e/cli/assets) |
| `impeccable` | [Impeccable Codex package](https://github.com/pbakaus/impeccable/tree/b49177d32788f2dc585c42831d845dacf1ebed2c/.agents/skills/impeccable) |

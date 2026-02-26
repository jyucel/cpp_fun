#pragma once

// Möjliga spelåtgärder, oberoende av inmatningsmetod.
// InputHandler omvandlar tangenttryckningar till dessa värden.
enum class Action {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Reveal,
    ToggleMark,
    Reset,
    Quit,
    // Okänd/irrelevant tangent — ignoreras av spelloopen
    Unknown
};
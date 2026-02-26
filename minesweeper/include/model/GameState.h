#pragma once

// Spelet kan ha fyra olika lägen.
// WaitingFirstClick betyder att spelet väntar på första klicket.
// Då placeras minorna, så att du aldrig kan förlora på första draget.
enum class GameState {
    WaitingFirstClick,
    Playing,
    Won,
    Lost
};
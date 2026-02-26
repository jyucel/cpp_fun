#pragma once

// Olika svårighetsgrader, precis som i klassiska spelet Minesweeper.
// Varje nivå har en bestämd storlek på spelplanen och antal minor.
enum class Difficulty {
    Beginner,       // Lätt: 9x9 rutor, 10 minor
    Intermediate,   // Medel: 16x16 rutor, 40 minor
    Expert          // Svår: 30x16 rutor, 99 minor
};
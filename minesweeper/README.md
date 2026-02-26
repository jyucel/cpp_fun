# Minesweeper

Ett Minesweeper-spel i C++17 med två gränssnitt: terminal och grafiskt (raylib).

## Bygga

```bash
mkdir build && cd build
cmake ..
make
```

Första bygget laddar ner raylib automatiskt via CMake FetchContent.

## Köra

```bash
./minesweeper_terminal   # Terminalversion
./minesweeper_gui        # Grafisk version
```

## Svårighetsgrader

| Nivå         | Storlek  | Bomber |
|--------------|----------|--------|
| Nybörjare    | 9 × 9    | 10     |
| Mellannivå   | 16 × 16  | 40     |
| Expert       | 30 × 16  | 99     |

## Kontroller

### Terminal
| Tangent      | Åtgärd         |
|--------------|----------------|
| Piltangenter | Flytta markör  |
| Enter        | Öppna cell     |
| F            | Flagga / fråga |
| R            | Starta om      |
| Q            | Avsluta        |

### GUI
| Inmatning          | Åtgärd         |
|--------------------|----------------|
| Vänsterklick       | Öppna cell     |
| Högerklick         | Flagga / fråga |
| R                  | Starta om      |
| Escape             | Tillbaka till menyn |
| Q                  | Avsluta        |

## Krav

- CMake 3.16+
- C++17-kompatibel kompilator
- Internetanslutning vid första bygget (för raylib)

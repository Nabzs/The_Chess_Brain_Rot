# Chess Brainrot

Jeu d'echecs probabiliste en C++17 avec SFML 2.6 et TGUI 1.x. Chaque piece possede des mecaniques aleatoires basees sur 7 distributions de probabilite. Les pieces sont representees par des images brainrot et chaque action a son propre son.

## Technologies

- **C++17**
- **SFML 2.6** : rendu graphique (sprites, formes) + audio
- **TGUI 1.7** : popups UI (promotion, victoire)
- **CMake** avec FetchContent (telecharge SFML et TGUI automatiquement)

## Build (Windows / MinGW)

```bash
cd proba
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

L'executable est genere dans `bin/ChessBrainrot.exe`.

## Lancement

Lancer `bin/ChessBrainrot.exe`. Le jeu se lance en plein ecran.

- **Clic gauche** : selectionner / deplacer une piece
- **Clic droit** : deselectionner
- **Echap** : quitter

## Pieces et mecaniques

| Piece | Code | Mecanique speciale | Loi de probabilite |
|-------|------|--------------------|--------------------|
| Roi | K | Skibidi Panique : 55% de chance de bouger tout seul apres chaque tour | Bernoulli (p=0.55) |
| Dame | Q | Piece classique, pas de mecanique speciale | - |
| Tour | R | Tours Jumelles : 5% de destruction si deux tours alliees sont adjacentes | Geometrique (p=0.05) |
| Fou | B | Explose apres ~4 mouvements, detruit tout dans un rayon de 2 | Poisson (lambda=4) |
| Cavalier | Kn | Piece classique, pas de mecanique speciale | - |
| Pion | P | Promotion via popup TGUI (Dame/Tour/Fou/Cavalier) | - |

## Mecaniques globales

| Mecanique | Description | Loi |
|-----------|-------------|-----|
| Fatigue | Meme piece bougee 2x de suite = bloquee 1-4 tours | Erlang (k=2) |
| Expansion | Le plateau grandit d'une colonne, proba croissante avec le temps | Bernoulli (p variable) |
| Couleur des pieces | Chaque piece a une teinte unique en niveaux de gris | Normale / Box-Muller |
| Positions initiales | Le plateau demarre avec des permutations aleatoires | Uniforme |

## Structure du projet

```
proba/
  include/
    piece.hpp              # Struct Piece (donnees par case)
    board.hpp              # Classe Board (plateau 2D)
    moves.hpp              # Validation des mouvements
    chess_controller.hpp   # Logique du jeu (selection, deplacement, events)
    chess_renderer.hpp     # Rendu SFML
    game_state.hpp         # Detection de victoire + popup
    texture_manager.hpp    # Chargement des 12 textures PNG
    sound_manager.hpp      # Chargement des 14 sons
  src/
    main.cpp               # Boucle principale SFML + events
    board.cpp              # Initialisation plateau + Poisson
    moves.cpp              # isValidMove() + getValidMoves()
    chess_controller.cpp   # movePiece(), fatigue, explosion, expansion...
    chess_renderer.cpp     # Rendu (sprites, overlays, couleurs)
    game_state.cpp         # Victoire + popup TGUI
    texture_manager.cpp    # Charge 12 PNG (6 types x 2 couleurs)
    sound_manager.cpp      # Charge 14 OGG (6 pieces + 8 events)
  assets/
    white_king.png, black_king.png, ...   # 12 images de pieces
    sounds/
      king.ogg, queen.ogg, ...            # 6 sons de pieces
      explosion.ogg, capture.ogg, ...     # 8 sons d'evenements
```

## Sons

- **6 sons par piece** : chaque type de piece fait un bruit different au deplacement
- **8 sons d'evenements** : explosion, tours jumelles, fatigue, panique du roi, expansion, promotion, victoire, capture

## Les 7 distributions de probabilite

1. **Uniforme** : permutations initiales du plateau
2. **Poisson (lambda=4)** : countdown d'explosion des fous
3. **Normale / Box-Muller** : couleur des pieces (niveaux de gris)
4. **Erlang (k=2)** : duree de fatigue
5. **Bernoulli (p=0.55)** : roi nerveux
6. **Geometrique (p=0.05)** : tours jumelles
7. **Bernoulli (p variable)** : expansion du plateau
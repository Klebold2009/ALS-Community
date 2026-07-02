# Copy this README into your project root or read it on GitHub

Installation and quick start

1) Rebuild the project
   - Open the project in your IDE (Visual Studio / Rider) and build the project so new modules are recognized.

2) Add the components to your characters and weapons
   - Add UHealthComponent to your Character (in C++ constructor or in Blueprint via Add Component).
   - For hitscan weapons: add UWeaponHitscanComponent to the weapon or character and call ServerFire from input.
   - For projectile weapons: spawn AProjectileBase on the server using GetWorld()->SpawnActor.

3) Test multiplayer
   - In Editor: Play -> New Editor Window (PIE) -> Number of Players = 2.
   - Fire from one player and verify other player's health decreases in UI (subscribe to OnHealthChanged or check variable replication).

Notes
- All damage is applied on the server. Effects are multicast to clients.
- Replace the API macro name if your module's macro differs. These files use ALSV4_CPP_API.

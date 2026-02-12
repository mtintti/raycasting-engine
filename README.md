

Tämä projekti on tehty harjoitukseksi graafiseen koodaukseen, pelimoottoreihin, reaaliaikaiseen renderöintiin (20+ FPS) sekä raycastingin opetteluun. Projektin ikkuna on jaettu OpenGL:ää käyttäen kahteen osaan, joissa on karttanäkymä ja raycast-näkymä ympyrästä.

Vasemmalla puolella nähdään projektissa generoitu kartta ja pelaaja. Kartta koostuu 21 × 21 -kokoisesta matriisista, jossa on määritetty seinät, polut, alku ja loppu. Matriisista tehdään vertex-taulukko, jossa on asetettu jokaisen kartan ruudun kulmapisteet, jotka yhdistetään GPU:ssa kokonaiseksi kartaksi.

Pelaajan pisteestä generoidaan säteet havainnollistamaan pelaajan näkölinjaa. Pelaajan näkökentästä lisätään askeleittain säteitä niin kauan, kunnes ne löytävät seinän kartasta. Oikealla puolella ikkunaa on testikuva raycastatusta näkymästä, joka muodostaa pallon keskelle ruudulle, kun säteet osuvat siihen. Tulevaisuudessa testikuvan ray–object-leikkausten sijaan aion yhdistää pelaajan näkymän säteet ja käyttää niitä pseudo-3D-pelinäkymän tekemiseen.
```
Koodi repon kansion selitys:
/
├── Shaders/                 # Shader OpenGl koodi (sightline, pelaaja, maze kartta GPU puolelle)
├── glm/                     # GLM vec3 ja matrix matikka grafiikka lib
├── include/                 # Project header 
├── lib/                     # libs and DLLs
├── Game.cpp / Game.h        # Pelilogiikka + liikkuminen, pelaajan fov generointi ja raytrace
├── Maze.cpp / Maze.h        # Maze kartta generoiminen ja tilemap:in värien asettaminen
├── Renderer.cpp / Renderer.h# VAO ja VBO kartalle, fov ja renderöinti näkymä ikkunalle + puolitus
├── main.cpp                 # fps ja peli moottori
```


Demo video projektista:
https://github.com/user-attachments/assets/65436342-abd1-455a-99fc-8650de82f7a3

